#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/err.h>
#include<linux/uaccess.h>
#include<linux/init.h>
#include<linux/sysfs.h>
#include<linux/fs.h>
#include<linux/kobject.h>
#include<linux/interrupt.h>

#define IRQ_NO 11

static irqreturn_t irq_handler(int irq,void *dev_id){
	printk(KERN_INFO "Shared IRQ: Interrupt Occurred");
 	 return IRQ_HANDLED;
}

volatile int value = 0;

dev_t dev=0;

static struct class *dev_class;
static struct cdev cdev;
struct kobject *kobj_ref;

static int __init dev_driver_init(void);
static void __exit driver_exit(void);

static int my_open(struct inode *inode,struct file *file);
static int my_close(struct inode *inode,struct file *file);
static ssize_t my_read(struct file *filp , char __user *buf,size_t len,loff_t *off);
static ssize_t my_write(struct file*filp , const char __user *buf, size_t len,loff_t *off);

static ssize_t system_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf);
static ssize_t system_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf,size_t count);

//struct kobj_attribute isr_attr = __ATTR(value , 0666 , system_show , system_store);
struct kobj_attribute isr_attr = __ATTR(value, 0644, system_show, system_store);

static struct file_operations my_fops=
{
	.owner = THIS_MODULE,
	.read  = my_read,
	.write = my_write,
	.open  = my_open,
	.release = my_close,
};

static int my_open(struct inode *inode,struct file *file)
{

        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}
static int my_close(struct inode *inode,struct file *file)
{

        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}
static ssize_t my_read(struct file *filp , char __user *buf,size_t len,loff_t *off)
{
	printk(KERN_INFO "Read function\n");
//        asm("int $0x3B");  // Corresponding to irq 11
       irq_handler(IRQ_NO, NULL);
  
        return 0;
}
static ssize_t my_write(struct file*filp , const char __user *buf, size_t len,loff_t *off)
{

        printk(KERN_INFO "Write Function\n");
        return len;
}
    
static ssize_t system_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
{
	pr_info("sysfs-Read \n");
	return sprintf(buf,"%d",value);
}

static ssize_t system_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf,size_t count)
{
        printk(KERN_INFO "Sysfs - Write!!!\n");
        sscanf(buf,"%d",&value);
        return count;
}
static int __init dev_driver_init(void)
{
	if((alloc_chrdev_region(&dev,0,1,"dev_num"))<0)
	{
		pr_err("device number not allocated\n");
		return -1;
	}
	pr_info("major and minor number allocated MAJOR:MINOR  %d:%d\n",MAJOR(dev),MINOR(dev));
	
	cdev_init(&cdev,&my_fops);

	if((cdev_add(&cdev,dev,1))<0)
	{
		pr_err("cannot add the device to the system\n");
		goto class_del;
	}
	
	dev_class = class_create("isr_class");
	if(IS_ERR(dev_class))
	{
		pr_err("class not cretaed\n");
		goto class_del;
	}
	
	if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"isr_device"))){
		pr_err("cannot cretae device\n");
		goto device_del;
	}	
	
	kobj_ref = kobject_create_and_add("isr_sysfs",kernel_kobj);
	
	if(sysfs_create_file(kobj_ref,&isr_attr.attr)){
		printk(KERN_INFO"Cannot create sysfs file......\n");
                goto r_sysfs;
	}
	if(request_irq(IRQ_NO , irq_handler , IRQF_SHARED , "isr_device",(void*)(irq_handler))){
		printk(KERN_INFO "my_device: cannot register IRQ ");
                    goto irq;
	}
	printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    	return 0;
device_del:
	class_destroy(dev_class);
	unregister_chrdev_region(dev,1);
class_del:
	unregister_chrdev_region(dev,1);
irq:
	free_irq(IRQ_NO,(void*)(irq_handler));
r_sysfs:
	kobject_put(kobj_ref); 
        sysfs_remove_file(kernel_kobj, &isr_attr.attr);
	return -1;
}

	
static void __exit driver_exit(void)
{
	free_irq(IRQ_NO,(void*)(irq_handler));
	kobject_put(kobj_ref);
        sysfs_remove_file(kernel_kobj, &isr_attr.attr);
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
        unregister_chrdev_region(dev,1);
  	printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
module_init(dev_driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");

