#ifndef BINOQUEUE_H
#define BINOQUEUE_H

#include "../nodearc.h" // dfn of node and arc

#ifndef ulong
typedef unsigned long ulong; // to get that extra bit
#endif

class SP;     // defined in sp.h
class Stack;  // defined in stack.h
struct Level; // defined below; defined here for Bucket

// Node structure for binomial heap，use left-child next sibling
typedef struct BinoNode
{
    Node *value; // store content is node，need to judge Node.dist
    BinoNode *child;
    BinoNode *sibling;
};

class BinoQueue
{
private:
    BinoNode **head; // store an array of pointer to BinoTree,could be null if no tree in this level.
    ulong capacity;
    BinoNode *combineTrees(BinoNode *tree1, BinoNode *tree2); // combine two trees
    void mergeQueue(BinoQueue *queue);                // merge param queue into this queue
    
    long findMin();                              // find the index of min binonode in the queue
    
public:
    // constructor, get space ,need to know the number of nodes.
    BinoQueue(ulong N,Node *nodes);
    BinoQueue(BinoNode **head, ulong capacity) : head(head), capacity(capacity) {} //get data directly
    ~BinoQueue();

    Node *Insert(Node *node);
    Node *Delete(Node *node);
    
    void dijkstra(Node *source, SP *sp); // run dijkstra's algorithm
    void PrintStats(long tries){};       // no need to see the detail of state.

    void buildQueue(ulong N,Node *nodes); // build the binomial queue
    
    Node *RemoveMin(); // remove min, important

    int InHeap(Node *node) { return node->where == IN_BINOQUEUE; }
};

#endif