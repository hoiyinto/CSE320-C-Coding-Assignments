#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<sys/wait.h>
#include <stdlib.h>

pid_t pid;

void siginthandler(){
	kill(pid,SIGINT);
}

int main(int argc, char** argv){ 
 	char* command=malloc(sizeof(char)*255);
    *command=' ';

    printf("You may type 'help' to see the list of commands\n");

    while(strcmp(command,"exit\n")!=0)
    {
      fgets(command,255,stdin);

      if(strncmp(command,"run",3)==0)
      {

      	signal(SIGINT, siginthandler);

        pid=fork();

        if(pid==0){
          char* application;
          char *argv[12]; 

          const char s[2] = " \n";
   		  char *token;
   
   		  /* get the first token */
   		  token = strtok(command, s);
            
           int i=0;
		   /* walk through other tokens */
		   while( token != NULL ) {
		      token = strtok(NULL, s);
		      argv[i]=token;
		      if(i==0)
		      {
		      	application=token;
		      }
		      i++;
		  }
		 
          if(execvp(application,argv)<0)
          {
          	char* helper=malloc(3);
          	strcpy(helper,"./");
          	strcat(helper, application);

          	if(execvp(helper,argv)<0)
          	{
          		printf("Failed to execute appication X!\n");
      		}
      		free(helper);	
          }
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
            signal(SIGINT, SIG_DFL);
        }
      }
      else if(strcmp(command,"help\n")==0)
	  {
	  	printf("Type 'run X' to execute application X\nType 'exit' to exit application\n\nPlease enter your option: ");
	  }
	  else if(strcmp(command,"exit\n")==0)
	  {

	  }
	  else
      {
        printf("Invalid input! Please try again!\n");
      }
    }
 
      free(command);
      exit(0);
}

