#include<linux/kernel.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/init.h>

static struct class *dev_class; 
dev_t dev=0;

static int __init number_init(void)
{
	pr_info("%s\n",__func__);
	if((alloc_chrdev_region(&dev,0,1,"chr_device")<0)){
		pr_info("cannot allocate major and minor number\n");
		return -1;
	}
	pr_info("MAJOR = %d , MINOR = %d",MAJOR(dev),MINOR(dev));
	
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
	unregister_chrdev_region(dev,1);
	pr_info("kernel module removed successfully\n");
}

module_init(number_init);
module_exit(number_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VINOD");

		
