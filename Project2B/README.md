# CS 111 - Project 2B - Lock Granularity and Performance

A description of the project can be found in **P2B.html**.

## Known Issues

* The mutex tests seem quite slow (and some don't return at all - with certain yield options). Possibly there's a bug?

## Questions and Responses

### QUESTION 2.3.1
a) CPU time in the 1 and 2 thread tests is likely spent traversing the list for various add, search, and remove operations.

b) As adds accumulate, the list grows. All of these operations require some sort of traversal of linked list and this traversal gets longer linearly with the list size. Given that there is not much overhead otherwise, the traversal is likely taking up most of the CPU's time.

c) For the high thread spin-lock tests, likely most of the CPU time is being spent spinning while waiting for a lock.

d) For the high thread mutex tests, most of the CPU time is likely spent in locking operations (acquiring, checking, releasing, waking up, etc.).

### QUESTION 2.3.2
a) The line of code that grabs/waits for the lock consumes the most CPU time.

b) This operation becomes expensive with many threads because there is high contention for the lock and OS scheduling is non-deterministic with respect to which thread that is waiting will happen to wake up first when the lock becomes available.

### QUESTION 2.3.3
a) Average lock wait time rises as the number of threads rises because as more threads are waiting for a lock, this implies that certain threads will need to wait even longer as other waiting threads acquire the lock - possibly being starved.

b) Average time per operation rises less dramatically because we are not accounting for the time waiting for the lock - just completing the operation itself.

c) While context switches result in overhead, the overhead is relatively constant per switch. However, lock waiting goes up at an increasing rate as contention (number of threads) increases

### QUESTION 2.3.4
a) As the number of sublists increases, the performance increases as the probability of contention in any given sublist is minimized and thus less overall time is spent waiting for locks to free up.

b) There is obviously a theoretical bound for number of sublists at the total list size - and thus throughput cannot continue to grow indefinitely. But in general performance would start to converge back to non-parallel execution.

c) This is not the case according to the graphs. There are many factors that could generate this result but likely there is just more general overhead with larger lists and thread counts. relative to smaller lists and thread counts