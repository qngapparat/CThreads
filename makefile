task1: create_threads.c
	gcc create_threads.c -o task1.out -pthread

task2: thread_and_mutex.cpp
	g++ thread_and_mutex.cpp -o task2.out -pthread

task3: thread_and_mutex_wspinlock.cpp
	g++ thread_and_mutex.cpp -o task3spin.out -pthread
