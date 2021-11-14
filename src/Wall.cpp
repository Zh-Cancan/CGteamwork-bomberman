#include "../include/Wall.h"
Wall::Wall(){
}
bool Wall::InitWall(string filepath) {
    fstream f;
    f.open(filepath.c_str());
    if (!f) {
        cout << "open error!" << endl;
        return false;
    }
    int i = 0;
    while (!f.eof()) {
        string row;
        getline(f, row);
        for (int j = 0; j < MAPSIZE; j++) {
            if (row[j] == '#')
                walls[i][j] = 1;
            else {
                walls[i][j] = 0;
                if (row[j] == 'p')
                    this->p_pos.set(i, j);
                else if (row[j] == 'e') {
                    POS temp;
                    temp.set(i, j);
                    this->e_pos.push_back(temp);
                }
            }
        }
        i++;
    }
    return true;
}
POS Wall::GetFirstSpace() {
    POS pos = { 1,1 };
    while (pos.x < 11 && pos.y < 11) {
        if (!this->walls[pos.x][pos.y] &&
            ((!this->walls[pos.x][pos.y + 1] && !this->walls[pos.x][pos.y + 2]) ||
            (!this->walls[pos.x][pos.y + 1] && !this->walls[pos.x + 1][pos.y]) ||
            (!this->walls[pos.x][pos.y - 1] && !this->walls[pos.x + 1][pos.y]) ||
            (!this->walls[pos.x][pos.y + 1] && !this->walls[pos.x - 1][pos.y]) ||
            (!this->walls[pos.x][pos.y - 1] && !this->walls[pos.x - 1][pos.y]))) {
            break;
        }
        else {
            if (pos.y == 10) {
                pos.x++;
                pos.y = 1;
            }
            else {
                pos.y++;
            }
        }
    }
    return pos;
}
POS Wall::GetLastSpace() {
    POS pos = { 10,10 };
    while (pos.x > 1 && pos.y > 1) {
        if (!this->walls[pos.x][pos.y] && 
            ((!this->walls[pos.x][pos.y - 1] && !this->walls[pos.x][pos.y - 2]) ||
            (!this->walls[pos.x][pos.y + 1] && !this->walls[pos.x + 1][pos.y]) ||
            (!this->walls[pos.x][pos.y - 1] && !this->walls[pos.x + 1][pos.y]) ||
            (!this->walls[pos.x][pos.y + 1] && !this->walls[pos.x - 1][pos.y]) ||
            (!this->walls[pos.x][pos.y - 1] && !this->walls[pos.x - 1][pos.y]))) {
            break;
        }
        else {
            if (pos.y == 1) {
                pos.x--;
                pos.y = 10;
            }
            else {
                pos.y--;
            }
        }
    }
    return pos;
}
bool Wall::isEdge(POS p) {
    if (p.x >= MAPSIZE - 1 || p.y >= MAPSIZE - 1 || p.x < 1 || p.y < 1) {
        return true;
    }
    return false;
}
void Wall::Update(POS bomPos) {
    for (int i = 0; i < 4; i++) {
        POS pos = { bomPos.x + BomSpan[i][0],bomPos.y + BomSpan[i][1] };
        if (!this->isEdge(pos) && walls[pos.x][pos.y]) {
            walls[pos.x][pos.y] = 0;
        }
    }
}
