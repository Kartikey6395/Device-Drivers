//character driver
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/completion.h>
#include <linux/wait.h>

#define DEV wait_q

struct completion comp;
struct semaphore sem1;
//struct semaphore sem2;
wait_queue_head_t drv_q;
char Kbuff[30];
int flag = 0;

//function prototypes
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
    int MAJOR,MINOR,result;
    sema_init(&sem1,1);
    //sema_init(&sem2,1);
    //init_completion(&comp);
    init_waitqueue_head(&drv_q);

    result = alloc_chrdev_region(&DevNum,11,1,"Dev_wait_q");
    MAJOR = MAJOR(DevNum);
    MINOR = MINOR(DevNum);
    printk("major no-->>%d.....minor no-->>%d \n",MAJOR,MINOR);
    if(result<0)
    {
        printk(KERN_ALERT "\nthe region is not obtainable");
        return(-1);
    }
    printk("device num-->>%d\n",DevNum);

    my_cdev = cdev_alloc(); //allocte memory to char device structure
    my_cdev->ops = &fops;   //link our file operatons to char device
   // cdev_init(my_cdev,&fops);

    result = cdev_add(my_cdev,DevNum,1);
     if(result<0)
    {
        printk(KERN_ALERT "\nthe char device has not been created...!!\n");
        unregister_chrdev_region(DevNum,1);
        return(-1);
    }
    return 0;

}

//cleanup function
static void __exit CharDevice_exit(void)
{
    unregister_chrdev_region(DevNum,1);
    cdev_del(my_cdev); 
    printk("\n unregistered my device and allocated memory\n");
    return;

}

//open system call
int DEV_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "MyDev is now open mode \n");
    return 0;
}

//read syscall
ssize_t DEV_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{
    unsigned long result;
    size_t ret;
    int ret_wait;
    //down_trylock(&sem2);
    //(wait_for_completion_interruptible(&comp)
    //copy Kbuff from kernel to Ubuff into user : copy-to-user
    ret_wait = wait_event_interruptible(drv_q,(flag==1));
    if(ret_wait==0)
    {
    result = copy_to_user((char *)Ubuff,(char *)Kbuff,sizeof(Kbuff));  
    flag=0;
    printk("Read-->>%s\n",Ubuff);
    }
    else
    return (-ERESTARTSYS);
    //printk("no data read \n");
    
    
    if(result == 0)
    {
        //printk("Read success-->>\n");
        //printk("bytes of data read= %ld \n",count);
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
    unsigned long result;
    size_t ret;
    down(&sem1);
    //copy from Ubuff from user to Kbuff in kernel : copy-from-user   
    result = copy_from_user(Kbuff,Ubuff,count); 
    flag = 1;
    printk(KERN_INFO "writer says-->>%s\n",Kbuff);
    wake_up_interruptible(&drv_q);
    up(&sem1);
    
    //complete(&comp);
    
    if(result==0)
    {
        //printk("Data written successfully by user into kernel\n");
       // printk(KERN_INFO "writer says-->>%s\n",Kbuff);
        
        //printk("no of bytes written = %ld \n",count);
        ret = count;
        return ret;
    }
    else if(result>0)
    {
        printk("Data remaining.. not written completely \n");
        ret = count - result;
        printk(KERN_ALERT "DATA process------%s",Kbuff);
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
    printk(KERN_ALERT "MyDev is now released from its duties \n");
    return 0;
}



module_init(CharDevice_init);
module_exit(CharDevice_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("KT");
MODULE_DESCRIPTION("read operations");

