all:
	gcc -pthread create_threads.c error_handler.c -o task1.out

tmp:
	gcc -pthread tmp.c -o tmp.out
