#include "cse320_functions.h"

pthread_t tid;
int run=1;

struct addr_in_use{
	void* addr;
	int ref_count;

	struct addr_in_use* next;
};

struct files_in_use{
	char* filename;
	int ref_count;
	FILE* ptr;

	struct files_in_use* next;
};

struct addr_in_use* Ahead=NULL;
struct files_in_use* Fhead=NULL;

extern int errno;
struct itimerval value;

int countA=0;
int countF=0;

pthread_mutex_t mutex=(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2=(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
int count=0;

//methods
void* cse320_malloc(ssize_t size)
{
	pthread_mutex_lock(&mutex);
	
	if(Ahead==NULL){
		void* temp=malloc(size);
		struct addr_in_use* newNode=(struct addr_in_use*)malloc(sizeof(struct addr_in_use));
		newNode->addr=temp;
		newNode->ref_count=1;
		countA++;
		Ahead=newNode;
		pthread_mutex_unlock(&mutex);
		return temp;
	}
	else
	{
		struct addr_in_use* cursor=Ahead;
		while(cursor->next!=NULL)
		{
			cursor = cursor->next;
		}
		void* temp=malloc(size);
		struct addr_in_use* newNode=(struct addr_in_use*)malloc(sizeof(struct addr_in_use));
		newNode->addr=temp;
		newNode->ref_count=1;
		countA++;
		cursor->next=newNode;
		pthread_mutex_unlock(&mutex);
		return temp;
	}
}

void cse320_free(void* ptr)
{
	pthread_mutex_lock(&mutex);

	int found=0;
	struct addr_in_use* cursor=Ahead;
	while(cursor!=NULL)
	{
		if((cursor->addr)==ptr)
		{
			if((cursor->ref_count)==0)
			{
				printf("Free: Double free attempt");
				errno=EADDRNOTAVAIL;
				pthread_mutex_unlock(&mutex);
				cse320_clean();
				exit(-1);
			}
			else
			{
				cursor->ref_count=0;
				found=1;
			}
		}
		cursor = cursor->next;
	}

	if(found==0)
	{
		printf("Free: Illegal address");
		errno=EFAULT;
		pthread_mutex_unlock(&mutex);
		cse320_clean();
		exit(-1);
	}
	pthread_mutex_unlock(&mutex);
}

FILE* cse320_fopen(const char *filename, const char *mode)
{
	pthread_mutex_lock(&mutex);
	
		FILE* file = fopen(filename, mode);
		if (!file) {
			perror("Cannot open the file");
			pthread_mutex_unlock(&mutex);
			cse320_clean();
			exit(-1);
		}
		if(Fhead==NULL)
		{

			struct files_in_use* newNode=(struct files_in_use*)malloc(sizeof(struct files_in_use));
			newNode->ptr=file;
			newNode->ref_count=1;
			newNode->filename=(char*)filename;
			countF++;
			Fhead=newNode;
			pthread_mutex_unlock(&mutex);
			return file;
		}
		else
		{

			struct files_in_use* cursor=Fhead;
			while(cursor!=NULL)
			{
				if(strcmp(cursor->filename,filename)==0)
				{
					cursor->ref_count+=1;
					fclose(file);
					pthread_mutex_unlock(&mutex);
					return cursor->ptr;
				}	
				cursor = cursor->next;
			}

			struct files_in_use* cursor2=Fhead;
			while(cursor2->next!=NULL)
			{
				cursor2 = cursor->next;
			}

			struct files_in_use* newNode=(struct files_in_use*)malloc(sizeof(struct files_in_use));
			newNode->ptr=file;
			newNode->ref_count=1;
			newNode->filename=(char*)filename;
			countF++;
			cursor2->next=newNode;
			pthread_mutex_unlock(&mutex);
			return file;
		}
}

void cse320_fclose(FILE *stream)
{
	pthread_mutex_lock(&mutex);
	int found=0;

	struct files_in_use* cursor=Fhead;
			while(cursor!=NULL)
			{
				if((cursor->ptr)==stream)
				{
					if((cursor->ref_count)==0)
					{
						printf("Close: Ref count is zero");
						errno=EINVAL;
						pthread_mutex_unlock(&mutex);
						cse320_clean();
						exit(-1);
					}
					else
					{
						cursor->ref_count-=1;
						found=1;
					}
				}
				cursor = cursor->next;
			}
	if(found==0)
	{
		printf("Close: Illegal filename");
		errno=ENOENT;
		pthread_mutex_unlock(&mutex);
		cse320_clean();
		exit(-1);
	}
	pthread_mutex_unlock(&mutex);
}

void cse320_clean()
{
	pthread_mutex_lock(&mutex);

	struct addr_in_use* cursor1=Ahead;
	struct files_in_use* cursor2=Fhead;
	
			while(cursor1!=NULL)
			{
				free(cursor1->addr);
				struct addr_in_use* temp=cursor1->next;
				free(cursor1);
				cursor1 = temp;
			}
			while(cursor2!=NULL)
			{
				fclose(cursor2->ptr);
				struct files_in_use* temp=cursor2->next;
				free(cursor2);
				cursor2 = temp;
			}

	pthread_mutex_unlock(&mutex);
}


//PART3
pid_t cse320_fork()
{
	signal(SIGALRM, cse320_reap);
	return fork();
}

void cse320_settimer(int t)
{
	pthread_mutex_lock(&mutex2);
	value.it_interval.tv_sec = t;       
    value.it_interval.tv_usec = 0;  
    value.it_value.tv_sec = 1;          
    value.it_value.tv_usec = 0;     
	setitimer(ITIMER_REAL,&value,NULL);
	pthread_mutex_unlock(&mutex2);
}

void cse320_reap() 
{
   while(wait(NULL)>0) 
   {

   }
}





//EC3
void *gc()
{
	while(run)
	{
		pthread_mutex_lock(&mutex);
		printf("Start checking\n");

		//if head has ref count 0
		while(Ahead!=NULL&&Ahead->ref_count==0)
		{
				free(Ahead->addr);
				struct addr_in_use* temp=Ahead->next;
				free(Ahead);
				Ahead = temp;
				printf("malloc ref count 0 detected\n");
		}
		while(Fhead!=NULL&&Fhead->ref_count==0)
		{
				fclose(Fhead->ptr);
				struct files_in_use* temp=Fhead->next;
				free(Fhead);
				Fhead = temp;
				printf("file ref count 0 detected\n");
		}

		if(Ahead!=NULL)
		{
			struct addr_in_use* cursor1a=Ahead;
			struct addr_in_use* cursor1b=Ahead->next;
			while(cursor1a!=NULL&&cursor1b!=NULL)
			{
						
				if(cursor1b->ref_count==0)
				{
					printf("malloc ref count 0 detected\n");
					fflush(stdin);

					free(cursor1b->addr);
					struct addr_in_use* temp=cursor1b->next;
					free(cursor1b);
					cursor1a->next=temp;
					cursor1b=temp;
				}
				else
				{
					cursor1a=cursor1b;
					cursor1b=cursor1b->next;
				}
			}
		}

		if(Fhead!=NULL)
		{
			struct files_in_use* cursor2a=Fhead;
			struct files_in_use* cursor2b=Fhead->next;
			
			while(cursor2a!=NULL&&cursor2b!=NULL)
			{
				if(cursor2b->ref_count==0)
				{
					printf("file ref count 0 detected\n");
					fflush(stdin);

					fclose(cursor2b->ptr);
					struct files_in_use* temp=cursor2b->next;
					free(cursor2b);
					cursor2a->next=temp;
					cursor2b = temp;
				}
				else
				{
					cursor2a=cursor2b;
					cursor2b=cursor2b->next;
				}
				
			}
		}
		printf("Done checking\n");
		pthread_mutex_unlock(&mutex);

		sleep(5);
	}	
}

void cse320_garbageCollector() 
{
    pthread_create(&tid, NULL, gc, NULL);
}

void cse320_KILLgarbageCollector() 
{
    run=0;
    pthread_join(tid, NULL);
}

void print()
{
	printf("ref count: %d",Fhead->ref_count);
}