#include "widget.h"

#include <QApplication>
#include <QPainter>
#include<QVector>
#include<iostream>
#include<vector>
#include<queue>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.setFixedSize(400,600);
    w.show();
    return a.exec();
}
