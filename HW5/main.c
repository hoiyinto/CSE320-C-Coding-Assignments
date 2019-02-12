#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

struct cacheBlock
{
	int valid;
	int pa;
	int value;
};

struct cacheBlock cache[4];

struct thread
{
	pthread_t id;
	int valid;
};

struct firstTable{
	int valid[8];
	struct secondTable* ptr[8];
};

struct secondTable{
	int valid[8];
	int pa[8];
};

//THREADS
struct thread threadTable[4];
int numOfThread=0;
int currentThread=0; //index

//EACH THREAD CONTAINS
struct firstTable* ft1;
int numOfST1=0; 
struct secondTable* st1[8];
unsigned int VA1[64]; 
int numOfVA1;

struct firstTable* ft2;
int numOfST2=0;
struct secondTable* st2[8];
unsigned int VA2[64]; 
int numOfVA2;

struct firstTable* ft3;
int numOfST3=0;
struct secondTable* st3[8];
unsigned int VA3[64]; 
int numOfVA3;

struct firstTable* ft4;
int numOfST4=0;
struct secondTable* st4[8];
unsigned int VA4[64];
int numOfVA4;

int fd;
// FIFO file path
char * myfifo = "/tmp/myfifo";


void init()
{
	ft1=malloc(sizeof(struct firstTable));
	st1[0]=malloc(sizeof(struct secondTable));
	ft1->ptr[0]=st1[0];
	ft1->valid[0]=1;

	ft2=malloc(sizeof(struct firstTable));
	st2[0]=malloc(sizeof(struct secondTable));
	ft2->ptr[0]=st2[0];
	ft2->valid[0]=1;

	ft3=malloc(sizeof(struct firstTable));
	st3[0]=malloc(sizeof(struct secondTable));
	ft3->ptr[0]=st3[0];
	ft3->valid[0]=1;

	ft4=malloc(sizeof(struct firstTable));
	st4[0]=malloc(sizeof(struct secondTable));
	ft4->ptr[0]=st4[0];
	ft4->valid[0]=1;
}

//from online resources
//sorry I can't find the link :(
unsigned int binaryToDecimal(const char* binstr)
{      
    unsigned int result = 0 ;
    int bit = 0; 
    while( binstr[bit] != '\0' )
    {
        if( binstr[bit] == '1' )
        {
            result |= 1 ;
        }

        bit++ ;
        if( binstr[bit] != '\0' )
        {
            result <<= 1 ;
        }
    }
    return result;
}

//from online resources
//https://www.geeksforgeeks.org/binary-representation-of-a-given-number/
void bin(unsigned int n)
{
	unsigned int i;
	for (i = 1 << 31; i > 0; i = i / 2)
		(n & i)? printf("1"): printf("0");
}

