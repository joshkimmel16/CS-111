#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

extern int errno;

/*Method to read in a file bit by bit and output to another file bit by bit*/
void ReadAndWrite (int fileDescriptorIn, int fileDescriptorOut)
{
    /*initialize output char buffer and bytes to read*/ 
    const size_t BYTES_TO_READWRITE = 1;
    char* buffer;
    buffer = (char*) malloc(sizeof(char));
    
    /*read the first byte from the file*/
    ssize_t readResult = read(fileDescriptorIn, buffer, BYTES_TO_READWRITE);
    
    /*while there are more bytes to read*/
    while (readResult > 0)
    {
        /*write out the byte that was just read in*/
        ssize_t writeResult = write(fileDescriptorOut, buffer, BYTES_TO_READWRITE);
        
        /*we encountered an error writing to output*/
        if (writeResult == -1)
        {
            fprintf(stderr, "Error occurred while writing to output!");
            break;
        }
        
        /*read in the next byte*/
        readResult = read(fileDescriptorIn, buffer, BYTES_TO_READWRITE);
    }
    
    /*we encountered an issue while reading the input*/
    if (readResult == -1) 
    {
        fprintf(stderr, "Error occurred while reading from input!");
        exit(1);
    }
    
    /*free allocated memory for char buffer*/
    free(buffer);
}

/*Signal Handling method for SIGSEGV - segmentation fault*/
void SegFaultHandler (int signal)
{
    if (signal == SIGSEGV)
    {
        fprintf(stderr, "A segmentation fault has been thrown!");
        exit(4);
    }
}

/*main program executable*/
int main (int argc, char** argv)
{
    /*define available options for program*/
    static struct option longOptions [] = 
    {
        { "input", required_argument, 0, 'i' },
        { "output", required_argument, 0, 'o'},
        { "segfault", no_argument, 0, 's'},
        { "catch", no_argument, 0, 'c'}
    };
    
    /*define argument variables*/
    char* input = NULL;
    char* output = NULL;
    int segfault = 0; /*defaults to false*/
    int catch = 0; /*defaults to false*/
    int currentOpt = 0; /*to capture result of iterative calls to getopt*/
    
    /*use getopt to capture provided argument values to main*/
    while ((currentOpt = getopt_long(argc, argv, "i:o:sc", longOptions, NULL)) > 0)
    {
        switch (currentOpt)
        {
            /*set input to current argument value*/
            case 'i':
                input = optarg;
                break;
            /*set output to currrent argument value*/
            case 'o':
                output = optarg;
                break;
            /*force segmentation fault*/
            case 's':
                segfault = 1;
                break;
            /*register ability to catch segmentation fault*/
            case 'c':
                catch = 1;
                break;
            /*we do not allow any other arguments, so error out*/
            default:
                fprintf(stderr, "Invalid argument: %d. Arguments must conform exactly to the following structure:\n--input or -i = input file name (optional)\n--output or -o = output file name (optional)\n--segfault or -s = force a segmentation fault (optional)\n--catch or -c = catch segmenation fault (optional)\n", currentOpt);
                exit(1);
        }
    }
           
    /*if catch was specified, register handler*/
    if (catch == 1)
    {
        signal(SIGSEGV, SegFaultHandler);
    }
           
    /*if segfault was specified, force segmentation fault*/
    if (segfault == 1)
    {
        char* iAmNull = NULL;
        char noYouArent = *iAmNull; /*foolishly try accessing a pointer that was just set to NULL*/
    }
    
    /*if input specified, copy input onto stdin*/
    int fdIn = 0;
    if (input)
    {
        fdIn = open(input, O_RDONLY); /*attempt to open the file in readonly mode*/
        if (fdIn >= 0)
        {
            close(0);
            dup(fdIn); /*since 0 is now the lowest open file descriptor*/
            close(fdIn);
        }
        /*an error was encoutered while opening the input file - so error out*/
        else
        {
            fprintf(stderr, "The following error was encountered while attempting to open the file provided in the --input (-i) argument: %s\n", strerror(errno));
            exit(2);
        }
    }
    
    /*if output specified, open it as new stdout*/
    int fdOut = 1;
    if (output)
    {
        fdOut = creat(output, S_IRWXU); /*set read, write, search, or execute for file owner*/
        if (fdOut >= 0)
        {
            close(1);
            dup(fdOut); /*since 1 is now the lowest open file descriptor*/
            close(fdOut);
        }
        /*an error was encoutered while opening the output file - so error out*/
        else
        {
            fprintf(stderr, "The following error was encountered while attempting to open the file provided in the --output (-o) argument: %s\n", strerror(errno));
            exit(3);
        }
    }
    
    /*perform byte-wise copy of stdin to stdout*/
    ReadAndWrite(0, 1);
    
    exit(0);
}