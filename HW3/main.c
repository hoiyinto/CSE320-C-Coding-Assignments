#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>

int main(int argc, char** argv) { 

  //check arguments
  if(argc<2)
  {
  	printf("ERROR\n");
  	exit(1);
  }	
 
  //read file
  char* filename = *(argv + 1);
  FILE *file;
  file = fopen(filename, "r");

  if(file==NULL){
  	printf("FILE NOT FOUND\n");
  	exit(1);
  }
  else{
    char* command=malloc(sizeof(char)*255);
    *command=' ';

    while(strcmp(command,"quit")!=0)
    {
      printf("solver for the application 1\ntrace for the application 2\nfib for the application 3\nquit to exit application\nchange to change input file name\n\nPlease enter your option: ");
      scanf("%s", command);

      if(strcmp(command,"solver")==0)
      {
        pid_t pid;
        pid=fork();

        if(pid==0){
          char *envp[] = { NULL };
          char *argv[] = { "./solver", filename, NULL };
          execve("./solver",argv,envp);
          exit(0);
        }
        else if(pid<0)
        {
          perror("Error: ");
          exit(1);
        }
        else
        {
            wait(NULL);
        }
      }
      else if(strcmp(command,"trace")==0)
      {
        pid_t pid;
        pid=fork();

        if(pid==0){
          char *envp[] = { NULL };
          char *argv[] = { "./trace", filename, NULL };
          execve("./trace",argv,envp);
          exit(0);
        }
        else if(pid<0)
        {
          perror("Error: ");
          exit(1);
        }
        else
        {
            wait(NULL);
        }
      }
      else if(strcmp(command,"fib")==0)
      {
        pid_t pid;
        pid=fork();

        if(pid==0){
          char *envp[] = { NULL };

          char* n=malloc(sizeof(char)*255);
          fscanf((FILE*)file,"%s",n);
          file = fopen(filename, "r");
          
          char *argv[] = { "./fib",n, NULL };
          execve("./fib",argv,envp);
          free(n);
          exit(0);
        }
        else if(pid<0)
        {
          perror("Error: ");
          exit(1);
        }
        else
        {
            wait(NULL);
        }
      }
      else if(strcmp(command,"quit")==0)
      {

      }
      else if(strcmp(command,"change")==0)
      {
        printf("Please enter the file name: ");
        char* filename2=malloc(sizeof(char)*255);
        scanf("%s", filename2);
        file = fopen(filename2, "r");

        if(file==NULL){
            printf("FILE NOT FOUND! Please try again!\n");
            file = fopen(filename, "r");
            free(filename2);
        }
        else
        {
          strcpy(filename,filename2);
          free(filename2);
        }
      }
      else
      {
        printf("Invalid input! Please try again!\n");
      }
    }
    fclose(file);
    free(command);
    exit(0);
  }
}