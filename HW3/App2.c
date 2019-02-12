#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char** argv) { 

  //check arguments
  if(argc<2)
  {
  	printf("ERROR\n");
  	exit(1);
  }	
 
  //read file
  char* filename = *(argv + 1);
  FILE *file;
  file = fopen(filename, "r");

  if(file==NULL){
  	printf("FILE NOT FOUND\n");
  	exit(1);
  }

  else{
    int row1;
    int col1;
    int row2;
    int col2;
    int r;
    int c;

     //1st matrix
     fscanf((FILE*)file,"%d",&row1);
     fscanf((FILE*)file,"%d",&col1);

     int m1[row1][col1];
     for(r=0;r<row1;r++)
     {
        for(c=0;c<col1;c++)
        {
          fscanf((FILE*)file,"%d",&m1[r][c]);
        }
     }

     //2rd matrix
     fscanf((FILE*)file,"%d",&row2);
     fscanf((FILE*)file,"%d",&col2);

     int m2[row2][col2];
     for(r=0;r<row2;r++)
     {
       for(c=0;c<col2;c++)
       {
          fscanf((FILE*)file,"%d",&m2[r][c]);
       }
     }
    
     fclose(file);

    int dot[row1][col2];
    int sum=0;
    int d;
    int k;

    for ( c = 0 ; c < row1 ; c++ )
    {
      for ( d = 0 ; d < col2 ; d++ )
      {
        for ( k = 0 ; k < row2 ; k++ )
        {
          sum = sum + m1[c][k]*m2[k][d];
        }
 
        dot[c][d] = sum;
        sum = 0;
      }
    }

    int tr=0;
    for(c=0;c<row1;c++)
    {
      tr=tr+dot[c][c];
    }

    FILE *fp;
    fp = fopen("part2_trace.dat", "w");
    fprintf(fp,"%d\n",tr);
    fclose(fp);
  }
}