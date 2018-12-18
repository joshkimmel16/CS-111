#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <poll.h>
#include <sys/wait.h>

extern int errno;

/*global variables*/
char* buffer;
const unsigned int BUFFER_MAX = 256;
struct termios originalTerminalState;
int pipeIn[2];
int pipeOut[2];
struct pollfd fds[2];

/*restore original terminal state - handler for exit of main*/
void RestoreTerminal ()
{
    int stdInFd = STDIN_FILENO;
    tcsetattr(stdInFd, TCSANOW, &originalTerminalState); /*force change immediately*/
}

/*create a pipe between 2 file descriptors*/
void InitializePipe (int p[2])
{
    if (pipe(p) < 0)
    {
        /*pipe initialization failed so throw error*/
        fprintf(stderr, "The following error was encountered while attempting to open the provided pipe: %s\n", strerror(errno));
        exit(1);
    }
}

/*method to correctly set up piping and execute the provided shell*/
void ExecuteShell (char* shell)
{    
	/*duplicate reading end of pipeIn to shell's stdin. Shell will now read from pipeIn's reading end*/
    /*duplicate writing end of pipeOut to shell's stdout, stderr. Shell's output will now be written to pipeOut's writing end*/
    dup2(pipeIn[0], 0);
	dup2(pipeOut[1], 1);
	dup2(pipeOut[1], 2);
    
	/*execute provided shell*/
    if (execvp(shell, NULL) == -1)
	{
		/*something went wrong with executing provided shell so error out*/
        fprintf(stderr, "The following error was encountered while attempting to execute the provided shell: %s\n", strerror(errno));
		exit(1);
	}
}

/*read in on one file descriptor and output to another file descriptor*/
void ExecuteReadWrite (int fileDescriptorIn, int fileDescriptorOut)
{
    int currentOffset = 0;
    ssize_t currentRead = read(fileDescriptorIn, buffer, BUFFER_MAX);
    while (currentRead > 0)
    {
        /*process characters read in one at a time*/
        while (currentOffset < currentRead)
        {
            /*check for ^D - close program if so*/
            if (*(buffer + currentOffset) == 4) /*ctrl-D is EOT=4 in ASCII*/
            {
                exit(0);
            }
            
            /*map <cr> or <lf> to <cr><lf> - special case so write now and continue*/
            if (*(buffer + currentOffset) == '\r' || *(buffer + currentOffset) == '\n')
            {
                char temp[2] = {'\r','\n'};
                write(fileDescriptorOut, temp, 2);
                currentOffset++;
                continue;
            }
            
            /*write characters one at a time to fd out*/
            write(fileDescriptorOut, buffer+currentOffset, 1);
            currentOffset++;
        }
        
        /*perform next read once entire write is done*/
        currentRead = read(fileDescriptorIn, buffer, BUFFER_MAX);
        currentOffset = 0;
    }
}

void ExecuteReadWriteShell (int fileDescriptorIn, int fileDescriptorOut)
{
    /*configure polling options - listen for output and error*/
    fds[0].fd = fileDescriptorIn;
    fds[1].fd = pipeOut[0];
    fds[0].events = POLLIN | POLLHUP | POLLERR;
    fds[1].events = POLLIN | POLLHUP | POLLERR;
    int pollReturn;
    int endFlag = 1;
    int errorFlag = 0;
    
    pollReturn = poll(fds, 2, 0);
    if (pollReturn < 0)
    {
        /*clean up pipes and error out*/
        endFlag = 0;
        fprintf(stderr, "The following error was encountered while attempting to poll file descriptors: %s\n", strerror(errno));
    }
    
    while (endFlag > 0)
    {
        /*handle input from terminal*/
        if ((fds[0].revents & POLLIN))
        {
            int termOffset = 0;
            ssize_t termRead = read(fileDescriptorIn, buffer, BUFFER_MAX);
            while (termOffset < termRead)
            {
                /*check for ^D - close program if so*/
                if (*(buffer + termOffset) == 4) /*ctrl-D is EOT=4 in ASCII*/
                {
                    /*no need to continue reading - let's move on*/
                    endFlag = 0;
                    break;
                }

                /*map <cr> or <lf> to <cr><lf> - special case so write now and continue*/
                if (*(buffer + termOffset) == '\r' || *(buffer + termOffset) == '\n')
                {
                    char temp[2] = {'\r','\n'};
                    write(fileDescriptorOut, temp, 2);
                    write(pipeIn[1], temp, 2);
                    termOffset++;
                    continue;
                }

                /*write characters one at a time to fd out*/
                write(fileDescriptorOut, buffer+termOffset, 1);
                write(pipeIn[1], buffer+termOffset, 1);
                termOffset++;
            }
        }
        
        /*handle output from shell*/
        if ((fds[1].revents & POLLIN))
        {
            int shellOffset = 0;
            ssize_t shellRead = read(pipeOut[0], buffer, BUFFER_MAX);
            while (shellOffset < shellRead)
            {
                /*write characters one at a time to fd out*/
                write(fileDescriptorOut, buffer+shellOffset, 1);
                shellOffset++;
            }
        }
        
        /*deal with polling error reports on either terminal or shell*/
        if ((fds[0].revents & POLLERR) || (fds[0].revents & POLLHUP) || (fds[1].revents & POLLERR) || (fds[1].revents & POLLHUP))
        {
            endFlag = 0;
            errorFlag = 1;
        }
        
        pollReturn = poll(fds, 2, 0);
        if (pollReturn < 0)
        {
            /*something went wrong with next poll, so error out*/
            endFlag = 0;
            errorFlag = 1;
        }
    }
    
    /*do one last read/write from shell in case there is pending output*/
    if ((fds[1].revents & POLLIN))
    {
        int shellOffset = 0;
        ssize_t shellRead = read(pipeOut[0], buffer, BUFFER_MAX);
        while (shellOffset < shellRead)
        {
            /*map <cr> or <lf> to <cr><lf> - special case so write now and continue*/
            if (*(buffer + shellOffset) == '\r' || *(buffer + shellOffset) == '\n')
            {
                char temp[2] = {'\r','\n'};
                write(fileDescriptorOut, temp, 2);
                shellOffset++;
                continue;
            }

            /*write characters one at a time to fd out*/
            write(fileDescriptorOut, buffer+shellOffset, 1);
            shellOffset++;
        }
    }
    
    /*deal with generic error from process*/
    if (errorFlag > 0)
    {
        fprintf(stderr, "The following error was encountered while reading/writing over the pipes: %s\n", strerror(errno));
        exit(1);
    }
}

