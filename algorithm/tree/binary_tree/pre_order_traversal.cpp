#include <stack>
#include <unordered_set>

#include "tree.h"
#include "contruct_tree.hpp"

void preOrder(BinaryTree* tree) {
    if (tree == nullptr ||
        tree->head == nullptr) {
        return;
    }
    std::stack<Node*> s;
    std::unordered_set<Node*> accessed;
    s.push(tree->head);
    accessed.insert(tree->head);
    while (!s.empty()) {
        Node* node = s.top();
        if (node->left == nullptr ||
            (node->left != nullptr && accessed.find(node->left) == accessed.end())) {
            node->access();
        }
        if (node->left != nullptr && accessed.find(node->left) == accessed.end()) {
            s.push(node->left);
            accessed.insert(node->left);
        }
        else {
            s.pop();
            if (node->right != nullptr && accessed.find(node->right) == accessed.end()) {
                s.push(node->right);
                accessed.insert(node->right);
            }
        }
    }
}

void preOrderImpl(Node* node) {
    if (node == nullptr) {
        return;
    }
    node->access();
    preOrderImpl(node->left);
    preOrderImpl(node->right);
}

void preOrderRecursive(BinaryTree* tree) {
    if (tree == nullptr ||
        tree->head == nullptr) {
        return;
    }
    preOrderImpl(tree->head);
}

int main()
{
    BinaryTree* tree = constructTree();
    preOrder(tree);
    //preOrderRecursive(tree);
    destructTree(tree);
    return 0;
}
