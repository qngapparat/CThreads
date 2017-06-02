#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include "error_handler.c"
#include <sys/types.h>

void cleanup(void* arg){

    //TODO
    FILE* f = (FILE*) arg;

    if(fclose(f) == EOF){
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
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    sleep(rand() % 2);

    fprintf(f, "%d", (int)gettid());



}


int main(int argc, char const *argv[]) {

    pthread_t thread0, thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8, thread9;



    return EXIT_SUCCESS;
}
