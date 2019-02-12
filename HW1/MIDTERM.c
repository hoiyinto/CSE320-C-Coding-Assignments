#include <stdio.h>
#include <stdlib.h> //malloc
#include <ctype.h> //toupper
#include <string.h> 

void majorFormat(char* x);

int main(int argc, char** argv) { 
	printf("It's a test\n");
	char a[5];
	a[0]='b';

	printf("%c\n",a[0]);

	a[0]='c';

	printf("%c\n",a[0]);

	char* string=malloc(sizeof(char) * 255);
	*string='a';
	*(string+1)='b';

	//*string='x';
	//*(string+1)='y';


	printf("%s\n",string);

	majorFormat(string);

	printf("%s\n",string);

	char sarry[5];
    strcpy(sarry,"abcdef");

    printf("%c\n",sarry[16]);	

	free(string);

	printf("%s\n",string);
	*string='a';
	*(string+1)='b';
	printf("%s\n",string);

	int val;
	scanf("%d",&val);

	val=4;
	printf("The int is %d\n",val);

	return 0;
}

void majorFormat(char* x){
	int i;
	for(i=0;*x!='\0';x++)
	{
		*x=toupper(*x);
	}
}
