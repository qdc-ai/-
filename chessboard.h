#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QObject>
#include "cell.h"
#include <QVector>

class Chessboard : public QObject
{
    Q_OBJECT
public:
    explicit Chessboard(QObject *parent = nullptr);

signals:

private:
    QVector<QVector<cell>> board;
};

#endif // CHESSBOARD_H
