#include "cse320_functions.h"

//EC4
pid_t cse320_forkEC();
void cse320_reapThreadEC();
void cse320_killReapThreadEC();


int main(int argc, char** argv){ 
	cse320_reapThreadEC();

	pid_t a=cse320_forkEC();
	if(a==0)
	{
		printf("child\n");
		exit(0);
	}
	
	pid_t b=cse320_forkEC();

	if(b==0)
	{
		printf("child\n");
		exit(0);
	}

	pid_t c=cse320_forkEC();
	if(c==0)
	{
		printf("child\n");
		sleep(5);
		exit(0);
	}

	while(1)
	{

	}
	void cse320_killReapThreadEC();
}