
/*linux kernel version 3.0*/

#include <linux/module.h>  
#include <linux/types.h>  
#include <linux/fs.h>  
#include <linux/errno.h>  
#include <linux/mm.h>  
#include <linux/init.h>  
#include <asm/uaccess.h>  
#include <linux/timer.h>  
#include <asm/atomic.h>  
#include <linux/slab.h>  
#include <linux/proc_fs.h>

static int temp = 0;    

static ssize_t demo_get(char *buffer, char **buffer_location,off_t offset, int buffer_length, int *eof, void *data)
{
    sprintf(buffer,"read ,demo temp is %d",temp);
    printk(KERN_NOTICE "read ,demo temp is %d\n",temp);
    return 0;
}

static ssize_t demo_set(struct file *file, const char *buffer, size_t count,loff_t *lof)
{
    sscanf(buffer, "%d", &temp);

    printk(KERN_NOTICE "write ,demo temp is %d\n",temp);
    return count;
}

int demo_init(void)  
{  
    struct proc_dir_entry *entry;      

    entry= create_proc_entry("driver/demo",0666,NULL);
    if(entry)
    {
        entry->read_proc = (void *)demo_get;
        entry->write_proc = (void *)demo_set;
    }

    printk(KERN_NOTICE "======== demo_init end\n");  
    return 0;  

}  

void demo_exit(void)    /*模块卸载*/  
{  
    printk(KERN_NOTICE "End demo\n");   

    remove_proc_entry("driver/demo",NULL);

}  

MODULE_LICENSE("Dual BSD/GPL");  
module_param(temp, int, S_IRUGO);  
module_init(demo_init);  
module_exit(demo_exit); 
