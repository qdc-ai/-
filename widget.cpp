#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QDir>
#include <QComboBox>
#include <QLabel>
#include <QTimer>
#include <QFont>
#include <QMessageBox>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , qcbb(nullptr)
    , m_gb(nullptr)
    , q_minelabel(nullptr)
    , q_mineedit(nullptr)
    , img(nullptr)
    , timeredit(nullptr)
    , timer(nullptr)
    ,timerWidget(nullptr)
    ,timerIcon(nullptr)
    , m_elapsedTime(0)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    initWidget();
    setWindowTitle("扫雷游戏");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::cleanupTimer()
{
    // 停止并删除计时器对象
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
    // 如果 timeredit 存在，找到其父容器（timerWidget）并一起删除
    if (timeredit) {
        QWidget *parentWidget = timeredit->parentWidget();
        delete timeredit;
        timeredit = nullptr;
        if (parentWidget && parentWidget != this) {
            delete parentWidget;  // 删除包含计时器的容器控件
        }
    }
}

void Widget::onDifficultyChanged(int index)
{
    // 游戏进行中需要确认
    if (m_gb && m_gb->getgameInProgress()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认",
                                      "切换难度会重置当前游戏，确定吗？",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            qcbb->blockSignals(true);
            qcbb->setCurrentIndex(gameConfig::getm_currentDiffcaultyindex());
            qcbb->blockSignals(false);
            return;
        }
    }

    // 重置计时器显示（新游戏开始）
    m_elapsedTime = 0;

    // 根据索引设置难度
    switch (index) {
    case 0: gameConfig::setDiffcaulty(gameConfig::SOEASY); break;
    case 1: gameConfig::setDiffcaulty(gameConfig::EASY);   break;
    case 2: gameConfig::setDiffcaulty(gameConfig::MEDIUM); break;
    case 3: gameConfig::setDiffcaulty(gameConfig::HARD);   break;
    }

    // 断开旧连接
    if (m_gb) {
        disconnect(m_gb, nullptr, this, nullptr);
    }

    // 删除旧的游戏面板及相关控件
    delete m_gb;          m_gb = nullptr;
    delete q_minelabel;   q_minelabel = nullptr;
    delete q_mineedit;    q_mineedit = nullptr;
    delete img;           img = nullptr;
    delete timerIcon; timerIcon = nullptr;
    delete timerWidget; timerWidget = nullptr;
    timer->stop();
    delete timer;           timer = nullptr;


    // 清理计时器对象（避免重复创建）
   // cleanupTimer();

    // 重新初始化界面
    initWidget();

    // 更新下拉框位置
    if (qcbb) {
        qcbb->move((gameConfig::getcloCount() - 2) * gameConfig::getcellPixel(), 40);
    }
}

