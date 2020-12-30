#include <linux/init.h>
#include <linux/module.h>
#include <linux/rbtree.h>
#include <linux/slab.h>

struct rb_root demo_root;

typedef struct rbtree_data{
	struct rb_node data_node;
	int num;
}rbtree_data_t;

static void demo_rbroot_init(struct rb_root * root)
{
	if(!root)
		return ;
	/* 初始化树 */
	*root = RB_ROOT;
}

/* 将节点插入红黑树 */
int demo_rbtree_insert(struct rb_root *root, rbtree_data_t *data)
{
    struct rb_node **tmp = &(root->rb_node), *parent = NULL; 
	rbtree_data_t * this;

	if(!root)
		return -EINVAL;

    /* Figure out where to put new node */
    while (*tmp) {
		/* 取存放红黑树的结构体 */
		this = container_of(*tmp, rbtree_data_t, data_node);
		/* 存放父节点 */
		parent = *tmp;
		/* 对比数据 如小于树中的数据取节点的左节点 */
		if (data->num < this->num)
			tmp = &((*tmp)->rb_left);
		/* 对比数据 如大于树中的数据取节点的左节点 */
		else if (data->num > this->num)
			tmp = &((*tmp)->rb_right);
		else
		/* 数据相等，数据节点已存在 */
			return -EEXIST;
    }
    /* Add new node and rebalance tree. */
	/* 初始化节点 data的红黑树节点，parent父节点，tmp父节点的子节点*/
    rb_link_node(&data->data_node, parent, tmp);
	/* 将节点插入root中 */
    rb_insert_color(&data->data_node, root);
    return 0;
}

/* 查找对应值得红黑树节点 */
rbtree_data_t * demo_rbtree_search(struct rb_root *root, int num)
{
    struct rb_node *node = root->rb_node;
	rbtree_data_t * data = NULL;

	if(!root)
		return NULL;

    while (node) {
		/* 从红黑树中查找到对应子节点 */
		data = container_of(node, rbtree_data_t, data_node);
		if (num < data->num)
			node = node->rb_left;
		else if (num > data->num)
			node = node->rb_right;
		else
			return data;
    }
    return NULL;
}

/* 遍历红黑树 查找到对应节点删除 */
int demo_rbtree_delete(struct rb_root *root, int num)
{

    rbtree_data_t * data = demo_rbtree_search(root, num);

	if(!root)
		return -EINVAL;

    if (!data) {
		printk(KERN_ERR"not found num\n");
		return -ENOMEM;
    }

	/* 将节点从树中删除 */
    rb_erase(&data->data_node, root);
    kfree(data);
	printk(KERN_INFO"%s num = %d\n",__func__,num);

	return 0;
}

/* 添加节点到红黑树 */
static int demo_rbtree_add(unsigned int num)
{
	rbtree_data_t * demo_node;
	demo_node = (rbtree_data_t *)kmalloc(sizeof(rbtree_data_t), GFP_KERNEL);

	if(!demo_node)
		return -ENOMEM;

	demo_node->num = num;

	demo_rbtree_insert(&demo_root, demo_node);

	return 0;
}

/* 遍历红黑树删除节点 */
void demo_rbtree_clear(struct rb_root * root)
{
	struct rb_node *node;
	rbtree_data_t * data;

	for(node = rb_first(root); node; node = rb_next(node)){
		data = rb_entry(node, rbtree_data_t, data_node);
		rb_erase(&(data->data_node),root);
		kfree(data);
	}
}

/* 遍历打印红黑树 */
void demo_print_rbtree(struct rb_root *tree)
{
    struct rb_node * node;
    for (node = rb_first(tree); node; node = rb_next(node))
		printk("%d ", rb_entry(node, rbtree_data_t , data_node)->num);
    printk("\n");
}

static void demo_rbtree_test(void)
{
	int index = 0;

	for(index = 0; index < 50; index++){
		demo_rbtree_add(index);
	}

	printk(KERN_INFO"demo retree add is finish\n");

	demo_print_rbtree(&demo_root);

	printk(KERN_INFO"demo print rbtree is finish\n");

	demo_rbtree_clear(&demo_root);

	printk(KERN_INFO"demo rbtree clear is finish\n");

	demo_print_rbtree(&demo_root);

	printk(KERN_INFO"demo print rbtree is two finish\n");

}


static int demo_rbtree_init(void)
{

	demo_rbroot_init(&demo_root);

	demo_rbtree_test();

	printk(KERN_INFO"demo retree init ...\n");
	return 0;
}

static void demo_rbtree_exit(void)
{
	printk(KERN_INFO"demo retree exit ...\n");
}


module_init(demo_rbtree_init);
module_exit(demo_rbtree_exit);
