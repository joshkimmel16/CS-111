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
#include "SortedList.h"

extern int errno;
int opt_yield = 0;
#define BIL 1000000000L
#define KEY_LEN 16
#define NUM_ALPHABETS 26
int numThreads = 1;
int iterations = 1;
int listLength = 0;
char* syncArg = NULL;
char* yieldArg = NULL;
pthread_mutex_t mutex;
int testSet = 0;
long lockWait = 0;
long* threadWaits = NULL;
int numLists = 1;

SortedList_t **lists;
SortedListElement_t* elements;

/*parse provided yield modes appropriately*/
void ParseYieldInput(char* input)
{
	const char accepted[] = { 'i','d','l'};
	for (int i = 0; *(input + i) != '\0'; i++)
	{
		if(*(input + i) == accepted[0])
		{
			opt_yield |= INSERT_YIELD;
		}
		else if(*(input + i) == accepted[1])
		{
			opt_yield |= DELETE_YIELD;
		}
		else if(*(input + i) == accepted[2])
		{
			opt_yield |= LOOKUP_YIELD;
		}
		else
		{
			fprintf(stderr, "An invalid yield argument input was provided: %s\n", input);
            exit(1);
		}
	}
}

/*method to randomly populate the initial element array with keys*/
void PopulateElements()
{
	//Seed the random generator
	srand(time(NULL));

	for (int i = 0; i < listLength; i++)
	{
		//generate random key
		int length = rand() % KEY_LEN + 5; //generate between 5 and KEY_LEN
		int letter = rand() % NUM_ALPHABETS;
		char* key = malloc((length + 1) * sizeof(char));
		for (int j = 0; j < length; j++)
		{
			key[j] = 'a' + letter;
			letter = rand() % 26;
		}
		key[length] = '\0';	
		elements[i].key = key;
	}
}

