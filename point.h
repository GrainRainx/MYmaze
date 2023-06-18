#ifndef POINT_H
#define POINT_H



class point
{
public:
    point();
    point(int x,int y,int stat);

    bool operator!=(const point&p);

public:
    int x;
    int y;
    int status;
    //点的状态
    //0时路，1是墙，2是起点，3是终点
    int pionner;
};

#endif // POINT_H
