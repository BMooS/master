/*
**二叉搜索树的接口
*/
#define TREE_TYPE int

/*
* TreeNode 结构包含了值和两个指向某个树节点的指针
*/
typedef struct TREE_NODE {
    TREE_TYPE value;
    struct TREE_NODE *left;
    struct TREE_NODE *right;
} TreeNode;

/*
*insert 向树种插入一个新值，参数是需要被添加的值，它必须是树中没有的值
*/
void insert(TREE_TYPE value);

/*
*find 查找一个值，这个值作为参数传递给函数，返回这个值的地址，若无，则返回NULL
*/
TREE_TYPE *find(TREE_TYPE value);

/*
*max 查找树中最大值，也就是树的最有叶节点的值
*/
TREE_TYPE max_tree(TreeNode *link);

/*
*remove 删除一个值，这个值作为参数传递给函数，它必须是树中具有的值
*/
void remove_value(TREE_TYPE value);

/*
*pre_order_traverse
*执行树的前序遍历，参数是一个回调函数指针，它所指向的函数将在树中处理每个节点被调用
*节点的值作为被调函数的参数
*/
void pre_order_traverse(void (*callback)(TREE_TYPE value));