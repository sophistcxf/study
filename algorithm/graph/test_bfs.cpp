/*************************************************************************
  > File Name: test_bfs.cpp
  > Author: cxxxxf
  > Mail: sophistcxf@gmail.com
  > Created Time: Sat 25 Jun 2016 07:50:58 PM PDT
 ************************************************************************/

#include <iostream>
using namespace std;

#include "graph.h"

Graph* genGraph()
{
  Graph* graph = new GraphAdjacencyList;
  graph->addNode('a');
  graph->addNode('b');
  graph->addNode('c');
  graph->addNode('d');
  graph->addNode('e');
  graph->addEdge('a', 'b', 10);
  graph->addEdge('a', 'c', 3);
  graph->addEdge('b', 'c', 1);
  graph->addEdge('b', 'd', 2);
  graph->addEdge('c', 'b', 4);
  graph->addEdge('c', 'd', 8);
  graph->addEdge('c', 'e', 2);
  graph->addEdge('d', 'e', 7);
  graph->addEdge('e', 'd', 9);
  return graph;
}

/*!
 * 算法导论中练习22.3-2的图
 */
Graph* genGraph2()
{
  Graph* graph = new GraphAdjacencyList;
  graph->addNode('v');
  graph->addNode('s');
  graph->addNode('w');
  graph->addNode('q');
  graph->addNode('t');
  graph->addNode('x');
  graph->addNode('z');
  graph->addNode('y');
  graph->addNode('r');
  graph->addNode('u');
  graph->addEdge('v', 'w');
  graph->addEdge('s', 'v');
  graph->addEdge('w', 's');
  graph->addEdge('q', 's');
  graph->addEdge('q', 'w');
  graph->addEdge('q', 't');
  graph->addEdge('t', 'x');
  graph->addEdge('t', 'y');
  graph->addEdge('x', 'z');
  graph->addEdge('z', 'x');
  graph->addEdge('y', 'q');
  graph->addEdge('r', 'y');
  graph->addEdge('r', 'u');
  graph->addEdge('u', 'y');
  return graph;
}

Graph* genGraph3()
{
  Graph* graph = new GraphAdjacencyList;
  graph->addNode('a');
  graph->addNode('b');
  graph->addNode('c');
  graph->addNode('d');
  graph->addNode('e');
  graph->addNode('f');
  graph->addNode('g');
  graph->addNode('h');
  graph->addNode('i');
  graph->addNode('j');
  graph->addNode('k');
  graph->addNode('l');
  graph->addEdge('a', 'f');
  graph->addEdge('f', 'd');
  graph->addEdge('a', 'b');
  graph->addEdge('b', 'c');
  graph->addEdge('c', 'd');
  graph->addEdge('d', 'e');
  graph->addEdge('g', 'h');
  graph->addEdge('h', 'i');
  graph->addEdge('i', 'e');
  graph->addEdge('h', 'j');
  graph->addEdge('j', 'k');
  graph->addEdge('e', 'l');
  graph->addEdge('k', 'l');
  return graph;
}

int main()
{
  Graph* graph = genGraph3();
  graph->dfsStack();
  graph->bfsQueue();
  graph->dependency();
  return 0;
}