unsigned int cse320_malloc()
{
	char arr2[225];
	if(currentThread==0)
    {
		strcpy(arr2, "allocate 0");
    }
    else if(currentThread==1)
	{
		strcpy(arr2, "allocate 1");
	}
	else if(currentThread==2)
    {
		 strcpy(arr2, "allocate 2"); 	
	}
	else
	{
		strcpy(arr2, "allocate 3");
	}		  	

	char arr1[225];
    // Open FIFO for write only
     fd = open(myfifo, O_WRONLY);
 
     // Write the input arr2ing on FIFO
     // and close it
     write(fd, arr2, strlen(arr2)+1);
     close(fd);
 
     // Open FIFO for Read only
    fd = open(myfifo, O_RDONLY);
 
    // Read from FIFO
    read(fd, arr1, sizeof(arr1));
 
    // Print the read message
    //printf("User2: %s\n", arr1);
    close(fd);

	int num = atoi(arr1);

	int pa=num;//get it from mem.c

	int ftnum=0;
	int stnum=0;

	if(currentThread==0)	
	{
		struct secondTable* ptr=(ft1->ptr)[numOfST1];
		if((ptr->valid)[7]!=0)
		{
			//gotta create a new second table
			numOfST1++;
			st1[numOfST1]=malloc(sizeof(struct secondTable));
			(ft1->valid)[numOfST1]=1;
			(ft1->ptr)[numOfST1]=st1[numOfST1];

			ptr=(ft1->ptr)[numOfST1];
			(ptr->valid)[0]=1;
			(ptr->pa)[0]=pa;

			ftnum=numOfST1;
			stnum=0;

			unsigned int va=(ftnum<<22|stnum<<12);
			VA1[numOfVA1]=va;
			numOfVA1++;
			return va;

		}
		else
		{
			ftnum=numOfST1;
			while((ptr->valid)[stnum]!=0)
			{
				stnum++;
			}
			(ptr->valid)[stnum]=1;
			(ptr->pa)[stnum]=pa;

			unsigned int va=(ftnum<<22|stnum<<12);
			VA1[numOfVA1]=va;
			numOfVA1++;
			return va;
		}
	}
	else if(currentThread==1)
	{
		struct secondTable* ptr=(ft2->ptr[numOfST2]);
		if((ptr->valid[7])!=0)
		{
			//gotta create a new second table
			numOfST2++;
			st2[numOfST2]=malloc(sizeof(struct secondTable));
			(ft2->valid[numOfST2])=1;
			(ft2->ptr[numOfST2])=st2[numOfST2];

			ptr=(ft2->ptr[numOfST2]);
			(ptr->valid[0])=1;
			(ptr->pa[0])=pa;

			ftnum=numOfST2;
			stnum=0;

			unsigned int va=(ftnum<<22|stnum<<12);
			VA2[numOfVA2]=va;
			numOfVA2++;
			return va;

		}
		else
		{
			ftnum=numOfST2;
			while((ptr->valid[stnum])!=0)
			{
				stnum++;
			}
			(ptr->valid[stnum])=1;
			(ptr->pa[stnum])=pa;

			unsigned int va=(ftnum<<22|stnum<<12);
			VA2[numOfVA2]=va;
			numOfVA2++;
			return va;
		}
	}	
	else if(currentThread==2)
	{
		struct secondTable* ptr=(ft3->ptr)[numOfST3];
		if((ptr->valid)[7]!=0)
		{
			//gotta create a new second table
			numOfST3++;
			st3[numOfST3]=malloc(sizeof(struct secondTable));
			(ft3->valid)[numOfST3]=1;
			(ft3->ptr)[numOfST3]=st3[numOfST3];

			ptr=(ft3->ptr)[numOfST3];
			(ptr->valid)[0]=1;
			(ptr->pa)[0]=pa;

			ftnum=numOfST3;
			stnum=0;

			unsigned int va=(ftnum<<22|stnum<<12);
			VA3[numOfVA3]=va;
			numOfVA3++;
			return va;

		}
		else
		{
			ftnum=numOfST3;
			while((ptr->valid)[stnum]!=0)
			{
				stnum++;
			}
			(ptr->valid)[stnum]=1;
			(ptr->pa)[stnum]=pa;

			unsigned int va=(ftnum<<22|stnum<<12);
			VA3[numOfVA3]=va;
			numOfVA3++;
			return va;
		}
	}
	else
	{
		struct secondTable* ptr=(ft4->ptr)[numOfST4];
		if((ptr->valid)[7]!=0)
		{
			//gotta create a new second table
			numOfST4++;
			st4[numOfST4]=malloc(sizeof(struct secondTable));
			(ft4->valid)[numOfST4]=1;
			(ft4->ptr)[numOfST4]=st4[numOfST4];

			ptr=(ft4->ptr)[numOfST4];
			(ptr->valid)[0]=1;
			(ptr->pa)[0]=pa;

			ftnum=numOfST4;
			stnum=0;

			unsigned int va=(ftnum<<22|stnum<<12);
			VA4[numOfVA4]=va;
			numOfVA4++;
			return va;

		}
		else
		{
			ftnum=numOfST4;
			while((ptr->valid)[stnum]!=0)
			{
				stnum++;
			}
			(ptr->valid)[stnum]=1;
			(ptr->pa)[stnum]=pa;

			unsigned int va=(ftnum<<22|stnum<<12);
			VA4[numOfVA4]=va;
			numOfVA4++;
			return va;
		}
	}
}

