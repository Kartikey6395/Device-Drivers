#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main()
{
    int Ubuff[1];
    int Kbuff[]={45,10};
    int fd;
    fd = open("/dev/MySUB_DEV",O_RDWR);
    if(fd<0)
    {
        perror("device is not accessible\n \n");
        return(-1);
    }

    write(fd,Kbuff,sizeof(Kbuff));
    printf(" writing from user---%d \n ",Kbuff[0]);
    printf("device accessed successfully from user space\n");
    read(fd,Ubuff,sizeof(Ubuff));
    printf("reading data in user buff through kernel\n ");
    printf("Data read ---%d\n",Ubuff[0]);
    

    return close(fd);

}