#include "point.h"

point::point()
{
    this->x=-1;
    this->y=-1;
    this->status=5;//?
    this->pionner=-1;//?
}



point::point(int x,int y,int stat)
{
    this->x=x;
    this->y=y;
    this->status=stat;
    this->pionner=-1;//这个到底表示什么意思
}

bool point::operator!=(const point&p)
{
    if(this->x!=p.x||this->y!=p.y)
    {
        return true;
    }
    return false;
}
