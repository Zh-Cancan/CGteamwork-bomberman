#pragma once
#include "base.h"
struct AStar {
    AStar * parent;//路径的父结点
    POS pos;//当前结点的位置
    int f;//启发式
    int g;//实际距离
    AStar(AStar * p, POS pos) {
        this->parent = p;
        this->pos = pos;
    }
    AStar(POS pos, int f0, int g0) {
        this->parent = NULL;
        this->pos = pos;
        this->f = f0;
        this->g = g0;
    }
    AStar(AStar * p,POS pos,int f0,int g0) {
        this->parent = p;
        this->pos = pos;
        this->f = f0;
        this->g = g0;
    }
    bool operator <(AStar a) {
        return this->f < a.f;//以启发函数作为优先级
    }
};
class Enemy{
    POS pos;//敌人的位置
    vector<BOMB> bPos;//敌人放置的炸弹位置
    clock_t bomStartTime;//敌人放置炸弹的起始时间
    vector<AStar> open;//A*算法的open表
    vector<AStar> close;//A*算法的close表
    clock_t lastFindTime;//记录上次寻路时间，每五秒更新一次路径
    clock_t lastMoveTime;//记录上次移动的时间
    int walkback;
    POS lastPos;//记录上一步位置
    POS secondLastPos;//记录上两步位置
    vector<POS> path;//寻找到的路径
    friend class Player;
public:
    Enemy();
    void InitEnemy(POS);//初始化敌人位置
    bool JudgeDeath(POS bomPos);//判断敌人是否死亡
    int calculateF(POS src, POS des, int g);//对某个结点计算启发函数值
    void findPath(POS des, int m[MAPSIZE][MAPSIZE]);//利用A*算法为敌人寻找最优路径
    //void Walk(POS des);//敌人的行进、用到寻路算法、主要是对pos的更改、放置炸弹
    
};
