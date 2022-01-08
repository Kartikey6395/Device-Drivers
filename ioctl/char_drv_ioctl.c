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
#include <linux/ioctl.h>

#include "ioctl.h"

#define DEV dev_ioctl1

struct uconfig
{
    int set_baud_rate;
    int get_baud_rate; 
    int parity;
    int set_bit;
    int clear_bit; 
};

//function Prototypes
int DEV_open(struct inode *inode, struct file *filp);
int DEV_release(struct inode *inode, struct file *filp);
//ssize_t DEV_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);
//ssize_t DEV_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp);
long  DEV_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);


//Structure of file operations for the driver
struct file_operations fops =
{

    .open    = DEV_open,
    //.read    = DEV_read,
    //.write   = DEV_write,
    .release = DEV_release,
    .unlocked_ioctl   = DEV_ioctl,

};

//structure for character driver
struct cdev *my_cdev;

//init Module
static int __init CharDevice_init(void)
{
    int result;
    int MAJOR,MINOR;
    dev_t DevNum;
    DevNum = MKDEV(255,0);//Creating device number
    MAJOR = MAJOR(DevNum);
    MINOR = MINOR(DevNum);
    printk("\n major no--%d ... minor no --%d\n",MAJOR,MINOR);
    result = register_chrdev_region(DevNum,1,"DEV_ioctl");    //register device region
    if(result<0)
    {
        printk(KERN_ALERT "\nthe region is not obtainable");
        return(-1);
    }

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
    int MAJOR,MINOR;
    dev_t DevNum;
    DevNum = MKDEV(255,0);//Creating 32bit device number 
    MAJOR = MAJOR(DevNum);
    MINOR = MINOR(DevNum);
    printk("\n major no--%d ... minor no --%d\n",MAJOR,MINOR);
    
    unregister_chrdev_region(DevNum,1);
    cdev_del(my_cdev);
    printk("\n unregistered my device and allocated memory\n");
    return;

}

//open system call
int DEV_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "DEV_ioctl is now open mode \n");
    return 0;
}

//syscal for ioctl functions
long DEV_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{   unsigned int BAUD_VALUE,retval1,retval2,retval3,retval4,SET,RESET,result;
    struct uconfig *ptr;
    int sendBR = 9600;
    int Ubuff[20];
    //ptr = (struct uconfig *)kmalloc(sizeof(struct uconfig),GFP_DMA);
    switch(cmd)
    {
        case ALL_DEFINE_CONFIGS:
            ptr = (struct uconfig *)Ubuff;
            result = copy_from_user(ptr, (struct uconfig *)arg, sizeof(struct uconfig));
            printk("struct baud rate %d\n",ptr->set_baud_rate);
            printk("Parity check -- %d\n",ptr->parity);
            printk("set all bits HIGH --%d\n",ptr->set_bit);
            printk("reset LSB --%d\n",ptr->clear_bit);
            printk("copy from user return %d ",result);
            break;
        case SET_BAUD_RATE:
            retval1 = __get_user(BAUD_VALUE,(int __user*)arg);
            printk(KERN_INFO "BAUD RATE ---%d\n",BAUD_VALUE);
            break;
        case SET_BIT:
            retval2 = __get_user(SET,(int __user*)arg);
            printk( "SET BIT---%d\n",SET);
            break;
        case RESET_BIT:
            retval3 = __get_user(RESET,(int __user*)arg);
            printk(KERN_INFO "RESET BIT---%d\n",RESET);
            break;
        case GET_BAUD_RATE:
            retval4 = __put_user(sendBR,(int __user*)arg);
            break;
        default:
            printk("wrong input\n");
            return(-EINVAL);
    }
    
    
    
    //printk(KERN_INFO "Set bit copy_from_user--%d",ptr->set_baud_rate);
    printk("/*----*********----*/\n");
    
    return 0;
}



//close system call
int DEV_release(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "DEV_ioctl is now released from its duties \n");
    return 0;
}

module_init(CharDevice_init);
module_exit(CharDevice_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("KT");
MODULE_DESCRIPTION("export symbol for mod1 mod2");