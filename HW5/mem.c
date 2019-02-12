#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int valid[1024];
int written[1024];


int main(int argc, char** argv){ 

	void* mem=malloc(1024);

	int fd1;
    // FIFO file path
    char * myfifo = "/tmp/myfifo";
 
    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);
 
    char str1[225], str2[225];
    
    while (1)
    {
        // First open in read only and read
        fd1 = open(myfifo,O_RDONLY);
        read(fd1, str1, 225);
 
        // Print the read string and close
        //printf("User1: %s\n", str1);
        close(fd1);

        sleep(5);

        if(strncmp(str1,"allocate",8)==0)
        {
          char* input;

          const char s[3] = " \n\0";
   		  char *token;
   
   		  /* get the first token */
   		  token = strtok(str1, s);
            
           int i=0;
		   /* walk through other tokens */
		   while( token != NULL ) {
		      token = strtok(NULL, s);
		      if(i==0)
		      {
		      	input=token;
		      }
		      i++;
		  }

		  int num=atoi(input);
		  
		  if(num==0)
		  {
		  	int counter=0;
		  	int done=0;
		  	while(counter<=255&&done==0)
		  	{
		  		if(valid[counter]==0)
		  		{
		  			fflush(stdout);
		  			valid[counter]=1;
    				sprintf(str2, "%d", counter);
		  			done=1;
		  		}
		  		counter=counter+4;
		  	}
		  }
		  else if(num==1)
		  {
		  	int counter=256;
		  	int done=0;
		  	while(counter<=511&&done==0)
		  	{
		  		if(valid[counter]==0)
		  		{
		  			fflush(stdout);
		  			valid[counter]=1;
    				sprintf(str2, "%d", counter);
		  			done=1;
		  		}
		  		counter=counter+4;
		  	}
		  }
		  else if(num==2)
		  {
		  	int counter=512;
		  	int done=0;
		  	while(counter<=767&&done==0)
		  	{
		  		if(valid[counter]==0)
		  		{
		  			fflush(stdout);
		  			valid[counter]=1;
    				sprintf(str2, "%d", counter);
		  			done=1;
		  		}
		  		counter=counter+4;
		  	}
		  }
		  else
		  {
		  	int counter=768;
		  	int done=0;
		  	while(counter<=1023&&done==0)
		  	{
		  		if(valid[counter]==0)
		  		{
		  			fflush(stdout);
		  			valid[counter]=1;
    				sprintf(str2, "%d", counter);
		  			done=1;
		  		}
		  		counter=counter+4;
		  	}
		  }  
        }
        else if (strncmp(str1,"read",4)==0)
        {
 	      char* input;

          const char s[3] = ",\n\0";
   		  char *token;
   
   		  /* get the first token */
   		  token = strtok(str1, s);
            
           int i=0;
		   /* walk through other tokens */
		   while( token != NULL ) {
		      token = strtok(NULL, s);
		      if(i==0)
		      {
		      	input=token;
		      }
		      i++;
		  }		  

		  int pa=atoi(input);

		    if(pa<0||pa>1023)
			{
				strcpy(str2, "error,address out of range");				
			}  
			else if(pa%4!=0)
			{
				strcpy(str2, "error,address is not aligned");	
			}
			else
			{
				  if(written[pa]==0||valid[pa]==0)
				  {
				  	//error
				  	strcpy(str2, "error");//failure
				  }
				  else
				  {
				  	void* ptr=mem;
				  	int* found=(int*)(ptr+pa);
				  	sprintf(str2, "%d", *found);
				  }
        	}
        }
        else if (strncmp(str1,"write",5)==0)
        {
        	char* input;
        	char* input2;

	          const char s[3] = ",\n\0";
	   		  char *token;
	   
	   		  /* get the first token */
	   		  token = strtok(str1, s);
	            
	           int i=0;
			   /* walk through other tokens */
			   while( token != NULL ) {
			      token = strtok(NULL, s);
			      if(i==0)
			      {
			      	input=token;
			      }
			      else if(i==1)
			      {
			      	input2=token;
			      }
			      i++;
			  }

			  int pa=atoi(input);
			  int value=atoi(input2);

			if(pa<0||pa>1023)
			{
				strcpy(str2, "error,address out of range");				
			}  
			else if(pa%4!=0)
			{
				strcpy(str2, "error,address is not aligned");	
			}
			else
			{
				if(valid[pa]==0)
			  	{
			  		//error
			  		//mem havent benn allocated yet
			  		strcpy(str2, "error,memory has not been allocated yet");//failure	
			  	}
			  	else
			  	{
			  		void* ptr=mem;
				  	int* found=(int*)(ptr+pa);
				  	*found=value;
				  	written[pa]=1;
				  	sprintf(str2, "%d", 1); //on success
			  	}	
		    }
        }
        else if(strncmp(str1,"clean",5)==0)
		{

		  char* input;

          const char s[3] = " \n\0";
   		  char *token;
   
   		  /* get the first token */
   		  token = strtok(str1, s);
            
           int i=0;
		   /* walk through other tokens */
		   while( token != NULL ) {
		      token = strtok(NULL, s);
		      if(i==0)
		      {
		      	input=token;
		      }
		      i++;
		  }

		  int num=atoi(input);
		  
		  if(num==0)
		  {
		  	int counter=0;
		  	while(counter<=255)
		  	{
		  		if(valid[counter]!=0)
		  		{
		  			valid[counter]=0;
		  			if(written[counter]!=0)
		  				written[counter]==0;
		  		}
		  		counter=counter+4;
		  	}
		  }
		  else if(num==1)
		  {
		  	int counter=256;
		  	while(counter<=511)
		  	{
		  		if(valid[counter]!=0)
		  		{
		  			valid[counter]=0;
		  			if(written[counter]!=0)
		  				written[counter]==0;
		  		}
		  		counter=counter+4;
		  	}
		  }
		  else if(num==2)
		  {
		  	int counter=512;
		  	while(counter<=767)
		  	{
		  		if(valid[counter]!=0)
		  		{
		  			valid[counter]=0;
		  			if(written[counter]!=0)
		  				written[counter]==0;
		  		}
		  		counter=counter+4;
		  	}
		  }
		  else
		  {
		  	int counter=768;
		  	while(counter<=1023)
		  	{
		  		if(valid[counter]!=0)
		  		{
		  			valid[counter]=0;
		  			if(written[counter]!=0)
		  				written[counter]==0;
		  		}
		  		counter=counter+4;
		  	}
		  }  
		  sprintf(str2, "%d", 1); //on success
		}
        else
        {
        	//error
			 strcpy(str2, "error,invalid command");//

        }
 		//printf("%s\n",str2);
        // Now open in write mode and write
        // string taken from user.
        fd1 = open(myfifo,O_WRONLY);
        write(fd1, str2, strlen(str2)+1);
        close(fd1);
    }

    return 0;
}
