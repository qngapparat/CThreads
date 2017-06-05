all:
	gcc -pthread create_threads.c error_handler.c -o task1.out
	g++ thread_and_mutex.cpp -pthread
