#include <stack>
#include <unordered_set>

#include "tree.h"
#include "contruct_tree.hpp"

using namespace std;

void inOrder(BinaryTree* tree) {
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
        if (node->left != nullptr && accessed.find(node->left) == accessed.end()) {
            s.push(node->left);
            accessed.insert(node->left);
        }
        else {
            node->access();
            s.pop();
            if (node->right != nullptr) {
                s.push(node->right);
                accessed.insert(node->right);
            }
        }
    }
}

/**
 * 这个应该是最简单的写法了
 */
void inOrder_V2(BinaryTree* tree) {
    if (tree == nullptr ||
        tree->head == nullptr) {
        return;
    }

    Node* p = tree->head;
    std::stack<Node*> s;

    while (p != nullptr || (!s.empty())) {
        while (p != nullptr) {
            s.push(p);
            p = p->left;
        }
        p = s.top();
        s.pop();
        p->access();
        p = p->right;
    }
}

// morris 方法
void inOrder_V3(BinaryTree* tree) {
    	if(tree==nullptr){
        	return;
        }
    	stack<BinaryTree*> st;
    	st.push(tree);
    	Node* node;
    	while(!st.empty() || node !=nullptr){
        
        	if(node!=nullptr){
              	st.push(node);
            	node =node->left;
            }else{
            	BinaryTree* node2 = st.top();
              	st.pop();
              	node = node2->right;
            }
        }
}

void inOrderImpl(Node* node) {
    if (node == nullptr) {
        return;
    }
    inOrderImpl(node->left);
    node->access();
    inOrderImpl(node->right);
}

void inOrderRecursive(BinaryTree* tree) {
    if (tree == nullptr ||
        tree->head == nullptr) {
        return;
    }
    inOrderImpl(tree->head);
}

int main()
{
    BinaryTree* tree = constructTree();
    //inOrder(tree);
    //inOrder_V2(tree);
    inOrder_V3(tree);
    //inOrderRecursive(tree);
    destructTree(tree);
    return 0;
}
