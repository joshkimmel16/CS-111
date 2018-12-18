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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <mcrypt.h>
#include <sys/stat.h>

extern int errno;

/*global variables*/
char* buffer;
const unsigned int BUFFER_MAX = 256;
int pipeIn[2];
int pipeOut[2];
struct pollfd fds[2];
int sockfd, newsockfd, portno, clilen, n;
struct sockaddr_in serv_addr, cli_addr;
pid_t shellPid;
MCRYPT encryptFd, decryptFd;
int keyLength;

/*method to read an encryption key from file*/
char* RetrieveKey (char* keyfile)
{
    struct stat keyStat;
	int keyFd = open(keyfile, O_RDONLY);
	if (fstat(keyFd, &keyStat) < 0) 
    { 
        fprintf(stderr, "The following error was encountered while attempting to determine key file statistics: %s\n", strerror(errno));
        exit(1);
    }
	char* key = (char*) malloc(keyStat.st_size * sizeof(char));
	if (read(keyFd, key, keyStat.st_size) < 0) 
    { 
        fprintf(stderr, "The following error was encountered while attempting to read the key file: %s\n", strerror(errno));
        exit(1); 
    }
	keyLength = keyStat.st_size;
	return key;
}

/*encrypt the given text*/
int Encrypt (char* text, int length)
{
	if (mcrypt_generic(encryptFd, text, length) != 0) 
    { 
        /*throw error*/
        return -1;
    }
    return 1;
}

/*decrypt the given text*/
int Decrypt (char* text, int length)
{
	if (mdecrypt_generic(decryptFd, text, length) != 0) 
    { 
        /*throw error*/
        return -1;
    }
    return 1;
}

/*initialize encryption/decryption mechanisms*/
void InitializeCrypto (char* key, int keyLength)
{
	encryptFd = mcrypt_module_open("blowfish", NULL, "cfb", NULL);
	if (encryptFd == MCRYPT_FAILED) 
    { 
        fprintf(stderr, "The following error was encountered while attempting to initialize encryption mechanism: %s\n", strerror(errno));
        exit(1); 
    }
	if (mcrypt_generic_init(encryptFd, key, keyLength, NULL) < 0) 
    { 
        fprintf(stderr, "The following error was encountered while attempting to initialize encryption mechanism: %s\n", strerror(errno));
        exit(1); 
    }

	decryptFd = mcrypt_module_open("blowfish", NULL, "cfb", NULL);
	if (decryptFd == MCRYPT_FAILED) 
    { 
        fprintf(stderr, "The following error was encountered while attempting to initialize decryption mechanism: %s\n", strerror(errno));
        exit(1);  
    }
	if (mcrypt_generic_init(decryptFd, key, keyLength, NULL) < 0) 
    { 
        fprintf(stderr, "The following error was encountered while attempting to initialize decryption mechanism: %s\n", strerror(errno));
        exit(1); 
    }
}

/*clean up crypto objects*/
void CleanupCrypto ()
{
	mcrypt_generic_deinit(encryptFd);
	mcrypt_module_close(encryptFd);

	mcrypt_generic_deinit(decryptFd);
	mcrypt_module_close(decryptFd);
}

/*establish a socket for communication with a client*/
int CreateSocket (char* port)
{
    portno = atoi(port); /*convert port string to required port number format*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0); /*initialize socket file descriptor*/
    if (sockfd < 0)
    {
        fprintf(stderr, "The following error was encountered while attempting to open a socket: %s\n", strerror(errno));
        return -1;
    }
    
    /*configure parameters for socket (family, type, port)*/
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    /*attempt to connect to server over socket*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "The following error was encountered while attempting to establish the socket on the provided port: %s\n", strerror(errno));
        return -1;
    }
    
    /*listen for incoming client connection*/
    listen(sockfd, 5);
    
    /*attempt to accept connection from client*/
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
    {
        fprintf(stderr, "The following error was encountered while attempting to establish the socket on the provided port: %s\n", strerror(errno));
        return -1;
    }
    
    return 1;
}

