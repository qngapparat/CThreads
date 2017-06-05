#!/bin/bash

make task2 || echo "failed to compile task 2"
make task3 || echo "failed to compile task 3"


#using -e to enable interpretation of \n
echo "Task 2 (using pthread_mutex):" > times.txt
{ time ./task2.out;} 2>> times.txt
echo -e "\nTask 3 (using pthread_spinlock):" >> times.txt
{ time ./task3spin.out;} 2>> times.txt
echo -e "\nTask 3 (using pthread_cond):" >> times.txt
{ time ./task3convar.out;} 2>> times.txt
