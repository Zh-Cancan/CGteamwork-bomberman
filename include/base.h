#pragma once
#define A_pos 65
#define W_pos 87
#define S_pos 83
#define D_pos 68
#define BackSpace 13
#include <fstream>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
//#include <bits/stdc++.h>
using namespace std;
const int MAPSIZE=12;
const int BomSpan[4][2]={{0,-1},{-1,0},{0,1},{1,0}};
struct POS{
    int x;
    int y;
    bool operator !=(POS a){
        return this->x!= a.x||this->y!=a.y;
    }
    bool operator ==(POS a) {
        return this->x == a.x && this->y == a.y;
    }
    bool equal(int i,int j){
        return x==i&&y==j;
    }
    void set(int i,int j){
        x=i;
        y=j;
    }
    POS move(int dx, int dy) {
        POS p;
        p.set(this->x + dx, this->y + dy);
        return p;
    }
};
struct BOMB {
    POS pos;
    clock_t startTime;
    BOMB(POS p, clock_t t) {
        pos = p;
        startTime = t;
    }
    bool operator ==(POS a) {
        return pos == a;
    }
    bool operator !=(BOMB a)
    {
        return pos != a.pos || startTime != a.startTime;
    }
};