/*method to close the server socket*/
void Cleanup (pid_t shellPid)
{
    close(newsockfd);
    close(sockfd);
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

/*method to read from socket (client), pipe to shell, and output back to client*/
void ExecuteReadWriteShell (int encrypt)
{
    /*configure polling options - listen for output and error*/
    fds[0].fd = newsockfd;
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
        /*handle input from socket*/
        if ((fds[0].revents & POLLIN))
        {
            int termOffset = 0;
            ssize_t termRead = read(newsockfd, buffer, BUFFER_MAX);
            while (termOffset < termRead)
            {
                /*make sure to decrypt this stuff if needed*/
                if (encrypt == 1)
                {
                    Decrypt(buffer+termOffset, 1);
                }
                
                /*check for ^D - close program if so*/
                if (*(buffer + termOffset) == 4) /*ctrl-D is EOT=4 in ASCII*/
                {
                    /*no need to continue reading - let's move on*/
                    endFlag = 0;
                    break;
                }

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
                if (encrypt == 1)
                {
                    Encrypt(buffer+shellOffset, 1);
                    write(newsockfd, buffer+shellOffset, 1);
                }
                else
                {
                    write(newsockfd, buffer+shellOffset, 1);
                }
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
                write(newsockfd, temp, 2);
                shellOffset++;
                continue;
            }

            /*write characters one at a time to fd out*/
            write(newsockfd, buffer+shellOffset, 1);
            shellOffset++;
        }
    }
    
    /*deal with generic error from process*/
    if (errorFlag > 0)
    {
        fprintf(stderr, "The following error was encountered while reading/writing over the socket and pipes: %s\n", strerror(errno));
    }
}

/*main program executable*/
int main (int argc, char** argv)
{
    buffer = (char*)malloc(BUFFER_MAX * sizeof(char));
    
    /*define available options for program*/
    static struct option longOptions [] = 
    {
        { "port", required_argument, 0, 'p' },
        { "shell", required_argument, 0, 's' },
        { "encrypt", required_argument, 0, 'e' }
    };
    
    /*define argument variables*/
    char* port = NULL;
    char* shell = NULL;
    char* keyFile = NULL;
    
    /*use getopt to capture provided argument values to main*/
    int currentOpt = 0;
    while ((currentOpt = getopt_long(argc, argv, "p:s:e", longOptions, NULL)) > 0)
    {
        switch (currentOpt)
        {
            /*port indicated*/
            case 'p':
                port = optarg;
                break;
            /*shell indicated*/
            case 's':
                shell = optarg;
                break;
            /*keyfile indicated*/
            case 'e':
                keyFile = optarg;
                break;
            default:
                break;
        }
    }
    
    /*check to ensure received required arguments*/
    if (!port)
    {
        fprintf(stderr, "Missing required argument port (--port or -p)!");
        exit(1);
    }
    if (!shell)
    {
        fprintf(stderr, "Missing required argument shell (--shell or -s)!");
        exit(1);
    }
    
    /*initialize encryption/decryption mechanisms*/
    if (keyFile)
    {
        char* key = RetrieveKey(keyFile);
        InitializeCrypto(key, keyLength);
    }
    
    /*initialize pipes*/
    InitializePipe(pipeIn);
    InitializePipe(pipeOut);
    
    /*execute shell*/
    shellPid = fork();
        
    if (shellPid < 0)
    {
        /*something went wrong with forking our process so exit*/
        fprintf(stderr, "The following error was encountered while trying to fork the main process: %s\n", strerror(errno));
        exit(1);
    }
    else if (shellPid == 0)
    {
        /*we are now the child process, so execute the shell (hardcoded to bash)*/
        ExecuteShell(shell);
    }
    else
    {
        /*we are the server, so open up the socket and start piping*/
        /*then clean everything up after we're done*/
        int sockResult = CreateSocket(port);
        if (sockResult > 0)
        {
            int enc = 0;
            if (keyFile)
            {
                enc = 1;
            }
            ExecuteReadWriteShell(enc);
        }
        Cleanup(shellPid);
        CheckShell(shellPid);
        
        /*clean up encryption mechanisms*/
        if (keyFile)
        {
            CleanupCrypto();
        }
    }
    
    free(buffer);
    exit(0);
}