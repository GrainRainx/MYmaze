#include "widget.h"
#include "ui_widget.h"
#include<QPainter>
#include<iostream>
#include <QPalette>
#include<vector>
#include<queue>
#include<math.h>
using namespace  std;




namespace A_star_
{
    #define fi first
    #define se second
    using namespace std;
    typedef pair<int,int>PII;
    int n,m;
    int dx[]={-1,0,1,0};
    int dy[]={0,1,0,-1};
    vector<vector<char>>graph;
    vector<PII>ans;
    struct node
    {
        int x,y;
    };
    node start,endpo;
    void init()
    {
        graph.resize(n+1);
        for(int i=1;i<=n;i++)graph[i].resize(m+1);
    }
    int get_f(int x,int y)
    {
        return abs(x-endpo.x)+abs(y-endpo.y);
    }
    bool A_star()
    {
        vector<vector<bool>>st(n+1,vector<bool>(m+1,false));
        vector<vector<PII>>pre(n+1,vector<PII>(m+1,{-1,-1}));
        vector<vector<int>>dist(n+1,vector<int>(m+1,1000000));
        priority_queue<pair<int,PII>,vector<pair<int,PII>>,greater<pair<int,PII>>>heap;
        heap.push({0,{start.x,start.y}});
        dist[start.x][start.y]=0;
        bool flag=false;
        while(heap.size())
        {
            auto t=heap.top();
            heap.pop();
            if(t.se.fi==endpo.x&&t.se.se==endpo.y)
            {
                flag=true;
                break;
            }
            int cur_x=t.se.fi,cur_y=t.se.se;
            for(int i=0;i<4;i++)
            {
                int nx=cur_x+dx[i],ny=cur_y+dy[i];
                if(nx<1||nx>n||ny<1||ny>m)continue;
                if(graph[nx][ny]=='*')continue;
                if(dist[nx][ny]==1000000||dist[nx][ny]>dist[cur_x][cur_y]+1)
                {
                    dist[nx][ny]=dist[cur_x][cur_y]+1;
                    pre[nx][ny]={cur_x,cur_y};
                    int get_gu=dist[nx][ny]+get_f(nx,ny);
                    pair innode = make_pair(get_gu,make_pair(nx,ny));
                    heap.push(innode);
                }
            }
        }
        if(!flag)return false;
        int thx=endpo.x,thy=endpo.y;
        while(1)
        {
            ans.push_back({thx,thy});
            if(thx==start.x&&thy==start.y)break;

            auto t=pre[thx][thy];

            thx=t.fi,thy=t.se;
        }
        reverse(ans.begin(),ans.end());
        return true;
    }
}
using namespace A_star_;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->star_label->setStyleSheet("QLabel{background-color:rgb(152,245,255);}");
    ui->end_label->setStyleSheet("QLabel{background-color:rgb(255,192,203);}");
    ui->path_label->setStyleSheet("QLabel{background-color:rgb(255,255,255);}");
    ui->wall_label->setStyleSheet("QLabel{background-color:rgb(139,69,19);}");
    ui->through_path_label->setStyleSheet("QLabel{background-color:rgb(82,139,139);}");
    bul = new Widget_maze;
    bul=nullptr;
}

Widget::~Widget()
{
    delete ui;
}

Widget_maze::Widget_maze(QWidget *parent)
{
    this->row=1;
    this->col=1;
    maze_record.clear();
    maze_data.clear();
}
Widget_maze::~Widget_maze()
{
    delete ui;
}

void Widget::init_bul()
{
    bul->maze_data.clear();
    bul->maze_record.clear();
    bul->row=1;
    bul->col=1;
    bul->star=nullptr;
    bul->end=nullptr;
}





void Widget_maze::resetMazeData()
{
    this->maze_data=this->maze_record;
}

int Widget_maze::randomNumber(int x, int y)
{
    return QRandomGenerator::global()->bounded(x, y);
}

