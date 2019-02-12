#include "cse320_functions.h"

//EC3
//void cse320_garbageCollector(); 
//void cse320_KILLgarbageCollector();
//void print();

void *test();
int main(int argc, char** argv){ 
	
  //cse320_garbageCollector();
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

 /**
  char* x[30];
  int j=0;
  for(j=0; j < 30; j++)
  {
      x[j]=cse320_malloc(sizeof(char*));
  }

	
  //double free
  cse320_free(x[0]);
   //cse320_free(x[0]);
  cse320_free(x[3]);
  cse320_free(x[20]);
  cse320_free(x[25]);


  //free illegal address
  //char* abc=malloc(30);
  //cse320_free(abc);
  **/

  //ref count is 0
 int counter;
 FILE* f[26];
 char* filename=malloc(20);

  for(counter=1;counter<26;counter++)
  {
    //int* pt=malloc(20);
    //*pt=counter;

    char* filename2=malloc(20);
    sprintf(filename2,"%d",counter);
    
    filename=strcat(filename2,".dat");
    printf("Opened %s\n",filename);
    f[counter]=cse320_fopen(filename,"r");
  }


 	FILE* c=cse320_fopen("test.txt","r");
 	//FILE* b=cse320_fopen("test.txt","r");
  //cse320_fclose(b);
	//cse320_fclose(b);

 //print();
 printf("DONE\n");

	//cse320_fclose(c);

  //illegal address
  //cse320_free(c);
  //cse320_free(x[28]);
  //cse320_free(x[29]);
  //sleep(90);

  //cse320_KILLgarbageCollector(); 
  //cse320_clean();
}

void *test()
{
	cse320_malloc(30);
	return NULL;
}
