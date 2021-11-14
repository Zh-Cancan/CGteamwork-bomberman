#include "../include/Enemy.h"
Enemy::Enemy(){}
void Enemy::InitEnemy(POS p){
    this->pos=p;
    this->lastPos = p;
    this->secondLastPos = p;
    this->walkback = 0;
    this->lastMoveTime = 0;
}
bool Enemy::JudgeDeath(POS bomPos){
    for(int i=0;i<4;i++){
        if(this->pos.equal(bomPos.x+BomSpan[i][0],bomPos.y+BomSpan[i][1])){
            return true;
        }
    }
    return false;
}
//对某个结点计算启发函数值
int Enemy::calculateF(POS src,POS des,int g) {
    if (src.equal(-1,-1))//起点
        return  abs(des.x - src.x) + abs(des.y - src.y);
    else
        return g + 1 + abs(des.x - src.x) + abs(des.y - src.y);
}
int findDes(vector<AStar> table,POS des) {
    for (int i = 0; i < table.size(); i++) {
        if (table[i].pos == des)
            return i;
    }
    return -1;
}
//利用A*算法为敌人寻找最优路径
void Enemy::findPath(POS des,int m[MAPSIZE][MAPSIZE]) {
    this->path.clear();//清空原来的路径
    this->open.clear();
    this->close.clear();
    AStar* start = new AStar(this->pos, abs(des.x - this->pos.x) + abs(des.y - this->pos.y), 0);
    this->open.push_back(*start);
    int p = -1;
    while (!this->open.empty()) {
        if ((p = findDes(open, des)) != -1)
            break;
        int min = min_element(this->open.begin(), this->open.end()) - this->open.begin();//open表中f最小的元素的下标
        AStar* now = new AStar(this->open[min].parent, this->open[min].pos, this->open[min].f, this->open[min].g);
        this->open.erase(this->open.begin() + min);//从open表中拿出
        this->close.push_back(*now);
        for (int i = 0; i < 4; i++) {
            POS next = now->pos.move(BomSpan[i][0], BomSpan[i][1]);
            if (!m[next.x][next.y]||des.equal(next.x,next.y)) {//不是地图块或炸弹放置的位置才可以作为路径
                AStar* child = new AStar(now, next);
                child->f = calculateF(child->pos, des, now->g);
                child->g = now->g + 1;
                int j;
                if (findDes(this->close, child->pos) != -1) {
                    continue;
                }
                else if ((j = findDes(this->open, child->pos)) != -1) {
                    if (this->open[j].f > child->f) {//open表中的这个节点的f值更大，则替换
                        this->open[j].f = child->f;
                        this->open[j].parent = child->parent;
                    }
                }
                else {//不在open列表中，将其加入
                    this->open.push_back(*child);
                }
            }
        }
    }
    if (p != -1) {
        //构造寻找到的路径
        this->path.push_back(open[p].pos);
        AStar* temp = open[p].parent;
        while (temp != NULL) {
            this->path.push_back(temp->pos);
            temp = temp->parent;
        }
        reverse(this->path.begin(), this->path.end());
        this->path.erase(this->path.begin() + 0);
    }
}
/*void Enemy::Walk(POS des) {
    if (clock_t nowTime = clock() - this->lastFindTime > 5000 || this->path.size() <= 1) {
        this->findPath(des);
        this->lastFindTime = clock();
    }
    if(this->path[0])
    this->lastPos = this->pos;
    this->secondLastPos = this->lastPos;
    this->pos = this->path[0];
    this->path.erase(this->path.begin() + 0);
}*/
