#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <queue>

void* thread_function(void* arg){

    unsigned queue_entry;
    int local_sum = 0;

    while(1){

        //wait until it's possible to read form queue
        pthread_mutex_lock(&mutex);
        queue_entry = myQueue.pop();
        pthread_mutex_unlock(&mutex);

        if(queue_entry != 0){
            local_sum += queue_entry;
        }

        else{
            printf("Thread #%d. Sum: %d", (int)pthread_self(), local_sum);
            pthread_exit(NULL);
        }
    }
}



int main(int argc, char const *argv[]) {

    const int MAX_THREADS = 4;
    queue<unsigned> myQueue;
    pthread_t thread_array[MAX_THREADS];

    //start 4 threads
    for(int i = 0; i < MAX_THREADS; i++){
        
        pthread_create(&thread_array[i], NULL, &thread_function, NULL);
    }

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


    return EXIT_SUCCESS;
}
