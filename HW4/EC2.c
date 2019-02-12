#include "cse320_functions.h"

struct addr_in_use{
	void* addr;
	int ref_count;
};

struct files_in_use{
	char* filename;
	int ref_count;
	FILE* ptr;
};

struct addr_in_use a[25];
struct files_in_use f[25];

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
	if(countA==25)
	{
		int i;
		//check if any is not in used
		for(i=0;i<countA;i++)
		{
			if((a[i].ref_count)==0)
			{
				free(a[i].addr);
				void* temp=malloc(size);
				a[i].addr=temp;
				a[i].ref_count=1;
				pthread_mutex_unlock(&mutex);
				return temp;
			}
		}
		printf("Not enough memory");
		errno=ENOMEM;
		pthread_mutex_unlock(&mutex);
		cse320_clean();
		exit(-1);
	}
	else
	{
		void* temp=malloc(size);
		a[countA].addr=temp;
		a[countA].ref_count=1;
		countA++;
		pthread_mutex_unlock(&mutex);
		return temp;
	}
}

void cse320_free(void* ptr)
{
	pthread_mutex_lock(&mutex);
	int i;
	int found=0;
	for(i=0;i<countA;i++)
	{
		if((a[i].addr)==ptr)
		{
			if((a[i].ref_count)==0)
			{
				printf("Free: Double free attempt");
				errno=EADDRNOTAVAIL;
				pthread_mutex_unlock(&mutex);
				cse320_clean();
				exit(-1);
			}
			else
			{
				a[i].ref_count=0;
				found=1;
			}
		}
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
	if(countF==25)
	{
		int j;
		//check if already opened
		for(j=0;j<countF;j++)
		{
			if(strcmp(f[j].filename,filename)==0)
			{
				f[j].ref_count+=1;
				pthread_mutex_unlock(&mutex);
				return f[j].ptr;
			}	
		}
		int i;
		//check if any is not in used
		for(i=0;i<countF;i++)
		{
			if((f[i].ref_count)==0)
			{
				FILE* file = fopen(filename, mode);
				if (!file) {
					perror("Cannot open the file");
					pthread_mutex_unlock(&mutex);
					cse320_clean();
					exit(-1);
				}
				fclose(f[i].ptr);
				f[i].ref_count=1;
				f[i].filename=(char*)filename;
				f[i].ptr=file;
				pthread_mutex_unlock(&mutex);
				return file;
			}	
		}

		printf("Too many opened files");
		errno=ENFILE;
		pthread_mutex_unlock(&mutex);
		cse320_clean();
		exit(-1);
	}
	else
	{
		FILE* file = fopen(filename, mode);
		if (!file) {
			perror("Cannot open the file");
			pthread_mutex_unlock(&mutex);
			cse320_clean();
			exit(-1);
		}
		if(countF==0)
		{
			f[0].ref_count+=1;
			f[0].filename=(char*)filename;
			f[0].ptr=file;
			countF++;
			pthread_mutex_unlock(&mutex);
			return file;
		}
		else
		{
			int i;
			for(i=0;i<countF;i++)
			{
				if(strcmp(f[i].filename,filename)==0)
				{
					f[i].ref_count+=1;
					fclose(file);
					pthread_mutex_unlock(&mutex);
					return f[i].ptr;
				}	
			}
			f[countF].ref_count+=1;
			f[countF].filename=(char*)filename;
			f[countF].ptr=file;
			countF++;
			pthread_mutex_unlock(&mutex);
			return file;
		}
	}
}

void cse320_fclose(FILE *stream)
{
	pthread_mutex_lock(&mutex);
	int i;
	int found=0;
	for(i=0;i<countF;i++)
	{
		if((f[i].ptr)==stream)
		{
			if((f[i].ref_count)==0)
			{
				printf("Close: Ref count is zero");
				errno=EINVAL;
				pthread_mutex_unlock(&mutex);
				cse320_clean();
				exit(-1);
			}
			else
			{
				f[i].ref_count-=1;
				found=1;
			}
		}
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
	int i;
	for(i=0;i<countA;i++)
	{
		free(a[i].addr);
	}
	int j;
	for(j=0;j<countF;j++)
	{
		fclose(f[j].ptr);
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