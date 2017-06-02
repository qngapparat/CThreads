#include <stdio.h>
#include <stdlib.h>
#include "error_handler.h"

void die(char* msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

void watch(int return_value, char* function_name){
    if(return_value == -1){
        perror(function_name);
        exit(EXIT_FAILURE);
    }

    return;
}
