#ifndef BINHEAP_H
#define BINHEAP_H

#include "binheap_core.h"
#include "nodearc.h"

#include "sp.h" //get shortest path wrapper class

#ifndef ulong
typedef unsigned long ulong; // to get that extra bit
#endif


typedef struct BinArc;

typedef BinNode<BinArc*> BinoNode;

//with a dummy head, arc use only in fiboheap.
typedef struct BinArc{
    long long len;  //len of arc
    BinoNode *head; //the conjunctive node
    BinArc *next;   //NULL if no other edge
} ;

class BinoHeap_Wrapper{
private:
    BinHeap<BinArc *> * instance;
    void initHeap(ulong N, Node *nodes);
public:
    BinoHeap_Wrapper(ulong N,Node *nodes);

    ~BinoHeap_Wrapper();
    void dijkstra(Node *source, SP *sp);
    BinoNode *RemoveMin();
    void reInit(ulong N, Node *nodes);
    void printfStats();
};

#endif
