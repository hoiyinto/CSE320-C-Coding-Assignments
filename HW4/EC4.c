#include "cse320_functions.h"

pthread_t tid;
int run=1;

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
pthread_mutex_t mutex3=(pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
int count=0;


pid_t pid[25];
int countPID=0;


//methods
void* cse320_malloc(ssize_t size)
{
	pthread_mutex_lock(&mutex);
	if(countA==25)
	{
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


//part3
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



//EC4
pid_t cse320_forkEC()
{
	pthread_mutex_lock(&mutex3);
    pid_t pid2;

    if ((pid2 = fork()) < 0)
    {
		printf("fork error");
		exit(1);
    }
    if(pid2>0)
    {
    	int i;
    	for(i=0;i<25;i++)
		{
			if(pid[i]==0)
			{
				pid[i]=pid2;
			}
		}
	}
	pthread_mutex_unlock(&mutex3);
    return pid2;
}

void *reapEC()
{
	int counter = 3;
	while(run)
	{
			pthread_mutex_lock(&mutex3);
			int i;
			for(i=0;i<25;i++)
			{
				int check;
				if((check=waitpid(pid[i], NULL, WNOHANG))>0)
				{
					pid[i]=0;
					if (--counter == 0) {
						printf("done\n"); fflush(stdout);
						sleep(10);
					}
					printf("hi\n");
					fflush(stdout);
				}
				printf("chop chop\n");fflush(stdout);
			}
			pthread_mutex_unlock(&mutex3);
	}
}

void cse320_reapThreadEC()
{
    pthread_create(&tid, NULL, reapEC, NULL);
}

void cse320_killReapThreadEC()
{
    run=0;
    pthread_join(tid, NULL);
}