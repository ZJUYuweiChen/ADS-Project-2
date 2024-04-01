
#include "fiboheap.h"
#include "nodearc.h"


using namespace std;


FiboHeap_Wrapper::FiboHeap_Wrapper(ulong N, Node *nodes){
    initHeap(N, nodes);
}


FibArc *_All_Dummy = NULL;
FiboNode *_All_FNode = NULL;

void initFNode(FiboNode *n){
    n->visited = false;
    n->key = 9999999;
    n->degree = 0;
    n->mark = false;
    n->left = n;
    n->right = n;
    n->child = NULL;
    n->parent = NULL;
}

void FiboHeap_Wrapper::initHeap(ulong N, Node *nodes)
{
    //translate to local arc data structure.
    instance = new FibHeap<FibArc*>();
    //luckily arcs is everlasting in one program
    _All_Dummy = new FibArc[N];
    //make all FiboNode then
    _All_FNode = new FiboNode[N];
    const int NodeSize = sizeof(Node *);

    for(int q=0;q<N;q++){
        Node *curNode =(nodes+q);
        FiboNode *curFNode = _All_FNode+q;
        FibArc *curArc = _All_Dummy+q;
        curArc->next=NULL; //dummy head
        initFNode(curFNode);
        curFNode->key = curNode->dist;
        curFNode->element = curArc;

        //traverse all edge of this node.
        Arc *lastArc = (curNode+1)->first - 1,*arc;
        if(q == N-1) lastArc = curNode->first;
        for ( arc = curNode->first; arc <= lastArc; arc++ )
        {
            FibArc * farc = (FibArc *)malloc(sizeof(FibArc)); //make new edge
            
            farc->len = arc->len;
            farc->head = _All_FNode + (arc->head - nodes);

            farc->next = NULL;
            curArc->next = farc;
            curArc = farc;
        }
        instance->Insert(curFNode);
    }
}

FiboHeap_Wrapper::~FiboHeap_Wrapper()
{
    instance->Destroy();
    delete instance;
    // delete _All_Dummy;
    // delete _All_FNode;
}

FiboNode *FiboHeap_Wrapper::RemoveMin()
{
    if (instance->m_min == NULL)
        return NULL;
    FiboNode *res = instance->m_min;
    instance->RemoveMin();
    return res;
}

void FiboHeap_Wrapper::reInit(ulong N, Node *nodes)
{
    instance->Destroy();
    delete instance;
    instance = new FibHeap<FibArc*>();
    for(int q=0;q<N;q++){
        FiboNode *fnode = _All_FNode+q;
        //use already exist node! much faster!
        instance->Insert(fnode);
    }
}

void FiboHeap_Wrapper::dijkstra(Node *source, SP *sp) // source是节点列表，sp是算法主类
{
    FiboNode *currentNode, *adj; // newNode is beyond our current range
    FibArc *arc = NULL;          // last arc of the current node
    
    sp->curTime++;                    // 有多个测试点，用 time标记
    Node *allRaw = sp->getNodes();
    // 将整个图装填到数据结构中
    long nodeNum =sp->getNodeNum(),srcIndex = source-allRaw; 
    for(int q=0;q<nodeNum;q++){
        initFNode(_All_FNode+q); // 初始化所有节点
        if(q == srcIndex)
            _All_FNode[q].key = 0; // 将源点的距离设为0
        
    }
    reInit(sp->getNodeNum(), allRaw); // 重新建堆
    _All_FNode[(source-allRaw)].key = 0; // 将源点的距离设为0

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
        arc = currentNode->element->next; // first arc of the current node
        
        while(arc !=NULL)
        {
            adj = arc->head; // where our arc ends up 遍历相邻节点
            //cout<<"arc scan: "<<arc->len<<" adj: "<<adj->key<<endl;
            if (adj->visited){
                arc = arc->next;
                continue; // 已经确定最短路径，不再处理
            }
            if (currentNode->key + arc->len < adj->key) // 经典的 dijkstra 松弛条件
            {
                //堆数据结构维护，代替 Multi Bucket
                instance->Decrease(adj,currentNode->key + arc->len); // 更新最短路径
            }
            arc = arc->next;
        }
    } while (1);
}

void FiboHeap_Wrapper::printfStats(){
    for(int q=0;q<4;q++){
        cout<<_All_FNode[q].key<<endl;
    }
}