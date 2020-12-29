/*
 * Linux version:5.4.0.56
 * name : ftrace.ko
 * author: gaoyanwei
 * email: gaoyanwei_tx@163.com
 * note: 用于hook sys_mkdir
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/ftrace.h>

typedef struct ftrace_demo_hook{
	const char * name;
	void * function;
	void * original;
	unsigned long address;
	struct ftrace_ops ops;
} demo_hook;

#define F_HOOK(_name, _function, _original) \
	{			\
		.name = (_name),	\
		.function = (_function),		\
		.original = (_original),		\
	}

asmlinkage long (*real_sys_mkdirat)(int dfd, const char __user * pathname, umode_t mode);

long fh_sys_mkdirat(int dfd, const char __user * pathname, umode_t mode)
{
	int ret = -13;
#if 0		//此处打开会出现内核crash现象 目前不清楚是哪里出现的问题。
	char * buf = (char *)kmalloc(1024,GFP_KERNEL);
	memset(buf,0,1024);

	if(pathname){
		copy_from_user(buf,pathname,1024);
		printk("%s pathname: %s ret: %d\n",__func__,buf,ret);
	}

	kfree(buf);
#endif
	ret = real_sys_mkdirat(dfd,pathname, mode);

	printk("%s\n",__func__);
	return ret;
}

static demo_hook hook_ops[]	= {
	F_HOOK("__x64_sys_mkdir",fh_sys_mkdirat, &real_sys_mkdirat),
};

static int resolve_hook_address(demo_hook * hook)
{
	hook->address = kallsyms_lookup_name(hook->name);

	printk("hook->address %#x\n",hook->address);

	if(!hook->address){
		pr_debug("unresolved symbol: %s\n",hook->name);
		return -ENOENT;
	}

	*((unsigned long *)hook->original) = hook->address;
	return 0;
}

static void notrace fh_ftrace_thunk (unsigned long ip, unsigned long parent_ip,
                struct ftrace_ops *ops, struct pt_regs *regs)
{
    demo_hook *hook = container_of(ops, demo_hook, ops);
 
    /* Skip the function calls from the current module. */
    /* (1) 防止递归 */
    if (!within_module(parent_ip, THIS_MODULE))
            /* (2) 最核心的技巧：
                通过修改`ftrace_caller()/ftrace_regs_caller()`函数的返回函数来实现hook
                原本执行完ftrace hook后返回原函数`cmdline_proc_show()`
                将其替换成新函数`fh_cmdline_proc_show()`
             */
            regs->ip = (unsigned long) hook->function;
}

static int demo_install_hook(demo_hook * hook)
{
	int err = 0;
	err = resolve_hook_address(hook);
	if (err)
		return err;
	hook->ops.func = fh_ftrace_thunk;
	hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_IPMODIFY;

	err = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
	if(err){
		pr_debug("ftrace_set_filter_ip failed %d\n",err);
		return err;
	}

	err = register_ftrace_function(&hook->ops);
	if(err){
		pr_debug("register_ftrace_function failed %d\n",err);
		ftrace_set_filter_ip(&hook->ops,hook->address, 1, 0);
		return err;
	}

	return err;
}

void demo_remove_hook(demo_hook * hook)
{
	int err = 0;
	err =  unregister_ftrace_function(&hook->ops);
	if(err){
		pr_debug("unregister_ftrace_function failed %d\n",err);
	}

	err = ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
	if(err){
		pr_debug("ftrace_set_filter_ip failed %d\n",err);
	}
}


static int __init demo_ftrace_init(void)
{
	int ret = 0;
	int i = 0;

	for(i = 0; i < sizeof(hook_ops) / sizeof(demo_hook); i++){
		ret = demo_install_hook(&hook_ops[i]);
		if(ret){
			printk("install ftrace hook fail\n");
			return ret;
//			return ;
		}
	}
	printk("old address %#x\n",*((unsigned long*)hook_ops[0].original));
	printk("ftrace_init running ...\n");
	return ret;
}

static void __exit demo_ftrace_exit(void)
{
	int i = 0;
	for(i = 0; i < sizeof(hook_ops) / sizeof(demo_hook); i++){
		demo_remove_hook(&hook_ops[i]);
	}
	printk("ftrace_exit running ...\n");
}

module_init(demo_ftrace_init);
module_exit(demo_ftrace_exit);
MODULE_LICENSE("GPL");
