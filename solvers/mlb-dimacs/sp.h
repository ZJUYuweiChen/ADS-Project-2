// sp.h  by Andrew Goldberg
//     Started 5/21/01
//     The main driver for shortest-path routines.  Whenever you want
//     to use an SP, you include sp.h and call sp::sp with the
//     algorithm you want, taken from the #defines below.

#ifndef SP_H
#define SP_H

#include <stdlib.h>
#include "values.h"
#include "nodearc.h"
#include "stack.h"
#include "smartq.h"

#include "binheap.h"
#include "fiboheap.h"

#define VERY_FAR            922337203685477 // LLONG_MAX
#define FAR                 MAXLONG

#define SP_DIK_SMARTQ       1024       // use SP_S_Q + # of levels

#define DIK_BUCKETS_DEFAULT  2         // default of two bucket levels


int sp_openarc(Arc *arc);              // whether SP thinks an arc is usable
extern void ArcLen(long cNodes, Node *nodes,
		   long long *pMin = NULL, long long *pMax = NULL);


class FiboHeap_Wrapper;
class BinHeap_Wrapper;

class SP {
 private:
   long cNodes;

   int spType;
   void BFSInit(Node *source);


   //change this node to other data structure.
   Node *nodes;

   
   SmartQ *smartq;                    // for dijkstra_smartq
   Bucket *BFSqueue;                 // for baseline BSF
//** add new SP data structure here **//
   FiboHeap_Wrapper *fibHeap;         // for dijkstra_fibheap
   BinHeap_Wrapper *binHeap;          // for dijkstra_binominalheap

   
 public:
   SP(long cNodesGiven, Node *nodesGiven, ulong levels, ulong maxDelta,
      bool doBFS);
   ~SP();
   void init();
   void initS(Node *source);
   void initNode(Node *source);
   long getNodeNum(){return cNodes;}
   Node *getNodes(){return nodes;}
#ifdef SINGLE_PAIR
   bool sp(Node *source, Node *sink);
#else
   void sp(Node *source);
#endif
   // these must be public, alas, so Heap and Bucket can modify them
   long cCalls;         // # of times SP has been called since initialization
   long long cScans;         // # of nodes SP algorithm has looked at (since init)
   long long cUpdates;       // # of times a node value was lowered (since init)

   void PrintStats(long tries);
   void initStats();
   int nodeId(Node *i);
   long BFS(Node *source);   // to get baseline timing
   unsigned int curTime;
};

#endif
