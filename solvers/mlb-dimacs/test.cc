
#include <iostream>

#include "fiboheap.h"
#include "sp.h"

using namespace std;

int main(){
    Node list[] = {{0,NULL,NULL,0,0}, //一个简单的有向图，用于测试
        {997,NULL,NULL,0,0},
        {998,NULL,NULL,0,0},
        {999,NULL,NULL,0,0}};
    
    Arc alist[] = {{10,list+1},
        {60,list+2},
        {5,list+3},
        {200,list},
        {4,list+2}};

    list[0].first = alist; //第一个节点连有两条边，与第二和第三个节点相连
    list[1].first = alist+2; //第二个节点只与最后一个节点相连
    list[2].first = alist+3; //第三个节点与一个节点相连
    list[3].first = alist+4; //第四个节点与第三个节点相连

    for(int q=0;q<4;q++){
        std::cout<<list[q].dist<<" ";
    }
    std::cout<<endl;

    SP *sp = new SP(4,list,1,1,false);
    sp->sp(list); //以第一个节点为起点
    
    sp->PrintStats(0);
    return 0;
}