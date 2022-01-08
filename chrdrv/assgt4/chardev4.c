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

#define ADD_DEV MyADD_DEV
#define SUB_DEV MySUB_DEV

//function Prototypes
int ADD_DEV_open(struct inode *inode, struct file *filp);
int ADD_DEV_release(struct inode *inode, struct file *filp);
ssize_t ADD_DEV_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);
ssize_t ADD_DEV_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp);

int SUB_DEV_open(struct inode *inode, struct file *filp);
int SUB_DEV_release(struct inode *inode, struct file *filp);
ssize_t SUB_DEV_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);
ssize_t SUB_DEV_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp);

int sum,diff;

//Structure of file operations for the Add device
struct file_operations Add_fops =
{

    .open    = ADD_DEV_open,
    .read    = ADD_DEV_read,
    .write   = ADD_DEV_write,
    .release = ADD_DEV_release,

};
//Structure of file operations for the Sub device
struct file_operations Sub_fops =
{

    .open    = SUB_DEV_open,
    .read    = SUB_DEV_read,
    .write   = SUB_DEV_write,
    .release = SUB_DEV_release,

};

//structure for character driver
struct cdev *add_cdev;
struct cdev *sub_cdev;

//init Module
static int __init CharDevice_init(void)
{
    int result,add_result,sub_result;
    int MAJOR,MINOR;
    dev_t DevNum;
    DevNum = MKDEV(101,0);//Creating device number
    MAJOR = MAJOR(DevNum);
    MINOR = MINOR(DevNum);
    printk("\n major no--%d ... minor no --%d\n",MAJOR,MINOR);
    result = register_chrdev_region(DevNum,2,"CalCulater");    //register device region
    if(result<0)
    {
        printk(KERN_ALERT "\nthe region is not obtainable");
        return(-1);
    }

    add_cdev = cdev_alloc(); //allocte memory to char device structure
    add_cdev->ops = &Add_fops;   //link our file operatons to char device

    sub_cdev = cdev_alloc();
    sub_cdev->ops = &Sub_fops;


    add_result = cdev_add(add_cdev,DevNum,1); //notify the kernel about add dvice
    if(add_result<0)
    {
        printk(KERN_ALERT "\nthe char device has not been created...!!\n");
        unregister_chrdev_region(DevNum,1);
        return(-1);
    }

    sub_result = cdev_add(sub_cdev,DevNum,2); //notify the kernel about sub dvice
    if(sub_result<0)
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
    //int result;
    int MAJOR,MINOR;
    dev_t DevNum;
    DevNum = MKDEV(101,0);//Creating device number
    MAJOR = MAJOR(DevNum);
    MINOR = MINOR(DevNum);
    printk("\n major no--%d ... minor no --%d\n",MAJOR,MINOR);
    unregister_chrdev_region(DevNum,2);
    cdev_del(add_cdev);
    cdev_del(sub_cdev);
    printk("\n unregistered my device and allocated memory\n");
    return;

}

//SYSTEM CALL FUNCTIONS for ADDITION
//open ADD system call
int ADD_DEV_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "MyCharDevice is now open mode \n");
    return 0;
}



//read system call
ssize_t ADD_DEV_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{
    int Kbuff[1];
    unsigned long result;
    size_t ret;
    Kbuff[0] = sum;
    //copy Kbuff from kernel to Ubuff into user : copy-to-user
    result = copy_to_user(Ubuff,Kbuff,sizeof(Kbuff));
    if(result == 0)
    {
        printk("Data (ADD)read successfully by user");
        printk("value of count = %ld \n",count);
        printk("Reading sum value %d\n",sum);
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

//write syscall for ADD
ssize_t ADD_DEV_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp)
{
    int Kbuff[2];
    //int sum;
    unsigned long result;
    size_t ret;
    //copy from Ubuff from user to Kbuff in kernel : copy-from-user
    result = copy_from_user(Kbuff,Ubuff,count);
    if(result==0)
    {
        printk("Data(ADD) written successfully by user into kernel");
        printk(KERN_ALERT "DATA--%d %d ",Kbuff[0],Kbuff[1]);
        printk("value of count..bytes written = %ld \n",count);
        ret = count;

        //addition of numbers written     
            sum = Kbuff[0] + Kbuff[1];         
            printk(KERN_ALERT "(writing)ADD of numbers= %d \n",sum);
                   
        return sum;
    }
    if(result>0)
    {
        printk("Data remaining.. not written completely \n");
        ret = count - result;
        printk(KERN_ALERT "DATA--%d",Kbuff[0]);
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

//close ADD system call
int ADD_DEV_release(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "Calculater is now released from its duties \n");
    return 0;
}

/*....*****....*/

//SYSTEM CAL FUNCTONS for SUBTRACTION

//open SUB system call
int SUB_DEV_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "Calculater(SUB) is now open mode \n");
    return 0;
}

 //SUB read syscall
ssize_t SUB_DEV_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{
    int Kbuff[1];
    unsigned long result;
    size_t ret;
    Kbuff[0] = diff;
    //copy Kbuff from kernel to Ubuff into user : copy-to-user
    result = copy_to_user(Ubuff,Kbuff,sizeof(Kbuff));
    if(result == 0)
    {
        printk("Data read successfully by user");
        printk("value of count = %ld \n",count);
        printk("Reading diff value %d\n",diff);
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

//Write syscall for SUB
ssize_t SUB_DEV_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp)
{
    int Kbuff[2];
    
    unsigned long result;
    size_t ret;
    //copy from Ubuff from user to Kbuff in kernel : copy-from-user
    result = copy_from_user(Kbuff,Ubuff,count);
    if(result==0)
    {
        printk("(SUB)Data written successfully by user into kernel");
        printk(KERN_ALERT "DATA--%d",Kbuff[0]);
        printk("value of count..bytes written = %ld \n",count);
        //ret = count;

        //difference of numbers written
            diff = Kbuff[0] - Kbuff[1];
            printk(KERN_ALERT "(writing)SUB of numbers= %d \n",diff);
            
        return diff;
    }
    if(result>0)
    {
        printk("Data remaining.. not written completely \n");
        ret = count - result;
        printk(KERN_ALERT "DATA--%d",Kbuff[0]);
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

//close SUB system call
int SUB_DEV_release(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "Calculater is now released from its duties \n");
    return 0;
}




module_init(CharDevice_init);
module_exit(CharDevice_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("KT");
MODULE_DESCRIPTION("read operations");
