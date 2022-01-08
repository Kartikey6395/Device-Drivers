//char driver implementation

//Initialization functions

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DEV MyCharDevice

//function Prototypes
int DEV_open(struct inode *inode, struct file *filp);
int DEV_release(struct inode *inode, struct file *filp);
ssize_t DEV_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);
ssize_t DEV_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp);


//Structure of file operations for the driver
struct file_operations fops =
{

    .open    = DEV_open,
    .read    = DEV_read,
    .write   = DEV_write,
    .release = DEV_release,

};

//structure for character driver
struct cdev *my_cdev;
dev_t DevNum;

//init Module
static int __init CharDevice_init(void)
{
    int result;
    int MAJOR,MINOR;
    
    //DevNum = MKDEV(255,0);//Creating device number
    
    result = alloc_chrdev_region(&DevNum,11,1,"MyCharDevice");    //dynamic register device region
    MAJOR = MAJOR(DevNum);
    MINOR = MINOR(DevNum);
    printk("\n major no--%d ... minor no --%d\n",MAJOR,MINOR);
    if(result<0)
    {
        printk(KERN_ALERT "\nthe region is not obtainable");
        return(-1);
    }
    printk("\n device number-- %d\n",DevNum);

    my_cdev = cdev_alloc(); //allocte memory to char device structure
    my_cdev->ops = &fops;   //link our file operatons to char device

    result = cdev_add(my_cdev,DevNum,1); //notify the kernel about new dvice
    if(result<0)
    {
        printk(KERN_ALERT "\nthe char device has not been created...!!\n");
        unregister_chrdev_region(DevNum,1);
        return(-1);
    }
    return 0;
}

//cleanup module
static void __exit CharDevice_exit(void)
{
    int result;
    //int MAJOR,MINOR;
    //dev_t DevNum;
    //DevNum = MKDEV(255,0);//Creating device number
    //MAJOR = MAJOR(DevNum);
    //MINOR = MINOR(DevNum);
    //printk("\n major no--%d ... minor no --%d\n",MAJOR,MINOR);
    
    unregister_chrdev_region(DevNum,1);
    cdev_del(my_cdev);
    printk("\n unregistered my device and allocated memory\n");
    return;

}

//open system call
int DEV_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "MyCharDevice is now open mode \n");
    return 0;
}

//read system call
ssize_t DEV_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{
    char Kbuff[] = "This data is in kernel buff to user buff \n";
    unsigned long result;
    size_t ret;
    //copy Kbuff from kernel to Ubuff into user : copy-to-user
    result = copy_to_user((char *)Ubuff,(char *)Kbuff,sizeof(Kbuff));
    if(result == 0)
    {
        printk("Data read successfully by user");
        printk("value of count = %ld \n",count);
        ret = count;
        return ret;
    }
    else if(result > 0)
    {
        printk("Data remaining not read completely \n");
        ret = count - result;
        printk("bytes of data read succesfully== %ld\n",ret);
        printk("bytes left to read to user ==%ld \n",result);
        return ret;

    }
    else
    {
        printk("error in writing to user from kernel buff \n");
        ret  =-EFAULT;
        return ret;
    }
}

//write syscall
ssize_t DEV_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp)
{
    char Kbuff[100];
    unsigned long result;
    size_t ret;
    //copy from Ubuff from user to Kbuff in kernel : copy-from-user
    result = copy_from_user((char *)Kbuff,(char *)Ubuff,count);
    if(result==0)
    {
        printk("Data written successfully by user into kernel");
        printk(KERN_ALERT "DATA--%s",Kbuff);
        printk("value of count..bytes written = %ld \n",count);
        ret = count;
        return ret;
    }
    if(result>0)
    {
        printk("Data remaining.. not written completely \n");
        ret = count - result;
        printk(KERN_ALERT "DATA--%s",Kbuff);
        printk("bytes of data written succesfully== %ld\n",ret);
        printk("bytes left to write into kernel ==%ld \n",result);
        return ret;
    }
    else
    {
        printk("error in reading from user to kernel buff \n");
        ret  =-EFAULT;
        return ret;
    }

}

//close system call
int DEV_release(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "MyCharDevice is now released from its duties \n");
    return 0;
}



module_init(CharDevice_init);
module_exit(CharDevice_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("KT");
MODULE_DESCRIPTION("read operations");

