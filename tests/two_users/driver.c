#include<linux/device.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/init.h>

int str_len=0;
char str[100];
char extend[]="from kernel";
char final_str[100];

dev_t two_dev;
static struct cdev two_cdev;
static struct class *two_dev_class;

static int __init my_init(void);
static void __exit my_diver_exit(void);

static int my_open(struct inode *inode , struct file *file);
static int my_close(struct inode *inode , struct file *file);
static ssize_t my_read (struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write (struct file *filp, const char __user *buf, size_t len, loff_t *off);

static struct file_operations my_fops = 
{
    .owner = THIS_MODULE,
    .read  =  my_read,
    .write =  my_write,
    .open  =  my_open,
    .release = my_close,
};

static int __init my_init(void)
{
    if((alloc_chrdev_region(&two_dev,0,1,"dev_num"))<0)
    {
        pr_info("major and minor number not allocated\n");
        return -1;
    }
    pr_info("major :%d minor :%d",MAJOR(dev),MINOR(dev));

    cdev_init(&two_cdev,&my_fops);
    if((cdev_add(&two_cdev,dev,1))<0)
    {
        pr_info("driver not linked with kernel\n");
        goto class_del;
    }

    two_dev_class = class_create("mydev_class");
    if(IS_ERR(two_dev_class))
    {
        pr_err("class not allocated\n");
        goto class_del;
    }

    if(IS_ERR(device_create(two_dev_class,NULL,dev,NULL,"mydev_device")))
    {
        pr_err("device cretaion failed\n");
        goto device_del;
    }
    pr_info("module inserted successful\n");
    return 0;
class_del:
    unregister_chrdev_region(two_dev,1);
device_del:
    class_destroy(two_dev_class);
    unregister_chrdev_region(two_dev,1);
    return -1;
}
static void __exit my_diver_exit(void)
{
    device_destroy(two_dev_class,two_dev);
    class_destroy(two_dev_class);
    unregister_chrdev_region(two_dev,1);
}

static int my_open(struct inode *inode , struct file *file)
{
    pr_info("device invoked open  \n");
    return 0;
}
static int my_close(struct inode *inode , struct file *file)
{
    pr_info("device invoked close  \n");
    return 0;
}
static ssize_t my_read (struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    pr_info("writing from  kernel buffer\n");
    //sprintf(final_str,"%s %s",str,extend);
    if(copy_to_user(buf,str,str_len))
    {
        return -EINVAL;
    }
   pr_info("kernel sending msg: %s\n",str);
   memset(str,'\0',str_len);
    return len;
}
static ssize_t my_write (struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    str_len = len;
    pr_info("writing to  kernle buffer\n");
    if(copy_from_user(str,buf,len))
    {
        return -EINVAL;
    }
    pr_info("user sent msg : %s\n",str);
   
    return len;
}
module_init(my_init);
module_exit(my_diver_exit);

MODULE_LICENSE("GPL");
