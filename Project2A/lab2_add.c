#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

extern int errno;
#define BIL 1000000000L
int iterations = 1;
int opt_yield = 0;
char* syncArg = NULL;
pthread_mutex_t mutex;
int testSet = 0;

/*method that will be tested*/
void add(long long *pointer, long long value) 
{
    long long sum = *pointer + value;
    if (opt_yield)
    {
        sched_yield();   
    }
    *pointer = sum;
}

/*special method to implement compare and swap spin lock*/
void AddCAndS(long long *pointer, long long value)
{
	long long prev;
	long long sum;
	do
	{
		prev = *pointer;
		sum = prev + value;
		if (opt_yield) { sched_yield(); }
	} while(__sync_val_compare_and_swap(pointer, prev, sum) != prev);
}

/*method that a given thread will run that calls add*/
void* ExecuteThread(void* counter)
{
    /*loop through adding 1*/
    for (int i=0; i<iterations; i++)
    {
        /*Mutex lock indicated*/
        if (syncArg && strcmp(syncArg, "m"))
        {
            pthread_mutex_lock(&mutex);
            add((long long*)counter, 1);
            pthread_mutex_unlock(&mutex);
        }
        /*spin lock with 1*/
        else if (syncArg && strcmp(syncArg, "s"))
        {
            while(__sync_lock_test_and_set(&testSet, 1));
            add((long long*)counter, 1);
            __sync_lock_release(&testSet);
        }
        /*spin lock with 2*/
        else if (syncArg && strcmp(syncArg, "c"))
        {
            AddCAndS((long long*)counter, 1);
        }
        /*no locks*/
        else
        {
            add(counter, 1);
        }
    }
    
    /*loop through subtracting 1*/
    for (int j=0; j<iterations; j++)
    {
        /*Mutex lock indicated*/
        if (syncArg && strcmp(syncArg, "m"))
        {
            pthread_mutex_lock(&mutex);
            add((long long*)counter, -1);
            pthread_mutex_unlock(&mutex);
        }
        /*spin lock with 1*/
        else if (syncArg && strcmp(syncArg, "s"))
        {
            while(__sync_lock_test_and_set(&testSet, 1));
            add((long long*)counter, -1);
            __sync_lock_release(&testSet);
        }
        /*spin lock with 2*/
        else if (syncArg && strcmp(syncArg, "c"))
        {
            AddCAndS((long long*)counter, -1);
        }
        /*no locks*/
        else
        {
            add((long long*)counter, -1);
        }
    }
    
    return NULL;
}

/*main program executable*/
int main (int argc, char** argv)
{
    /*global variables*/
    struct timespec start, end;
    
    /*define available options for program*/
    static struct option longOptions [] = 
    {
        { "threads", required_argument, 0, 't' },
        { "iterations", required_argument, 0, 'i' },
        { "yield", no_argument, 0, 'y' },
        { "sync", required_argument, 0, 's' }
    };
    char* testName = malloc(100 * sizeof(char));
    strcpy(testName, "add-");
    int numThreads = 1;
    int currentOpt = 0;
    while ((currentOpt = getopt_long(argc, argv, "t:iys", longOptions, NULL)) > 0)
    {
        switch (currentOpt)
        {
            /*threads indicated*/
            case 't':
                numThreads = atoi(optarg);
                break;
            /*iterations indicated*/
            case 'i':
                iterations = atoi(optarg);
                break;
            /*yield indicated*/
            case 'y':
                opt_yield = 1;
                break;
            /*sync indicated*/
            case 's':
                syncArg = optarg;
                break;
            default:
                break;
        }
    }
    if (opt_yield)
    {
        strcat(testName, "yield-");
    }
    if (syncArg)
    {
        strcat(testName, syncArg);
    }
    else
    {
        strcat(testName, "none");
    }
    
    /*conditionally initialize the mutex*/ 
    if (syncArg && strcmp(syncArg, "m"))
    { 
        pthread_mutex_init(&mutex, NULL); 
    }
    
    /*initialize test variables and note start time*/
    long long counter = 0;
    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) { 
        fprintf(stderr, "The following error was encountered while trying to retrieve the start clock time: %s\n", strerror(errno));
        exit(1); 
    }
    
    /*loop through number of threads creating each*/
    for (int i=0; i<numThreads; i++)
    {
        int status = pthread_create(&threads[i], NULL, ExecuteThread, &counter);
        if (status < 0)
        {
            fprintf(stderr, "The following error was encountered while trying to start the thread at index %d: %s\n", i, strerror(errno));
            exit(1);
        }
    }
    for (int j=0; j<numThreads; j++)
    {
        int status = pthread_join(threads[j], NULL);
        if (status < 0)
        {
            fprintf(stderr, "The following error was encountered while trying to merge the thread at index %d back into the main thread: %s\n", j, strerror(errno));
            exit(1);
        }
    }
    
    /*capture ending time*/
    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) 
    { 
        fprintf(stderr, "The following error was encountered while trying to retrieve the end clock time: %s\n", strerror(errno)); 
        exit(1);
    }
    
    /*write to stdout results of the test*/
    int numOperations = 2 * iterations * numThreads;
    long totalTime = BIL * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);
    long averageTime = totalTime / numOperations;
    fprintf(stdout, "%s,%d,%d,%d,%ld,%ld,%lld\n", testName, numThreads, iterations, numOperations, totalTime, averageTime, counter);
    
    exit(0);
}