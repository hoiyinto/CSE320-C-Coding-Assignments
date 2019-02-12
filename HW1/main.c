#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>
#include <ctype.h>

int size(char* x);
int same(char* x,char* y);
int letter(char* x);
void nameFormat(char* x);
void majorFormat(char* x);
int digit(char* x);

int main(int argc, char** argv) { 

  //check arguments
  if(argc<3)
  {
  	printf("NO QUERY PROVIDED\n");
  	exit(1);
  }	
 
  //read filename first
  char* filename = *(argv + 1);

  //check flags
  int option;
  int vflag=0, iflag=0, fflag=0, mflag=0, oflag=0, gflag=0;
  char *iarg;
  char *farg;
  char *marg;
  char *oarg;
  extern char *optarg;

  while ((option= getopt(argc, argv, ":vi:f:m:o:g")) != -1){
		switch (option) {
			case 'v':
				vflag = 1;
				break;
			case 'i':
				iflag = 1;
				iarg = optarg;
				break;
			case 'f':
				fflag = 1;
				farg = optarg;
				nameFormat(farg);
				break;
			case 'm':
				mflag = 1;
				marg = optarg;
				majorFormat(marg);
				break;
			case 'o':
				oflag = 1;
				oarg = optarg;
				break; 
			case 'g':
				gflag = 1;
				break;
			case '?':
				printf("OTHER ERROR\n");
  				exit(0);
			default:
				break;	
		}
	}

	//NO FLAGS
	if(vflag==0&&iflag==0&&fflag==0&&mflag==0&&oflag==0&&gflag==0)
	{
		printf("NO QUERY PROVIDED\n");
  		exit(1);
	}

	//only -o
	if(vflag==0&&iflag==0&&fflag==0&&mflag==0&&oflag==1&&gflag==0)
	{
		printf("OTHER ERROR\n");
  		exit(1);
	}

	//WRONG COMBINATION OF FLAGS
	if(vflag==1)
	{
		if(iflag==1||fflag==1||mflag==1)
		{
			printf("OTHER ERROR\n");
  			exit(1);
		}
	}

	//if -o 
	if(oflag==1)
	{
		FILE *file2;
  		file2 = fopen(oarg, "r");
  		if(file2!=NULL)
  		{
  			char choice;
  			//override or not
  			printf("Do you want to override the output file? (y/n) ");
  			scanf("%c", &choice);

  			if(choice=='n')
  			{
  				printf("FILE EXISTS\n");
  				exit(1);
  			}
  		}
	}

	//check flag arguments
	if(iflag==1)
	{
		if(digit(iarg)!=0)
		{
			printf("OTHER ERROR\n");
  			exit(1);
  			if((atoi(iarg))<0)
  			{
  				printf("OTHER ERROR\n");
  				exit(1);
  			}

		}
	}

	if(fflag==1)
	{
		if(letter(farg)!=0||size(farg)<3||size(farg)>10)
		{
			printf("OTHER ERROR\n");
  			exit(1);
		}

	}

	if(mflag==1)
	{
		if(letter(marg)!=0||size(marg)!=3)
		{
			printf("OTHER ERROR\n");
  			exit(1);
		}
	}


  //linked list
  typedef struct student_records{
	  int id ;
	  char* first_name;
	  char* last_name;
	  float gpa ;
	  char* major;
	  struct student_records* next;
  }student_records;

  student_records* head=NULL;


  //read file
  FILE *file;
  file = fopen(filename, "r");
  char* input= malloc(sizeof(char) * 255);
  char* command= malloc(sizeof(char) * 30);
  char* add="ADD";
  char* update="UPDATE";
  char* delete="DELETE";


  if(file!=NULL){
	  	while(fgets(input, 255, (FILE*)file)!=NULL) {
	   	   
	   	   sscanf( input, "%s",command);
	   	   
           //ADD
           if(size(command)==3&&same(command,add)==0){
           		
           		student_records* new_node = (student_records*)malloc(sizeof(student_records));
           		new_node->first_name= malloc(sizeof(char) * 20);
           		new_node->last_name= malloc(sizeof(char) * 20);
				new_node->major= malloc(sizeof(char) * 10);
           		sscanf( input, "%s %d %s %s %f %s",command,&(new_node->id),new_node->first_name,new_node->last_name,&(new_node->gpa),new_node->major);	
           		
           		nameFormat(new_node->first_name);
           		nameFormat(new_node->last_name);
				majorFormat(new_node->major);

                if(letter(new_node->first_name)!=0||letter(new_node->last_name)!=0||letter(new_node->major)!=0||size(new_node->major)!=3||size(new_node->first_name)<3||size(new_node->first_name)>10||size(new_node->last_name)<3||size(new_node->last_name)>10||(new_node->id)<=0||(new_node->gpa)<1||(new_node->gpa)>4)
                {
                	printf("FAILED TO PARSE FILE\n");
	   				exit(1);
                }

           		if(head==NULL)
           		{
           			head=new_node;
           		}
           		else{
           			  //check if the ID exists
           			  student_records* cursor = head;
					  while(cursor != NULL)
					  {
					      if((cursor->id)==(new_node->id))
					      {
					      	printf("ID NOT UNIQUE\n");
	   						exit(1);
					      }
					      cursor = cursor->next;
					  }

					  //tail
					  student_records* cursor2 = head;
					  while((cursor2->next)!= NULL)
					  {
					      cursor2 = cursor2->next;
					  }

					  //add new node to the list

					  //if smallest
					  if((head->id)>(new_node->id))
					  {
					  	new_node->next=head;
					  	head=new_node;
					  }
					  //if greatest
					  else if((cursor2->id)<(new_node->id))
					  {
					  	cursor2->next=new_node;
					  }
					  //in between
					  else{
					  	  student_records* cursor3 = head;
						  while((cursor3->next) != NULL)
						  {
						      if((cursor3->id)<(new_node->id)&&((cursor3->next)->id)>(new_node->id))
						      {
						      	new_node->next=cursor3->next;
								cursor3->next=new_node;
						      }
						      cursor3 = cursor3->next;
						  }
					  }
           		}	
           }
           //UPDATE
           else if(size(command)==6&&same(command,update)==0){
           		student_records* new_node = (student_records*)malloc(sizeof(student_records));
           		new_node->first_name= malloc(sizeof(char) * 20);
           		new_node->last_name= malloc(sizeof(char) * 20);
				new_node->major= malloc(sizeof(char) * 10);
           		sscanf( input, "%s %d %s %s %f %s",command,&(new_node->id),new_node->first_name,new_node->last_name,&(new_node->gpa),new_node->major);	
           		
           		nameFormat(new_node->first_name);
           		nameFormat(new_node->last_name);
				majorFormat(new_node->major);

                if(letter(new_node->first_name)!=0||letter(new_node->last_name)!=0||letter(new_node->major)!=0||size(new_node->major)!=3||size(new_node->first_name)<3||size(new_node->first_name)>10||size(new_node->last_name)<3||size(new_node->last_name)>10||(new_node->id)<=0||(new_node->gpa)<1||(new_node->gpa)>4)
                {
                	printf("FAILED TO PARSE FILE\n");
	   				exit(1);
                }

                	//check if the ID exists
           			  student_records* cursor = head;
           			  student_records* found=NULL;
					  while(cursor != NULL)
					  {
					      if((cursor->id)==(new_node->id))
					      {
					      	found=cursor;				      	
					      }
					      cursor = cursor->next;
					  }	

					  if(found==NULL)
					  {
					  	printf("STUDENT RECORD CANNOT BE DELETED NOR UPDATED\n");
					  	exit(1);
					  }
					  else{
					  	found->first_name=new_node->first_name;
					  	found->last_name=new_node->last_name;
						found->gpa=new_node->gpa;
						found->major=new_node->major;
					  }
           }
           //DELETE
           else if(size(command)==6&&same(command,delete)==0){
           			  
           			 student_records* new_node = (student_records*)malloc(sizeof(student_records));
           			 sscanf( input, "%s %d",command,&(new_node->id));

           			  //check if the ID exists
           			  student_records* cursor = head;
           			  student_records* found=NULL;
					  while(cursor != NULL)
					  {
					      if((cursor->id)==(new_node->id))
					      {  	
					      	found=cursor;				      	
					      }
					      cursor = cursor->next;
					  }


					  if(found==NULL||head==NULL)
					  {
					  	printf("STUDENT RECORD CANNOT BE DELETED NOR UPDATED\n");
					  	exit(1);
					  }

					  //del head
					  if(head==found)
					  {
					  	head=head->next;
					  }
					  //del
					  else{
						  student_records* cursor2 = head;
						  while(cursor2!=NULL&& (cursor2->next) != NULL)
						  {
						      if((cursor2->next)==found)
						      {
						      	cursor2->next=found->next;			      	
						      }
						      cursor2 = cursor2->next;
						  }
					  }
           }
           //UNKNOWN COMMAND
           else{
           	printf("FAILED TO PARSE FILE\n");
	   		exit(1);
           }
   		}
   		fclose ( file );
   }
   else{
	   	printf("FAILED TO PARSE FILE\n");
	   	exit(1);
   }


   //WORK ON THOSE FLAGS

   //print on file
   if(oflag==1)
   {
   		FILE *fp;
		fp = fopen(oarg, "w");

		//-v
	   	if(vflag==1)
	   	{
	   		float sum=0; 
			int counter=0;

			student_records* cursor = head;
			while(cursor != NULL)
			{
				counter++;
				sum+=cursor->gpa;
				fprintf(fp,"%d %s %s %.2f %s\n", cursor->id, cursor->first_name, cursor->last_name, cursor->gpa, cursor->major);
				cursor = cursor->next;
			}
			float average=sum/counter;

			if(gflag==1)
			{
				fprintf(fp,"%.2f\n",average);
			}

			exit(0);
	   	}

	   	//-i
	   	if(iflag==1)
	   	{
	   		int theID=atoi(iarg);
	   		student_records* cursor = head;
           	student_records* found=NULL;
		    while(cursor != NULL){
			 	if((cursor->id)==theID)
				{
						found=cursor;				      	
				}
			    cursor = cursor->next;
		     }	

			if(found==NULL)
			{
				printf("STUDENT RECORD NOT FOUND\n");
				exit(0);
			}

			if(fflag==1)
			{
				if(same((found->last_name),farg)!=0)
				{
					printf("STUDENT RECORD NOT FOUND\n");
					exit(0);			      	
				}
			}

			if(mflag==1)
			{
				if(same((found->major),marg)!=0)
				{
					printf("STUDENT RECORD NOT FOUND\n");
					exit(0);			      	
				}
			}

			//print that student
			fprintf(fp,"%d %s %s %.2f %s\n", found->id, found->first_name, found->last_name, found->gpa, found->major);

			if(gflag==1)
			{
				fprintf(fp,"%.2f\n",found->gpa);
			}
			exit(0);
	   	}

	   	//-f -m -g 

	   	//-f -m
	   	if(fflag==1&&mflag==1)
	   	{
	   		float sum=0; 
			int counter=0;
	   		student_records* cursor = head;
			while(cursor != NULL)
			{
				if(same(cursor->last_name,farg)==0&&same(cursor->major,marg)==0)
				{
					counter++;
					sum+=cursor->gpa;
					fprintf(fp,"%d %s %s %.2f %s\n", cursor->id, cursor->first_name, cursor->last_name, cursor->gpa, cursor->major);
				}
				cursor = cursor->next;
			}

			if(counter==0)
			{
				printf("STUDENT RECORD NOT FOUND\n");
				exit(0);	
			}

			float average=sum/counter;

			if(gflag==1)
			{
				fprintf(fp,"%.2f\n",average);
			}

			exit(0);
	   	}


        //just -f
	   	if(fflag==1)
	   	{
	   		float sum=0; 
			int counter=0;
	   		student_records* cursor = head;
			while(cursor != NULL)
			{
				if(same(cursor->last_name,farg)==0)
				{
					counter++;
					sum+=cursor->gpa;
					fprintf(fp,"%d %s %s %.2f %s\n", cursor->id, cursor->first_name, cursor->last_name, cursor->gpa, cursor->major);
				}
				cursor = cursor->next;
			}

			if(counter==0)
			{
				printf("STUDENT RECORD NOT FOUND\n");
				exit(0);	
			}

			float average=sum/counter;

			if(gflag==1)
			{
				fprintf(fp,"%.2f\n",average);
			}

			exit(0);
		}
	   		
	   		// just -m	
	   		if(mflag==1)
	   		{
	   			float sum=0; 
				int counter=0;
				student_records* cursor = head;
				while(cursor != NULL)
				{
					if(same(cursor->major,marg)==0)
					{
						counter++;
						sum+=cursor->gpa;
						fprintf(fp,"%d %s %s %.2f %s\n", cursor->id, cursor->first_name, cursor->last_name, cursor->gpa, cursor->major);
					}
					cursor = cursor->next;
				}

				if(counter==0)
				{
					printf("STUDENT RECORD NOT FOUND\n");
					exit(0);	
				}

				float average=sum/counter;

				if(gflag==1)
				{
					fprintf(fp,"%.2f\n",average);
				}

				exit(0);
			}

  		//just -g
		if(gflag==1)
  		{
			float sum=0; 
			int counter=0;

			student_records* cursor = head;
			while(cursor != NULL)
			{
				counter++;
				sum+=cursor->gpa;
				cursor = cursor->next;
			}
			float average=sum/counter;
			fprintf(fp,"%.2f\n",average);

  		}

   }
   //stdout
   else
   {
   		//-v
	   	if(vflag==1)
	   	{
	   		float sum=0; 
			int counter=0;

			student_records* cursor = head;
			while(cursor != NULL)
			{
				counter++;
				sum+=cursor->gpa;
				printf("%d %s %s %.2f %s\n", cursor->id, cursor->first_name, cursor->last_name, cursor->gpa, cursor->major);
				cursor = cursor->next;
			}
			float average=sum/counter;

			if(gflag==1)
			{
				printf("%.2f\n",average);
			}

			exit(0);
	   	}

	   	//-i
	   	if(iflag==1)
	   	{
	   		int theID=atoi(iarg);
	   		student_records* cursor = head;
           	student_records* found=NULL;
		    while(cursor != NULL){
			 	if((cursor->id)==theID)
				{
						found=cursor;				      	
				}
			    cursor = cursor->next;
		     }	

			if(found==NULL)
			{
				printf("STUDENT RECORD NOT FOUND\n");
				exit(0);
			}

			if(fflag==1)
			{
				if(same((found->last_name),farg)!=0)
				{
					printf("STUDENT RECORD NOT FOUND\n");
					exit(0);			      	
				}
			}

			if(mflag==1)
			{
				if(same((found->major),marg)!=0)
				{
					printf("STUDENT RECORD NOT FOUND\n");
					exit(0);			      	
				}
			}

			//print that student
			printf("%d %s %s %.2f %s\n", found->id, found->first_name, found->last_name, found->gpa, found->major);

			if(gflag==1)
			{
				printf("%.2f\n",found->gpa);
			}
			exit(0);
	   	}

	   	//-f -m -g 

	   	//-f -m
	   	if(fflag==1&&mflag==1)
	   	{
	   		float sum=0; 
			int counter=0;
	   		student_records* cursor = head;
			while(cursor != NULL)
			{
				if(same(cursor->last_name,farg)==0&&same(cursor->major,marg)==0)
				{
					counter++;
					sum+=cursor->gpa;
					printf("%d %s %s %.2f %s\n", cursor->id, cursor->first_name, cursor->last_name, cursor->gpa, cursor->major);
				}
				cursor = cursor->next;
			}

			if(counter==0)
			{
				printf("STUDENT RECORD NOT FOUND\n");
				exit(0);	
			}

			float average=sum/counter;

			if(gflag==1)
			{
				printf("%.2f\n",average);
			}

			exit(0);
	   	}


        //just -f
	   	if(fflag==1)
	   	{
	   		float sum=0; 
			int counter=0;
	   		student_records* cursor = head;
			while(cursor != NULL)
			{
				if(same(cursor->last_name,farg)==0)
				{
					counter++;
					sum+=cursor->gpa;
					printf("%d %s %s %.2f %s\n", cursor->id, cursor->first_name, cursor->last_name, cursor->gpa, cursor->major);
				}
				cursor = cursor->next;
			}

			if(counter==0)
			{
				printf("STUDENT RECORD NOT FOUND\n");
				exit(0);	
			}

			float average=sum/counter;

			if(gflag==1)
			{
				printf("%.2f\n",average);
			}

			exit(0);
		}
	   		
	   		// just -m	
	   		if(mflag==1)
	   		{
	   			float sum=0; 
				int counter=0;
				student_records* cursor = head;
				while(cursor != NULL)
				{
					if(same(cursor->major,marg)==0)
					{
						counter++;
						sum+=cursor->gpa;
						printf("%d %s %s %.2f %s\n", cursor->id, cursor->first_name, cursor->last_name, cursor->gpa, cursor->major);
					}
					cursor = cursor->next;
				}

				if(counter==0)
				{
					printf("STUDENT RECORD NOT FOUND\n");
					exit(0);	
				}

				float average=sum/counter;

				if(gflag==1)
				{
					printf("%.2f\n",average);
				}

				exit(0);
			}

  		//just -g
		if(gflag==1)
  		{
			float sum=0; 
			int counter=0;

			student_records* cursor = head;
			while(cursor != NULL)
			{
				counter++;
				sum+=cursor->gpa;
				cursor = cursor->next;
			}
			float average=sum/counter;
			printf("%.2f\n",average);

  		}
   }

  return 0;
}          

//helper methods
int size(char* x)
{
	int i;
	for(i=0;*x!='\0';x++)
	{
		i++;
	}
	return i;
} 

int same(char* x,char* y)
{
	int i;
	for(i=0;*x!='\0';x++,y++)
	{
		if(*x!=*y)
			i++;
	}
	return i;
}

int letter(char* x)
{
	int i;
	for(i=0;*x!='\0';x++)
	{
		if(!isalpha(*x))
			i++;
	}
	return i;
}

int digit(char* x)
{
	int i;
	for(i=0;*x!='\0';x++)
	{
		if(!isdigit(*x))
			i++;
	}
	return i;
}

void nameFormat(char* x)
{
	int i=0;
	for(i=0;*x!='\0';x++)
	{
		if(i==0)
			*x=toupper(*x);
		else
			*x=tolower(*x);
		i++;
	}
}

void majorFormat(char* x){
	int i;
	for(i=0;*x!='\0';x++)
	{
		*x=toupper(*x);
	}
}
