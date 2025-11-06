#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/err.h>
#include<linux/init.h>
#include<linux/device.h>
#include<linux/ctype.h>

#define BUF_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VINOD");
dev_t dev_num=0;
static struct cdev  cdev_num;
static struct class * dev_class;

static char kernel_buffer[BUF_SIZE];
static size_t data_size = 0;

static int my_open(struct inode *inode, struct file *file);
static int my_close(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);

static struct file_operations my_fops=
{
	.owner=THIS_MODULE,
	.read=my_read,
	.write=my_write,
	.open=my_open,
	.release=my_close
};

static int my_open(struct inode *inode, struct file *file)
{
	pr_info("device called open function\n");
	return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
	pr_info("device called close function\n");
	return 0;
}

static ssize_t my_write(struct file *filp,const char __user*buf,size_t len,loff_t*off)
{
	size_t i;
	if(len > BUF_SIZE -1)
		len = BUF_SIZE -1;
	if(copy_from_user(kernel_buffer , buf ,len))
		return -EFAULT;
	kernel_buffer[len]='\0';
	data_size=len;

	int a,b;
	sscanf(kernel_buffer,"%d %d",&a,&b);
	memset(kernel_buffer,0,sizeof(kernel_buffer));
	sprintf(kernel_buffer,"%d",a+b);
//	kernel_buffer[strlen(kernel_buffer)]='\0';
	pr_info("addition of %d and %d : %d \n",a,b,a+b);
	return len;	
} 
static ssize_t my_read(struct file *filp ,char __user * buf,size_t len, loff_t *off)
{
	
	if (*off >= data_size)
        return 0;

    if (len > data_size - *off)
        len = data_size - *off;

    if (copy_to_user(buf, kernel_buffer + *off, len))
        return -EFAULT;

    *off += len;
    pr_info("Sent to user: %s\n", kernel_buffer);
    return len;
}
static int __init dev_driver_init(void)
{
	if(alloc_chrdev_region(&dev_num,0,1,"cdev_num")<0)
	{
		pr_info("major and minor not allocated\n");
		return -1;
	}
	cdev_init(&cdev_num,&my_fops);
	if(cdev_add(&cdev_num,dev_num,1)<0)
	{	
		pr_info("cdev not registered\n");
		goto class_del;
	}
	dev_class=class_create("chrdev_class");
	if(IS_ERR(dev_class))
	{
		pr_info("class is not created\n");
		goto class_del;
	}
	if(IS_ERR(device_create(dev_class , NULL , dev_num , NULL , "chrdev_device")))
	{
		pr_info("device is not cretaed\n");
		goto device_del;
	}
	return 0;
class_del:
		unregister_chrdev_region(dev_num,1);
device_del:
		class_destroy(dev_class);
	return -1;
}

static void __exit driver_exit(void)
{
	device_destroy(dev_class,dev_num);
	class_destroy(dev_class);
	cdev_del(&cdev_num);
	unregister_chrdev_region(dev_num,1);
}

 module_init(dev_driver_init);
 module_exit(driver_exit); 
