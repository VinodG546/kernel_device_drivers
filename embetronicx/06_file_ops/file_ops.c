#include<linux/kernel.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>

static struct class *dev_class; 
dev_t dev=0;
static struct cdev u_cdev;

static int my_open(struct inode * inode,struct file *file);
static int my_release(struct inode *inode ,struct file *file);
static ssize_t my_read(struct file *filp,char __user * buf,ssize_t len,loff_t* offt);
static ssize_t my_write(struct file *filp,const char * buf,ssize_t len,loff_t* offt);

static struct file_operations my_fops = 
{
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
	.open = my_open,
	.release = my_release,
};

/*implementing fops functions*/
static int my_open(struct inode * inode,struct file *file)
{
	pr_info("Driver open function called Major : %d Minor : %d\n",MAJOR(dev),MINOR(dev));
	return 0;
}
static int my_release(struct inode *inode ,struct file *file)
{
	pr_info("Driver close function called Major : %d Minor : %d\n",MAJOR(dev),MINOR(dev));
	return 0;
}
static ssize_t my_read(struct file *filp,char __user * buf,ssize_t len,loff_t* offt)
{
	pr_info("Driver read function called Major:%d Minor:%d \n",MAJOR(dev),MINOR(dev));
	return len;
}
static ssize_t my_write(struct file *filp,const char * buf,ssize_t len,loff_t* offt)
{
	pr_info("Driver write function called Major : %d minor : %d\n",MAJOR(dev),MINOR(dev));
	return 0;
}
 
static int __init number_init(void)
{
	pr_info("%s\n",__func__);
	if((alloc_chrdev_region(&dev,0,1,"chr_device")<0)){
		pr_info("cannot allocate major and minor number\n");
		return -1;
	}
	pr_info("MAJOR = %d , MINOR = %d",MAJOR(dev),MINOR(dev));
	
	/*cretaing cdev struct*/
	cdev_init(&u_cdev,&my_fops);
	if(IS_ERR(cdev_add(u_cdev,dev,1))<0)
	{
		pr_err("cannot add the device to the system\n");
		goto r_class;
	}

	dev_class=class_create("device_class");
	if(IS_ERR(dev_class)){
		pr_err("cannot cretae the device\n");
		goto r_class;
	}
	
	if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"chr_dev"))){
		pr_err("cannot create the device\n");
		goto r_device;
	}
	
	pr_info("kernel module inserted successfully\n");
	return 0;
r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}

static void __exit number_exit(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(u_cdev);
	unregister_chrdev_region(dev,1);
	pr_info("kernel module removed successfully\n");
}

module_init(number_init);
module_exit(number_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VINOD");

		
