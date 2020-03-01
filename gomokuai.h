#ifndef GOMOKUAI_H
#define GOMOKUAI_H
#include <QObject>
#include "boardwidget.h"
#include <QMetaEnum>
#include <vector>
class GomokuAi:public QObject
{
 Q_OBJECT
public:
    explicit GomokuAi(QObject *parent=nullptr);
public slots:
    QPoint searchbestPos(Board board);
   //QPoint searchbestPos(int **board);
public:
    static const int AI_PLAYER=BoardWidget::BLACK_PLAYER;
private:
    QPoint bestPos;//最佳位置
    //int destinboard[BoardWidget::BOARD_WIDTH][BoardWidget::BOARD_HEIGHT];
    int evaluate(Board &board,int mypiece);//评估函数
    int analysisLine(int *line,int mypiece,int oppopiece);//判断一条线上自己棋能形成的棋形
    QPoint search(Board &board,int depth,int alpha,int beta);//寻找位置
    int  getScore();//对黑棋和白棋进行评分
    void evaluateFourDirect(int posx,int posy,Board &board);//对一个棋子的四个方向进行检查
    //int alphaBetaSearch(Board nboard, int player, int depth, int alpha, int beta);//剪枝
    std::vector<QPoint> genMove(Board &board,int mypiece);//得到棋盘上所有空的位置
    bool hasNeighbor(Board &board,int x,int y,int radius);//判断双方已下棋子范围内是否有空位置
    void copycount(double *array,bool isPiece);
    void reset(double *array);
    int minSearch(Board &board,int depth,int alpha,int beta);
    int maxSearch(Board &board,int depth,int alpha,int beta);
    int evaluatePointScore(Board board,int x,int y,int mypiece);//对某一个空位进行打分
    int getPointScore(double *count);//统计棋型的分数

private:
    //QVector<QPoint>emptyPos;
    int livefive=100000;//连五
    int livefour=10000;//活四
    int twothrees=8000;//双活三
    int sleepfour=1000;//眠四
    int livethree=1000;//活三
    int sleepthree=100;//眠三
    int livetwo=100;//活二
    int sleeptwo=10;//眠二
    //int unknown=0;

    int NONE=0,
    SLEEP_TWO=1,
    LIVE_TWO=2,
    SLEEP_THREE=3,
    LIVE_THREE=4,
    CHONG_FOUR=5,
    LIVE_FOUR=6,
    LIVE_FIVE=7;
    double mcount[8]={0};
    double ocount[8]={0};
    double count[8]={0};

    int SCORE_MAX=0x7fffffff;
    int SCORE_MIN=-1*SCORE_MAX;
    int SCORE_FIVE=10000;


};

#endif // GOMOKUAI_H
