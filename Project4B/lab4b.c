#include "mraa.h"
#include <math.h>
#include <poll.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

extern int errno;
int period = 1;
int scale = 0;
char* logFile = NULL;
int logFd;
int shutdownFlag = 0;
int sampleTime = -1;
int shouldSample = 1;

mraa_aio_context tempSensor;
mraa_gpio_context buttonSensor;

int B = 4275;
double R0 = 100000.0;

struct pollfd fds[1];
char* buffer;
char* currentCommand;
char* commandValue;
int commandMode = 0;
const unsigned int BUFFER_MAX = 256;

/*method to bring sensors online*/
void BringUpSensors ()
{
    mraa_init();
    
    /*bring up temperature sensor on A0*/
    tempSensor = mraa_aio_init(0);
    if (tempSensor == NULL)
    {
        fprintf(stderr, "The following error was encountered while trying to bring up the temperature sensor: %s\n", strerror(errno));
        exit(1);
    }
    
    /*bring up button sensor on D3*/
    buttonSensor = mraa_gpio_init(3);
    if( buttonSensor == NULL )
    {
        fprintf(stderr, "The following error was encountered while trying to bring up the button sensor: %s\n", strerror(errno));
        exit(1);
    }
    
    /*set directivity of button sensing*/
    mraa_gpio_dir(buttonSensor, MRAA_GPIO_IN);
}

/*method to take sensors offline*/
void BringDownSensors ()
{
    mraa_aio_close(tempSensor);
    mraa_gpio_close(buttonSensor);
}

/*simple signal handler to register*/
void HandleSignal(int signalNum)
{
    BringDownSensors();
    if (signalNum == SIGINT)
    {
        exit(0);
    }
    exit(1);
}

/*method to properly shutdown*/
void Shutdown()
{
    //capture current time
    time_t rawtime;
    struct tm *timeInfo;
    time(&rawtime);
    timeInfo = localtime(&rawtime);

    printf("%02d:%02d:%02d SHUTDOWN\n", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec ); /*to stdout*/
    if (logFile)
    {
        dprintf(logFd, "%02d:%02d:%02d SHUTDOWN\n", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec ); /*to log*/
    }

    BringDownSensors();
}

/*method to determine whether the shutdown button has been pressed*/
void CheckButton ()
{
    int buttonResult = mraa_gpio_read(buttonSensor);
    if (buttonResult == -1)
    {
        BringDownSensors();
        fprintf(stderr, "The following error was encountered while trying to read the button sensor: %s\n", strerror(errno));
        exit(1);
    }
    else if (buttonResult == 1)
    {
        /*indicate shutdown*/
        shutdownFlag = 1;
    }
}

/*method to read the temperature off of the sensor*/
void ReadTemperature ()
{
    //capture current time
    time_t rawtime;
    struct tm *timeInfo;
    time(&rawtime);
    timeInfo = localtime(&rawtime);
    
    /*read temperature*/
    int tempRead = mraa_aio_read(tempSensor);
    if (tempRead == -1)
    {
        BringDownSensors();
        fprintf(stderr, "The following error was encountered while trying to read the temperature sensor: %s\n", strerror(errno));
        exit(1);
    }

    /*compute temperature values*/
    double R = 1023.0/((double)tempRead) - 1.0;
    R = R0 * R;

    double celsius = 1.0/(log(R/R0)/B + 1/298.15) - 273.15;
    double fahrenheit = celsius * 9/5 + 32;

    /*depending on scale, craft report message*/
    if (scale == 0)
    {
        /*print result in Fahrenheit*/
        printf("%02d:%02d:%02d %0.1f\n", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, fahrenheit); /*to stdout*/
        if (logFile)
        {
            dprintf(logFd, "%02d:%02d:%02d %0.1f\n", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, fahrenheit); /*to log*/
        }
    }
    else
    {
        /*print result in Celsius*/
        printf("%02d:%02d:%02d %0.1f\n", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, celsius); /*to stdout*/
        if (logFile)
        {
            dprintf(logFd, "%02d:%02d:%02d %0.1f\n", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, celsius); /*to log*/
        }
    }
}

