# 二叉搜索树

> **树的递归定义：**
>
> 树是一种数据结构，它要么为空，要么具有一个值并具有零个或多个**孩子**，每个孩子本身也是一个树。

二叉树是树的特殊形式，它的每个节点至多有两个孩子，分别称作**左孩子**和**右孩子**。

二叉搜索树还具有一个额外的性质：每个节点的值比它的左子树的所有节点的值都要大，但比它的右子树的所有节点的值都要小。

注意：这个定义排除了树中存在值相同的节点可能

这使得二叉搜索树成为一种用关键值快速查找的工具，它的查找时间复杂度可为**O(log n)**，因为它是用到二分法快速锁定关键值。

![17Ilct.png](https://s2.ax1x.com/2020/02/12/17Ilct.png)

​																				（二叉搜索树）

## 在二叉搜索树的插入

​	当一个新值添加到一颗二叉搜索树时，它必须被放在合适的位置，继续保持二叉搜索树的属性。

插入的基本算法：

> ​	*如果树为空：*
>
> ​			*把新值作为根节点插入*
>
> ​	*否则：*
>
> ​			*如果新值小于当前节点的值：*
>
> ​					*把新值插入到当前节点的左子树中*
>
> ​			*否则：*
>
> ​					*把新值插入到当前节点的右子树中*

上述算法的**递归表达**正是对树的**递归定义**的直接结果。

( 注意：由于上述递归在算法的尾部出现<尾部递归>，所以我们可以用到迭代跟有效的实现这个算法 )

## 从二叉搜索树删除节点

​		从树的中部删除一个节点将会导致节点的子树和树的其余部分分离，我们必须重新连接它们。

所以从二叉搜索树删除节点分三种情况：

1. 删除节点为叶节点，则直接删除叶节点不会导致任何子树断开，所以不存在重新连接问题。
2. 删除节点只有一个孩子，则删除节点只需要将删除节点的父节点和它的孩子节点连接起来即可。
3. 删除节点具有两个孩子，则其中一种策略是找到该节点左子树的最大值，并将最大值节点删除，同时用这个最大值代替原先要删除那个节点的值。

## 在二叉搜索树中查找

​		根据二叉搜索树本身特性，则在树中查找值并不困难：

> *如果树为空：*
>
> ​		*这个值不在树中*
>
> *否则：*
>
> ​		*如果这个值和根节点的值相同:*
>
> ​				*成功找到这个值*
>
> ​		*否则：*
>
> ​				*如果这个值小于根节点的值:*
>
> ​						*查找左子树*
>
> ​				*否则：*
>
> ​						*查找右子树*

这个递归算法也属于**尾部递归**，所以采用迭代的方案更具效率。

## 在二叉搜索树中遍历

​		当你在检查这棵树的所有节点时，就是在遍历这棵树。

​		遍历树的节点有几种不同的遍历方式：前序，中序，后序，以及层次遍历。

![1HU0Wd.png](https://s2.ax1x.com/2020/02/12/1HU0Wd.png)

上图所描述的树的前序遍历为：20，12，5，16，25，28 （中->左->右）

中序遍历为：5，12，16，20，25，28（左->中->右）

后序遍历为：5，16，12，28，25，10（左->右->中）

层次遍历为：20，12，25，5，16，28

## 二叉搜索树的接口文件

```c
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
```

tree.h文件

## 二叉搜索树的线性结构

**数组形式的二叉搜索树**

用数组形式去实现二叉搜索树的关键是使用下标寻找节点的父节点和孩子节点。

下面有两套映射规则：

>*基于根节点为节点1，且数组从1开始的映射规则*
>
>​		节点N的父节点为节点N/2
>
>​		节点N的左孩子节点为节点2N
>
>​		节点N的右孩子节点为节点2N+1



> *基于根节点为节点0，且数组从0开始的映射规则*
>
> ​		节点N的父节点为节点(N+1)/2 - 1
>
> ​		节点N的左孩子节点为节点2N+1
>
> ​		节点N的右孩子节点为节点2N+2

上述两套规则都可以实现用数组来表示二叉搜索树

​		第一套可读性高，且符合现实习惯，但数组下标从1开始忽略了数组第一个元素的空间，在面对树中储存元素较大时浪费了空间。

​		在用数组实现二叉搜索树时，如何对未使用的数组元素初始化来表示该节点未使用也未赋值，用0表示也是一种方式，但0也是一个合法的数据值，这个时候就可以使用一个比较数组，它的元素是布尔类型，用于提示哪个节点被使用。

​		**缺陷：无论是静态数组，还是动态数组来实现二叉搜索树，都会存在一个问题，那就是面对不是那么平衡树来说，往往会浪费很大的空间，而且每次的新插入会使数组大小扩大一倍，这样使可用空间迅速耗尽。**

## 二叉搜索树的链式结构

最为常见的二叉搜索树实现方式，代码如下：

```c
/*
*使用动态分配的链式结构实现二叉搜索树
*/
#include "tree.h"
#include <assert.h>
#include <stdio.h>
#include <malloc.h>

/*
*指向树根节点的指针
*/
static TreeNode *tree;

/*
* insert
*/
void insert(TREE_TYPE value){
    TreeNode *current;
    TreeNode **link = &tree;

    while ((current = *link) != NULL) {
        if(value < current->value)
            link = &current->left;
        else {
            assert(value != current->value);
            link = &current->right;
        }
    }
    /*
    *分配一个新的节点
    */
    current = malloc(sizeof(TreeNode));
    assert(current != NULL);
    current->value = value;
    current->left = NULL;
    current->right = NULL;
    *link = current;
}

/*
* find
*/
TREE_TYPE *find(TREE_TYPE value){
    TreeNode *current = tree;

    while (current != NULL && current->value != value){
        if(value < current->value)
            current = current->left;
        else
            current = current->right;
    }

    if(current != NULL)
        return &current->value;
    else
        return NULL;
}

/*
* max_tree
*/
TREE_TYPE max_tree(TreeNode *link){
    while (link->right != NULL)
        link = link->right;
    return link->value;
}

/*
* remove
*/
void remove_value(TREE_TYPE value){
    TreeNode *current;
    TreeNode **link = &tree;
    /*
    *寻找节点
    */
    while ((current = *link)->value != value) {
        if (value < current->value)
            link = &current->left;
        else {
            assert(value != current->value);
            link = &current->right;
        }
    }
    /*
    *删除节点，分三种情况
    */
    if (current->left == NULL && current->right == NULL)
       *link = NULL;
    else if (current->left == NULL || current->right == NULL) {
        if (current->left == NULL)
            *link = current->right;
        else
            *link = current->left;
    } else {
        TREE_TYPE max = max_tree(current->left);
        remove_value(max);/*max必为叶节点*/
        current->value = max;
    }   
}

/*
* do_pre_order_traverse
* 执行一层前遍历序。这是一个辅助函数。
* 并不是用户接口
*/
static void do_pre_order_traverse(TreeNode *current, void (*callback)(TREE_TYPE value)){
    if (current != NULL){
        callback(current->value);
        do_pre_order_traverse(current->left, callback);
        do_pre_order_traverse(current->right, callback);
    }
}

/*
* pro_order_traverse
*/
void pro_order_traverse(void (*callback)(TREE_TYPE value)){
    do_pre_order_traverse(tree, callback);
}

/*
* 测试使用pro_order_traverse(test_printf)，用于前序遍历输出树的值
*/
void test_printf(TREE_TYPE value){
        printf("%d",value);
}
int main(){
    insert(2);
    insert(1);
    insert(3);
    remove_value(3);
    TREE_TYPE *k = find(3);
    if(k == NULL)
        printf("not find\n");
    else
        printf("find %d\n", *k);
    pro_order_traverse(test_printf);
    printf("\n");
    insert(4);
    insert(0);
    insert(9);
    printf("tree maximum %d\n",max_tree(tree));
    pro_order_traverse(test_printf);
}
```

link_tree.c文件

测试输出：

![1HWDpt.png](https://s2.ax1x.com/2020/02/12/1HWDpt.png)