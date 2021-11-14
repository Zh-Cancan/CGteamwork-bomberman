#pragma once
#include "base.h"
class Wall{
    int walls[MAPSIZE][MAPSIZE];//有方块置1，无方块置0
    POS p_pos;
    vector<POS> e_pos;
public:
    Wall();//读入地图
    bool InitWall(string filepath);
    POS GetFirstSpace();//获取地图中第一个符合条件的空格位置、用于初始化玩家位置-------->用于测试，现已弃用，改为读入方式
    POS GetLastSpace();//获取地图中最后一个符合条件的空格位置、用于初始化敌人位置-------->用于测试，现已弃用，改为读入方式
    bool isEdge(POS p);//当在p处放置炸弹后地图进行更新
    void Update(POS bombPos);//更新地图
    friend class Player;
};