/*method to hash the given index to return a particular sub-list*/
/*got this off the internet - need to cite*/
unsigned long Hash (const char* str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/*method that a given thread will run that calls appropriate list methods*/
void* ExecuteThread(void* threadStartIndex)
{
    long localLockWait = 0;
    struct timespec lockStart, lockEnd;
    
    /*start by inserting all elements in initialized array into the list*/
    for (long i=(long)threadStartIndex; i<listLength; i+=numThreads)
    {
        /*determine sublist to add the element to*/
        unsigned long subIndex = Hash(elements[i].key) % numLists;
        
        /*Mutex lock indicated*/
        if (syncArg && strcmp(syncArg, "m"))
        {
            clock_gettime(CLOCK_MONOTONIC, &lockStart);
            pthread_mutex_lock(&mutex);
            SortedList_insert(lists[subIndex], &elements[i]);
            pthread_mutex_unlock(&mutex);
            clock_gettime(CLOCK_MONOTONIC, &lockEnd);
            localLockWait = BIL * (lockEnd.tv_sec - lockStart.tv_sec) + (lockEnd.tv_nsec - lockStart.tv_nsec);
        }
        /*spin lock indicated*/
        else if (syncArg && strcmp(syncArg, "s"))
        {
            clock_gettime(CLOCK_MONOTONIC, &lockStart);
            while(__sync_lock_test_and_set(&testSet, 1));
            SortedList_insert(lists[subIndex], &elements[i]);
            __sync_lock_release(&testSet);
            clock_gettime(CLOCK_MONOTONIC, &lockEnd);
            localLockWait = BIL * (lockEnd.tv_sec - lockStart.tv_sec) + (lockEnd.tv_nsec - lockStart.tv_nsec);
        }
        /*no locks*/
        else
        {
            SortedList_insert(lists[subIndex], &elements[i]);
        }
    }
    
    /*finally, lookup and delete all of the elements that we just added*/
    for (long j=(long)threadStartIndex; j<listLength; j+=numThreads)
    {
        /*determine sublist to add the element to*/
        unsigned long subIndex = Hash(elements[j].key) % numLists;
        
        /*Mutex lock indicated*/
        if (syncArg && strcmp(syncArg, "m"))
        {
            clock_gettime(CLOCK_MONOTONIC, &lockStart);
            pthread_mutex_lock(&mutex);
            SortedList_lookup(lists[subIndex], elements[j].key);
            SortedList_delete(&elements[j]);
            pthread_mutex_unlock(&mutex);
            clock_gettime(CLOCK_MONOTONIC, &lockEnd);
            localLockWait = BIL * (lockEnd.tv_sec - lockStart.tv_sec) + (lockEnd.tv_nsec - lockStart.tv_nsec);
        }
        /*spin lock with 1*/
        else if (syncArg && strcmp(syncArg, "s"))
        {
            clock_gettime(CLOCK_MONOTONIC, &lockStart);
            while(__sync_lock_test_and_set(&testSet, 1));
            SortedList_lookup(lists[subIndex], elements[j].key);
            SortedList_delete(&elements[j]);
            __sync_lock_release(&testSet);
            clock_gettime(CLOCK_MONOTONIC, &lockEnd);
            localLockWait = BIL * (lockEnd.tv_sec - lockStart.tv_sec) + (lockEnd.tv_nsec - lockStart.tv_nsec);
        }
        /*no locks*/
        else
        {
            SortedList_lookup(lists[subIndex], elements[j].key);
            SortedList_delete(&elements[j]);
        }
    }
    
    threadWaits[(long)threadStartIndex] = localLockWait;
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
        { "yield", required_argument, 0, 'y' },
        { "sync", required_argument, 0, 's' },
        { "lists", required_argument, 0, 'l' }
    };
    char* testName = malloc(100 * sizeof(char));
    strcpy(testName, "list-");
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
                yieldArg = optarg;
                ParseYieldInput(optarg);
                break;
            /*sync indicated*/
            case 's':
                syncArg = optarg;
                break;
            /*lists indicated*/
            case 'l':
                numLists = atoi(optarg);
                break;
            default:
                break;
        }
    }
    if (opt_yield)
    {
        strcat(testName, yieldArg);
        strcat(testName, "-");
    }
    if (syncArg)
    {
        strcat(testName, syncArg);
    }
    else
    {
        strcat(testName, "none");
    }
    
    /*initialize list based on inputs provided*/
    listLength = iterations * numThreads;
    lists = malloc(numLists * sizeof(SortedList_t));
    for (int a=0; a<numLists; a++)
    {
        lists[a] = malloc(sizeof(SortedList_t));
        lists[a]->key = NULL;
	    lists[a]->next = lists[a];
	    lists[a]->prev = lists[a];
    }
    elements = malloc(listLength * sizeof(SortedListElement_t));
    PopulateElements();
    
    /*conditionally initialize the mutex*/ 
    if (syncArg && strcmp(syncArg, "m"))
    { 
        pthread_mutex_init(&mutex, NULL); 
    }
    
    /*initialize threads and capture start time*/
    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    threadWaits = malloc(numThreads * sizeof(long));
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) { 
        fprintf(stderr, "The following error was encountered while trying to retrieve the start clock time: %s\n", strerror(errno));
        exit(1); 
    }
    
    /*loop through number of threads creating each*/
    for (int i=0; i<numThreads; i++)
    {
        int status = pthread_create(&threads[i], NULL, ExecuteThread, (void*)(long)i);
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
    
    /*check the list one more time for length*/
    /*check each individual sublist*/
    int length = 0;
    for (int b=0; b<numLists; b++)
    {
        length += SortedList_length(lists[b]);
    }
    
    /*write to stdout results of the test*/
    int operations = 3 * numThreads * iterations;
    long totalTime = BIL * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);
    long averageTime = totalTime / operations;
    for (int k=0; k<numThreads; k++) { lockWait += threadWaits[k]; }
    long averageLockWait = lockWait / operations;
    fprintf(stdout, "%s,%d,%d,%d,%d,%ld,%ld,%ld,%d\n", testName, numThreads, iterations, numLists, operations, totalTime, averageTime, averageLockWait, length);
    
    exit(0);
}