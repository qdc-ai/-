#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>
#include "gameconfig.h"
enum CellState{
    COVERED,  //未翻开
    UNCOVERED, //已翻开
    FLAGGED,  //标记为地雷
    QUESTIONED  //标记为问号
};

class cell : public QPushButton
{
    Q_OBJECT
public:
    explicit cell(int num,QPoint point,QWidget *parent = nullptr);
    void initCell();
    void UpdateImage();
    void updateVisualStyle();
    void flipCell();
    int getm_num();
    CellState getm_state();
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void handleLeftButtonEvent();
    void handleRightButtonEvent();
    void handleLeftAndRightButtonEvent();
    void markFlagRed();
signals:
     void sigLeftReleaseInside(int x,int y);
     void gameover();
     void sigLeftAndRightReleaseInside(int x,int y);
    // void gamewin();
     void subchessnum();
     void subminenum();
     void addminenum();
private:
    int m_num;
    QIcon m_image;
    CellState m_state;
    QPoint m_position;
    struct keyState{
        bool leftButtonPressed = false;
        bool rightButtonPressed = false;
    }keystate;
};

#endif // CELL_H
