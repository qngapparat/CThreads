#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "error_handler.h"
#include <unistd.h>
#include <stdbool.h>

std::queue<unsigned> myQueue;
//spinlock instead of mutex:
pthread_spinlock_t spinlock;
bool producer_finished = false;


void* thread_function(void* arg){

    //since 0 is used as termination signal
    unsigned queue_entry = -1;
    int local_sum = 0;

    //NOTE Wait until the producer is finished to avoid the threads consume faster than the producer can push, thus leading to premature thread exits.
    while(!producer_finished);

    while(!myQueue.empty()){

        //wait for mutex
        pthread_spin_lock(&spinlock);
        queue_entry = myQueue.front();
        myQueue.pop();
        pthread_spin_unlock(&spinlock);

        if(queue_entry != 0){
            local_sum += queue_entry;
        }

        else{
            printf("Thread #%d. Sum: %d\n", (int)pthread_self(), local_sum);
            break;
        }
    }

    pthread_exit((void*)NULL);
}



int main(int argc, char const *argv[]) {

    const int MAX_THREADS = 4;
    pthread_t thread_array[MAX_THREADS];

    //init spinlock
    pthread_spin_init(&spinlock, 0);

    //start 4 threads
    for(int i = 0; i < MAX_THREADS; i++){
        pthread_create(&thread_array[i], NULL, &thread_function, NULL);
    }

    for(int i = 0; i < 100000; i++){
        myQueue.push(1u);
    }

    for( int i = 0; i < 4; i++){
        myQueue.push(0u);
    }

    //signal threads to start consuming. Not very parallel, but very safe in this case.
    producer_finished = true;

    //join threads
    for(int i = 0; i < MAX_THREADS; i++){
        //NULL means return value is discarded
        pthread_join(thread_array[i], NULL);
    }

    return EXIT_SUCCESS;
}
