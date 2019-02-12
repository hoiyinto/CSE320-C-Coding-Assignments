#include "cse320_functions.h"

void *test();
int main(int argc, char** argv){ 
	
  /**
	pthread_t thread_id[25];
      int i,j;

      for(i=0; i < 24; i++)
      {
        pthread_create( &thread_id[i], NULL, test, NULL );
      }
	
	for(j=0; j < 24; j++)
      {
         pthread_join( thread_id[j], NULL);
      }
  **/

  char* x[25];
  int j=0;
  for(j=0; j < 25; j++)
  {
      x[j]=cse320_malloc(30);
  }

  printf("DONE");
	
  //NOT ENOUGH MEM
  //char* abc=cse320_malloc(30);

  //free one space
  cse320_free(x[0]);
  char* abc=cse320_malloc(30);

  
 	//ref count is 0
  //FILE* c=cse320_fopen("test.txt","r");
  //FILE* b=cse320_fopen("test.txt","r");
  //cse320_fclose(b);
  //cse320_fclose(b);
  //cse320_fclose(c);

  //illegal address
  //cse320_free(c);

  cse320_clean();
}

void *test()
{
	cse320_malloc(30);
	return NULL;
}
