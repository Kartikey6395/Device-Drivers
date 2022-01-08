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
//ssize_t DEV_write(struct file *file, const char __user *Ubuff, size_t count, loff_t *offp);
//ssize_t DEV_read(struct file *file, char __user *Ubuff, size_t count, loff_t *offp);


//Structure of file operations for the driver
struct file_operations fops =
{

    .open    = DEV_open,
    //.read    = DEV_read,
    //.write   = DEV_write,
    .release = DEV_release,

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
    result = register_chrdev_region(DevNum,1,"MyCharDevice");    //register device region
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
    printk(KERN_ALERT "MyCharDevice is now open mode \n");
    return 0;
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
MODULE_DESCRIPTION("export symbol for mod1 mod2");