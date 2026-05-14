#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <QWidget>

#define CELLPIXEL 40
#define DEFAULTMODE SOEASY
class gameConfig : public QWidget
{
    Q_OBJECT

public:
    enum Diffcaulty
    {
        SOEASY,
        EASY, //简单(9*9) (10地雷)
        MEDIUM, //中等(14*14) (40地雷)
        HARD, //简单(14*25) (99地雷)
    };
    static QString getImage(int n);//获取对应的图片路径
    static void setDiffcaulty(Diffcaulty diff);//设置游戏难度
    static int getrowCount();
    static int getcloCount();
    static int getmineCount();
    static int getcellPixel();
    static int getm_currentDiffcaultyindex();

signals:

private:
    static const QVector<QString> a_imagepath;
    static Diffcaulty m_currentDiffcaulty;//当前游戏难度
    static int m_rowCount;//列数
    static int m_cloCount;//行数
    static int m_mineCount;//游戏地雷数
    static int m_cellPixel;//棋子像素
    static int m_currentDiffcaultyindex;//难度索引

};
static const  QVector<QString> a_imagepath();
#endif // GAMECONFIG_H
