#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

void* generate_file(void* thread_number_pointer){

    //cast to integer
    int thread_number;
    thread_number = (int) thread_number_pointer;

    //generate file name string
    char file_name[] = "./thread";
    char file_name_appendix[] = (char*)thread_number;

    strcat(file_name, file_name_appendix);
    strcat(file_name, ".txt");

    sleep(rand() % 2);

    int fd = open(file_name, O_RDWR | O_CREAT, 0666);
    
}


int main(int argc, char const *argv[]) {

    pthread_t thread0, thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8, thread9;



    return EXIT_SUCCESS;
}
