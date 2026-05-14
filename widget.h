#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "gameborad.h"
#include "gameConfig.h"
#include <QComboBox>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;
    void initWidget();
    void setWindowSize();
    void onDifficultyChanged(int index);
    void setTimer();
    void cleanupTimer();
private:
    Ui::Widget *ui;
    gameborad* m_gb;
    QComboBox* qcbb;
    QLabel* q_minelabel;
    QLabel* q_mineedit;
    QLabel* img;
    QLabel* timeredit;
    QTimer* timer;
    int m_elapsedTime;
    QWidget *timerWidget;
    QLabel *timerIcon;
};
#endif // WIDGET_H
