#ifndef WIDGET_H
#define WIDGET_H
#include"point.h"
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QStack>
#include <QList>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QProcess>
#include <QRandomGenerator>
#include<QPixmap>
#include<QVector>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget;
class Widget_maze;}
QT_END_NAMESPACE


class Widget_maze : public QWidget
{
    Q_OBJECT

public:


    int row;//记录行数
    int col;//记录列数
    QVector<QVector<int>> maze_record;//记录一个副本，因为在DFS寻路过程时会将数据打标记，这些标记的元素会放入到栈中
    //在最后寻路完成后，需要将原地图覆盖掉打完标记的地图，然后打完标记的数据存在栈中一格格弹出更新路劲地图，刷新。

    QVector<QVector<int>> maze_data;//记录数据应该刷成什么颜色
    // 0 刷成白色 路
    // -1 刷成黄色 路径
    // 1 刷成黑色 墙
    // 2 刷成绿色 起点
    // 3 刷成红色 表示终点
    point *star; //记录起点位置
    point *end;  //记录终点位置
    point *cur; // 记录现在的位置




public:



    void resetMazeData();

    int randomNumber(int x, int y);

    QVector<QVector<int>> autoSetMaze(int row, int col);

    void find_DFS();

    point* find_DFS_road(point * p);



public:
    void A___star();




public:
    void sleep(int seconds);


public:
    QPainter *paint;
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

    Widget_maze(QWidget *parent = nullptr);
    ~Widget_maze();




private slots:

private:
    Ui::Widget_maze *ui;
};



class Widget : public QWidget
{
    Q_OBJECT

public:

    Widget_maze *bul;


public:
    void init_bul();


//    QPainter *paint;
//    void paintEvent(QPaintEvent *event);

    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_hand_pushButton_clicked();

    void on_close_pushButton_clicked();

    void on_DFS_pushButton_clicked();

    void on_auto_pushButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H







