#include "cell.h"
#include "gameConfig.h"
cell::cell(int num,QPoint point,QWidget *parent)
    :QPushButton(parent),m_num(num),m_position(point),m_state(COVERED){
    initCell();

}
void cell::markFlagRed()
{
     m_state=FLAGGED;
    UpdateImage();
}
 int cell::getm_num()
{
     return m_num;
}
 void cell::mousePressEvent(QMouseEvent *event)
{
     if(event->button() == Qt::LeftButton)
     {
         keystate.leftButtonPressed=true;
     }
     if(event->button() == Qt::RightButton)
     {
         keystate.rightButtonPressed=true;
     }
}
//处理鼠标左键事件
void cell::handleLeftButtonEvent()
{
     if(m_state==COVERED)
      emit sigLeftReleaseInside(m_position.x(),m_position.y());
}

//处理鼠标右键事件
void cell::handleRightButtonEvent()
{
    if(m_state!=UNCOVERED){
        switch(m_state)
        {
        case FLAGGED:
            m_state=QUESTIONED;
          emit addminenum();
            break;
        case QUESTIONED:
            m_state=COVERED;
            break;
        case COVERED:
            m_state=FLAGGED;
              emit subminenum();
            break;
        }
        UpdateImage();
    }
}
void cell::handleLeftAndRightButtonEvent()
{
    if(m_state==UNCOVERED)
        emit sigLeftAndRightReleaseInside(m_position.x(),m_position.y());
}
void cell::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton&&keystate.leftButtonPressed&&!keystate.rightButtonPressed)
    {
        qDebug() << "检测到：鼠标左键被按下！";
        qDebug() << m_position.x()<<m_position.y();
        handleLeftButtonEvent();
        keystate.leftButtonPressed=false;
    }
    if(event->button() == Qt::RightButton&&!keystate.leftButtonPressed&&keystate.rightButtonPressed)
    {
        qDebug() << "检测到：鼠标右键被按下！";
         handleRightButtonEvent();
         keystate.rightButtonPressed=false;
    }
    if(keystate.leftButtonPressed&&keystate.rightButtonPressed)
    {
        qDebug() << "检测到：鼠标左右键被同时按下！";
        handleLeftAndRightButtonEvent();
         keystate.leftButtonPressed=false;
        keystate.rightButtonPressed=false;
    }
}
//初始化棋子大小
void cell::initCell()
{
    setFixedSize(CELLPIXEL,CELLPIXEL);

    //更新图片
    UpdateImage();
}

CellState cell::getm_state()
{
    return m_state;
}
void cell::flipCell()
{
    //qDebug()<<m_num;
    m_state=UNCOVERED;
    UpdateImage();
    if(m_num==9)
    {
        emit gameover();
    }else
    emit subchessnum();

}

//根据当前棋子状态更新对应的图片
void cell::UpdateImage()
{
    switch(m_state)
    {
    case COVERED:
        m_image=QIcon();
        break;
    case UNCOVERED:
        m_image=QIcon(gameConfig::getImage(m_num));
        break;
    case FLAGGED:
        m_image=QIcon(":/scanlei/flag.png");//标记为红旗
        break;
    case QUESTIONED:
        m_image=QIcon(":/scanlei/question.png");//标记未问号
        break;
    }
    setIcon(m_image);//设置图片

    setIconSize(QSize(CELLPIXEL,CELLPIXEL));

    //更新样式
    updateVisualStyle();
}

void cell::updateVisualStyle()
{
    QString cellStyle;
    if(m_state==UNCOVERED)
    {
        if(m_num == 9) //翻开且是炸弹
        {
            cellStyle = "QPushButton {border:1px solid #888888}; background-color:#FFD0D0;";

        }else{
            cellStyle = "QPushButton {border:1px solid #888888; background-color:#D0D0D0;}";
        }
    }
    else//未翻开 红旗
    {
        cellStyle = "QPushButton {border: 1px outset #808080; background-color:#BFBFBF;}"
                    "QPushButton:hover {background-color:#B0B0B0;}";
                   // "QPushButton:pressed {border: 2px inset #707070; background-color:#A0A0A0;}";
    }
    setStyleSheet(cellStyle);
}