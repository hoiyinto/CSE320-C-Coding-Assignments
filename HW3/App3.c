#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>

int main(int argc, char** argv) { 

  //check arguments
  if(argc<2)
  {
  	printf("ERROR\n");
  	exit(1);
  }	

  uint64_t n=atoll(*(argv + 1)); 

  FILE *fp;
  fp = fopen("part2_fib.dat", "w");
  
  if(n==0)
  {
  	uint64_t zero=0;
  	fprintf(fp,"%lu\n",zero);
  }
  else if(n==1)
  {
  	uint64_t one=1;
  	fprintf(fp,"%lu\n",one);
  }
  else
  {
	  uint64_t f[n+1];
	  uint64_t i;
	  f[0] = 0;
	  f[1] = 1;

		for (i = 2; i <= n; i++)
		{
		    f[i] = f[i-1] + f[i-2];
		}

		fprintf(fp,"%lu\n",f[n]);
   }
   fclose(fp);
}
