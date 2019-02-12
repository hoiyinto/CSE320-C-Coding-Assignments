#ifndef LIBS
#define LIBS

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/wait.h>

#endif

//PART 2

/*It allocates the requested memory and returns a pointer to it*/
/*size is the size of the memory block (in bytes)*/
void* cse320_malloc(ssize_t size);

/*It deallocates the memory previously allocated by a call to cse320_malloc()*/
/*ptr is a pointer to a memory block previously allocated with cse320_malloc()*/
void cse320_free(void* ptr);

/*It replicates the fopen() function, however when you try to open the same file, it will return the same file pointer instead*/
FILE* cse320_fopen(const char *filename, const char *mode);

/*The file is not closed until no pointer is pointing to this file*/
void cse320_fclose(FILE *stream);

/*You must call cse320_clean() before exiting the application to avoid memory leak*/
void cse320_clean();

//PART 3

/*You must first set up a timer before you call cse320_fork()*/
/*This timer is set to help you reap all children evert T seconds*/
void cse320_settimer(int t);

/*This is just a helper method to reap children*/
/*You should not be using this function*/
void cse320_reap();

/*This cse320_fork() function simulates the GNU version of fork but you will not have to worry about reaping children*/
pid_t cse320_fork();
