#pragma once
class pro
{
public:
    int x, y;
    bool Expanded; //是否完全拓展
    int now_children; //目前已有的孩子节点的个数
    int Children[81];
    double Value;
    double Times;
};

