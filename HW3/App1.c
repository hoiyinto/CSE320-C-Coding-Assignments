#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>

double f(double n,double c[],double x);

int main(int argc, char** argv) { 

  //check arguments
  if(argc<2)
  {
  	printf("ERROR\n");
  	exit(1);
  }	
 
  //read filename 
  char* filename = *(argv + 1);
  FILE *file;
  file = fopen(filename, "r");

  if(file==NULL){
  	printf("FILE NOT FOUND\n");
  	exit(1);
  }
  else
  {
    int n;
    double e;
    double A;
    double B;
    int r;

     fscanf((FILE*)file,"%d %lf %lf %lf",&n,&e,&A,&B);

     double c[n+1];

     for(r=0;r<=n;r++)
     {
        fscanf((FILE*)file,"%lf",&c[r]);
     }

     fclose(file);

     double m=(A+B)/2;

     while(fabs(f(n,c,m))>=e)
     {
        if ( (f(n,c,m) > 0 && f(n,c,A) < 0) || (f(n,c,m) < 0 && f(n,c,A) > 0) )
        {  
            // f(a) and f(m) have different signs: move b
           B = m;
        }
        else
        {  // f(a) and f(m) have same signs: move a
           A = m;
        }
        m = (A+B)/2;
     }

      FILE *fp;
      fp = fopen("part2_solver.dat", "w");
      fprintf(fp,"%lf\n",m);
      fclose(fp);
  }
}

double f(double n,double c[],double x)
{
  double ans=0;
  int index=0;

  while(n>=0)
  {
    ans=ans+(c[index]*pow(x,n));
    n--;
    index++;
  }

  return ans;
}