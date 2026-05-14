 #include "gameconfig.h"

gameConfig::Diffcaulty gameConfig::m_currentDiffcaulty=DEFAULTMODE;//当前游戏难度
int gameConfig::m_currentDiffcaultyindex=0;//当前游戏难度
int gameConfig::m_rowCount=6;//列数
int gameConfig::m_cloCount=6;//行数
int gameConfig::m_mineCount=5;//游戏地雷数
 int gameConfig::m_cellPixel=CELLPIXEL;//棋子像素
const QVector<QString> gameConfig::a_imagepath={
    ":/scanlei/none.png",":/scanlei/1.png",":/scanlei/2.png",":/scanlei/3.png",
     ":/scanlei/4.png",":/scanlei/5.png",":/scanlei/6.png",
    ":/scanlei/7.png",":/scanlei/8.png",":/scanlei/9.png"
};

int gameConfig::getm_currentDiffcaultyindex()
{
    return m_currentDiffcaultyindex;
}

 QString gameConfig::getImage(int n)
{
    return a_imagepath[n];
}

 int gameConfig::getrowCount()
{
     return m_rowCount;
}
int gameConfig::getcloCount(){
    return m_cloCount;
}
int gameConfig::getmineCount()
{
    return m_mineCount;
}
int gameConfig::getcellPixel()
{
    return m_cellPixel;
}

void gameConfig::setDiffcaulty(Diffcaulty diff)
{
    switch(diff)
    {
    case SOEASY:
        m_cloCount=6;
        m_rowCount=6;
        m_mineCount=5;
        m_currentDiffcaulty=SOEASY;
        m_currentDiffcaultyindex=0;
        break;
    case EASY:
        m_cloCount=9;
        m_rowCount=9;
        m_mineCount=10;
        m_currentDiffcaulty=EASY;
        m_currentDiffcaultyindex=1;
        break;
    case MEDIUM:
        m_cloCount=14;
        m_rowCount=14;
        m_mineCount=40;
        m_currentDiffcaulty=MEDIUM;
        m_currentDiffcaultyindex=2;
        break;
    case HARD:
        m_cloCount=24;
        m_rowCount=14;
        m_mineCount=99;
        m_currentDiffcaulty=HARD;
        m_currentDiffcaultyindex=3;
        break;
    }
}