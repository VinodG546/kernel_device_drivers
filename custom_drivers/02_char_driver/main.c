#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include<linux/export.h>

#define DEV_MEM_SIZE 512
char device_buffer[DEV_MEM_SIZE];

dev_t device_number;

struct cdev pcd_cdev;

struct device *chr_device;
struct class *chr_class;
// Prototypes
loff_t chr_llseek(struct file *filp, loff_t offt, int whence);
ssize_t chr_read(struct file *filp, char __user *buff, size_t count, loff_t *offt);
ssize_t chr_write(struct file *filp, const char __user *buff, size_t count, loff_t *offt);
int chr_open(struct inode *inode, struct file *filp);
int chr_release(struct inode *inode, struct file *filp);

loff_t chr_llseek (struct file *filp, loff_t offt, int whence)
{
	loff_t temp;
	pr_info("llseek requested\n");
	switch (whence)
	{
		case SEEK_SET:
			if((offt > DEV_MEM_SIZE) || (offt < 0))
				return -EINVAL;
			filp->f_pos = offt;
			break;
		case SEEK_CUR:
			temp = filp->f_pos + offt;
			if((temp>DEV_MEM_SIZE)||(temp<0))
				return -EINVAL;
			filp->f_pos = temp;
			break;
		case SEEK_END:
			temp = filp->f_pos + offt;
                        if((temp>DEV_MEM_SIZE)||(temp<0))
                                return -EINVAL;
			filp->f_pos = temp;
			break;
		default:
			return -EINVAL;
	}	
	return filp->f_pos;
}
ssize_t chr_read (struct file *filp, char __user *buff, size_t count, loff_t *offt)
{
	pr_info("read requested %zu bytes\n",count);
	pr_info("current file position = %lld\n",*offt);
        if((*offt + count)>DEV_MEM_SIZE)
        {
                count = DEV_MEM_SIZE - *offt;
        }

        if(copy_to_user(buff,device_buffer+*offt,count))
        {
                return -EFAULT;
        }

        *offt+=count;

        pr_info("Number of bytes successfully read = %zu\n",count);

        pr_info("updated file position = %lld\n",*offt);
        return count;

}
ssize_t chr_write (struct file *filp, const char __user *buff, size_t count, loff_t *offt)
{
	pr_info("write request %zu bytes\n",count);
	pr_info("current file position = %lld\n",*offt);
	if((*offt + count)>DEV_MEM_SIZE)
	{
		count = DEV_MEM_SIZE - *offt;
	}

	if(copy_from_user(device_buffer+*offt,buff,count))
	{
		return -EFAULT;
	}

	*offt+=count;

	pr_info("Number of bytes successfully written  = %zu\n",count);

	pr_info("updated file position = %lld\n",*offt);
	return count;
}
int chr_open (struct inode *inode, struct file *filp)
{
	pr_info("open was successful\n");
	return 0;
}
int chr_release(struct inode *inode, struct file *filp)
{
	pr_info("close was successful\n");
	return 0;
}
struct file_operations pcd_fops=
{
	.open=chr_open,
	.write=chr_write,
	.read=chr_read,
	.release=chr_release,
	.owner=THIS_MODULE,
	.llseek=chr_llseek
};

static int __init char_dev_init(void)
{
	alloc_chrdev_region(&device_number,0,1,"char_dev");
	
	 pr_info("in init function : %s : MAJOR:%d  MINOR:%d\n",__func__,MAJOR(device_number),MINOR(device_number));
	pcd_cdev.owner = THIS_MODULE;

	cdev_init(&pcd_cdev,&pcd_fops);

	cdev_add(&pcd_cdev,device_number,1);

	chr_class = class_create("chr_class");

	chr_device= device_create(chr_class,NULL,device_number,NULL,"chr_device");

	pr_info("Module init successful\n");

	return 0;
}
static void __exit char_dev_exit(void)
{
	device_destroy(chr_class,device_number);

	class_destroy(chr_class);

	cdev_del(&pcd_cdev);

	unregister_chrdev_region(device_number,1);

	pr_info("in exit function: %s\n",__func__);
}

module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_LICENSE("GPL");