QVector<QVector<int>> Widget_maze::autoSetMaze(int row, int col)
{
    QVector<QVector<int>> res;
    res.resize(row);
    for(int i=0;i<row;i++)res[i].resize(col);
    QList<point*> path_list;
    for (int i = 0; i < row; i++) {
        int flag = i % 2;
        for (int j = 0; j < col; j++) {
            if (flag == 0) {
                // 单数行全为墙
                res[i][j] = 1;
            }else {
                // 偶数行墙和路依次分布
                if (j % 2 == 0) {
                    res[i][j] = 1;
                }else {
                    res[i][j] = 0;
                }
            }
        }
    }
    // 随机选取地图边缘的路
    int flag_xy = randomNumber(0, 10);
    int x = 0;
    int y = 0;
    if (flag_xy % 2 == 0) {
        x = 1;
        y = 2 * randomNumber(0, (col - 1) / 2) + 1;
    } else {
        y = 1;
        x = 2 * randomNumber(0, (row - 1) / 2) + 1;
    }
    // 将该点做标记
    res[x][y] = 10;
    // 将该点周围的墙加入数组中
    if (x != 1 && res[x - 1][y] != 10) {
        path_list.push_back(new point(x - 1, y, 0));
    }
    if (x != row - 2 && res[x + 1][y] != 10) {
        path_list.push_back(new point(x + 1, y, 0));
    }
    if (y != 1 && res[x][y - 1] != 10) {
        path_list.push_back(new point(x, y - 1, 0));
    }
    if (y != col - 2 && res[x][y + 1] != 10) {
        path_list.push_back(new point(x, y + 1, 0));
    }
    while (path_list.empty() == false) {
        // 数组中随机选取一个点
        int index = randomNumber(0, path_list.size());
        point * ptr = path_list[index];
        int ptr_x = ptr->x;
        int ptr_y = ptr->y;
        // 从左到右
        if (res[ptr_x - 1][ptr_y] == 10 && res[ptr_x + 1][ptr_y] == 0) {
            res[ptr_x][ptr_y] = 10;
            res[ptr_x + 1][ptr_y] = 10;
            // 将周围的墙加入数组中
            if (ptr_x + 1 != row - 2 && res[ptr_x + 3][ptr_y] != 10) {
                path_list.push_back(new point(ptr_x + 2, ptr_y, 0));
            }
            if (ptr_y != 1 && res[ptr_x + 1][ptr_y - 2] != 10) {
                path_list.push_back(new point(ptr_x + 1, ptr_y - 1, 0));
            }
            if (ptr_y != col - 2 && res[ptr_x + 1][ptr_y + 2] != 10) {
                path_list.push_back(new point(ptr_x + 1, ptr_y + 1, 0));
            }
        }
        // 从右到左
        if (res[ptr_x + 1][ptr_y] == 10 && res[ptr_x - 1][ptr_y] == 0) {
            res[ptr_x][ptr_y] = 10;
            res[ptr_x - 1][ptr_y] = 10;
            // 将周围的墙加入数组中
            if (ptr_x - 1 != 1 && res[ptr_x - 3][ptr_y] != 10) {
                path_list.push_back(new point(ptr_x - 2, ptr_y, 0));
            }
            if (ptr_y != 1 && res[ptr_x - 1][ptr_y - 2] != 10) {
                path_list.push_back(new point(ptr_x - 1, ptr_y - 1, 0));
            }
            if (ptr_y != col - 2 && res[ptr_x - 1][ptr_y + 2] != 10) {
                path_list.push_back(new point(ptr_x - 1, ptr_y + 1, 0));
            }
        }
        // 从上到下
        if (res[ptr_x][ptr_y - 1] == 10 && res[ptr_x][ptr_y + 1] == 0) {
            res[ptr_x][ptr_y] = 10;
            res[ptr_x][ptr_y + 1] = 10;
            // 将周围的墙加入数组中
            if (ptr_x != row - 2 && res[ptr_x + 2][ptr_y + 1] != 10) {
                path_list.push_back(new point(ptr_x + 1, ptr_y + 1, 0));
            }
            if (ptr_x != 1 && res[ptr_x - 2][ptr_y + 1] != 10) {
                path_list.push_back(new point(ptr_x - 1, ptr_y + 1, 0));
            }
            if (ptr_y + 1!= col - 2 && res[ptr_x][ptr_y + 3] != 10) {
                path_list.push_back(new point(ptr_x, ptr_y + 2, 0));
            }
        }
        // 从下到上
        if (res[ptr_x][ptr_y + 1] == 10 && res[ptr_x][ptr_y - 1] == 0) {
            res[ptr_x][ptr_y] = 10;
            res[ptr_x][ptr_y - 1] = 10;
            // 将周围的墙加入数组中
            if (ptr_x != row - 2 && res[ptr_x + 2][ptr_y - 1] != 10) {
                path_list.push_back(new point(ptr_x + 1, ptr_y - 1, 0));
            }
            if (ptr_x != 1 && res[ptr_x - 2][ptr_y - 1] != 10) {
                path_list.push_back(new point(ptr_x - 1, ptr_y - 1, 0));
            }
            if (ptr_y - 1 != 1 && res[ptr_x][ptr_y - 3] != 10) {
                path_list.push_back(new point(ptr_x, ptr_y - 2, 0));
            }
        }
        path_list.removeAt(index);
    }
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (res[i][j] == 10) {
                res[i][j] = 0;
            }
        }
    }

    if (col > 20 || row > 20) {
        for (int i = 0; i < row * 5; i++) {
            int x = randomNumber(0, 100) % (row - 2) + 1;
            int y = randomNumber(0, 100) % (col - 2) + 1;
            if (res[x][y] == 1) {
                int flag = 0;
                if (res[x - 1][y] == 1) {
                    flag++;
                }
                if (res[x + 1][y] == 1) {
                    flag++;
                }
                if (res[x][y - 1] == 1) {
                    flag++;
                }
                if (res[x][y + 1] == 1) {
                    flag++;
                }
                if (flag > 1) {
                    res[x][y] = 0;
                }
            }
        }
    }
    return res;
}

