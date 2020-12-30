#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
//#include "hlist.h"


//#define HLIST_MEM_ALLOC

#define DEMO_HASH_BITS 3
#define HLIST_LEN (1UL << DEMO_HASH_BITS)
#define DEMO_HASH_MASK (HLIST_LEN - 1)

typedef struct hdata_node {
	unsigned int hash_id;
	char prc_name[50];
	struct hlist_node list;
}hdata_node_t;

#ifdef HLIST_MEM_ALLOC
typedef struct hlist_table{
	struct hlist_head * hhead;
}hlist_table_t;

hlist_table_t * table;

#else

struct hlist_head  table[HLIST_LEN];
#define DEMO_HASH_HEAD(HASH) (&(table[HASH]))

#endif

static struct kmem_cache * demo_hlist_slab;
static char * demo_hlist_name = "demo_hlist_name";

static void * demo_slab_alloc(struct kmem_cache * slab)
{
	if(slab)
		return kmem_cache_zalloc(slab, GFP_KERNEL);
	return NULL;
}

static void demo_slab_free(struct kmem_cache * slab, void * ptr)
{
	if(!slab || !ptr)
		return ;
	kmem_cache_free(slab, ptr);
}

#ifdef HLIST_MEM_ALLOC
static int demo_hlist_init(void)
{
	int index = 0;
	table = (hlist_table_t * )kmalloc(sizeof(hlist_table_t), GFP_KERNEL);
	if(!table)
		return -ENOMEM;

	table->hhead = (struct hlist_head *)kmalloc(sizeof(struct hlist_head) * HLIST_LEN, GFP_KERNEL);

	if(!table->hhead)
		return -ENOMEM;

	for(index = 0;index < HLIST_LEN; index++){
		INIT_HLIST_HEAD(&(table->hhead[index]));
	}

	printk(KERN_INFO"demo hlist mem alloc init...\n");

	return 0;
}

int hlist_add_dataNode(unsigned int hash_id, char * prc_name)
{
	struct hlist_head * head = NULL;
	hdata_node_t * hD = demo_slab_alloc(demo_hlist_slab);
	if(!hD)
		return -ENOMEM;

	if(!(strlen(prc_name) <= 50))
		return -EINVAL;
	strcpy(hD->prc_name,prc_name);
	hD->hash_id = hash_id;

	INIT_HLIST_NODE(&(hD->list));

	head = &(table->hhead[hash_id % 8]);

	hlist_add_head(&(hD->list), head);

	return 0;
}


static void hlist_del_dateNode(hdata_node_t * hD);

static void hlist_search_del_dateNode(unsigned int hash_id, char * prc_name)
{
	struct hlist_node * pos, *n;
	hdata_node_t * tops;
	struct hlist_head * head;

	head = &(table->hhead[hash_id%8]);

	hlist_for_each_entry_safe(tops, pos, n ,head ,list){
		if(tops->hash_id == hash_id && !(strcmp(tops->prc_name,prc_name))){
			printk(KERN_INFO"tops->hash_id %d\n",tops->hash_id);
			printk(KERN_INFO"tops->prc_name %s\n",tops->prc_name);
			printk(KERN_INFO"hash_id %d\n",hash_id);
			printk(KERN_INFO"prc_name %s\n",prc_name);
			hlist_del_dateNode(tops);
		}
	}

}

static void hlist_del_dateNode(hdata_node_t * hD)
{
	hlist_del_init(&(hD->list));

	demo_slab_free(demo_hlist_slab, hD);
}

static void hlist_search_dateNode( void )
{
	int index = 0;
	struct hlist_node * pos;
	hdata_node_t * tops;
	struct hlist_head * head;

	for(index = 0; index < HLIST_LEN; index++){
		head = &(table->hhead[index]);
		hlist_for_each_entry(tops, pos, head, list){
			printk("%d %s\n",tops->hash_id,tops->prc_name);
		}
	}
}

static int demo_hlist_exit(void)

{
	int index ;
	hdata_node_t * tops;
	struct hlist_node * pos, *n;
	struct hlist_head * head;

	for(index = 0; index < HLIST_LEN; index++){
		head = &(table->hhead[index]);
#if 0
		hlist_for_each_safe(pos , node , head){
			hD = list_entry(pos, hdata_node_t, list);
			hlist_del_dateNode(hD);
		}
#else
		hlist_for_each_entry_safe(tops, pos, n , head, list){
			hlist_del_dateNode(tops);
		}
#endif
	}

	printk(KERN_INFO"demo hlist mem alloc exit...\n");

	return 0;
}

