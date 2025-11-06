/*main.c*/
#include<linux/ioctl.h>
#include<linux/kernel.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/err.h>

#define min_max _IOWR('a',1,int32_t*)
#define size _IOW('a',2,int32_t*)
#define get_max_min _IOR('a',3,int32_t*)

dev_t dev=0;
static struct cdev  cdev;
static struct class * dev_class;

#define MAX_LEN 1024
int  *arr;
int arr_size;
int data_size = 0;
int max,min;
static int __init init_driver(void);
static void __exit exit_driver(void);

static int my_open(struct inode *inode,struct file *file);
static int my_release(struct inode *inode,struct file *file);

static ssize_t my_read(struct file *filp,char __user *buf, size_t len,loff_t*off);
static ssize_t my_write(struct file *filp,const char *buf, size_t len,loff_t*off);
static ssize_t my_ioctl(struct file *filp,unsigned int cmd,unsigned long arg);

static struct file_operations my_fops={
  .owner = THIS_MODULE,
  .read  = my_read,
  .write = my_write,
  .open  = my_open,
  .release = my_release,
  .unlocked_ioctl = my_ioctl,
  };
  
static int my_open(struct inode *inode,struct file *file)
{
  pr_info("driver open called\n");
  return 0;
}
static int my_release(struct inode *inode,struct file *file)
{
  pr_info("driver close called\n");
  return 0;
}
static ssize_t my_read(struct file *filp,char __user *buf, size_t len,loff_t*off)
{
        int result[2];
        result[0]=max;
        result[1]=min;

        if(copy_to_user(buf,result,sizeof(int)*2))
                 return -EFAULT;
         return sizeof(result);

}
static ssize_t my_write(struct file *filp,const char *buf, size_t len,loff_t*off)
{
	arr_size = len/sizeof(int);
	
	arr = kmalloc_array(arr_size,sizeof(int),GFP_KERNEL);
	
	if(!arr)
		-ENOMEM;
	
	if(copy_from_user(arr,buf,arr_size * sizeof(int)))
		return -EFAULT;
	max=arr[0];
	min=arr[0];
	for(int i=1;i<arr_size;i++)
	{
		if(arr[i]<min)
			min=arr[i];	
		if(arr[i]>max)
			max=arr[i];
	}
	
	pr_info("max=%d min=%d",max,min);
	return len;
}
static ssize_t my_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
	switch(cmd){
		case min_max:
			arr=kmalloc(arr_size*sizeof(int),GFP_KERNEL);
			if(!arr)
				return -ENOMEM;
			if(copy_from_user(arr,(uint32_t *)arg,sizeof(int)*arr_size))
			{
				return -EINVAL;
			}
			int i;
			max=arr[0];
			min=arr[0];
			for(i=1;i<arr_size;i++)
			{
				if(arr[i]>max)	max=arr[i];
				if(arr[i]<min) 	min=arr[i];
			}
			break;
		case get_max_min:
			int mm_arr[2];
			mm_arr[0]=max;
			mm_arr[1]=min;
			if(copy_to_user((uint32_t*)arg,mm_arr,sizeof(mm_arr)))
			{
				return -EINVAL;
			}
			break;
		case size:
			if(copy_from_user(&arr_size,(uint32_t *)arg,sizeof(int)))
			{
				return -EINVAL;
			}
			break;
//		case default:
//			pr_info("default case");
//			break;
	}
	return 0;
}
			
static int __init init_driver(void)
{
  if((alloc_chrdev_region(&dev,0,1,"chr_num"))<0)
  {
      pr_info("major and minor alloc faile\n");
      return -1;
  }
  pr_info("major and minor allocated\n");
  
  cdev_init(&cdev,&my_fops);
  if((cdev_add(&cdev,dev,1))<0)
  {
    pr_info("cdev failed to connect with kernel\n");
    goto c_del;
  }
  dev_class = class_create("dev_class");
  if(IS_ERR(dev_class))
  {
    pr_info("class not created\n");
    goto c_del;
  }
  
  if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"dev_device")))
  {
    pr_info("device not created\n");
    goto d_del;
  }
  pr_info("device creation done successful\n");
  return 0;
d_del:
      class_destroy(dev_class);
      unregister_chrdev_region(dev,1);
c_del:
      unregister_chrdev_region(dev,1);
      return -1;
}
static void __exit exit_driver(void)
{
      cdev_del(&cdev);
      device_destroy(dev_class,dev);
      class_destroy(dev_class);
      unregister_chrdev_region(dev,1);
}

module_init(init_driver);
module_exit(exit_driver);

MODULE_LICENSE("GPL");
