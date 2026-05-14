#ifndef GAMEBORAD_H
#define GAMEBORAD_H

#include <QWidget>
#include <QVector>
#include "cell.h"
#include <QMessageBox>
namespace Ui {
class gameborad;
}

class gameborad : public QWidget
{
    Q_OBJECT
public:
    explicit gameborad(QWidget *parent = nullptr);
    void initgameborad();
    int getminenum(int x,int y);
    void initmineArray();
    bool isOut(int x,int y);
    void findSafe(int x,int y);
    void chordBorad(int x,int y);
    void openTheFlag(int x,int y);
    void boom();
    void subnotIsMine();
    void openAllMine();
    void jugeIsWin();
    bool getgameInProgress();
    int getisMineNum();
    void subMineNum();
    void addMineNum();
    ~gameborad();
signals:
    //void updatenotIsMineNum();
    void updateIsMineNum();
    void gamewin();
    void gamedefeat();
    void reset();
public slots:

private:
    Ui::gameborad *ui;
    int notIsMine;
    int isMine;
    QVector<QVector<cell*>> m_boradarray;
    QVector<QVector<bool>> m_minearray;
    int direaction[8][2]={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}} ;
    bool gameInProgress;
};

#endif // GAMEBORAD_H
