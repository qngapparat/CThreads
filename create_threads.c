#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include "error_handler.c"
#include <sys/types.h>

const int MAX_THREADS = 10;

void cleanup(void* arg){

    //TODO
    FILE* f = (FILE*) arg;

    if((fclose(f) == EOF)){
        perror("fclose");
        exit(EXIT_FAILURE);
    }

    return;
}


void* generate_file(void* thread_number_pointer){

    //cast to integer
    int thread_number;
    thread_number = (int) thread_number_pointer;

    //generate file name string
    char file_name[] = "./thread";
    char file_name_appendix[] = (char*)thread_number;

    strcat(file_name, file_name_appendix);
    strcat(file_name, ".txt");

    FILE* f = fopen(file_name, "w");

    //create and push cancellation handler function to threads' handler stack
    pthread_cleanup_push(cleanup, f);
    //disable all cancellation handler blockers

    sleep(rand() % 2);

    fprintf(f, "%d", (int)gettid());

    //exit thread after calling cancellation handler
    pthread_exit(gettid());

}


int main(int argc, char const *argv[]) {

    srand(time(NULL));

    pthread_t thread_id_array[MAX_THREADS];
    int thread_return_array[MAX_THREADS];

    //create 10 threads starting at function generate_file(i).
    for(int i = 0; i < MAX_THREADS; i++){
        watch( pthread_create(&thread_id_array[i], NULL, &generate_file, (void*) i) , "pthread_create");
    }

    //cancel every thread with 50% probability
    for(int i = 0; i < MAX_THREADS; i++){
        if(rand()%2 == 1){
            pthread_cancel(thread_id_array[i]);
        }
    }

    //wait for threads to join, save return values in array
    for(int i = 0; i < MAX_THREADS; i++){
        pthread_join(&thread_id_array[i], (void**) &thread_return_array[i]);
        if(thread_id_array[i] <= 0){
            printf("Thread #%d (%d) cancelled.\n", i, thread_id_array[i]);
        }

        else{
            printf("Thread #%d (%d) exited correctly.\n", i, thread_id_array[i]);
        }
    }

    return EXIT_SUCCESS;
}