int cse320_virt_to_phys(unsigned int va)
{
	unsigned int temp1=va;
	unsigned int temp2=va;
	unsigned int ft=temp1>>22;
	unsigned int st=(temp2<<10)>>22;

	if(ft<0||ft>7||st<0||st>7)
	{
		return -1;
	}

	if(currentThread==0)
	{
		if((ft1->valid)[ft]==1)
		{
			struct secondTable* ptr=(ft1->ptr)[ft];
			if((ptr->valid)[st]==1)
			{
				return (ptr->pa)[st];
			}
			else
				return -1;
		}
		else
			return -1;
	}
	else if(currentThread==1)
	{
		if((ft2->valid)[ft]==1)
		{
			struct secondTable* ptr=(ft2->ptr)[ft];
			if((ptr->valid)[st]==1)
			{
				return (ptr->pa)[st];
			}
			else
				return -1;
		}
		else
			return -1;
	}
	else if(currentThread==2)
	{
		if((ft3->valid)[ft]==1)
		{
			struct secondTable* ptr=(ft3->ptr)[ft];
			if((ptr->valid)[st]==1)
			{
				return (ptr->pa)[st];
			}
			else
				return -1;
		}
		else
			return -1;	
	}
	else
	{
		if((ft4->valid)[ft]==1)
		{
			struct secondTable* ptr=(ft4->ptr)[ft];
			if((ptr->valid)[st]==1)
			{
				return (ptr->pa)[st];
			}
			else
				return -1;
		}
		else
			return -1;	
	}
}

void threadexit()
{
	pthread_exit(NULL);
}

void* create(){
	signal(SIGUSR1,threadexit);
	while(1)
	{

	}
}


