#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
//#include "boardwidget.h"
#include "gomokuai.h"

class GameWidget:public QWidget
{
    Q_OBJECT //
public:
    GameWidget(QWidget *parent=0);
    ~GameWidget();
private:
    void initWidget();//初始化游戏窗口界面
    void showWinner(int winner);//显示获胜者
    void switchPlayerLabel(bool player);//切换playerLabel标签显示内容
    void switchHumanGame();//切换至双人对战模式
    void switchAiGame();//切换至Ai对战模式
    void nextDropPiece(bool player);//接受下棋信号，如果是Ai模式，搜索最佳位置下棋
    void newGame();//新游戏
    void undo();//悔棋
private:
    GomokuAi *ai;//ai为对象指针
    bool isGameWithAi;//是否为ai对战模式
    BoardWidget *boardWidget;//棋盘控件
    QLabel *playerLabel;//用于提示下一步轮哪一方
    QLabel *winLabel;//提示获胜的标签
    QPushButton *newGameBtn;//新游戏按钮
    QPushButton *humanGameBtn;//双人游戏按钮
    QPushButton *aiGameBtn;//ai对战按钮
    QPushButton *undoBtn;//悔棋按钮
};
#endif // GAMEWIDGET_H
