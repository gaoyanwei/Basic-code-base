/************************************
 *Linux verson:5.4.0.56
 *name : ioctl char driver
 *author: gaoyanwei
 *email: gaoyanwei_tx163.com
 ***********************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/kallsyms.h>

#include "test.h"


#define DeviceName "test"
#define ClassName "test_class"


struct cdev test_cdev;
int	dsn;
int	acount_of_devices;
dev_t	dev;

struct class * D_class;
struct device * D_device;

#define MEM_SIZE 20

unsigned char * mem_addr;


int open_test(struct inode * inode,struct file * filp)
{
	printk("open successfull...\n");	
	return 0;
}

int release_test(struct inode * inode,struct file * filp){
	printk("release successfull...\n");
	return 0;
}

void read_tc(void){
	int i = 0,j = 0;
	int count = 0;
	for(j =0;j < 10000; j++){
		for(i = 0 ; i < 10000; i++){
			count++;
			if(count % 10000 == 0)
				printk("%d\n",count);
		}
	}
}

static ssize_t read_test(struct file *file,
                        char __user *buf,
                        size_t count,
                        loff_t *ppos)
{
#if 0
	int i = 0;
	for(i = 0;i < 10;i++){
		read_tc();
	}
#endif
	printk("read is successfull\n");
	return 0;
}

long test_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
	int retval = 0;
	int ioctl_w_value = 0;
	int ioctl_r_value = 2222;
	char ioctl_cw_value = 'a';

	/* 检测命令的有效性 */
	if (_IOC_TYPE(cmd) != FC_TGC_IOC_MAGIC) {
		return -ENODEV;
	}

	/* 根据命令类型，检测参数空间是否可以访问 */
	/* access_ok返回值为1(成功)或0(失败) */
#if 1
	if (_IOC_DIR(cmd) & _IOC_READ){
		retval = access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	}else if (_IOC_DIR(cmd) & _IOC_WRITE){
		retval = access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	}
#else
	if (_IOC_DIR(cmd) & _IOC_READ){
		retval = access_ok((void __user *)arg, _IOC_SIZE(cmd));
	}else if (_IOC_DIR(cmd) & _IOC_WRITE){
		retval = access_ok((void __user *)arg, _IOC_SIZE(cmd));
	}

#endif
	if (!retval)
		return -EFAULT;

	/* 根据命令，执行相应的操作 */
	switch (cmd)
	{
	case FC_TGC_IOC_CW:
		pr_info("CSDN_IOC_GET \n");
		retval = copy_to_user((void __user *)arg,(void *)&ioctl_cw_value, sizeof(char));
		pr_info("retval: %d, ioctl_w_value: %c\n", retval, ioctl_cw_value);
		break;

	case FC_TGC_IOC_W:
		pr_info("CSDN_IOC_W \n");
		retval = copy_from_user(&ioctl_w_value, (int *)arg, sizeof(int));
		pr_info("retval: %d, ioctl_w_value: %d\n", retval, ioctl_w_value);
		break;

	case FC_TGC_IOC_R:
		pr_info("CSDN_IOC_R \n");
		retval = copy_to_user((void __user *)arg, (void *)&ioctl_r_value, sizeof(int));
		pr_info("retval: %d\n", retval);
		break;

	case FC_TGC_IOC_RW:
		pr_info("CSDN_IOC_WR \n");
		retval = copy_from_user(&ioctl_w_value, (int *)arg, sizeof(int));
		pr_info("retval: %d, ioctl_w_value: %d\n", retval, ioctl_w_value);

		ioctl_r_value = 3333;
		retval = copy_to_user((void __user *)arg, (void *)&ioctl_r_value, sizeof(int));
		pr_info("retval: %d\n", retval);
		break;

	default:
		pr_warn("unsupport cmd:0x%x\n", cmd);
		break;
	}

	return 0;
}

static struct file_operations fops_test=
{
    .open = open_test,
    .read = read_test,
    .release = release_test,
    .unlocked_ioctl = test_ioctl,
};

static int __init test_init(void){
	int result = 0;
	dsn = 3;
	acount_of_devices = 1;

	unsigned long addr = (unsigned long)kallsyms_lookup_name("kallsyms_lookup_name");

	printk("addr %#x \n",addr);

	result = alloc_chrdev_region(&dev, dsn, acount_of_devices, DeviceName);
	if(result < 0)
	{
		result = -1;
		printk(KERN_INFO "Fail:alloc_chrdev_region()\n");
		goto Exit_1;
	}

	cdev_init(&test_cdev,&fops_test);
	test_cdev.owner = THIS_MODULE;

	result = cdev_add(&test_cdev, dev, dsn + 1);
	if(result < 0)
	{
		result = -2;
		printk(KERN_INFO "Fail:cdev_add()\n");
		goto Exit_2;
	}

	D_class = class_create(THIS_MODULE,ClassName);

	if(D_class == NULL){
		result = -3;
		printk(KERN_INFO "Fail:class_create()\n");
		goto Exit_3;
	}

	D_device = device_create(D_class, NULL, dev, NULL, DeviceName);

	if(D_device == NULL){
		result = -4;
		printk(KERN_INFO "Fail:device_create()\n");
		goto Exit_4;
	}

	mem_addr = (char *)kmalloc(MEM_SIZE,GFP_KERNEL);

	if(mem_addr == NULL)
	{
		result = -5;
		printk(KERN_INFO "Fail:kmalloc() for ChrDevp->mem_datas\n");
		goto Exit_5;
	}

	goto Exit_1;

Exit_5:
	device_destroy(D_class,dev);
Exit_4:
	class_destroy(D_class);
Exit_3:
	cdev_del(&test_cdev);
Exit_2:
	unregister_chrdev_region(dev,acount_of_devices);
Exit_1:
	printk("test init...\n");
	return result;
}

static void __exit test_exit(void){
	kfree(mem_addr);
	device_destroy(D_class,dev);
	class_destroy(D_class);
	cdev_del(&test_cdev);
	unregister_chrdev_region(dev,acount_of_devices);
	printk("test exit...\n");
	return;
}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");

