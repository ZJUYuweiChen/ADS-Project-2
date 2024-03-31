#ifndef FIBHEAP_H
#define FIBHEAP_H

#include "fiboheap_core.h"
#include "nodearc.h"
#include "sp.h" //get shortest path wrapper class

#ifndef ulong
typedef unsigned long ulong; // to get that extra bit
#endif

typedef struct FibArc;

typedef FibNode<FibArc*> FiboNode;

//with a dummy head, arc use only in fiboheap.
typedef struct FibArc{
    long long len;  //len of arc
    FiboNode *head; //the conjunctive node
    FibArc *next;   //NULL if no other edge
} ;

class FiboHeap_Wrapper{
private:
    FibHeap<FibArc *> * instance;
    void initHeap(ulong N, Node *nodes);
public:
    FiboHeap_Wrapper(ulong N,Node *nodes);

    ~FiboHeap_Wrapper();
    void dijkstra(Node *source, SP *sp);
    FiboNode *RemoveMin();
    void reInit(ulong N, Node *nodes);
    void printfStats();
};

#endif
