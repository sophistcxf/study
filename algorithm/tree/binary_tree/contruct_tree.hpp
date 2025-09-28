#include "tree.h"

/*
                       0
                     /   \
                    1     2
                   /     / \
                  3     7    8
                 / \          \
                4   5          9
                     \
                      6
 */

// inorder:4 3 5 6 1 0 7 2 8 9
// post order:4 6 5 3 1 7 9 8 2 0
static BinaryTree* constructTree() {
    BinaryTree* tree = new BinaryTree();
    Node* head = new Node(0);
    tree->head = head;
    head->left = new Node(1);
    head->right = new Node(2);
    head->left->left = new Node(3);
    head->left->left->left = new Node(4);
    head->left->left->right = new Node(5);
    head->left->left->right->right = new Node(6);
    head->right->left = new Node(7);
    head->right->right = new Node(8);
    head->right->right->right = new Node(9);
    return tree;
}

static void destructTree(BinaryTree* tree) {
    if (tree != nullptr) {
        delete tree;
    }
}
