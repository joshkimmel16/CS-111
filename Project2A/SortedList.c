#include "SortedList.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

/*implementation for insert method*/
void SortedList_insert(SortedList_t *list, SortedListElement_t *element)
{
	/*should already have an initialized list and element*/
    if (list == NULL || element == NULL) 
    { 
        return; 
    }
	SortedListElement_t *curr = list->next;
	
    /*loop through list, ensuring we don't end up back at the first item (i.e. loop forever)*/
	while (curr != list)
	{
		/*looking for location to place the new item*/
		if (strcmp(element->key, curr->key) <= 0)
        {
            break;   
        }
        
        /*didn't find it, so move on*/
		curr  = curr->next;
	}
    
    /*conditionally yield at this point*/
	if (opt_yield & INSERT_YIELD)
    {
        sched_yield();
    }

	/*insert the new element into the list*/
	element->next = curr;
	element->prev = curr->prev;
	curr->prev->next = element;
	curr->prev = element;
}

/*implementation for delete method*/
int SortedList_delete(SortedListElement_t *element)
{
    /*make sure our element exists*/
    if (element == NULL)
    {
        return 1;
    }
    
    /*ensure list is properly formatted*/
    if (element->prev->next == element->next->prev)
    {
        /*conditionally yield at this point*/
        if (opt_yield & DELETE_YIELD)
        {
            sched_yield();
        }
        
        /*perform deletion*/
        element->prev->next = element->next;
        element->next->prev = element->prev;
    }
    else
    {
        return 1;
    }
    
    return 0;
}

/*implementation for lookup method*/
SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key)
{
    /*check for bad inputs*/
    if (list == NULL || key == NULL)
    {
       return NULL; 
    }
    SortedListElement_t *curr = list->next;
    
    /*loop through list, ensuring we don't end up back at the first item (i.e. loop forever)*/
    while (curr != list)
    {
        /*looking for location to place the new item*/
		if (strcmp(key, curr->key) == 0)
        {
            return curr;   
        }
        
        /*conditionally yield at this point*/
        if (opt_yield & LOOKUP_YIELD)
        {
            sched_yield();
        }
        
        /*didn't find it, so move on*/
		curr  = curr->next;
    }
    
    return NULL;
}

/*implementation for length method*/
int SortedList_length(SortedList_t *list)
{
    /*check for bad input*/
    if (list == NULL)
    {
        return -1;
    }
    
    int count = 0;
    SortedListElement_t *curr = list->next;
    while (curr != list)
    {
        count++;
        
        /*conditionally yield at this point*/
        if (opt_yield & LOOKUP_YIELD)
        {
            sched_yield();
        }
        
        curr  = curr->next;
    }
    
    return count;
}