void Widget::initWidget()
{
    // 确保下拉框只创建一次
    if (!qcbb) {
        qcbb = new QComboBox(this);
        qcbb->addItem("超简单", QVariant(1));
        qcbb->addItem("简单",   QVariant(10));
        qcbb->addItem("中等",   QVariant(40));
        qcbb->addItem("困难",   QVariant(99));
        qcbb->move((gameConfig::getcloCount() - 2) * gameConfig::getcellPixel(), 40);
        connect(qcbb, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &Widget::onDifficultyChanged);
        qcbb->show();
    }

    // 创建游戏面板
    m_gb = new gameborad(this);
    m_gb->move(QPoint(0, 100));

    // 剩余地雷标签
    q_minelabel = new QLabel(this);
    q_minelabel->setText("剩余地雷：");
    q_minelabel->move(gameConfig::getcellPixel() / 2, 40);
    q_minelabel->show();

    // 地雷数量显示
    q_mineedit = new QLabel(this);
    q_mineedit->setText(QString::number(m_gb->getisMineNum()));
    q_mineedit->move(gameConfig::getcellPixel() * 2 - 20, 40);
    q_mineedit->show();

    // 表情图标
    img = new QLabel(this);
    QPixmap pixmap(":/scanlei/smile.png");
    img->setPixmap(pixmap);
    img->move((gameConfig::getcloCount() / 2) * gameConfig::getcellPixel() - 20, 40);
    img->setFixedSize(25, 25);
    img->setScaledContents(true);
    img->show();

    // ========== 创建计时器 UI ==========
    timerWidget = new QWidget(this);
    timerWidget->move((gameConfig::getcloCount() / 2) * gameConfig::getcellPixel() - 60, 10);
    timerWidget->setFixedSize(120, 30);
    timerWidget->setStyleSheet(
        "QWidget {"
        "background-color: #2C2C2C;"
        "border-radius: 5px;"
        "border: 1px solid #555555;"
        "}"
        );

    timerIcon = new QLabel(timerWidget);
    timerIcon->setFixedSize(20, 20);
    timerIcon->move(5, 7);
    timerIcon->setStyleSheet(
        "QLabel {"
        "background-color: transparent;"
        "color: #FFD700;"
        "font-size: 14px;"
        "}"
        );
    timerIcon->setAlignment(Qt::AlignCenter);
    timerIcon->setText("⏱️");

    timeredit = new QLabel(timerWidget);
    timeredit->move(30, 5);
    timeredit->setFixedSize(80, 25);
    timeredit->setAlignment(Qt::AlignCenter);
    QFont timerFont("Consolas", 14, QFont::Bold);
    timeredit->setFont(timerFont);
    timeredit->setStyleSheet(
        "QLabel {"
        "background-color: #1A1A1A;"
        "color: #00FF00;"
        "border-radius: 3px;"
        "border: 1px solid #00AA00;"
        "padding: 2px;"
        "}"
        );
    timeredit->setText("000");
    timeredit->show();
    timerWidget->show();

    // 创建计时器对象（每秒触发）
    timer = new QTimer(this);
    timer->setSingleShot(false);
    connect(timer, &QTimer::timeout, [this]() {
        m_elapsedTime++;
        timeredit->setText(QString("%1").arg(m_elapsedTime, 3, 10, QChar('0')));

        // 根据时间改变颜色
        if (m_elapsedTime < 10) {
            timeredit->setStyleSheet(
                "QLabel {"
                "background-color: #1A1A1A;"
                "color: #00FF00;"
                "border-radius: 3px;"
                "border: 1px solid #00AA00;"
                "padding: 2px;"
                "}"
                );
        } else if (m_elapsedTime < 60) {
            timeredit->setStyleSheet(
                "QLabel {"
                "background-color: #1A1A1A;"
                "color: #FFFF00;"
                "border-radius: 3px;"
                "border: 1px solid #AAAA00;"
                "padding: 2px;"
                "}"
                );
        } else {
            timeredit->setStyleSheet(
                "QLabel {"
                "background-color: #1A1A1A;"
                "color: #FF6600;"
                "border-radius: 3px;"
                "border: 1px solid #AA5500;"
                "padding: 2px;"
                "}"
                );
        }
    });
    timer->start(1000);  // 新游戏开始，启动计时器

    setWindowSize();

    // 连接游戏信号
    connect(m_gb, &gameborad::updateIsMineNum, [this]() {
        if (q_mineedit && m_gb) {
            q_mineedit->setText(QString::number(m_gb->getisMineNum()));
        }
    });

    connect(m_gb, &gameborad::reset, [this]() {
        // 重置计时器
        m_elapsedTime = 0;
        if (timeredit) {
            timeredit->setText("000");
            timeredit->setStyleSheet(
                "QLabel {"
                "background-color: #1A1A1A;"
                "color: #00FF00;"
                "border-radius: 3px;"
                "border: 1px solid #00AA00;"
                "padding: 2px;"
                "}"
                );
        }
        if (timer && !timer->isActive()) {
            timer->start(1000);  // 重新启动计时器
        }
        QPixmap pixmap(":/scanlei/smile.png");
        img->setPixmap(pixmap);
    });

    connect(m_gb, &gameborad::gamewin, [this]() {
        if (timer) timer->stop();
        QPixmap pixmap(":/scanlei/cool.png");
        img->setPixmap(pixmap);
    });

    connect(m_gb, &gameborad::gamedefeat, [this]() {
        if (timer) timer->stop();
        QPixmap pixmap(":/scanlei/sob.png");
        img->setPixmap(pixmap);
    });

    m_gb->show();
}

void Widget::setWindowSize()
{
    setFixedSize(QSize(gameConfig::getcloCount() * gameConfig::getcellPixel(),
                       gameConfig::getrowCount() * gameConfig::getcellPixel() + 100));
}