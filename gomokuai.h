#ifndef GOMOKUAI_H
#define GOMOKUAI_H
#include <QObject>
#include "boardwidget.h"
class GomokuAi:public QObject
{
 Q_OBJECT
public:
    explicit GomokuAi(QObject *parent=nullptr);
public slots:
    QPoint searchGoodPos(Board nboard);
public:
    static const int AI_PLAYER=BoardWidget::BLACK_PLAYER;
private:
    QPoint bestPos;//最佳位置
    int destinboard[BoardWidget::BOARD_WIDTH][BoardWidget::BOARD_HEIGHT];
    int evaluate(Board nboard);
private:
    int livefive;//连五
    int livefour;//活四
    int sleepfour;//眠四
    int livethree;//活三
    int livetwo;//活二
    int sleepthree;//眠三
    int liveone;//活一
    int sleeptwo;//眠二
    int sleepone;//眠一
    int unknown;

};

#endif // GOMOKUAI_H
