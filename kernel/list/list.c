#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>

struct node {
	int val;
	struct list_head list;
};

struct list_head demo_list_head;

static int list_init(void)
{
	int rc = 0;

	INIT_LIST_HEAD(&demo_list_head);

	return rc;
}

static void list_add_new(struct list_head *new)
{
	list_add(new,&demo_list_head);
}

static void list_test(void)
{
	int index = 0;

	while(index < 10)
	{
		struct node *new = (struct node *)kmalloc(sizeof(struct node),GFP_KERNEL);

		memset(new, 0, sizeof(struct node));

		new->val = index;

		list_add_new(&new->list);

		index++;
	}
}

static void list_print(void)
{
	struct node * node = NULL;
	list_for_each_entry(node, &demo_list_head, list){
		printk("node->val %d\n",node->val);
	}
}

static void list_del_all(void)
{
	struct node * node = NULL;
	struct node * tmp_node = NULL;
	list_for_each_entry_safe(node, tmp_node, &demo_list_head, list){
		if(node){
			list_del(&node->list);
			printk("del: node->val %d\n",node->val);
			kfree(node);
		}
	}
}

static int demo_init(void)
{
	list_init();

	list_test();

	list_print();

	return 0;
}

static void demo_exit(void)
{
	list_del_all();

	list_print();

	printk(KERN_INFO"demo exit ...\n");
}

module_init(demo_init);
module_exit(demo_exit);
