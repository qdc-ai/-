#include "gameborad.h"
#include "ui_gameborad.h"
#include <random>
#include "cell.h"
#include <QTimer>
gameborad::gameborad(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gameborad)
{
    ui->setupUi(this);

    initgameborad();
}

gameborad::~gameborad()
{
    delete ui;
}
bool gameborad::isOut(int x,int y)
{
    if(x<0||x>=gameConfig::getrowCount()||y<0||y>=gameConfig::getcloCount())
        return false;
    return true;
}

void gameborad::findSafe(int x,int y)
{
    m_boradarray[x][y]->flipCell();
    if(m_boradarray[x][y]->getm_num()!=0)
    {
        return;
    }
    for(int i=0;i<8;i++)
    {
        int newx=x+direaction[i][0];
        int newy=y+direaction[i][1];
        if(isOut(newx,newy)&&m_boradarray[newx][newy]->getm_num()!=9&&m_boradarray[newx][newy]->getm_state()==COVERED)
        {
            findSafe(newx,newy);
        }
    }
}

//左右键一起点：和弦
void gameborad::chordBorad(int x,int y)
{
    int count=0;
    for(int i=0;i<8;i++)
    {
        int newx=x+direaction[i][0];
        int newy=y+direaction[i][1];
        if(isOut(newx,newy)&&m_boradarray[newx][newy]->getm_state()==FLAGGED)
        {
            count++;
        }
    }
    if(count==m_boradarray[x][y]->getm_num())
    {
        openTheFlag(x,y);
    }
}
void gameborad::openTheFlag(int x,int y)
{
    for(int i=0;i<8;i++)
    {
        int newx=x+direaction[i][0];
        int newy=y+direaction[i][1];
        if(isOut(newx,newy)&&m_boradarray[newx][newy]->getm_state()==FLAGGED)
        {
            m_boradarray[newx][newy]->flipCell();
        }
    }
}
 bool gameborad::getgameInProgress()
{
     return gameInProgress;
}
void gameborad::boom()
{
   // qDebug()<<"lose1";
if(!gameInProgress) return;  // 防止重复触发
    gameInProgress=false;
    // qDebug()<<"lose2";
    emit gamedefeat();
    //initgameborad();
    QMessageBox::warning(this,"提示消息","翻到地雷,游戏结束！");
   //  qDebug()<<"lose3";
   //  gameInProgress=true;
  //  延迟重置，确保所有鼠标事件处理完毕
    QTimer::singleShot(100, this, [this]() {
        initgameborad();
        gameInProgress = true;
    });
}
void gameborad::openAllMine()
{
    int row=gameConfig::getrowCount();
    int clo=gameConfig::getcloCount();
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<clo;j++)
        {
            if(m_minearray[i][j])
            {
                m_boradarray[i][j]->markFlagRed();
            }
        }
    }
}
void  gameborad::jugeIsWin()
{
    if(notIsMine==0)
    {
//qDebug()<<"win1";
        QMessageBox::information(this,"提示消息","翻开了所有空白棋子,游戏胜利！");
        openAllMine();
       // qDebug()<<"win2";
        gameInProgress=false;
        emit gamewin();

       // initgameborad();
    }
}
void gameborad::subnotIsMine()
{
    notIsMine--;
    jugeIsWin();
}
void gameborad::initgameborad()
{

    emit reset();
    // 先删除旧的 cell
    for(int i = 0; i < m_boradarray.size(); i++) {
        for(int j = 0; j < m_boradarray[i].size(); j++) {
            if(m_boradarray[i][j]) {
                delete m_boradarray[i][j];  // 必须 delete
                m_boradarray[i][j] = nullptr;
            }
        }
    }
    m_boradarray.clear();
    gameInProgress=true;
    //设置棋盘大小
    setFixedSize(QSize(gameConfig::getcloCount()*gameConfig::getcellPixel(),gameConfig::getrowCount()*gameConfig::getcellPixel()));
    //初始化地雷数组
    initmineArray();
    int row=gameConfig::getrowCount();
    int clo=gameConfig::getcloCount();
    qDebug()<<row;
    qDebug()<<clo;
    notIsMine=row*clo-gameConfig::getmineCount();
    isMine=gameConfig::getmineCount();
    m_boradarray.resize(row,QVector<cell*> (clo,nullptr));
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<clo;j++)
        {
            if(m_minearray[i][j])
            {
                m_boradarray[i][j]=new cell(9,QPoint(i,j),this);
            }else{
                m_boradarray[i][j]=new cell(gameborad::getminenum(i,j),QPoint(i,j),this);
            }
        connect(m_boradarray[i][j],&cell::sigLeftReleaseInside,this,&gameborad::findSafe);
        connect(m_boradarray[i][j],&cell::sigLeftAndRightReleaseInside,this,&gameborad::chordBorad);
         connect(m_boradarray[i][j],&cell::gameover,this,&gameborad::boom);
         //connect(m_boradarray[i][j],&cell::gamewin,this,&gameborad::jugeIsWin);
         connect(m_boradarray[i][j],&cell::subchessnum,this,&gameborad::subnotIsMine);
         connect(m_boradarray[i][j],&cell::subminenum,this,&gameborad::subMineNum);
         connect(m_boradarray[i][j],&cell::addminenum,this,&gameborad::addMineNum);
            m_boradarray[i][j]->move(j*CELLPIXEL, i*CELLPIXEL);
            m_boradarray[i][j]->show();
        }

    }
}
void gameborad::subMineNum()
{
    isMine--;
    emit updateIsMineNum();
}
void gameborad::addMineNum()
{
    isMine++;
    emit updateIsMineNum();
}
int gameborad::getisMineNum()
{
    return isMine;
}
int  gameborad::getminenum(int x,int y)
{
    int count=0;
    for(int i=0;i<8;i++)
    {
        int newx=x+direaction[i][0];
        int newy=y+direaction[i][1];
        if(isOut(newx,newy)&&m_minearray[newx][newy])
        {
            count++;
        }
    }
    return count;
}
void gameborad::initmineArray()
{
    int row=gameConfig::getrowCount();
    int clo=gameConfig::getcloCount();
    m_minearray.resize(row,QVector<bool> (clo,false));

    QVector<bool> flag(row*clo,false);
    int minenum=gameConfig::getmineCount();
    for(int i=0;i<minenum;i++)
    {
        flag[i]=true;
    }

    std::random_device rd;

    std::mt19937 g(rd());
    std::shuffle(flag.begin(),flag.end(),g);

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<clo;j++)
        {
            m_minearray[i][j]=flag[i*clo+j];
        }
    }
}
