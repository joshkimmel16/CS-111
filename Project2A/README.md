# CS 111 - Project 2A - Races and Synchronization

A description of the project can be found in **P2A.html**.

## Known Issues

* Makefile - testList3 doesn't work for certain mutex tests (yield options il, id, dl, idl).
* For list portion, the mutex tests seem quite slow (and some don't return at all - with certain yield options). Possibly there's a bug?

## Questions and Responses

### QUESTION 2.1.1
a) Many iterations are required because the probability of an unfortunate context switch timing that would result in errors is extremely low. Thus, many iterations must be executed to amplify the expected number of failures to a point where it would be reasonable to expect to see one or more.

b) For the same probability argument as above, the extremely low probability of an unfortunate timing would result in a very low expected number of failures given fewer iterations - so much so that it would be unlikely even to see one.

### QUESTION 2.1.2
a) Yield runs are slower because we are forcing context switches that might not otherwise have occurred. This adds overhead to all threads/iterations.

b) The additional time is going to the overhead of the additional (forced) context switches.

c) It is not possible to get valid per-operation timings with yields because the overhead skews the average given our measurement technique (just capturing beginning and end times).

d) See (c) for the explanation - overhead skews average given our measurement technique.

### QUESTION 2.1.3
a) Thread creation generates a relatively large amount of overhead. Higher numbers of iterations distribute this overhead across more computation, causing the average to decrease.

b) By extending the number of iterations to infinity, we can infer the "correct" cost per iteration (maximally distributing the thread creation overhead).

### QUESTION 2.1.4
a) For low numbers of threads, all options perform similarly because the contention for the shared resource is minimal and thus synchronization enhancements do not need to be invoked - thereby adding more overhead.

b) As the number of threads increases, so does the contention for the shared resource. The synchronization enhancements must be invoked more frequently - causing more and more overhead.

### QUESTION 2.2.1
a) Time per mutex-protected operation increased for the linked list relative to the add. This is because the critical section for linked lists is more complex (more instructions) and thus locks are held for longer periods of time - driving the average time to complete the given operation up.

b) As the number of threads increases, both curves increase in time per operation as there is more contention.

c) The rate of increase for the linked list is greater than that of the add function. Again, this is likely due to the fact that the complexity of the linked list operations relative to the add operations is amplified by the increased amount of contention with higher thread counts.

### QUESTION 2.2.2
a) For both mutexes and spin-locks, the time per operation increases as the thread count increases. This is because in either case higher contention rates imply longer wait times for locks of any kind.

b) Mutex operation time tends to increase more rapidly than spin-wait operation time. This is because there is more overhead in the pthread mutex implementation to handle other performance consequences of locking (such as being efficient with CPU). Spin-locks are simple and do not try to deal with these more complex consequences and thus, in the scope of our test, perform better.