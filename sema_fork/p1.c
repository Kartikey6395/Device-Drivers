//fork operation
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc,char const *argv[])
{
    pid_t id;
    int fd;
    ssize_t retval,retval2;
    char arr[]="writing from parent process ****";
    char Buff[]="writing from child process ####";
    char buffer[40];

    fd=open("/dev/MyDev",O_RDWR,0777);
    if(fd<0)
    {
        perror("device is not accessible\n %d\n");
        return(-1);
    }
    
    id=fork();              //opaque return type pid_t for fork
    printf("ID-->%d\n",id);
   
   
    if( id == 0)
    {
    printf("Child Process-->%d\n",getpid());
   // printf("Child Process-->%d\n",getppid());
    retval = write(fd,Buff,sizeof(Buff));
    if(retval<0)
    perror("child not wrote\n");
    printf("retval---%d",(int)retval);
    

    }
    else
    {
	 retval2 = read(fd,buffer,sizeof(buffer));
    if(retval2<0)
    perror("parent could not read \n");
    printf("reading buffer-----%s",buffer);

    printf("parent process--> %d\n",getpid());
    if(write(fd,arr,sizeof(arr))<0)
    perror("parent not wrote \n");

   
    }
    close(fd);
}
