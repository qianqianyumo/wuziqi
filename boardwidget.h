#ifndef BOARDWIDGET_H
#define  BOARDWDGET_H

#include <QWidget>
#include <QStack>
#include <QPoint>
#include <QSet>
typedef int (*Board)[15];

class BoardWidget : public QWidget
{
    Q_OBJECT

public:
    static const QSize WIDGET_SIZE;//棋盘大小
    static const QSize CELL_SIZE;//棋盘单元格大小
    static const QPoint START_POS;//棋盘单元格开始位置
    static const QPoint ROW_NUM_START;//行标号开始位置
    static const QPoint CLU_NUM_START;//列标号开始位置
    static const int BOARD_WIDTH=15;//棋盘列数
    static const int BOARD_HEIGHT=15;//棋盘行数
    static const int NO_PIECE=0;//棋子标志，表示无子
    static const int WHITE_PIECE=1;//棋子标志，表示白子
    static const int BLACK_PIECE=2;//棋子标志，表示黑子
    static const bool WHITE_PLAYER=true;//棋手标志，表示白方
    static const bool BLACK_PLAYER=false;//棋手标志，表示黑方
    void setTrackPos(const QPoint &value);//设置当前鼠标在棋盘中的位置

public:
   explicit BoardWidget(QWidget *parent=nullptr);
    //设置棋盘控件接受的下棋方
    void setReceivePlayers(const QSet<int> &value);
    //获取棋盘信息
    Board getBoard();
protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    void initBoard();//初始化棋盘控件
    void switchNextPlayer();//切换下一位下棋者
    void checkWinner();//检查是否有获胜者或者平局
    //判断从(x,y)开始，是否有5个同色棋子在一条线上
    bool isFivePieceForm(int x,int y);
    //进行四个方向的判断
    bool isVFivePieceForm(int x,int y);
    bool isHFivePieceForm(int x,int y);
    bool isFSFivePieceForm(int x,int y);
    bool isBSFivePieceForm(int x,int y);

signals:
    void gameOver(int winner);
    void turnNextPlayer(int player);
public slots:
    void newGame();//清除棋盘信息，开始新游戏
    void undo(int steps);//悔棋
    void downPiece(int x,int y);//落子
private:
    bool endGame;//游戏是否结束
    int nextPlayer;//表示下一位棋手
    QPoint trackpos;//记录鼠标在棋盘中的位置
    QVector<QPoint> winPoses;//获胜的五子位置
    QSet<int>receivePlayers;//棋盘接受点击下棋的棋手
    QStack<QPoint>dropedPieces;//每一步落子的位置
    int board[BOARD_WIDTH][BOARD_HEIGHT];//棋盘信息
};
#endif // BOARDWIDGET_H
