//pthreads -->mutex :avoiding racecondition
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

          //shared resource
int fd;


void *read_thread_1(void*arg)
{
    //READER_1
    ssize_t retval;
    char read_buff[30];
    retval = read(fd,read_buff,sizeof(read_buff));
    if(retval<0)
    {
        printf("could not read \n");
    }
    printf("READER 1 says--->>%s\n",read_buff);
    
    
      
}

/*void *read_thread_2(void*arg)
{
    while(1)//READER_2
     {   
        
         read
        printf("Increment %d \n",count);
        
    
     }  
}*/

void *write_thread_1(void*arg)
{
    //WRITER_1
       
    ssize_t retval;
    char write1_buff[] = "morning";
    retval = write(fd,write1_buff,sizeof(write1_buff));
    if(retval<0)
    {
        printf("write_1 cannot happen\n");
    }
    printf("Writer 1 says--->>%s\n",write1_buff);
    
     
}

void *write_thread_2(void*arg)
{
    //WRITER_2
    ssize_t retval;
    char write2_buff[] = "evening";
    retval = write(fd,write2_buff,sizeof(write2_buff));
    if(retval<0)
    {
        printf("write_2 cannot happen \n");
    }
    printf("Writer 2 says--->>%s\n",write2_buff);
    

}


int main(int argc,const char *argv[])
{
    pthread_t writer_1_id,reader_1_id,reader_2_id,writer_2_id;
    
    printf("main thread:before creating hello thread \n");
               //semaphore init (opaque_data_type,0-->thread,sem=1)
    
    fd = open("/dev/wait_q",O_RDWR,0777);
    if(fd<0)
    {
        perror("device not accessible \n");
        return(-1);
    }

    pthread_create(&reader_1_id,NULL, read_thread_1,NULL);

    //pthread_create(&reader_2_id,NULL, read_thread_2,NULL);

    pthread_create(&writer_1_id,NULL, write_thread_1,NULL);  

    pthread_create(&writer_2_id,NULL, write_thread_2,NULL);

    pthread_join(reader_1_id,NULL);
    //pthread_join(reader_2_id,NULL);
    pthread_join(writer_1_id,NULL);
    pthread_join(writer_2_id,NULL);
    
    close(fd);
    printf("main thread:after creating Bye thread \n");
    return 0;
}