/*method to clean up pipes and shell process*/
void Cleanup (pid_t shellPid)
{
    /*close all open file descriptors*/
    close(pipeIn[0]);
	close(pipeIn[1]);
	close(pipeOut[0]);
	close(pipeOut[1]);
    
    /*kill shell process*/
    int killStatus = kill(shellPid, SIGINT);
    if (killStatus < 0)
    {
        fprintf(stderr, "The following error was encountered while attempting to kill the shell process: %s\n", strerror(errno));
        exit(1);
    }
}

/*method to check on exit status of the shell process*/
void CheckShell (pid_t shellPid)
{
    int status = 0;
    waitpid(shellPid, &status, 0);
    if (WIFEXITED(status)) 
    { 
        printf("Shell Exit Status: %d\n", WEXITSTATUS(status)); 
    }
    else if (WIFSIGNALED(status)) 
    { 
        printf("Shell Exit due to Signal Num: %d\n", WTERMSIG(status)); 
    }
    else 
    { 
        printf("Shell Exited Normally"); 
    }
}

/*main program executable*/
int main (int argc, char** argv)
{
    buffer = (char*)malloc(BUFFER_MAX * sizeof(char));
    
    /*define available options for program*/
    static struct option longOptions [] = 
    {
        { "shell", required_argument, 0, 's' }
    };
    
    /*define argument variables*/
    char* shell = NULL;
    
    /*use getopt to capture provided argument values to main*/
    int currentOpt = 0;
    while ((currentOpt = getopt_long(argc, argv, "s", longOptions, NULL)) > 0)
    {
        switch (currentOpt)
        {
            /*shell option indicated*/
            case 's':
                shell = optarg;
                break;
            default:
                break;
        }
    }
    
    /*determine whether or not stdin refers to an open terminal - using global stdin fd*/
    int stdInFd = STDIN_FILENO;
    if (!isatty(stdInFd))
    {
        /*not an open terminal, so terminate*/
        fprintf(stderr, "Standard In is not a terminal! Cannot proceed.");
        exit(1);
    }
    
    /*save existing terminal state*/
    tcgetattr(stdInFd, &originalTerminalState);
    
    /*register terminal restoration on normal exit*/
    atexit(RestoreTerminal);
    
    /*retrieve new terminal state for modification, make appropriate changes, then apply them*/
    struct termios newTerminalState;
    tcgetattr(stdInFd, &newTerminalState);
    newTerminalState.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
    newTerminalState.c_cc[VMIN] = 1;
    newTerminalState.c_cc[VTIME] = 0;
    if (tcsetattr(stdInFd, TCSANOW, &newTerminalState) < 0)
	{
		/*something went wrong setting new state so exit*/
		fprintf(stderr, "The following error was encountered while changing the state of the terminal: %s\n", strerror(errno));
        exit(1);
	}
    
    /*initialize pipes*/
    InitializePipe(pipeIn);
    InitializePipe(pipeOut);
    
    /*if a shell is provided, fork current process and execute shell*/
    pid_t shellPid;
    if (shell)
    {
        shellPid = fork();
        
        if (shellPid < 0)
        {
            /*something went wrong with forking our process so exit*/
		    fprintf(stderr, "The following error was encountered while trying to fork the main process: %s\n", strerror(errno));
            exit(1);
        }
        else if (shellPid == 0)
        {
            /*we are now the child process, so execute the shell*/
            ExecuteShell(shell);
        }
        else
        {
            /*execute piping on stdin, stdout*/
            /*then clean everything up after we're done*/
            ExecuteReadWriteShell(0, 1);
            Cleanup(shellPid);
            CheckShell(shellPid);
        }
    }
    else
    {
        /*execute piping on stdin, stdout*/
        ExecuteReadWrite(0, 1);
    }
    
    free(buffer);
    exit(0);
}