int main(int argc, char** argv){ 
	init();
 
    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);

	char* command=malloc(sizeof(char)*255);
    *command=' ';

    while(strcmp(command,"exit\n")!=0)
    {
    	printf("1. create\n2. kill X\n3. list\n4. mem X\n5. allocate X\n6. read X Y\n7. write X Y Z\n8. exit\n\nPlease enter your option: ");
		fgets(command,255,stdin);

      if(strcmp(command,"create\n")==0)
	  {
	  	if(numOfThread==4)
	  	{
	  		printf("TOO MANY THREADS\n");
	  	}
	  	else
	  	{
	  		int i=0;
	  		while(threadTable[i].valid!=0)
	  		{
	  			i++;
	  		}
	  		pthread_create( &(threadTable[i].id), NULL, create, NULL );
	  		threadTable[i].valid=1;
	  		numOfThread++;
	  	}

	  }
      else if(strncmp(command,"kill",4)==0)
      {
		  char* input;
          char *argv[12]; 

          const char s[2] = " \n";
   		  char *token;
   
   		  /* get the first token */
   		  token = strtok(command, s);
            
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
		  unsigned long inputul= strtoul(input,NULL,10);

      	int j;
      	int success=0;
	  	for(j=0;j<4;j++)
	  	{
	  		if(threadTable[j].valid==1)
	  		{
	  			unsigned long id=threadTable[j].id;
	  			if(id==inputul)
	  			{
	  				threadTable[j].valid=0;
	  				pthread_kill(id,SIGUSR1);
					pthread_join(id, NULL);
	  				numOfThread--;
	  				success=1;

	  				char arr2[225];
	  				//clean stuff
	  				if(j==0)
	  				{
						int c=0;
						while(c<=numOfST1)
						{
							free(ft1->ptr[c]);
							ft1->valid[c]=0;
							c++;
						}

						st1[0]=malloc(sizeof(struct secondTable));
						ft1->ptr[0]=st1[0];
						ft1->valid[0]=1;
						numOfST1=0;

						int i=0;
						while(i<numOfVA1)
						{
							VA1[i]=0;
							i++;
						}
						numOfVA1=0;

						//clean pa
						strcpy(arr2, "clean 0");
	  				}
	  				else if(j==1)
	  				{
	  					int c=0;
						while(c<=numOfST2)
						{
							free(ft2->ptr[c]);
							ft2->valid[c]=0;
							c++;
						}

						st2[0]=malloc(sizeof(struct secondTable));
						ft2->ptr[0]=st2[0];
						ft2->valid[0]=1;
						numOfST2=0;

						int i=0;
						while(i<numOfVA2)
						{
							VA2[i]=0;
							i++;
						}
						numOfVA2=0;

						//clean pa
						strcpy(arr2, "clean 1");
	  				}
	  				else if(j==2)
	  				{
	  					int c=0;
						while(c<=numOfST3)
						{
							free(ft3->ptr[c]);
							ft3->valid[c]=0;
							c++;
						}

						st3[0]=malloc(sizeof(struct secondTable));
						ft3->ptr[0]=st3[0];
						ft3->valid[0]=1;
						numOfST3=0;

						int i=0;
						while(i<numOfVA3)
						{
							VA3[i]=0;
							i++;
						}
						numOfVA3=0;

						//clean pa
						strcpy(arr2, "clean 2");
	  				}
	  				else
	  				{
	  					int c=0;
						while(c<=numOfST4)
						{
							free(ft4->ptr[c]);
							ft4->valid[c]=0;
							c++;
						}

						st4[0]=malloc(sizeof(struct secondTable));
						ft4->ptr[0]=st4[0];
						ft4->valid[0]=1;
						numOfST4=0;

						int i=0;
						while(i<numOfVA4)
						{
							VA4[i]=0;
							i++;
						}
						numOfVA4=0;

						//clean pa
						strcpy(arr2, "clean 3");
	  				}

					char arr1[225];
				    // Open FIFO for write only
				     fd = open(myfifo, O_WRONLY);
				 
				     // Write the input arr2ing on FIFO
				     // and close it
				     write(fd, arr2, strlen(arr2)+1);
				     close(fd);
				 
				     // Open FIFO for Read only
				    fd = open(myfifo, O_RDONLY);
				 
				    // Read from FIFO
				    read(fd, arr1, sizeof(arr1));
				 
				    // Print the read message
				    //printf("User2: %s\n", arr1);
				    close(fd);

					int num = atoi(arr1);
	  			}
		  	}
	  	}
	  	if(success==0)
	  	{
	  		printf("Invalid Process ID\n");
	  	}

      }
	  else if(strcmp(command,"list\n")==0)
	  {
	  	int i;
	  	int count=0;
	  	for(i=0;i<4;i++)
	  	{
	  		if(threadTable[i].valid==1)
	  		{
	  			printf("%lu\n",threadTable[i].id);
	  			count++;
	  		}
	  	}
	  	if(count==0)
	  		printf("No Current Thread Running\n");
	  	
	  }
	  else if(strncmp(command,"mem",3)==0)
	  {
	  	 command=command+4;
	  	 char* input;
          char *argv[12]; 

          const char s[2] = " \n";
   		  char *token;
   
   		  /* get the first token */
   		  token = strtok(command, s);
   		  input=token;
            
		  unsigned long inputul= strtoul(input,NULL,10);

      	int j;
      	int success=0;
	  	for(j=0;j<4;j++)
	  	{
	  		if(threadTable[j].valid==1)
	  		{
	  			unsigned long id=threadTable[j].id;
	  			if(id==inputul)
	  			{
	  				success=1;
	  				if(j==0)
	  				{
	  					if(numOfVA1==0)
	  						printf("No Virtual Address Yet\n");
	  					else{
		  					int pos=0;
		  					while(pos<numOfVA1)
		  					{
		  						bin(VA1[pos]);
		  						printf("\n");
		  						pos++;
		  					}
		  				}
	  				}
	  				else if(j==1)
	  				{
	  					if(numOfVA2==0)
	  						printf("No Virtual Address Yet\n");
	  					else{
		  					int pos=0;
		  					while(pos<numOfVA2)
		  					{
		  						bin(VA2[pos]);
		  						printf("\n");
		  						pos++;
		  					}
		  				}
	  				}
	  				else if(j==2)
	  				{
						if(numOfVA3==0)
	  						printf("No Virtual Address Yet\n");
	  					else{
		  					int pos=0;
		  					while(pos<numOfVA3)
		  					{
		  						bin(VA3[pos]);
		  						printf("\n");
		  						pos++;
		  					}
		  				}
	  				}
	  				else
	  				{
	  					if(numOfVA4==0)
	  						printf("No Virtual Address Yet\n");
	  					else{
		  					int pos=0;
		  					while(pos<numOfVA4)
		  					{
		  						bin(VA4[pos]);
		  						printf("\n");
		  						pos++;
		  					}
		  				}
	  				}
	  			}
		  	}
	  	}
	  	if(success==0)
	  	{
	  		printf("Invalid Process ID\n");
	  	}
	  	command=command-4;
	  }
	  else if(strncmp(command,"allocate",8)==0)
	  {
	  	char* input;
          char *argv[12]; 

          const char s[2] = " \n";
   		  char *token;
   
   		  /* get the first token */
   		  token = strtok(command, s);
            
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
		  unsigned long inputul= strtoul(input,NULL,10);

      	int j;
      	int full=0;
      	int success=0;
	  	for(j=0;j<4;j++)
	  	{
	  		if(threadTable[j].valid==1)
	  		{
	  			unsigned long id=threadTable[j].id;
	  			if(id==inputul)
	  			{
	  				success=1;
	  				if(j==0)
	  				{
	  					if(numOfVA1==64)
	  						full=1;
	  				}
	  				else if(j==1)
	  				{
	  					if(numOfVA2==64)
	  						full=1;
	  				}
	  				else if(j==2)
	  				{
	  					if(numOfVA3==64)
	  						full=1;
	  				}
	  				else
	  				{
	  					if(numOfVA4==64)
	  						full=1;
	  				}
	  				if(full==0)
	  				{
		  				currentThread=j;
		  				cse320_malloc();
		  			}
	  			}
		  	}
	  	}
	  	if(success==0)
	  	{
	  		printf("Invalid Process ID\n");
	  	}
	  	if(full==1)
	  	{
	  		printf("TOO MANY VIRTUAL ADDRESSES\n");
	  	}
	  }
	  else if(strncmp(command,"read",4)==0)
	  {
	  	 
	  	 command=command+5;
	  	 char* X;
	  	 char* Y; 

          const char s[2] = " \n";
   		  char *token;
   
   		  /* get the first token */
   		  token = strtok(command, s);
   		  X=token;
            
           int i=0;
		   /* walk through other tokens */
		   while( token != NULL ) {
		      token = strtok(NULL, s);
		      if(i==0)
		      {
		      	Y=token;
		      }
		      i++;
		  }
		  unsigned long Xul= strtoul(X,NULL,10);
		  const char* Yc=Y;
          unsigned int va=binaryToDecimal(Yc);

      	int j;
      	int success=0;
	  	for(j=0;j<4;j++)
	  	{
	  		if(threadTable[j].valid==1)
	  		{
	  			unsigned long id=threadTable[j].id;
	  			if(id==Xul)
	  			{
	  				success=1;
	  				currentThread=j;
	  				unsigned int pa=cse320_virt_to_phys(va);
	  				if(pa==-1)
	  				{
	  					printf("Invalid Virtual Address\n");
	  				}
	  				else
	  				{
	  					//TRY TO FIND IN CACHE
	  					if(cache[pa%4].valid==1&&cache[pa%4].pa==pa)
	  					{
	  						printf("cache hit\n");
	  						printf("The integer is %d\n",cache[pa%4].value);
	  					}
	  					else
	  					{
	  						printf("cache miss\n");
		  					//connect to mem.c
			  				char arr2[225];
			  				char arr3[20];
			  				strcpy(arr2,"read,");
			  				sprintf(arr3, "%d",pa);
							strcat(arr2,arr3);
						    	  	

							char arr1[225];
						    // Open FIFO for write only
						     fd = open(myfifo, O_WRONLY);
						 
						     // Write the input arr2ing on FIFO
						     // and close it
						     write(fd, arr2, strlen(arr2)+1);
						     close(fd);
						 
						     // Open FIFO for Read only
						    fd = open(myfifo, O_RDONLY);
						 
						    // Read from FIFO
						    read(fd, arr1, sizeof(arr1));
						 
						    // Print the read message
						    //printf("User2: %s\n", arr1);
						    close(fd);

							if(strcmp(arr1,"error")==0)
							{
								printf("CANNOT READ VALUES\n");
							}
							else
							{
								int num = atoi(arr1); //return value
								if(cache[pa%4].valid==1)
								{
									printf("eviction\n");
								}	
								cache[pa%4].valid=1;
								cache[pa%4].pa=pa;
								cache[pa%4].value=num;

								printf("The integer is %d\n",num);
							}
						}
	  				}
	  			}
		  	}
	  	}
	  	if(success==0)
	  	{
	  		printf("Invalid Process ID\n");
	  	}
	  	command=command-5;
	  }
	  else if(strncmp(command,"write",5)==0)
	  {
	  	  char* X;
	  	  char* Y;
	  	  char* Z;

          const char s[2] = " \n";
   		  char *token;
   
   		  /* get the first token */
   		  token = strtok(command, s);
            
           int i=0;
		   /* walk through other tokens */
		   while( token != NULL ) {
		      token = strtok(NULL, s);
		      if(i==0)
		      {
		      	X=token;
		      }
		      else if(i==1)
		      {
		      	Y=token;
		      }
		      else if(i==2)
		      {
		      	Z=token;
		      }
		      i++;
		  }
		  unsigned long Xul= strtoul(X,NULL,10);
		  const char* Yc=Y;
           unsigned int va=binaryToDecimal(Yc);

      	int j;
      	int success=0;
	  	for(j=0;j<4;j++)
	  	{
	  		if(threadTable[j].valid==1)
	  		{
	  			unsigned long id=threadTable[j].id;
	  			if(id==Xul)
	  			{
	  				success=1;
	  				currentThread=j;
	  				unsigned int pa=cse320_virt_to_phys(va);
	  				if(pa==-1)
	  				{
	  					printf("Invalid Virtual Address\n");
	  				}
	  				else
	  				{
	  					if(cache[pa%4].valid==1&&cache[pa%4].pa==pa)
	  					{
	  						printf("cache hit\n");
	  					}
	  					else
	  					{
	  						printf("cache miss\n");
	  					}
	  					//connect to mem.c
	  					char arr2[225];
		  				char arr3[30];
		  				char arr4[30];
		  				const char* integer=Z;
		  				strcpy(arr2,"write,");
		  				sprintf(arr3, "%d",pa);
						strcat(arr2,arr3);
						strcat(arr2,",");
					    strcat(arr2,integer);	  	

						char arr1[225];
					    // Open FIFO for write only
					     fd = open(myfifo, O_WRONLY);
					 
					     // Write the input arr2ing on FIFO
					     // and close it
					     write(fd, arr2, strlen(arr2)+1);
					     close(fd);
					 
					     // Open FIFO for Read only
					    fd = open(myfifo, O_RDONLY);
					 
					    // Read from FIFO
					    read(fd, arr1, sizeof(arr1));
					 
					    // Print the read message
					    //printf("User2: %s\n", arr1);
					    close(fd);

						strcat(arr1, "\0");
						int num = atoi(arr1); //return value

						int value=atoi(integer);
						if(cache[pa%4].valid==1&&cache[pa%4].pa!=pa)
						{
							printf("eviction\n");
						}
						cache[pa%4].valid=1;
						cache[pa%4].pa=pa;
						cache[pa%4].value=value;
	  				}
	  			}
		  	}
	  	}
	  	if(success==0)
	  	{
	  		printf("Invalid Process ID\n");
	  	}

	  }
	  else if(strcmp(command,"exit\n")==0)
	  {
	  	//clean stuff
		int c=0;
		while(c<=numOfST1)
		{
			free(ft1->ptr[c]);
			c++;
		}
		free(ft1);

		c=0;
		while(c<=numOfST2)
		{
			free(ft2->ptr[c]);
			c++;
		}
		free(ft2);	

		c=0;
		while(c<=numOfST3)
		{
			free(ft3->ptr[c]);
			c++;
		}
		free(ft3);

		c=0;
		while(c<=numOfST4)
		{
			free(ft4->ptr[c]);
			c++;
		}
		free(ft4);	

		c=0;
		while(c<4)
		{
			if(threadTable[c].valid==1)
			{
				pthread_kill(threadTable[c].id,SIGUSR1);
				pthread_join(threadTable[c].id, NULL);
			}
			c++;
		}						
	  }
	  else
      {
        printf("Invalid input! Please try again!\n");
      }
    }
      free(command);
      exit(0);
}