void Widget_maze::sleep(int seconds)
{
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<seconds);
}


void Widget_maze::find_DFS()
{
    this->resetMazeData();
    // DFS寻路通过使用栈来寻找路径
    QStack<point*> pointStack;
    if (this->star == nullptr || this->end == nullptr) {
        // 如果起点和终点未设置则报错并退出
        QMessageBox::information(nullptr, "警告", "没有设置起点或终点，请设置");
        return;
    }

    //将start入栈并设为已经过（设为-1）
    pointStack.push_back(this->star);
    this->maze_data[this->star->x][this->star->y] = -1;
    //当栈不为空且栈顶元素不为end时，判断栈顶结点是否有后继未经过结点
    //     如果有未经过结点则入栈，否则栈顶弹出并将栈顶元素标为已经过
    while (pointStack.empty() == false && *(pointStack.top()) != *(this->end)) {
        point * next_point = find_DFS_road(pointStack.top());
        if (next_point->x == -1 || next_point->y == -1)
        {
            pointStack.pop();
            continue;
        }
        this->maze_data[next_point->x][next_point->y] = -1;
        pointStack.push(next_point);
    }
    if (pointStack.empty() == true) {
        QMessageBox::information(nullptr, "警告", "未找到通路");
        return;
    }

    QStack<point*> temp_stack;
    while (pointStack.empty() == false) {
        temp_stack.push_back(pointStack.top());
        pointStack.pop();
    }

    // 将栈中元素画到mazedata中
    this->resetMazeData();
    while (temp_stack.empty() == false) {
        this->maze_data[temp_stack.top()->x][temp_stack.top()->y] = -1;
        sleep(50);
        this->repaint();
        temp_stack.pop();
    }
    // 将起点和终点重新绘制
    this->maze_data[this->star->x][this->star->y] = 2;
    this->maze_data[this->end->x][this->end->y] = 3;
    this->repaint();
    QMessageBox::information(nullptr, "恭喜！", "路劲寻找成功");

}

