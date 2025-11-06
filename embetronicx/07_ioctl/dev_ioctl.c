#include<linux/kernel.h>
#include<linux/device.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/init.h>
#include<linux/err.h>
#include<linux/module.h>
#include<linux/ioctl.h>
#include<linux/uaccess.h>
#include<linux/cdev.h>

#define WR_VALUE _IOW('a','a',int32_t)
#define RD_VALUE _IOR('a','a',int32_t)

int32_t value =0;

dev_t dev=0;
static struct class *dev_class;
static struct cdev cdev;

static int      __init ioctl_driver_init(void);
static void     __exit ioctl_driver_exit(void);
static int      ioctl_open(struct inode *inode, struct file *file);
static int      ioctl_release(struct inode *inode, struct file *file);
static ssize_t  ioctl_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  ioctl_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     ioctl_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops={
	.owner = THIS_MODULE,
	.read  = ioctl_read,
	.open  = ioctl_open,
	.release = ioctl_release,
	.write = ioctl_write,
	.unlocked_ioctl = ioctl_ioctl,
};

static int      ioctl_open(struct inode *inode, struct file *file)
{
	pr_info("ioctl_open invoked\n");
	return 0;
}
static int      ioctl_release(struct inode *inode, struct file *file)
{
	pr_info("ioctl_close invoked\n");
	return 0;
}
static ssize_t  ioctl_read(struct file *filp, char __user *buf, size_t len,loff_t * off)
{
	pr_info("Read Function\n");
        return 0;
}
static ssize_t  ioctl_write(struct file *filp, const char *buf, size_t len, loff_t * off)
{
	pr_info("Write function\n");
        return len;
}
static long     ioctl_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd){
		case WR_VALUE:
			if(copy_from_user(&value,(int32_t*) arg,sizeof(value)))
			{	
				pr_err("Data write: ERR!\n");
			}
			pr_info("value = %d\n",value);
			break;
		case RD_VALUE:
			if(copy_to_user((int32_t *)arg,&value,sizeof(value)))
			{	
				pr_err("Data Read : ERR!\n");
			}
			break;
		default:
			pr_info("Default\n");
			break;
	}
	return 0;
}
static int __init ioctl_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
                pr_err("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&cdev,dev,1)) < 0){
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if(IS_ERR(dev_class = class_create("etx_class"))){
            pr_err("Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device"))){
            pr_err("Cannot create the Device 1\n");
            goto r_device;
        }
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}	
static void __exit ioctl_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(ioctl_driver_init);
module_exit(ioctl_driver_exit);
 
MODULE_LICENSE("GPL");
