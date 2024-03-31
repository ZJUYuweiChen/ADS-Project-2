
#include "binheap.h"
#include "nodearc.h"

using namespace std;


BinoHeap_Wrapper::BinoHeap_Wrapper(ulong N, Node *nodes){
    initHeap(N, nodes);
}


BinArc *_Bin_All_Dummy = NULL;
BinoNode *_All_BNode = NULL;

void initFNode(BinoNode *n){
    n->visited = false;
    n->key = 9999999;
    n->degree = 0;
    n->child = NULL;
    n->parent = NULL;
    n->next = NULL;
}

void BinoHeap_Wrapper::initHeap(ulong N, Node *nodes)
{
    //translate to local arc data structure.
    instance = new BinHeap<BinArc*>();
    //luckily arcs is everlasting in one program
    _Bin_All_Dummy = new BinArc[N];
    //make all BinoNode then
    _All_BNode = new BinoNode[N];
    const int NodeSize = sizeof(Node *);

    for(int q=0;q<N;q++){
        Node *curNode =(nodes+q);
        BinoNode *curFNode = _All_BNode+q;
        BinArc *curArc = _Bin_All_Dummy+q;
        curArc->next=NULL; //dummy head
        initFNode(curFNode);
        curFNode->key = curNode->dist;
        curFNode->element = curArc;

        //traverse all edge of this node.
        Arc *lastArc = curNode->first - 1,*arc;
        for ( arc = nodes->first; arc <= lastArc; arc++ )
        {
            BinArc * farc = (BinArc *)malloc(sizeof(BinArc)); //make new edge
            farc->head = _All_BNode + ((arc->head - nodes));
            farc->len = arc->len;
            farc->next = NULL;
            curArc->next = farc;
            curArc = farc;
        }
        instance->Insert(curFNode);
    }
    
    cout<<"finish build heap:"<<_All_BNode->key<<endl;
}

BinoHeap_Wrapper::~BinoHeap_Wrapper()
{
    instance->Destroy();
    delete instance;
    delete _Bin_All_Dummy;
    delete _All_BNode;
}

BinoNode *BinoHeap_Wrapper::RemoveMin()
{
    
    BinoNode *res = instance->GetMin();
    if (res == NULL)
        return NULL;
    instance->RemoveMin();
    return res;
}

void BinoHeap_Wrapper::reInit(ulong N, Node *nodes)
{
    //cout<<"reinit"<<endl;
    instance->Destroy();
    delete instance;
    instance = new BinHeap<BinArc*>();
    for(int q=0;q<N;q++){
        BinoNode *fnode = _All_BNode+q;
        //cout<<"reinit half: "<<fnode->key<<endl;
        initFNode(fnode);
        //use already exist node! much faster!
        instance->Insert(fnode);
    }
}

void BinoHeap_Wrapper::dijkstra(Node *source, SP *sp) // source是节点列表，sp是算法主类
{
    BinoNode *currentNode, *adj; // newNode is beyond our current range
    BinArc *arc = NULL;          // last arc of the current node
    
    sp->curTime++;                    // 有多个测试点，用 time标记
    sp->initNode(source);
    Node * allRaw = sp->getNodes();
    // 将整个图装填到数据结构中
    reInit(sp->getNodeNum(), allRaw); // 重新建堆
    _All_BNode[(source-allRaw)].key = 0; // 将源点的距离设为0

    source->tStamp = sp->curTime;
    Node *nodeEnd = source + sp->getNodeNum();
    do
    {
        currentNode = RemoveMin(); // 在所有未确定节点堆中寻找最短节点，
        if (currentNode == NULL)
        {
            break; // 走完所有顶点
        }
        //cout<<"dist: "<<currentNode->key<<endl;

        currentNode->visited = true; // 已经从堆中取出，标记finish
        sp->cScans++; // 遍历顶点数 的 计数， 和 cRuns 类似，都是统计用
        // scan node
        arc = currentNode->element->next; // last arc of the current node
        
        while(arc !=NULL)
        {
            //cout<<"arc: "<<arc->len<<endl;
            adj = arc->head; // where our arc ends up 遍历相邻节点
            if (adj->visited){
                arc = arc->next;
                continue; // 已经确定最短路径，不再处理
            }
            if (currentNode->key + arc->len < adj->key) // 经典的 dijkstra 松弛条件
            {
                //堆数据结构维护，代替 Multi Bucket
                instance->DecreaseKey(adj,currentNode->key + arc->len); // decrease key
            }
            arc = arc->next;
        }
    } while (1);
}

void BinoHeap_Wrapper::printfStats(){
    instance->Print();
}