#else

static void demo_hlist_init(void)
{
	int index = 0;

	for(index = 0; index < HLIST_LEN; index++){
		INIT_HLIST_HEAD(&(table[index]));
	}

}

static int hlist_add_dataNode(unsigned int hash_id, char * prc_name)
{
	struct hlist_head * head;
	hdata_node_t * node ;
	int key = 0;

	if(!prc_name || strlen(prc_name) >= 50)
		return -EINVAL;

	node= demo_slab_alloc(demo_hlist_slab);

	if(!node)
		return -ENOMEM;
	strcpy(node->prc_name, prc_name);
	node->hash_id = hash_id;

	INIT_HLIST_NODE(&(node->list));

	key = hash_id % 8;
	head = DEMO_HASH_HEAD(key);

	hlist_add_head(&(node->list), head);

	return 0;
}

static void hlist_search_dateNode(void)
{
	int index = 0;
	struct hlist_head * head;
	struct hlist_node * pos;
	hdata_node_t * tops;

	for(index = 0; index < HLIST_LEN; index++){
		head = DEMO_HASH_HEAD(index);
		hlist_for_each_entry(tops, pos, head, list){
			if(tops)
				printk(KERN_INFO"%s hash_id %d prc_name %s\n",__func__,tops->hash_id,tops->prc_name);
		}

	}
}

static int hlist_del_dateNode(hdata_node_t * hd)
{
	if(hd == NULL)
		return -EINVAL;

	printk(KERN_INFO"hd->hash_id %d\n",hd->hash_id);
	printk(KERN_INFO"hd->prc_name %s\n",hd->prc_name);

	hlist_del_init(&(hd->list));

	demo_slab_free(demo_hlist_slab, hd);

	return 0;
}

static void hlist_search_del_dateNode(unsigned int hash_id,char * name)
{
	struct hlist_head * head;
	struct hlist_node * pos, *n;
	hdata_node_t * tops;

	head = DEMO_HASH_HEAD(hash_id % 8);

	hlist_for_each_entry_safe(tops, pos, n, head, list){
		if(hash_id == tops->hash_id && !strcmp(name,tops->prc_name)){
			hlist_del_dateNode(tops);
			break;
		}
	}

}

static void demo_hlist_exit(void)
{
	int index = 0;
	struct hlist_head * head = NULL;
	hdata_node_t * tops;
	struct hlist_node * pos , *n;

	for(index = 0;index < HLIST_LEN; index++){
		head = DEMO_HASH_HEAD(index);
		hlist_for_each_entry_safe(tops, pos, n, head, list){
			hlist_del_dateNode(tops);
		}
	}

	printk(KERN_INFO"demo hlist exit ...\n");
}

#endif

static int demo_hlist_test( void )
{
	int index = 0;
	int rc = 0;
	char * a = "a";
	char buf[50];
	for(index = 0;index < 49; index++){
		strcpy(buf + index,a);
		rc = hlist_add_dataNode(index,buf);
		if(rc){
			printk(KERN_ERR"hlist add dataNode is fail %d",rc);
			return rc;
		}
	}

	hlist_search_dateNode();

	for(index = 49;index >= 0; index--){
		buf[index+1] = '\0';
		hlist_search_del_dateNode(index,buf);
	}
	return 0;
}


static int demo_init(void)
{
	demo_hlist_slab = kmem_cache_create(demo_hlist_name, sizeof(hdata_node_t), 0, SLAB_PANIC, NULL);

	if(!demo_hlist_slab){
//		printk(KERN_ERR,"slab create is fail %s\n",demo_hlist_name);
		printk(KERN_ERR"slab create is fail %s\n",demo_hlist_name);
		return -ENOMEM;
	}

	demo_hlist_init();

	printk(KERN_INFO"demo init ...\n");

	demo_hlist_test();

	printk(KERN_INFO"demo test ...\n");

	return 0;
}

static void demo_exit(void)
{

	demo_hlist_exit();

	if(demo_hlist_slab)
		kmem_cache_destroy(demo_hlist_slab);

	printk(KERN_INFO"demo exit ...\n");
}

module_init(demo_init);
module_exit(demo_exit);