point* Widget_maze::find_DFS_road(point * p)
{
    point * res = new point();
    res->status = 0;
    if (p->x - 1 >= 0 && this->maze_data[p->x - 1][p->y] != 1 && this->maze_data[p->x - 1][p->y] != -1) {
        res->x = p->x - 1;
        res->y = p->y;
    } else if (p->x + 1 < this->row && this->maze_data[p->x + 1][p->y] != 1 && this->maze_data[p->x + 1][p->y] != -1) {
        res->x = p->x + 1;
        res->y = p->y;
    } else if (p->y - 1 >= 0 && this->maze_data[p->x][p->y - 1] != 1 && this->maze_data[p->x][p->y - 1] != -1) {
        res->x = p->x;
        res->y = p->y - 1;
    } else if (p->y + 1 < this->col && this->maze_data[p->x][p->y + 1] != 1 && this->maze_data[p->x][p->y + 1] != -1) {
        res->x = p->x;
        res->y= p->y + 1;
    }
    return res;
}



void Widget::on_hand_pushButton_clicked()
{
    if(bul!=nullptr)
    {
        int get_row=ui->x_textEdit->toPlainText().toInt();
        int get_col=ui->y_textEdit->toPlainText().toInt();
        if(get_row==0||get_col==0)
        {
            QMessageBox::information(nullptr, "警告", "迷宫的横纵大小设置不对，请重新设置");
            return ;
        }
        init_bul();

        bul->row=get_row;
        bul->col=get_col;
        bul->maze_record.resize(bul->row);
        for(int i=0;i<bul->row;i++)
        {
            bul->maze_record[i].resize(bul->col);
        }
        for(int i=0;i<bul->row;i++)
            for(int j=0;j<bul->col;j++)
                bul->maze_record[i][j]=0;

        bul->resetMazeData();
        bul->update();
        bul->show();
    }
    else
    {
        int get_row=ui->x_textEdit->toPlainText().toInt();
        int get_col=ui->y_textEdit->toPlainText().toInt();
        if(get_row==0||get_col==0)
        {
            QMessageBox::information(nullptr, "警告", "迷宫的横纵大小设置不对，请重新设置");
            return ;
        }
        bul = new Widget_maze;
        init_bul();
        bul->setFixedSize(700,700);

        cout<<get_row<<endl;
        bul->row=get_row;
        bul->col=get_col;


        bul->maze_record.resize(bul->row);
        for(int i=0;i<bul->row;i++)
        {
            bul->maze_record[i].resize(bul->col);
        }
        for(int i=0;i<bul->row;i++)
            for(int j=0;j<bul->col;j++)
                bul->maze_record[i][j]=0;

        bul->resetMazeData();
        bul->show();
    }

}

