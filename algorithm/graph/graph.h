/*************************************************************************
  > File Name: graph.h
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat 25 Jun 2016 07:38:39 PM PDT
 ************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>

class Graph
{
public:
  virtual bool addNode(char c) = 0;
  virtual bool addEdge(char b, char e, int w = 0) = 0;
  virtual void dfsStack() = 0;
  virtual void bfsQueue() = 0;
  virtual void dependency() = 0;
};

class GraphAdjacencyList : public Graph
{
public:
  virtual bool addNode(char value)
  {
    if (getNode(value) != NULL) return false;
    _nodes.push_back(Node(value));
  }
  virtual bool addEdge(char b, char e, int w = 0)
  {
    Node* node_b = getNode(b);
    Node* node_e = getNode(e);
    if (node_b == NULL || node_e == NULL) return false;
    while (node_b->next != NULL && node_b->next->value != e) node_b = node_b->next; 
    if (node_b->next == NULL) { node_b->next = new Node(e); }
    node_b->next->weight = w;
    return true;
  }
  virtual void dfsStack()
  {
    std::map<char, bool> is_access;
    std::stack<Node*> s;
    for (int i = 0; i < _nodes.size(); ++i) {
      is_access.insert(make_pair(_nodes[i].value, false));
      s.push(&_nodes[_nodes.size() - i -1]);
    }
    while (!s.empty()) {
      Node* t = s.top();
      s.pop();
      if (is_access[t->value]) { continue; }
      cout << t->value << "\t";
      is_access[t->value] = true;
      Node* c = t->next;
      while (c != NULL) {
        s.push(c);
        c = c->next;
      }
    }
    cout << endl;
  }

  // 这个算法有个问题，当一个节点有多个入度，只要其中一个入度探索到此节点
  // 就会访问此节点
  virtual void bfsQueue()
  {
    // 0: white, 1: gray, 2: black
    std::map<char, int> is_access;
    std::queue<Node*> q;
    for (int i = 0; i < _nodes.size(); ++i) {
      is_access.insert(make_pair(_nodes[i].value, 0));
    }
    for (int i = 0; i < _nodes.size(); ++i) {
      if (is_access[_nodes[i].value] == 2) continue;
      q.push(&_nodes[i]);
      cout << _nodes[i].value << "\t";
      is_access[_nodes[i].value] = 1;
      while (!q.empty()) {
        Node* t = q.front();
        q.pop();
        Node* c = t->next;
        while (c != NULL) {
          if (is_access[c->value] == 0) {
            cout << c->value << "\t";
            is_access[c->value] = 1;
            q.push(getNode(c->value));
          }
          c = c->next;
        }
        is_access[t->value] = 2;
      }
    }
    cout << endl;
  }

  virtual void dependency() {
    // 先找到所有节点的入度
    std::map<char, std::unordered_set<char> > node_in;

    for (const auto& node : _nodes) {
      node_in[node.value] = std::unordered_set<char>();
    }

    for (const auto& node : _nodes) {
      Node* next = node.next;
      while (next != nullptr) {
        node_in[next->value].insert(node.value);
        next = next->next;
      }
    }

    std::map<char, int> is_access;
    std::queue<Node*> q;
    for (int i = 0; i < _nodes.size(); ++i) {
      is_access.insert(make_pair(_nodes[i].value, 0));
      q.push(&_nodes[i]);
    }

    while (!q.empty()) {
        Node* t = q.front();
        q.pop();
        if (is_access[t->value] == 1) {
          continue;
        }
        if (!node_in[t->value].empty()) {
          q.push(t);
          continue;
        }
        std::cout << t->value << "\t";
        is_access[t->value] = 1;
        Node* c = t->next;
        while (c != nullptr) {
          node_in[c->value].erase(t->value);
          q.push(getNode(c->value));
          c = c->next;
        }
    }
    cout << std::endl;
  }
private:
  struct Node
  {
    Node(char v) : value(v), next(NULL) {}
    char value;
    int weight;
    Node* next;
    bool operator == (const Node& node) const { 
      return node.value == value;
    }
    bool operator == (const char& v) const {
      return value == v;
    }
  };
  Node* getNode(char value)
  {
    std::vector<Node>::iterator iter = std::find_if(_nodes.begin(), _nodes.end(), std::bind1st(std::equal_to<Node>(), value)); 
    return iter == _nodes.end() ? NULL : &(*iter);
  }
  std::vector<Node> _nodes; 
};

class GraphAdjacentyMatrix: public Graph
{
};
