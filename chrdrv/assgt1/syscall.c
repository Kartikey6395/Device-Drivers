#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    int fd;
    fd = open("/dev/MyCharDevice",O_RDONLY);
    if(fd<0)
    {
        perror("device is not accessible\n %d\n",errno);
        return(-1);
    }
    printf("device accessed successfully from user space\n");

    return close(fd);

}