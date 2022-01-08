#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include "ioctl.h"



//structure for ALL operatons accessiblility
struct uconfig
{
    int set_baud_rate;
    int get_baud_rate; 
    int parity;
    int set_bit;
    int clear_bit; 
};


int main()
{
    struct uconfig config ={9600,0,1,11,10};
    int fd,retval1,retval2,retval3,retval4,retval5;
    unsigned int Baudrate = 15200;
    int set=1,reset=0;
    unsigned int GetBaudRate;
    fd = open("/dev/dev_ioctl1",O_RDWR,0777);
    if(fd<0)
    {
        perror("device is not accessible\n %d\n");
        return(-1);
    }
    printf("device accessed successfully from user space\n");

    if((retval1 = ioctl(fd,ALL_DEFINE_CONFIGS,&config))<0)
    printf("Struct config--error ");
    //printf("get_baud_rate from kernel --%d",config.get_baud_rate);

    if((retval2 = ioctl(fd,SET_BAUD_RATE,&Baudrate))<0)
    printf("Baud rate set---error ");

    if((retval3 = ioctl(fd,SET_BIT,&set))<0)
    printf("set bit---error ");

    if((retval4 = ioctl(fd,RESET_BIT,&reset))<0)
    printf(" Reset bit---error ");

     if((retval5 = ioctl(fd,GET_BAUD_RATE,&GetBaudRate))<0)
    printf("Baud rate not got from kernel---error ");
    printf("get_baud_rate from kernel --%d",GetBaudRate);
    printf("baudrate %d\n ",config.set_baud_rate);

    return close(fd);

}