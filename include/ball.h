#include "usingOpengl.h"


const GLfloat PI= 3.14159265358979323846f;
//将球横纵划分成50*50的网格
const int Y_SEGMENTS = 50;
const int X_SEGMENTS = 50;

class Ball
{
protected:

public:
    unsigned int genVAO();
    void draw();
};