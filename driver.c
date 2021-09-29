#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/semaphore.h>

#define NAME MyCharDevice
//function prototype
int NAME_open(struct inode *inode, struct file *filp);
int NAME_release(struct inode *inode, struct file *filp);
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp);
ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);
//file operation
struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = NAME_open,
	.read = NAME_read,
	.write = NAME_write,
	.release = NAME_release,
};

struct cdev *my_cdev;
//semaphore operation
struct semaphore sem_dev;


char Kbuff[80];
int flag=1;


static int __init chardev_init(void)
{
	int result;
	dev_t Mydev;
	Mydev = MKDEV(255,0);

	result = register_chrdev_region(Mydev,2,"MyCharDevice");
	if(result<0)
	{
		printk("\nError");
		return(-1);
	}
	my_cdev = cdev_alloc();
	my_cdev->ops = &fops;

	result = cdev_add(my_cdev,Mydev,2);
	if(result<0)
	{
		printk("\nerror obtaining region");
		unregister_chrdev_region(Mydev,2);
		return(-1);
	}
	return 0;
}


void __exit chardev_exit(void)
{
	dev_t Mydev;
	Mydev = MKDEV(255,0);
	unregister_chrdev_region(Mydev,2);
	cdev_del(my_cdev);
	printk("\nUnregistered");
	return;
	
}
//open function
int NAME_open(struct inode *inode, struct file *filp)
{
	printk("\nThis is an open call");
	return 0;
}

//release function
int NAME_release(struct inode *inode, struct file *filp)
{
	printk("\nThis is a close call");
	return 0;
}
//read function
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{
	char Kbuff[100] = "This is the data sent to device";
	ssize_t ret;
	unsigned long remaining;
	printk("\nread command called :");
	remaining = copy_to_user((char *)Ubuff,(char *)Kbuff,count);

	if(remaining == 0)
	{
		printk("\ndata succesfully copied");
		return count;
	}
	else if(remaining>0)
	{
		printk("data still remaining to be read");
		return (count - remaining);
	}
	else
	{
		printk("\n error reading data\n");
		ret = -EFAULT;
		return ret;
	}
	return 0;
}

//write function
ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp)
{
	char Kbuff[100];
	unsigned long remaining;
	ssize_t ret;
	printk("\nwriting data: \n");
	remaining = copy_from_user((char *)Kbuff,(char *)Ubuff,count);
	if(remaining == 0)
	{
		printk("successfully read from user : %s",Ubuff);
		return count;
	}
	else if(remaining>0)
	{
		printk("partially read from user : %s",Kbuff);
		return (count-remaining);
	}
	else
	{
		printk("error writing data\n");
		ret = -EFAULT;
		return ret;
	}
	
	return 0;
}


module_init(chardev_init);
module_exit(chardev_exit);


MODULE_LICENSE("GPL");
