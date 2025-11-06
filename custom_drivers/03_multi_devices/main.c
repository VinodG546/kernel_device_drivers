#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include<linux/export.h>
#include <linux/errno.h>


#define NO_OF_DEVICES 4

#define DEV_MEM_SIZE_DEV1 1024
#define DEV_MEM_SIZE_DEV2 512
#define DEV_MEM_SIZE_DEV3 1024
#define DEV_MEM_SIZE_DEV4 512

/* permission flags */
#define RDONLY   0x1
#define WRONLY   0x10
#define RDWR     0x11

/*devices memory*/
char device_buffer1[DEV_MEM_SIZE_DEV1];
char device_buffer2[DEV_MEM_SIZE_DEV2];
char device_buffer3[DEV_MEM_SIZE_DEV3];
char device_buffer4[DEV_MEM_SIZE_DEV4];

/*Device private data structure*/
struct pcdev_private_data{
	char *buffer;
	unsigned size;
	const char *serial_number;
	int perm;
	struct cdev cdev;
	
};

/*driver private data structure*/
struct pcdrv_private_data
{
	int total_devices;
	dev_t device_number;
	struct device *chr_device;
 	struct class *chr_class;
	struct pcdev_private_data pcdev_data[NO_OF_DEVICES];
};

struct pcdrv_private_data pcdrv_data=
{
	.total_devices=4,
	.pcdev_data = {
		[0]={
			.buffer = device_buffer1,
			.size  = DEV_MEM_SIZE_DEV1,
			.serial_number="PCDEV1XYZ123",
			.perm = 0x1/*RDONLY*/
		},
		 [1]={
                        .buffer = device_buffer2,
                        .size  = DEV_MEM_SIZE_DEV2,
                        .serial_number="PCDEV2XYZ123",
                        .perm = 0x10/*WRONLY*/
                },
		 [2]={
                        .buffer = device_buffer3,
                        .size  = DEV_MEM_SIZE_DEV3,
                        .serial_number="PCDEV3XYZ123",
                        .perm = 0x11/*RDWR*/
                },
		 [3]={
                        .buffer = device_buffer4,
                        .size  = DEV_MEM_SIZE_DEV4,
                        .serial_number="PCDEV4XYZ123",
                        .perm = 0x11/*RDWR*/
                }
	}
};



// Prototypes
loff_t chr_llseek(struct file *filp, loff_t offt, int whence);
ssize_t chr_read(struct file *filp, char __user *buff, size_t count, loff_t *offt);
ssize_t chr_write(struct file *filp, const char __user *buff, size_t count, loff_t *offt);
int chr_open(struct inode *inode, struct file *filp);
int chr_release(struct inode *inode, struct file *filp);

loff_t chr_llseek (struct file *filp, loff_t offt, int whence)
{
#if 0
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
#endif
	return 0;
}
ssize_t chr_read (struct file *filp, char __user *buff, size_t count, loff_t *offt)
{
	struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;
	int max_size = pcdev_data -> size;
	
	pr_info("read requested %zu bytes\n",count);
	pr_info("current file position = %lld\n",*offt);
        /*adjust the 'count'*/
	if((*offt + count)>max_size)
        {
                count = max_size - *offt;
        }

        if(copy_to_user(buff,pcdev_data->buffer+(*offt),count))
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
	struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;
        int max_size = pcdev_data -> size;

	pr_info("write request %zu bytes\n",count);
	pr_info("current file position = %lld\n",*offt);
	if((*offt + count)>max_size)
	{
		count = max_size - *offt;
	}

	if(copy_from_user(pcdev_data->buffer+*offt,buff,count))
	{
		return -EFAULT;
	}

	*offt+=count;

	pr_info("Number of bytes successfully written  = %zu\n",count);

	pr_info("updated file position = %lld\n",*offt);
	return count;

}

/* ------------ Permission Helper Function ------------ */
static int check_permission(int dev_perm, int access_mode)
{
    if (dev_perm == RDWR)
        return 0;
    if ((dev_perm == RDONLY) && (access_mode & FMODE_WRITE))
        return -EPERM;
    if ((dev_perm == WRONLY) && (access_mode & FMODE_READ))
        return -EPERM;
    return 0;
}

int chr_open (struct inode *inode, struct file *filp)
{
	int ret;
	int minor_n;
	struct pcdev_private_data *pcdev_data;
	
	/*find out on which device file open was attempted by the user space*/
	minor_n = MINOR(inode->i_rdev);
	pr_info("minor access = %d\n",minor_n);
	
	/*get device's private data structure*/
	pcdev_data = container_of(inode->i_cdev,struct pcdev_private_data,cdev);

	/*to supply device private data to other methods of the driver*/
	filp->private_data = pcdev_data;
	
	/*check permissions*/
	ret = check_permission(pcdev_data->perm, filp->f_mode);

	(!ret)?pr_info("open was successful\n"):pr_info("open was unsuccessful\n");
	return ret;
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

	int ret,i;
	ret = alloc_chrdev_region(&pcdrv_data.device_number,0,NO_OF_DEVICES,"char_multi_dev");
	if(ret<0)
	{
		pr_err("Alloc multi chr dev is failed\n");
	}
	pcdrv_data.chr_class = class_create("chr_class");
	if (IS_ERR(pcdrv_data.chr_class)) {
		pr_err("class_create failed\n");
		ret = PTR_ERR(pcdrv_data.chr_class);
		goto unreg;
	}
	for(i=0;i<NO_OF_DEVICES;i++)
	{
		pr_info("Device number major:%d  minor:%d \n",MAJOR(pcdrv_data.device_number+i),MINOR(pcdrv_data.device_number+i));
	

		pcdrv_data.pcdev_data[i].cdev.owner = THIS_MODULE;

		cdev_init(&pcdrv_data.pcdev_data[i].cdev,&pcd_fops);

		ret=cdev_add(&pcdrv_data.pcdev_data[i].cdev,pcdrv_data.device_number+i,1);

		if (ret < 0) {
        	    pr_err("Cdev add failed for %d\n", i);
        	    goto cleanup;
        	}
		//pcdrv_data.chr_class = class_create("chr_class");

		pcdrv_data.chr_device= device_create(pcdrv_data.chr_class,NULL,pcdrv_data.device_number+i,NULL,"chr_device-%d",i);
	}
	pr_info("Module init successful\n");
	return 0;
cleanup:
    		for (; i >= 0; i--) {
        		device_destroy(pcdrv_data.chr_class, pcdrv_data.device_number + i);
        		cdev_del(&pcdrv_data.pcdev_data[i].cdev);
    		}
         	class_destroy(pcdrv_data.chr_class);
unreg:
	unregister_chrdev_region(pcdrv_data.device_number, NO_OF_DEVICES);
    return ret;

}
static void __exit char_dev_exit(void)
{
	int i;
	for (i=0; i < NO_OF_DEVICES; i++) {
                 device_destroy(pcdrv_data.chr_class, pcdrv_data.device_number + i);
                 cdev_del(&pcdrv_data.pcdev_data[i].cdev);
        }
        class_destroy(pcdrv_data.chr_class);
	unregister_chrdev_region(pcdrv_data.device_number,NO_OF_DEVICES);

	pr_info("module exit successfull: %s\n",__func__);

}

module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_LICENSE("GPL");

