#include "helper.h"
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

extern int errno ;
void* fp;

//EC1
void superfree();

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

    fp=ram;

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
    //check for errors

    uint64_t* block =  (uint64_t*) ram;
    uint64_t* head=(uint64_t*)tmp_buf;
    uint64_t* tail=(uint64_t*)tmp_buf;

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

        //first block
        else if(head==tail)
        {
            memmove(head,block,size);
            read+=size;
            bytesput+=size;
            tail=tail+(size/8);
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
            uint64_t* pointer=(uint64_t*)head;
            int put=0;

            while(put==0)
            {
                uint64_t p_size=((*pointer >> 3)<<3);
                uint64_t p_id=((*pointer <<61)>>62);
                uint64_t p_flag=(*pointer & 1);
                int bytespassed=0;

                if(pointer==tail)
                {
                    memmove(tail,block,size);
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
            tail=tail+(size/8);
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
    *tail=lastblock;
    bytesput+=8;

    if(bytesput>=1024)
    {
        printf("SBRK ERROR\n");
        exit(errno);
    }

    tail++;
    *tail=lastblock;
    bytesput+=8;

    tail--;

    //coalesce
    uint64_t* pointer=(uint64_t*)head;
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
        if(next==tail)
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

    /*
     * Do not modify code below.
     */
    cse320_check();
    cse320_free();
    return ret;
}

//EC
void superfree()
{
    uint64_t*  pointer1=  (uint64_t*) fp;
    int done1=0;

    //change ID to 0
    while(done1==0)
    {
        uint64_t p_size=((*pointer1 >> 3)<<3);
        uint64_t p_id=((*pointer1 <<61)>>62);
        uint64_t p_flag=(*pointer1 & 1);

        //last block
        if(p_size==16&&p_flag==0&&p_id==0)
        {
            done1=1;
        }
        else
        {
            if(p_flag==0)
            {
                *pointer1=p_size;
            }
        }
        pointer1+=(p_size/8);
    }

    //coalesce
    uint64_t* pointer=(uint64_t*)fp;
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
        if(n_size==16&&n_flag==0&&n_id==0)
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
}
