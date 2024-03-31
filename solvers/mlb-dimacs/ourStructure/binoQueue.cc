#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>                // has stderr, etc.
#include <string.h>               // has memset

#include <math.h>

#include "../sp.h" //get shortest path wrapper class

#include "binoQueue.h" //get binomial heap node structure and class



BinoQueue::BinoQueue(ulong N,Node *nodes)
{
    // initialize the binomial queue
    capacity = ulong(log2(N)) + 1;
    head = new BinoNode *[capacity]; //define a forest array
    memset(head, NULL, sizeof(BinoNode *) * capacity); //set null
    buildQueue(N,nodes); //build the binomial queue
}

BinoQueue::~BinoQueue()
{
    // delete the binomial tree one by one
    for (ulong i = 0; i < capacity; i++)
    {
        if (head[i] != NULL)
        {
            delete head[i];
        }
    }
    delete[] head;
}

//core function, first combine tree of same level ,then merge queue
BinoNode* BinoQueue::combineTrees(BinoNode *tree1, BinoNode *tree2)
{
    if(tree1->value->dist > tree2->value->dist) //swap the two trees to make sure smaller one is root.
    {
        BinoNode *temp = tree1;
        tree1 = tree2;
        tree2 = temp;
    }
    tree2->sibling = tree1->child; //make tree2 the child of tree1(from big to small)
    tree1->child = tree2;
    return tree1;
}

void BinoQueue::mergeQueue(BinoQueue *queue){ //make sure this queue is the larger one.
    if(queue == NULL) return;
    BinoNode *carry = NULL;
    for(ulong i = 0; i < queue->capacity; i++) //combine every tree at same level.
    {
        BinoNode *tree1 = head[i];
        BinoNode *tree2 = queue->head[i];
        //from small to big.
        switch(!!tree1 + 2 * !!tree2 + 4 * !!carry) //use bit operation...
        {
            case 0: //no tree
            case 1: //only tree1
                break;
            case 2: //only tree2
                head[i] = tree2;
                queue->head[i] = NULL;
                break;
            case 4: //only carry
                head[i] = carry;
                carry = NULL;
                break;
            case 3: //tree1 and tree2
                carry = combineTrees(tree1, tree2);
                head[i] = NULL;
                queue->head[i] = NULL;
                break;
            case 5: //tree1 and carry
                carry = combineTrees(tree1, carry);
                head[i] = NULL;
                break;
            case 6: //tree2 and carry
                carry = combineTrees(tree2, carry);
                queue->head[i] = NULL;
                break;
            case 7: //tree1, tree2 and carry
                head[i] = carry;
                carry = combineTrees(tree1, tree2);
                queue->head[i] = NULL;
                break;
        }
    }
}

long BinoQueue::findMin()
{
    long minIndex = -1;
    for (ulong i = 0; i < capacity; i++)
    {
        if (head[i] == NULL) return; // no tree in this place.
            if (minIndex == -1 || head[i]->value->dist < head[minIndex]->value->dist)
            { //find the min root among forest.
                minIndex = i;
            }
    }
    return minIndex;
}

Node * BinoQueue::RemoveMin(){
    long minIndex = findMin(); //1.firstly find min
    if(minIndex == -1) return NULL;

    BinoNode *minNode = head[minIndex];
    Node *minValue = minNode->value; //2.record the min value and take it out.
    head[minIndex] = NULL;

    BinoNode *child = minNode->child; //3.get the children of min root, set then as a new queue.
    delete minNode; //delete min root
    if(minIndex == 0) return minValue; //needless to merge
    BinoNode **newHead = new BinoNode *[minIndex];
    //load chidren into new forest, remember to extract from big to small
    long newTreeIndex = minIndex -1;
    while(child!=NULL || newTreeIndex >=0){
        newHead[newTreeIndex] = child;
        child = child->sibling;
        newTreeIndex--;
    }
    BinoQueue *newQueue = new BinoQueue(newHead,minIndex);

    mergeQueue(newQueue); //4.merge the new queue into this queue.
    return minValue;
}


void BinoQueue::buildQueue(ulong N,Node *nodes)
{
    // build the binomial queue
    for (ulong i = 0; i < N; i++)
    {
        nodes[i].dist = VERY_FAR; //initialize the node
        nodes[i].parent = NULL;
        Insert(&nodes[i]); //insert the node into the binomial queue, one by one.
    }
}

Node * BinoQueue::Insert(Node *node)
{
    BinoNode *newNode = new BinoNode; //wrapped this node.
    newNode->value = node;
    newNode->child = NULL; //init leaf node , very important
    newNode->sibling = NULL;
    BinoQueue *newQueue = new BinoQueue(1,node);
    newQueue->head[0] = newNode;
    mergeQueue(newQueue);
    return node;
}


void SmartQ::dijkstra(Node *source, SP *sp) //source是节点列表，sp是算法主类
{
   Node *currentNode, *newNode;   // newNode is beyond our current range
   Arc *arc, *lastArc;            // last arc of the current node
   Bucket *bckOld, *bckNew;
   //将source装填到数据结构中，替换成build函数
   reInit();                        // reset indices
   mu = 0;
   sp->curTime++;
   source->tStamp = sp->curTime;


   source->where = IN_F;
   F->Push(source);

   do
   {
     if (F->IsEmpty()) {
        
        //替换成DeleteMin函数
       currentNode = RemoveMin(); //在所有未确定节点堆中寻找最短节点，
       if (currentNode == NULL) {
	 assert(F->IsEmpty()); //没找到，说明已经找完（不然该图不连通，报错）
	 break;
       }
     }
     else {
       currentNode = (Node *) F->Pop(); //F 还没遍历完
     }
     
     //     printf(">>>scanning d %lld (mu %lld)\n", currentNode->dist, mu);
     sp->cScans++;      //遍历顶点数 的 计数， 和 cRuns 类似，都是统计用
     assert(currentNode->tStamp == sp->curTime);
     currentNode->where = IN_SCANNED; //找过该节点
     // scan node
     lastArc = (currentNode + 1)->first - 1;
     for ( arc = currentNode->first; arc <= lastArc; arc++ )
      {
	 newNode = arc->head;                      // where our arc ends up 遍历相邻节点
	 if (newNode->tStamp != sp->curTime)    //测试点更新了，存放的是上一次的dist，距离重新设为无限长
	   sp->initNode(newNode); 
    

	 if ( currentNode->dist + arc->len < newNode->dist ) //经典的 dijkstra 松弛条件
	 {
	   newNode->dist = currentNode->dist + arc->len; // we're shorter 更新距离，松弛
	   newNode->parent = currentNode;                // update sp tree 记录最短可达的父节点

       //以下是 多层桶 的数据结构保持过程，看不懂，替换成update维持函数即可
	   
	 }
      }
   } while (1);
}