/*method to process received commands from stdin*/
void ProcessCommands ()
{
    int readResult = read(STDIN_FILENO, &buffer, BUFFER_MAX);
    if (readResult <= 0)
    {
        fprintf(stderr, "The following error was encountered while attempting to read off of stdin: %s\n", strerror(errno));
        BringDownSensors();
        exit(1);
    }
    
    for (int i=0; i<readResult; i++)
    {
        char c = buffer[i];
        
        /*we are parsing out a potential command*/
        if (commandMode == 0)
        {
            if (isalpha(c) || c == '=' || c == ' ')
            {
                char temp[] = { c };
                strcpy(currentCommand, temp);
                if (strcmp(currentCommand, "SCALE=") || strcmp(currentCommand, "START") || strcmp(currentCommand, "STOP") || strcmp(currentCommand, "PERIOD=") || strcmp(currentCommand, "LOG ") || strcmp(currentCommand, "OFF"))
                {
                    commandMode = 1;        
                }
                    
            }
            else
            {
                 memset(currentCommand, 0, BUFFER_MAX);
            }
        }
        /*we are parsing out the value of a known command*/
        else if (commandMode == 1)
        {
            /*execute the command*/
            if (c == '\n')
            {
                /*execute SCALE command*/
                if (strcmp(currentCommand, "SCALE="))
                {
                    if (strcmp(commandValue, "C"))
                    {
                        scale = 1;
                    }
                    else if (strcmp(commandValue, "F"))
                    {
                        scale = 0;
                    }
                }
                /*execute START command*/
                else if (strcmp(currentCommand, "START"))
                {
                    if (strlen(commandValue) == 0)
                    {
                        shouldSample = 1;
                    }
                }
                /*execute STOP command*/
                else if (strcmp(currentCommand, "STOP"))
                {
                    if (strlen(commandValue) == 0)
                    {
                        shouldSample = 0;
                    }
                }
                /*execute PERIOD command*/
                else if (strcmp(currentCommand, "PERIOD="))
                {
                    int len = strlen(commandValue);
                    int runPeriod = 1;
                    for (int j=0; j<len; j++)
                    {
                        if (!isdigit(commandValue[j]))
                        {
                            runPeriod = 0;
                            break;
                        }
                    }
                    if (runPeriod == 1) 
                    {
                        period = atoi(commandValue);
                    }
                }
                /*execute LOG command*/
                else if (strcmp(currentCommand, "LOG "))
                {
                    if (strlen(commandValue) > 0)
                    {
                        dprintf(logFd, "%s\n", commandValue);
                    }
                }
                /*execute OFF command*/
                else if (strcmp(currentCommand, "OFF"))
                {
                    if (strlen(commandValue) == 0)
                    {
                        shutdownFlag = 1;
                    }
                }
                
                memset(currentCommand, 0, BUFFER_MAX);
                memset(commandValue, 0, BUFFER_MAX);
                commandMode = 0;
            }
            else
            {
                char temp[] = { c };
                strcpy(commandValue, temp);
            }
        }
    }
}

/*main program executable*/
int main (int argc, char** argv)
{
    /*global variables*/
    
    /*define available options for program*/
    static struct option longOptions [] = 
    {
        { "period", required_argument, 0, 'p' },
        { "scale", required_argument, 0, 's' },
        { "log", required_argument, 0, 'l' }
    };
    
    int currentOpt = 0;
    char* tempScale = NULL;
    while ((currentOpt = getopt_long(argc, argv, "p:s:l", longOptions, NULL)) > 0)
    {
        switch (currentOpt)
        {
            /*period indicated*/
            case 'p':
                period = atoi(optarg);
                break;
            /*scale indicated*/
            case 's':
                tempScale = optarg;
                if (strcmp(tempScale, "C"))
                {
                    scale = 1;
                }
                break;
            /*log indicated*/
            case 'l':
                logFile = optarg;
                break;
            default:
                break;
        }
    }
    
    /*create log file for report*/
    if (logFile)
    {
        logFd = creat(logFile, S_IRWXU);
        if (logFd < 0)
        {
            fprintf(stderr, "The following error was encountered while attempting to create the log file: %s\n", strerror(errno));
            exit(1);
        }
    }
    
    /*configure polling*/
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN | POLLHUP | POLLERR;
    int pollReturn = 0;
    buffer = malloc(BUFFER_MAX * sizeof(char));
    currentCommand = malloc(BUFFER_MAX * sizeof(char));
    
    /*register SIGINT handlder*/
    signal(SIGINT, HandleSignal);
    
    /*initialize the sensors*/
    BringUpSensors();
    
    /*execute reading*/
    while (shutdownFlag == 0)
    {
        /*check stdin for commands*/
        pollReturn = poll(fds, 1, 0);
        if ((fds[0].revents & POLLIN))
        {
            /*process command(s) from stdin*/
            ProcessCommands();
        }
        else if ((fds[0].revents & POLLHUP) || (fds[0].revents & POLLERR))
        {
            fprintf(stderr, "The following error was encountered while polling stdin: %s\n", strerror(errno));
            BringDownSensors();
            exit(1);
        }
        
        /*check current time to determine if ready for sampling based on period*/
        time_t rawtime;
        struct tm *timeInfo;
        time(&rawtime);
        timeInfo = localtime(&rawtime);
        int seconds = timeInfo->tm_sec;
        
        /*only sample if enabled*/
        if (shouldSample == 1)
        {
            /*execute temperature sensing if time to sample*/
            if (seconds == sampleTime || sampleTime == -1)
            {
                ReadTemperature();
            }   
        }
        
        /*check button for shutdown*/
        CheckButton();
        
        /*update sample time*/
        sampleTime = (seconds + period) % 60;
    }
    free(buffer);
    free(currentCommand);
    
    /*run shutdown sequence*/
    Shutdown();
    
    exit(0);
}