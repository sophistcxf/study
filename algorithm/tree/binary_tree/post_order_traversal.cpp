#include <stack>
#include <unordered_set>

#include "tree.h"
#include "contruct_tree.hpp"

void postOrder(BinaryTree* tree) {
    if (tree == nullptr ||
        tree->head == nullptr) {
        return;
    }
    std::stack<Node*> s;
    Node* pre = tree->head();
    while (pre != NULL || !s.empty()) {
        while (pre != NULL) {
            s.push(pre);
            pre = pre->left;
        }
    }
}

int main()
{
    BinaryTree* tree = constructTree();
    postOrder(tree);
    destructTree(tree);
    return 0;
}
