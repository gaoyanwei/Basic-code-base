#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include "hook.h"


#define _DEBUG

#ifdef _DEBUG
#define kprintk(fmt,args...) printk(KERN_DEBUG fmt, ##args)
#else
#define kprintk
#endif

static hook_atomic_t hook_counter = ATOMIC_INIT(0);

static inline void inc_ref(void)
{
    atomic_inc(&hook_counter);
}

static inline void dec_ref(void)
{
    atomic_dec(&hook_counter);
}

asmlinkage long (*old_sys_mkdir)(const char __user *pathname, int mode);

/* 用于设置内存的写的权限 cr0在64位机器上必须是unsigned long类型 */
unsigned long disable_wp ( void )
{
    unsigned long cr0;

    //preempt_disable();
    barrier();
    cr0 = read_cr0();
    write_cr0(cr0 & ~X86_CR0_WP);
    return cr0;
}

void restore_wp ( unsigned long cr0 )
{
    write_cr0(cr0);
    barrier();
    //preempt_enable_no_resched();
}

/* 通过kallsyms_lookup_name获取sys_call_table地址 */
void  get_sys_call_table(unsigned long * sys_call_table_addr)
{
    unsigned long temp_sys_call_table = 0;
    temp_sys_call_table = kallsyms_lookup_name("sys_call_table");

    if(temp_sys_call_table == 0)
        return;

    *sys_call_table_addr = temp_sys_call_table;
}

asmlinkage long my_sys_mkdir(const char __user * pathname, int mode)
{
    int rc = -13;
    inc_ref();

    rc = old_sys_mkdir(pathname, mode);

	printk("mdkir %s %d\n",__func__,rc);

    dec_ref();
	return rc;
}

void start_sys_hook(unsigned long sys_call_table_addr)
{
	unsigned long cr0 = 0;

	if(!sys_call_table_addr){
		kprintk("sys call table addr error");
		return;
	}

	old_sys_mkdir = ((unsigned long *)sys_call_table_addr)[__NR_mkdir];

    cr0 = disable_wp();
	((unsigned long *)sys_call_table_addr)[__NR_mkdir] = (void *)my_sys_mkdir;

    restore_wp(cr0);

	kprintk("new func : %p old func : %p\n",my_sys_mkdir,(void *)old_sys_mkdir);

}

void remove_sys_hook(void)
{
	unsigned long cr0 = 0;
    unsigned long sys_call_table_addr = 0;

    get_sys_call_table(&sys_call_table_addr);

    if(sys_call_table_addr == 0)
        return ;

	if(sys_call_table_addr && old_sys_mkdir){
        cr0 = disable_wp();
        ((unsigned long*)sys_call_table_addr)[__NR_mkdir] = (void *)old_sys_mkdir;
        restore_wp(cr0);
	}
}

int sys_hook_init(void)
{
    unsigned long sys_call_table_addr = 0;

    get_sys_call_table(&sys_call_table_addr);

    if(sys_call_table_addr != 0)
        start_sys_hook(sys_call_table_addr);

	kprintk("hook init...\n");

	return 0;
}

void sys_hook_exit(void)
{

    if(atomic_read(&hook_counter) == 0)
        remove_sys_hook();

	kprintk("hook exit...\n");
}

MODULE_LICENSE("GPL");
module_init(sys_hook_init);
module_exit(sys_hook_exit);
