#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>
#include<time.h>

int main(int argc, char** argv) { 

    char inbuf[20];
    int p[2],nbytes;
 
    if (pipe(p) < 0)
        exit(1);

    char* command=malloc(sizeof(char)*20);
    *command=' ';
    char* command2=malloc(sizeof(char)*20);
    *command2=' ';

    while(strcmp(command,"exit")!=0)
    {
      printf("Mode 1:\nstart - will create a child and transition into Mode 2\nexit - will exit this application.\n\nPlease enter your option: ");
      scanf("%s", command);

      if(strcmp(command,"start")==0)
      {
        if (pipe(p) < 0)
        {
          printf("Failed to use pipe!");
          exit(1);     
        }

        pid_t pid;
        pid=fork();
  
        if(pid==0){
          //child
          while ((nbytes = read(p[0], inbuf, 20)) > 0)
          {
                if(strcmp(inbuf,"create")==0)
                {
                  pid_t pid2;
                  pid2=fork();

                  if(pid2==0)
                  {
                    printf("%d\n",getpid());
                    exit(0);
                  }
                   
                }
                else if(strcmp(inbuf,"clean")==0)
                {
                    while(wait(NULL)>0) 
                    {

                    }
                }
                else if(strcmp(inbuf,"date")==0)
                {
                    int day, month, year;
 
                    time_t now;
                    time(&now);
                   
                    struct tm *local = localtime(&now);
                   
                    day = local->tm_mday;     // get day of month (1 to 31)
                    month = local->tm_mon + 1;    // get month of year (0 to 11)
                    year = local->tm_year + 1900; // get year since 1900
                   
                    // print current date
                    printf("Date is : %02d/%02d/%d\n", day, month, year);
                }
                else
                {
		                while(wait(NULL)>0) 
                    {

                    }

                  close(p[1]);
                  exit(0);
                }
          }
        }
        else if(pid<0)
        {
          perror("Error: ");
          exit(1);
        }
        else
        {
          //parent
          while(strcmp(command2,"exit")!=0)
          {
                printf("Mode 2:\n1. create - will send through pipe a command to the created child. That child should create a new child (use fork()). That new child should print its own pid and terminate. The parent should not reap child.\n\n2. clean - will send through pipe a command to the created child. That child should reap all its own children.\n\n3. date - will send though pipe a command to the created child. That child should print current date to the terminal.\n\n4. exit - transition to the Mode 1\n\n\nPlease enter your option: ");
                scanf("%s", command2);

                if(strcmp(command2,"create")==0)
                {
                    write(p[1],command2, 20);
                }
                else if(strcmp(command2,"clean")==0)
                {
                    write(p[1],command2, 20);
                }
                else if(strcmp(command2,"date")==0)
                {
                    write(p[1],command2, 20);
                }
                else if(strcmp(command2,"exit")==0)
                {
                    write(p[1],command2, 20);
                    close(p[1]);
                    wait(NULL);
                }
                else
                {
                  printf("Invalid input! Please try again!\n");
                }
                sleep(1);
          }
          *command2=' ';
        }     
      }
      else if(strcmp(command,"exit")==0)
      {

      }
      else
      {
        printf("Invalid input! Please try again!\n");
      }
    }

    free(command);
    free(command2);
    exit(0);
  }
