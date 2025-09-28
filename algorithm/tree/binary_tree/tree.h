#ifndef _BINARY_TREE_H__
#define _BINARY_TREE_H__

#include <iostream>

class  Node
{
public:
    Node(int v) : value_(v) {}
    void access() {
        std::cout << value_ << std::endl;
    }
public:
    Node* left = nullptr;
    Node* right = nullptr;
private:
    int value_ = 0;
};

class BinaryTree
{
public:
    Node* head = nullptr;
};

#endif