void Widget_maze::paintEvent(QPaintEvent *event)
{

    if(this->maze_data.isEmpty())return ;
    paint = new QPainter;
    paint->begin(this);
    paint->setPen(QPen(QColor(105,105,105,80), 1, Qt::SolidLine));//设置画笔形式
    // 得到迷宫中每个方块的大小
    int max_size = (this->row > this->col) ? this->row : this->col;
    int each_box_size = 700 / max_size;
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            // 重复对每个方块进行绘制
            if (this->maze_data[i][j] == 0) {
                // 如果是路则将画笔设为白色
                paint->setBrush(QBrush(Qt::white, Qt::SolidPattern));//设置画刷形式
            }else if(this->maze_data[i][j] == -1) {
                // 如果是路径则将画笔设为青绿
                paint->setBrush(QBrush(QColor(82,139,139,80), Qt::SolidPattern));
            }else if (this->maze_data[i][j] == 1) {
                // 如果是墙则将画笔设为棕色
                paint->setBrush(QBrush(QColor(139,69,19,80), Qt::SolidPattern));
            } else if (this->maze_data[i][j] == 2) {
                // 如果是起点则将画笔设为蓝色
                paint->setBrush(QBrush(QColor(152,245,255,80), Qt::SolidPattern));
            } else if (this->maze_data[i][j] == 3) {
                // 如果是终点则将画笔设为粉红色
                paint->setBrush(QBrush(QColor(255,192,203,80), Qt::SolidPattern));
            }
            paint->drawRect(each_box_size * i,  each_box_size * j, each_box_size, each_box_size);
        }
    }
    paint->end();
}


void Widget_maze::mousePressEvent(QMouseEvent *event)
{
    if(this->maze_record.isEmpty())return ;
    int x = event->x();
    int y = event->y();

    // 得到迷宫中每个方块的大小
    int max_size = (this->row > this->col) ? this->row : this->col;
    int each_box_size = 700/ max_size;
    // 计算出鼠标点击的方块位置
    int click_row = x / each_box_size;
    int click_col = y / each_box_size;
    // 改变方块当前状态


    if (x <= 0 || y <= 0 || this->maze_data.isEmpty()) {
        // 如果x，y小于0则直接跳出
        return;
    }

    if(click_row>=this->row||click_col>=this->col)
    {
        return ;
    }

    cout<<click_row<<' '<<click_col<<endl;

//    return ;

    if(event->button()==Qt::LeftButton)
    {
        if (this->maze_record[click_row][click_col] == 0) {
            // 如果是路则变为墙
            this->maze_record[click_row][click_col] = 1;
        }
        else if(this->maze_record[click_row][click_col]==1)
        {
            this->maze_record[click_row][click_col] = 0;
        }
        this->resetMazeData();
        this->repaint();
    }
    else if(event->button()==Qt::RightButton)
    {

        if(this->star==nullptr&&this->end==nullptr)
        {
                this->maze_record[click_row][click_col] = 2;
                this->star=new point(click_row,click_col,2);
        }
        else if(this->star==nullptr&&this->end!=nullptr)
        {
                if(click_row!=this->end->x||click_col!=this->end->y)
                {
                    this->maze_record[click_row][click_col]=2;
                    this->star=new point(click_row,click_col,2);
                }
                else
                {
                    this->maze_record[click_row][click_col]=0;
                    this->end=nullptr;
                }
        }
        else if(this->star!=nullptr&&this->end==nullptr)
        {
                if(click_row!=this->star->x||click_col!=this->star->y)
                {
                    this->maze_record[click_row][click_col]=3;
                    this->end=new point(click_row,click_col,3);
                }
                else
                {
                    this->maze_record[click_row][click_col]=0;
                    this->star=nullptr;
                }
        }
        else if(this->star!=nullptr&&this->end!=nullptr)
        {
                if(click_row==this->star->x&&click_col==this->star->y)
                {
                    this->maze_record[click_row][click_col]=0;
                    this->star=nullptr;
                }
                if(click_row==this->end->x&&click_col==this->end->y)
                {
                    this->maze_record[click_row][click_col]=0;
                    this->end=nullptr;
                }
        }
        if(star==nullptr)
        {
            cout<<"empty "<<endl;
        }
        else
        {
            cout<<this->star->x<<' '<<this->star->y<<endl;
        }
        if(this->end==nullptr)
        {
            cout<<"empty "<<endl;
        }
        else
        {
            cout<<this->end->x<<' '<<this->end->y<<endl;
        }
        this->resetMazeData();
        this->repaint();
    }

}


