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
struct termios originalTerminalState;
struct pollfd fds[2];
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
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

/*restore original terminal state - handler for exit of main*/
void RestoreTerminal ()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTerminalState); /*force change immediately*/
}

/*establish connection to server over a socket*/
void ConnectToServer (char* port, char* host)
{
    portno = atoi(port); /*convert port string to required port number format*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0); /*initialize socket file descriptor*/
    if (sockfd < 0)
    {
        fprintf(stderr, "The following error was encountered while attempting to open a socket: %s\n", strerror(errno));
        exit(1);
    }
    
    server = gethostbyname(host); /*retrieve the server info using the provided server name*/
    if (server == NULL) {
        fprintf(stderr, "The following error was encountered while attempting to retrieve the provided host: %s\n", strerror(errno));
        exit(1);
    }
    
    /*configure parameters for socket (type, host address, port)*/
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    
    /*attempt to connect to server over socket*/
    if (connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "The following error was encountered while attempting to connect to the provided host: %s\n", strerror(errno));
        exit(1);
    }
}

/*read in on one file descriptor and output to another file descriptor*/
void ExecuteReadWrite (int fileDescriptorIn, int fileDescriptorOut, int logFileDescriptor, int encrypt)
{
    /*configure polling options - listen for output and error*/
    fds[0].fd = fileDescriptorIn;
    fds[1].fd = sockfd;
    fds[0].events = POLLIN | POLLHUP | POLLERR;
    fds[1].events = POLLIN | POLLHUP | POLLERR;
    int pollReturn;
    int endFlag = 1;
    int inputFlag = 1;
    int errorFlag = 0;
    
    pollReturn = poll(fds, 2, 0);
    if (pollReturn < 0)
    {
        endFlag = 0;
        errorFlag = 1;
    }
    
    while (endFlag > 0)
    {
        /*check for socket closed on server-side*/
        if (inputFlag == 0)
        {
            /*try to read from the socket and output stuff until it is closed*/
            int socketOffset = 0;
            ssize_t socketRead = read(sockfd, buffer, BUFFER_MAX);
            if (socketRead > 0)
            {
                while (socketOffset < socketRead)
                {
                    /*write characters one at a time to fd out*/
                    if (encrypt == 1)
                    {
                        write(logFileDescriptor, buffer+socketOffset, 1);
                        Decrypt(buffer+socketOffset, 1);
                        write(fileDescriptorOut, buffer+socketOffset, 1);
                    }
                    else
                    {
                        write(fileDescriptorOut, buffer+socketOffset, 1);
                        write(logFileDescriptor, buffer+socketOffset, 1);
                    }
                    socketOffset++;
                }
            }
            else
            {
                endFlag = 0;
                continue;
            }
        }
        
        /*handle input from terminal - only if the user hasn't already initiated a shutdown*/
        if (inputFlag == 1 && (fds[0].revents & POLLIN))
        {
            int termOffset = 0;
            ssize_t termRead = read(fileDescriptorIn, buffer, BUFFER_MAX);
            while (termOffset < termRead)
            {
                /*check for ^D - close program if so*/
                if (*(buffer + termOffset) == 4) /*ctrl-D is EOT=4 in ASCII*/
                {
                    /*indicate that we are done reading from the terminal and send to server for shutdown*/
                    inputFlag = 0;
                    write(fileDescriptorOut, buffer+termOffset, 1);
                    if (encrypt == 1)
                    {
                        Encrypt(buffer+termOffset, 1);
                        write(sockfd, buffer+termOffset, 1);
                        write(logFileDescriptor, buffer+termOffset, 1);
                    }
                    else
                    {
                        write(sockfd, buffer+termOffset, 1);
                        write(logFileDescriptor, buffer+termOffset, 1);
                    }
                    break;
                }

                /*map <cr> or <lf> to <cr><lf> - special case so write now and continue*/
                if (*(buffer + termOffset) == '\r' || *(buffer + termOffset) == '\n')
                {
                    char temp[2] = {'\r','\n'};
                    write(fileDescriptorOut, temp, 2);
                    if (encrypt == 1)
                    {
                        char* text = temp;
                        Encrypt(text, 2);
                        write(sockfd, text, 2);
                        write(logFileDescriptor, text, 2);
                    }
                    else
                    {
                        write(sockfd, temp, 2);
                        write(logFileDescriptor, temp, 2);
                    }
                    termOffset++;
                    continue;
                }

                /*write characters one at a time to fd out*/
                write(fileDescriptorOut, buffer+termOffset, 1);
                if (encrypt == 1)
                {
                    Encrypt(buffer+termOffset, 1);
                    write(sockfd, buffer+termOffset, 1);
                    write(logFileDescriptor, buffer+termOffset, 1);
                }
                else
                {
                    write(sockfd, buffer+termOffset, 1);
                    write(logFileDescriptor, buffer+termOffset, 1);
                }
                termOffset++;
            }
        }
        
        /*handle output from server*/
        if (inputFlag == 1 && (fds[1].revents & POLLIN))
        {
            int socketOffset = 0;
            ssize_t socketRead = read(sockfd, buffer, BUFFER_MAX);
            while (socketOffset < socketRead)
            {
                /*write characters one at a time to fd out*/
                if (encrypt == 1)
                {
                    write(logFileDescriptor, buffer+socketOffset, 1);
                    Decrypt(buffer+socketOffset, 1);
                    write(fileDescriptorOut, buffer+socketOffset, 1);
                }
                else
                {
                    write(fileDescriptorOut, buffer+socketOffset, 1);
                    write(logFileDescriptor, buffer+socketOffset, 1);   
                }
                socketOffset++;
            }
        }
        
        /*deal with polling error reports on either terminal or socket*/
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
    
    /*deal with generic error from process*/
    if (errorFlag > 0)
    {
        fprintf(stderr, "The following error was encountered while passing data to/from the server: %s\n", strerror(errno));
        exit(1);
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
        { "log", required_argument, 0, 'l' },
        { "host", required_argument, 0, 'h' },
        { "encrypt", required_argument, 0, 'e' }
    };
    
    /*define argument variables*/
    char* port = NULL;
    char* log = NULL;
    char* host = NULL;
    char* keyFile = NULL;
    
    /*use getopt to capture provided argument values to main*/
    int currentOpt = 0;
    while ((currentOpt = getopt_long(argc, argv, "p:l:h:e", longOptions, NULL)) > 0)
    {
        switch (currentOpt)
        {
            /*port indicated*/
            case 'p':
                port = optarg;
                break;
            /*log indicated*/
            case 'l':
                log = optarg;
                break;
            /*host indicated*/
            case 'h':
                host = optarg;
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
    if (!log)
    {
        log = "./clientLog.txt";
    }
    if (!host)
    {
        host = "localhost";
    }
    
    /*determine whether or not stdin refers to an open terminal - using global stdin fd*/
    if (!isatty(STDIN_FILENO))
    {
        /*not an open terminal, so terminate*/
        fprintf(stderr, "Standard In is not a terminal! Cannot proceed.");
        exit(1);
    }
    
    /*save existing terminal state*/
    tcgetattr(STDIN_FILENO, &originalTerminalState);
    
    /*register terminal restoration on normal exit*/
    atexit(RestoreTerminal);
    
    /*retrieve new terminal state for modification, make appropriate changes, then apply them*/
    struct termios newTerminalState;
    tcgetattr(STDIN_FILENO, &newTerminalState);
    newTerminalState.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
    newTerminalState.c_cc[VMIN] = 1;
    newTerminalState.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newTerminalState) < 0)
	{
		/*something went wrong setting new state so exit*/
		fprintf(stderr, "The following error was encountered while changing the state of the terminal: %s\n", strerror(errno));
        exit(1);
	}
    
    /*try to connect to server via socket*/
    ConnectToServer(port, host);
    
    /*initialize encryption/decryption mechanisms*/
    if (keyFile)
    {
        char* key = RetrieveKey(keyFile);
        InitializeCrypto(key, keyLength);
    }
    
    /*create log file for client input/output logging*/
    int logFd = creat(log, S_IRWXU);
    if (logFd < 0)
    {
        fprintf(stderr, "The following error was encountered while attempting to create the log file: %s\n", strerror(errno));
        exit(1);
    }
    
    /*execute client-server communication on stdin, stdout*/
    int enc = 0;
    if (keyFile)
    {
        enc = 1;
    }
    ExecuteReadWrite(0, 1, logFd, enc);
    
    /*clean up encryption mechanisms*/
    if (keyFile)
    {
        CleanupCrypto();
    }
    
    free(buffer);
    exit(0);
}