#include "helper.h"
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

extern int errno;

int main(int argc, char** argv) {

    if (*(argv + 1) == NULL) {
        printf("You should provide name of the test file.\n");
        return 1;
    }
    void* ram = cse320_init(*(argv + 1));
    void* tmp_buf = cse320_tmp_buffer_init();
    int ret = 0;
    /*
     * You code goes below. Do not modify provided parts
     * of the code.
     */

    //check for errors
    if(ram==NULL)
    {
        printf("INIT ERROR\n");
        exit(errno);
    }

    void* incsize=cse320_sbrk(896);
    if(incsize==NULL)
    {
        printf("SBRK ERROR\n");
        exit(errno);
    }
    

    //scan all blocks in ram
    uint64_t* block =  (uint64_t*) ram;

    typedef struct mblock{
		uint64_t gap;
		uint64_t size;
        uint64_t id;
        uint64_t flag;
	}mblock;

	mblock* head=(mblock*)tmp_buf;
    mblock* tail=(mblock*)tmp_buf;

    int read=0;
    mblock* first=NULL;
    
    while(read<1024)
    {
    	uint64_t size=((*block >> 3)<<3);
        uint64_t id=((*block <<61)>>62);
        uint64_t flag=(*block & 1);
        
        if(first==NULL)
        {
        	first=head;
        	if(size==0)
        	{
        		first->gap=1;
        		first->size=8;

				//go to next block
        		block++;
            	read+=8;
        	}
        	else
        	{
        		first->gap=0;
	        	first->size=size;
	        	first->id=id;
	        	first->flag=flag;

	        	//go to next block
	        	read+=size;
		        block=block+(size/8);
        	}
        }
        else
        {
        	if(size==0)
        	{
        		if((tail->gap)==1)
        		{
        			tail->size+=8;

					//go to next block
	        		block++;
	            	read+=8;
        		}
        		else
        		{
        			tail++;
        			mblock* newB=tail;

        			newB->gap=1;
	        		newB->size=8;

					//go to next block
	        		block++;
	            	read+=8;
        		}
        	}
        	else
        	{
        		tail++;
        		mblock* newB=tail;

				newB->gap=0;
	        	newB->size=size;
	        	newB->id=id;
	        	newB->flag=flag;

	        	//go to next block
	        	read+=size;
		        block=block+(size/8);
        	}
        }
    }

    //print all blocks in ram
 	int option=0;

	while(option!=4)
	{
		//size
		mblock* pointer=head;
		printf("----------------------------------------\n");
		printf("Size:");
		while(pointer!=tail)
	    {
		    if((pointer->gap)==1)
		    {
		    	int counter=0;
		    	while(counter<(pointer->size))
		    	{
		    		printf(" ");
		    		counter++;
		    	}
		    }
		    else
				printf("|%lu|",pointer->size);
	    	pointer++;
	    }

	    	if((pointer->gap)==1)
		    {
		    	printf("|%lu more spaces|\n",pointer->size);
		    }
		    else
				printf("|%lu|\n",pointer->size);

		//id
		pointer=head;
		printf("ID:  ");
		while(pointer!=tail)
	    {
		    if((pointer->gap)==1)
		    {
		    	int counter=0;
		    	while(counter<(pointer->size))
		    	{
		    		printf(" ");
		    		counter++;
		    	}
		    }
		    else
				printf("|0%lu|",pointer->id);
	    	pointer++;
	    }

	    	if((pointer->gap)==1)
		    {
		    	printf("|%lu more spaces|\n",pointer->size);
		    }
		    else
				printf("|0%lu|\n",pointer->id);

		//flag
		pointer=head;
		printf("Flag:");
		while(pointer!=tail)
	    {
		    if((pointer->gap)==1)
		    {
		    	int counter=0;
		    	while(counter<(pointer->size))
		    	{
		    		printf(" ");
		    		counter++;
		    	}
		    }
		    else
				printf("|0%lu|",pointer->flag);
	    	pointer++;
	    }

	    	if((pointer->gap)==1)
		    {
		    	printf("|%lu more spaces|\n",pointer->size);
		    }
		    else
				printf("|0%lu|\n",pointer->flag);
		printf("----------------------------------------\n");

		//ordering
		pointer=head;
		int num=1;
		printf("     ");
		while(pointer!=tail)
	    {
		    if((pointer->gap)==1)
		    {
		    	int counter=0;
		    	while(counter<(pointer->size))
		    	{
		    		printf(" ");
		    		counter++;
		    	}
		    }
		    else
		    {
				printf("|0%d|",num);
				num++;
		    }
	    	pointer++;
	    }

	    	if((pointer->gap)==1)
		    {
		    	
		    }
		    else
		    {
				printf("|0%d|",num);
				num++;
		    }

		printf("\nYou may choose to:\n");
		printf("1. Sort memory blocks\n");
		printf("2. Choose a memory block number X (X is some number)\n");
		printf("3. Swap memory blocks X and Y of the same size\n");
		printf("4. Exit\n");
		printf("Please enter a number (1-4) :");
		scanf("%d",&option);
		printf("\n");

		if(option==1)
		{
			block =  (uint64_t*) ram;
		    uint64_t* head1=(uint64_t*)tmp_buf;
		    uint64_t* tail1=(uint64_t*)tmp_buf;

		    int read=0;
		    int bytesput=0;

		    while(read<1024)
		    {
		        uint64_t size=((*block >> 3)<<3);
		        uint64_t id=((*block <<61)>>62);
		        uint64_t flag=(*block & 1);

		        //gap
		        if(size==0)
		        {
		            block++;
		            read+=8;
		        }
		        
		        else if(size==16)
		        {
		            block++;
		            block++;
		            read+=16;
		        }

		        //first block
		        else if(head1==tail1)
		        {
		            memmove(head1,block,size);
		            read+=size;
		            bytesput+=size;
		            tail1=tail1+(size/8);
		            //go to next block
		            block=block+(size/8);

		            if(bytesput>=1024)
		            {
		                printf("SBRK ERROR\n");
		                exit(errno);
		            }
		        }
		        //we need to compare values 
		        else 
		        {
		            uint64_t* pointer=(uint64_t*)head1;
		            int put=0;

		            while(put==0)
		            {
		                uint64_t p_size=((*pointer >> 3)<<3);
		                uint64_t p_id=((*pointer <<61)>>62);
		                uint64_t p_flag=(*pointer & 1);
		                int bytespassed=0;

		                if(pointer==tail1)
		                {
		                    memmove(tail1,block,size);
		                    put=1;
		                }
		                else if(id==p_id)
		                {
		                    if(flag==1&&p_flag==0)
		                    {
		                        memmove((pointer+(size/8)),pointer,(bytesput-bytespassed));
		                        memmove(pointer,block,size);   
		                        put=1;  
		                    }
		                    else if(flag==1&&p_flag==1)
		                    {
		                        if(size>p_size)
		                        {
		                            bytespassed+=p_size;
		                            pointer+=(p_size/8);
		                        }
		                        else{
		                            memmove((pointer+(size/8)),pointer,(bytesput-bytespassed));
		                            memmove(pointer,block,size);   
		                            put=1;
		                        }

		                    }
		                    else if(flag==0&&p_flag==0)
		                    {
		                        if(size>p_size)
		                        {
		                            bytespassed+=p_size;
		                            pointer+=(p_size/8);
		                        }
		                        else{
		                            memmove((pointer+(size/8)),pointer,(bytesput-bytespassed));
		                            memmove(pointer,block,size);   
		                            put=1;    
		                        }
		                    }
		                    else
		                    {
		                        bytespassed+=p_size;
		                        pointer+=(p_size/8);
		                    }

		                }
		                else if(id<p_id)
		                {
		                    memmove((pointer+(size/8)),pointer,(bytesput-bytespassed));
		                    memmove(pointer,block,size);   
		                    put=1;            
		                }
		                else
		                {   
		                    bytespassed+=p_size;
		                    pointer+=(p_size/8);
		                }
		            }   

		            read+=size;
		            bytesput+=size;
		            tail1=tail1+(size/8);
		            //go to next block
		            block=block+(size/8);

		            if(bytesput>=1024)
		            {
		                printf("SBRK ERROR\n");
		                exit(errno);
		            }
		        }
		    }


		    if(bytesput>=1024)
		    {
		        printf("SBRK ERROR\n");
		        exit(errno);
		    }

		    //adding the last block
		    uint64_t lastblock=16;
		    *tail1=lastblock;
		    bytesput+=8;

		    if(bytesput>=1024)
		    {
		        printf("SBRK ERROR\n");
		        exit(errno);
		    }

		    tail1++;
		    *tail1=lastblock;
		    bytesput+=8;

		    tail1--;

		    //coalesce
		    uint64_t* pointer=(uint64_t*)head1;
		    int done=0;
		    while(done==0)
		    {
		        uint64_t p_size=((*pointer >> 3)<<3);
		        uint64_t p_id=((*pointer <<61)>>62);
		        uint64_t p_flag=(*pointer & 1);

		        uint64_t* next=pointer+(p_size/8);
		        uint64_t n_size=((*next >> 3)<<3);
		        uint64_t n_id=((*next <<61)>>62);
		        uint64_t n_flag=(*next & 1);
		        if(next==tail1)
		        {
		            done=1;
		        }
		        else
		        {
		            if(p_id==n_id&&p_flag==0&&n_flag==0)
		            {
		                uint64_t idid=(p_id<<1);
		                *pointer=(p_size+n_size+idid);
		                next+=((n_size/8)-1);
		                *next=(p_size+n_size+idid);
		            }
		            else
		            {
		                pointer+=(p_size/8);
		            }
		        }
		    }

		    //put back to ram
		    memmove(ram,tmp_buf,bytesput);
		    
		    //scan all blocks in ram
		    block =  (uint64_t*) ram;

			head=(mblock*)tmp_buf;
		    tail=(mblock*)tmp_buf;

		    read=0;
		    mblock* first=NULL;
		    
		    while(read<1024)
		    {
		    	uint64_t size=((*block >> 3)<<3);
		        uint64_t id=((*block <<61)>>62);
		        uint64_t flag=(*block & 1);
		        
		        if(first==NULL)
		        {
		        	first=head;
		        	if(size==0)
		        	{
		        		first->gap=1;
		        		first->size=8;

						//go to next block
		        		block++;
		            	read+=8;
		        	}
		        	else
		        	{
		        		first->gap=0;
			        	first->size=size;
			        	first->id=id;
			        	first->flag=flag;

			        	//go to next block
			        	read+=size;
				        block=block+(size/8);
		        	}
		        }
		        else
		        {
		        	if(size==0)
		        	{
		        		if((tail->gap)==1)
		        		{
		        			tail->size+=8;

							//go to next block
			        		block++;
			            	read+=8;
		        		}
		        		else
		        		{
		        			tail++;
		        			mblock* newB=tail;

		        			newB->gap=1;
			        		newB->size=8;

							//go to next block
			        		block++;
			            	read+=8;
		        		}
		        	}
		        	else
		        	{
		        		tail++;
		        		mblock* newB=tail;

						newB->gap=0;
			        	newB->size=size;
			        	newB->id=id;
			        	newB->flag=flag;

			        	//go to next block
			        	read+=size;
				        block=block+(size/8);
		        	}
		        }
		    }
		}
		else if(option==4)
		{
			cse320_free();
			exit(0);
		}
		//move
		else if(option==2)
		{
			int b1;
			printf("Please enter a number:");
			scanf("%d",&b1);
			printf("\n");
			if(b1<=0||b1>=num)
			{
				printf("Invalid input!\n\n");
			}
			else
			{
				char move;
				printf("press key 'a' to move the block left by 1 byte,or key 'd' to move the block right by 1 byte: ");
				scanf(" %c", &move);

				//count how many blocks are there
				int h=1;
				mblock* ptr=head;
				while (ptr!=tail)
				{
					h++;
					ptr++;
				}

				//clean all gap with 0 size 
								int n=1;
								mblock* ptrptr=head;
								while (ptrptr!=tail)
								{
									if(ptrptr->gap==1&&ptrptr->size==0)
									{
										memmove(ptrptr,ptrptr+1,((h-n)*sizeof(mblock)));
										tail--;
									}
									else
									{
										n++;
										ptrptr++;
									}
								}
								if(tail->gap==1&&tail->size==0)
									{
										tail--;
									}
								

				//that block
				mblock* block1;
					ptr=head;
					int i=0;
					while (ptr!=tail)
					{
						if(ptr->gap!=1){
							i++;
							if(i==b1)
							{
								block1=ptr;
							}
						}
						ptr++;
					}
						if(tail->gap!=1){
							i++;
							if(i==b1)
							{
								block1=tail;
							}
						}

				if(move=='a')
				{
					if((block1-1)->gap!=1||block1==head)
					{
						printf("ERROR\n\n");
					}
					else
					{
							(block1-1)->size=((block1-1)->size)-1;
							if(block1==tail)
							{
								tail++;
								mblock* newBlock=tail;
								newBlock->gap=1;
								newBlock->size=1;
							}
							else if((block1+1)->gap==1)
							{
								(block1+1)->size=((block1+1)->size)+1;
							}
							else
							{
								int n=1;
								mblock* ptrptr=head;
								while (ptrptr!=block1)
								{
									n++;
									ptrptr++;
								}
								memmove(block1+2,block1+1,((h-n)*sizeof(mblock)));
								(block1+1)->gap=1;
								(block1+1)->size=1;
								tail++;
							}
						}

				}
				else if(move=='d')
				{
					if((block1+1)->gap!=1||block1==tail)
					{
						printf("ERROR \n\n");
					}
					else
					{
						(block1+1)->size=((block1+1)->size)-1;
							if(block1==head)
							{
								memmove(head+1,head,h*sizeof(mblock));
								head->gap=1;
								head->size=1;
								tail++;
							}
							else if((block1-1)->gap==1)
							{
								(block1-1)->size=((block1-1)->size)+1;
							}
							else
							{
								
								int n=1;
								mblock* ptrptr=head;
								while (ptrptr!=block1)
								{
									n++;
									ptrptr++;
								}

								printf("h=%d, n=%d \n\n",h,n);
								memmove((block1+1),block1,((h-n+1)*sizeof(mblock)));
								tail++;
								block1->gap=1;
								block1->size=1;
							}
						}
				}	
				else
				{
					printf("Invalid input!\n\n");
				}	
			}		
		}
		//swap
		else if(option==3)
		{
			int b1;
			int b2;

			printf("Please enter a number:");
			scanf("%d",&b1);
			printf("\n");
			printf("Please enter a number:");
			scanf("%d",&b2);
			printf("\n");

			if(b1<=0||b2<=0||b1>=num||b2>=num)
			{
				printf("Invalid input!\n\n");
			}
			else
			{
				//check if same size
				
				mblock* block1;
				mblock* block2;

				mblock* ptr=head;
				int i=0;
				while (ptr!=tail)
				{
					if(ptr->gap!=1){
						i++;
						if(i==b1)
						{
							block1=ptr;
						}
					}
					ptr++;
				}
					if(tail->gap!=1){
						i++;
						if(i==b1)
						{
							block1=tail;
						}
					}

				ptr=head;
				i=0;
				while (ptr!=tail)
				{
					if(ptr->gap!=1){
						i++;
						if(i==b2)
						{
							block2=ptr;
						}
					}
					ptr++;
				}
					if(tail->gap!=1){
						i++;
						if(i==b2)
						{
							block2=tail;
						}
					}	

				if((block1->size)==16||(block2->size)==16)
				{
					printf("It's the last block!\n\n");
				}
				else if((block1->size)!=block2->size)
				{
					printf("These two blocks are not the same size!\n\n");
				}
				else
				{
					uint64_t tempId=block1->id;
					uint64_t tempFlag=block1->flag;
					block1->id=block2->id;
					block1->flag=block2->flag;
					block2->id=tempId;
					block2->flag=tempFlag;
				}
			}
		}
		else
		{
			printf("Invalid input!\n\n");
		}

	}
	/*
     * Do not modify code below.
     */
    //cse320_check();
    cse320_free();
    return ret;
}