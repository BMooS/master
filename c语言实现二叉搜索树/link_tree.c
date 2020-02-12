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