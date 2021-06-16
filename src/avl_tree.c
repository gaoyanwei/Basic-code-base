#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) (a > b ? a : b)
#define GET_HEIGHT(T) (T == NULL ? -1 : T->height)

typedef struct treeNode{
	int val;
	struct treeNode *left;
	struct treeNode *right;
	int height;
}avltree_t;

/* 找到树里固定值的节点 */
static avltree_t *tree_search(int val, avltree_t *root)
{
	if(root == NULL)
		return NULL;

	if(val < root->val)
		return tree_search(val, root->left);
	else if(val > root->val)
		return tree_search(val, root->right);
	else
		return root;
}

/* 寻找树里最小节点 */
static avltree_t *tree_search_min(avltree_t *root)
{
	if(root == NULL)
		return NULL;
	else if(root->left == NULL)
		return root;
	else
		return tree_search_min(root->left);

}

/* 寻找最大节点 */
static avltree_t *tree_search_max(avltree_t *root)
{
	if(root == NULL)
		return NULL;
	else if(root->right == NULL)
		return root;
	else
		return tree_search_min(root->right);
}

/* 获取当前节点在树里的高度 */
static int  get_height(avltree_t *root)
{
	return GET_HEIGHT(root);
}

/* 单次右单旋 插入在左子树时 会产生右单旋*/
static avltree_t *right_single_rotate(avltree_t *root)
{
	avltree_t *node = NULL;

	node = root->left;
	root->left = node->right;
	node->right = root;

	root->height = MAX(get_height(node->left), get_height(root->right)) + 1;
	node->height = MAX(get_height(node->left), root->height) + 1;

	return node;
}

/* 单次左单旋 插入在右子树时 会产生左单旋*/
static avltree_t *left_single_rotate(avltree_t *root)
{
	avltree_t *node = NULL;

	node = root->right;
	root->right = node->left;
	node->left = root;

	root->height = MAX(get_height(node->left), get_height(root->right)) + 1;
	node->height = MAX(get_height(node->left), root->height) + 1;

	return node;
}

/* 右左双旋 */
static avltree_t *right_left_single_rotate(avltree_t *root)
{
	root->left = right_single_rotate(root->left);

	return left_single_rotate(root);
}

/* 左右双旋 */
static avltree_t *left_right_single_rotate(avltree_t *root)
{
	//先向左旋一次 让树变成右旋平衡状态
	root->right = left_single_rotate(root->right);

	//右旋让树平衡
	return right_single_rotate(root);
}

/* 将数据插入树中 */
avltree_t *avl_insert(int val, avltree_t *root)
{
	//判断根节点是否为空，为空直接插入树中
	if (root == NULL) {
		root = (avltree_t *)malloc(sizeof(struct treeNode));
		root->val = val;
		root->left = NULL;
		root->right = NULL;
		root->height = 0;
	//判断当前值比传入的节点的值小
	} else if (val < root->val) {
		//进入左子树查找合适位置进行插入
		root->left = avl_insert(val, root->left);
		//判断当前节点的树是否已经失去平衡
		if (GET_HEIGHT(root->left) - GET_HEIGHT(root->right) == 2) {
			//val的值小于左子树时 只需要一次右旋即可
			if (val < root->left->val)
				root = right_single_rotate(root);
			//val的值大于左子树 此时是左右情况 需要先进行左旋在右旋
			else
				root =left_right_single_rotate(root);
		}
	//判断传入的参数是否比传入的节点的值大
	} else if (val > root->val) {
		//进入右子树查找合适的节点插入
		root->right = avl_insert(val, root->right);
		//判断当前节点为根的树是否失去平衡
		if(GET_HEIGHT(root->right) - GET_HEIGHT(root->left) == 2){
			//val的值大于右子树时 只需要一次左旋即可
			if(val > root->right->val)
				root = left_single_rotate(root);
			//val的值小于右子树 此时是右左情况 需要先进行右旋在左旋
			else
				root = right_left_single_rotate(root);
		}
	} else {
		printf("val is %d avl tree is exist\n", val);
	}

	//重新计算当前节点的高度
	root->height = MAX(get_height(root->left), get_height(root->right) + 1);

	return root;
}

avltree_t *avl_del(int val, avltree_t *root)
{
	avltree_t *node = NULL;
	if (root == NULL) {
		printf("avl root is NULL\n");
	//val的值小于传入节点的值进入左子树删除
	} else if (val < root->val) {
		root->left = avl_del(val, root->left);
		if(get_height(root->right) - get_height(root->left) == 2) {
			avltree_t *tmp = root->right;
			//当在左子树执行删除的时候 此时删除的是左节点 判断当前节点的右子树的左右
			//子树的深度 左子树比右子树深需要先进行右旋再左旋
			if(get_height(tmp->left) > get_height(tmp->right))
				root = right_left_single_rotate(root);
			else
				root = left_single_rotate(root);
		}
	//val值大于传入节点的值进入右子树删除
	} else if (val > root->val) {
		root->right = avl_del(val, root->right);
		if(get_height(root->left) - get_height(root->right) == 2) {
			avltree_t *tmp = root->left;
			//当在右子树执行删除的时候 判断当前节点的右子树的左右
			//子树的深度 右子树比左子树深需要先进行左旋再右旋
			if(get_height(tmp->right) > get_height(tmp->left))
				root = left_right_single_rotate(root);
			else
				root = right_single_rotate(root);
		}
	//前面条件都不满足 val的值等于当前节点的值 当前节点有左右子树
	} else if (root->left && root->right){
		//当前节点右子树比左子树深，取右子树最小值节点做当前节点
		if (get_height(root->right) > get_height(root->left)) {
			node = tree_search_min(root->right);
			root->val = node->val;
			root->right = avl_del(root->val, root->right);
		//当前节点左子树比右子树深，取左子树最大值节点做当前节点
		} else {
			node = tree_search_max(root->left);
			root->val = node->val;
			root->left = avl_del(root->val, root->left);
		}
	} else {//当前节点只有一颗子树 帮其子树的值修改为当前节点的值
		node = root;
		root = (root->left == NULL ? root->right : root->left );
		free(node);
	}

	return root;
}

//中序遍历
void in_order(avltree_t *root)
{
	if(root)
	{
		in_order(root->left);
		printf("%d ",root->val);
		in_order(root->right);
	}
}

int main()
{
	int i, arr[] = {1,28,45,34,98,9,4,35,23};

	avltree_t *root = NULL;

	for (i = 0; i < 9; i++)
	{
		root = avl_insert(arr[i], root);
	}

	in_order(root);
	printf("\n");

	for(i = 0; i < 9; i++ ){
		root = avl_del(arr[i], root);
	}

	in_order(root);
	printf("\n");

	return 0;
}
