#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "error_handler.h"
#include <unistd.h>
#include <stdbool.h>

std::queue<unsigned> myQueue;
//spinlock instead of mutex:
pthread_cond_t cond;
pthread_mutex_t mutex;


void* thread_function(void* arg){

    //since 0 is used as termination signal
    unsigned queue_entry = -1;
    int local_sum = 0;


    while(1){

        //set mutex to the required position for the following pthread_cond_wait.
        pthread_mutex_lock(&mutex);

        //effectively not a spinlock, since pthread_cond_wait sleeps until signal
        while(myQueue.empty()){
            //unlocks mutex , locks it again when signal comes in (new data available) and then proceeds
            pthread_cond_wait(&cond, &mutex);
        }

        //there must be new data in the queue now

        queue_entry = myQueue.front();
        myQueue.pop();

        //unlock mutex again for producer locked by the returning pthread_cond_wait
        pthread_mutex_unlock(&mutex);

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
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    //start 4 threads
    for(int i = 0; i < MAX_THREADS; i++){
        pthread_create(&thread_array[i], NULL, &thread_function, NULL);
    }

    for(int i = 0; i < 100000; i++){


        pthread_mutex_lock(&mutex);
        myQueue.push(1u);
        //signal that data is available
        pthread_cond_signal(&cond);
        //unlock condvar mutex
        pthread_mutex_unlock(&mutex);
    }

    for( int i = 0; i < 4; i++){

        pthread_mutex_lock(&mutex);
        myQueue.push(0u);
        //signal that data is available
        pthread_cond_signal(&cond);
        //unlock condvar mutex
        pthread_mutex_unlock(&mutex);    }

    //join threads
    for(int i = 0; i < MAX_THREADS; i++){
        //NULL means return value is discarded
        pthread_join(thread_array[i], NULL);
    }

    return EXIT_SUCCESS;
}
