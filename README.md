# CThreads
Practicing basic mutex functionality of POSIX threads to coordinate non-thread-safe operations in C. Task 2 includes usage examples of ```pthread_mutex```, ```pthread_spin``` and ```pthread_cond```.

#### Usage

$ ```bash measureTime.sh``` compiles all three variants of Task 2 (```mutex, spinlock, cond```), runs them and writes the runtime results obtained with ```time``` to ```times.txt```.

$ ```make task1``` compiles the Task 1 program.


#### Runtime and performance analysis
The following values are generated using BASH's build-in ```time``` (simplified version of ```/usr/bin/time```) with the syntax ```{ time ./prog.out;} 2>> runtime_log.txt``` (```time``` prints to ```stderr```, which has to be redirected with ```2>>```).

##### Using pthread_mutex:
Unlike spinlocks, mutexes don't use busy waiting, thus reducing CPU usage, which is especially important for longer waiting periods. Thread execution is stopped temporarily, until the thread is woken up when the mutex is available again. Mutexes are arguably slightly more secure than Spinlocks, which can in some cases enter undefined states.

##### Using pthread_spinlock:
A spinlock has various advantages compared to traditional mutexes. First of, the waiting threads don't enter a sleeping state or have to be woken up by a signal, but instead actively try to enter the code segment. This uses CPU resources, but it is faster in many cases (compare log below), expecially when mutex sections are small and/or accessed very often (in this case by a ```i = 0 -> i < 100000``` loop in every thread).

##### Using pthread_cond_var:
A conditional variable works similar to a semaphore, in that it allows threads to efficiently coordinate critical section timing and many producer-consumer problems, without relying on spinlocks.

However, it is necessary to use a while-loop as follows:

    ```while(!RESUME) {
        pthread_cond_wait(&queueCond, &queueMutex);
    }
```
This is because it is possible that ```pthread_cond_wait``` can experience random "spurious wakeups", which would otherwise lead to the execution being resumed without necessarily ```!RESUME``` being ```true```.  [[1](https://stackoverflow.com/documentation/pthreads/8614/conditional-variables#t=201706051640532619188)]

Surprisingly, this method yielded the longest execution time of the three by far. This might be due to the short length of the critical section, or the repeated access in the loop. Spinlocks, and even mutexes, requiring writes, seem to perform far better in this case. However, conditional variables elegantly solve the producer-consumer problem in this case, which makes it a trade off for speed.


#### Runtime measurements using time
```>>>bash measureTime.sh


Task 2 (using pthread_mutex):

real	0m0.021s
user	0m0.028s
sys	0m0.012s

Task 3 (using pthread_spinlock):

real	0m0.020s
user	0m0.032s
sys	0m0.000s

Task 3 (using pthread_cond):

real	0m0.108s
user	0m0.080s
sys	0m0.220s
```