void Widget::on_close_pushButton_clicked()
{
    if(bul!=nullptr)
    {
        bul->close();
        bul=nullptr;
    }
}


void Widget::on_DFS_pushButton_clicked()
{
    if(bul==nullptr)return ;
    if(bul->star==nullptr||bul->end==nullptr)
    {
        QMessageBox::information(nullptr, "警告", "没有设置起点或者终点");
        return ;
    }
    bul->find_DFS();
}


void Widget::on_auto_pushButton_clicked()
{
    int get_row=ui->x_textEdit->toPlainText().toInt();
    int get_col=ui->y_textEdit->toPlainText().toInt();
    if(get_row%2==0||get_col%2==0)
    {
        QMessageBox::information(nullptr, "警告", "自动生成迷宫请将行数与列数设置为奇数");
        return ;
    }
    if(bul!=nullptr)
    {
        bul->setFixedSize(700,700);
        this->init_bul();
        int get_row=ui->x_textEdit->toPlainText().toInt();
        int get_col=ui->y_textEdit->toPlainText().toInt();
        if(get_row==0||get_col==0)
        {
            QMessageBox::information(nullptr, "警告", "迷宫的横纵大小设置不对，请重新设置");
            return ;
        }
        bul->row=get_row;
        bul->col=get_col;
        if(!bul->maze_record.isEmpty())
        {
            bul->maze_record.clear();
            bul->maze_data.clear();
        }
        bul->maze_record=bul->autoSetMaze(get_row,get_col);
        bul->resetMazeData();
        bul->update();
        bul->show();
    }
    else
    {
        bul = new Widget_maze;
        this->init_bul();
        bul->setFixedSize(700,700);
        int get_row=ui->x_textEdit->toPlainText().toInt();
        int get_col=ui->y_textEdit->toPlainText().toInt();
        if(get_row==0||get_col==0)
        {
            QMessageBox::information(nullptr, "警告", "迷宫的横纵大小设置不对，请重新设置");
            return ;
        }
        bul->row=get_row;
        bul->col=get_col;
        if(!bul->maze_record.isEmpty())
        {
            bul->maze_record.clear();
            bul->maze_data.clear();
        }
        bul->maze_record=bul->autoSetMaze(get_row,get_col);
        bul->resetMazeData();
        bul->update();
        bul->show();
    }
}




void Widget_maze::A___star()
{
    n = this->col;
    m = this->row;
    init();
    cout<<"n = "<<n<<" m = "<<m<<endl;
    for(int i=0;i<this->col;i++)
    {
        for(int j=0;j<this->row;j++)
        {
            if(this->maze_record[j][i]==1)graph[i+1][j+1]='*';
            else graph[i+1][j+1]='.';
        }
    }
    start.x=this->star->y+1;
    start.y=this->star->x+1;

    endpo.x=this->end->y+1;
    endpo.y=this->end->x+1;

    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            cout<<graph[i][j];
        }
        cout<<endl;
    }

    cout<<start.x<<' '<<start.y<<endl;
    cout<<endpo.x<<' '<<endpo.y<<endl;
    if(!A_star())
    {
        QMessageBox::information(nullptr, "", "未成功找到路径");
        return ;
    }
    for(int i=1;i<(int)ans.size()-1;i++)
    {
        if(ans[i].fi==endpo.x&&ans[i].se==endpo.y)break;
        maze_data[ans[i].se-1][ans[i].fi-1]=-1;
        sleep(50);
        this->repaint();
        cout<<ans[i].fi<<' '<<ans[i].se<<endl;
    }
    QMessageBox::information(nullptr, "", "成功找到路径！");
}





void Widget::on_pushButton_clicked()
{
    if(bul==nullptr)return ;
    if(bul->star==nullptr||bul->end==nullptr)
    {
        QMessageBox::information(nullptr, "警告", "没有设置起点或者终点");
        return ;
    }

    bul->A___star();

}

