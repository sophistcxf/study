#include <iostream>
#include <vector>
using namespace std;

struct Node
{
  Node(int i) : index(i) {}
  int index;
  Node* next;
};

void sort_bubble(Node*& list)
{
  int length = 0;
  Node* head = list;
  while (head != NULL)
  {
    length++;
    head = head->next;
  }
  head = list;
  while (length > 0)
  {
    Node* cur = head;
    Node* pre = NULL;
    int step_times = length - 1;
    while (step_times-- > 0)
    {
      if (cur->index > cur->next->index)
      {
        if (pre == NULL)
        {
          head = cur->next;
          cur->next = cur->next->next;
          head->next = cur;
          pre = head;  
        }
        else
        {
          pre->next = cur->next;
          cur->next = cur->next->next;
          pre->next->next = cur;
          pre = pre->next;
        }
      }
      else
      {
        pre = cur;
        cur = cur->next;
      }
    }
    length--;
  }
  list = head;
}

Node* find_min(Node* list)
{
  if (list == NULL)
  {
    return NULL;
  }
  Node* min = list;
  Node* cur = list->next;
  while (cur != NULL)
  {
    if (min->index > cur->index)
    {
      min = cur;
    }
    cur = cur->next;
  }
  return min;
}

void sort_insert(Node*& list)
{
}

int main()
{
  vector<Node*> nodes;
  for (int i = 0; i < 10; ++i)
  {
    nodes.push_back(new Node(i));
  }
  // 6, 4, 5, 3, 2, 7, 1, 8, 9, 0
  Node* head = nodes[6];
  nodes[6]->next = nodes[4];
  nodes[4]->next = nodes[5];
  nodes[5]->next = nodes[3];
  nodes[3]->next = nodes[2];
  nodes[2]->next = nodes[7];
  nodes[7]->next = nodes[1];
  nodes[1]->next = nodes[8];
  nodes[8]->next = nodes[9];
  nodes[9]->next = nodes[0];
  nodes[0]->next = NULL;

  sort_bubble(head);

  while (head != NULL)
  {
    cout << head->index << endl;
    head = head->next;
  }

  return 0;
}
