#include<linux/kernel.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/init.h>


dev_t dev=0;

static int __init number_init(void)
{
	pr_info("%s\n",__func__);
	if((alloc_chrdev_region(&dev,0,1,"chr_device")<0)){
		pr_info("cannot allocate major and minor number\n");
		return -1;
	}
	pr_info("MAJOR = %d , MINOR = %d",MAJOR(dev),MINOR(dev));
	return 0;
}

static void __exit number_exit(void)
{
	unregister_chrdev_region(dev,1);
	pr_info("kernel module removed successfully\n");
}

module_init(number_init);
module_exit(number_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VINOD");

		
