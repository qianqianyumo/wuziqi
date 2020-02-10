#include "boardwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFile>
#include <QDataStream>

//类的静态数据成员定义
const QSize BoardWidget::WIDGET_SIZE(430,430);
const QSize BoardWidget::CELL_SIZE(25,25);
const QPoint BoardWidget::START_POS(40,40);
const QPoint BoardWidget::ROW_NUM_START(15,45);
const QPoint BoardWidget::CLU_NUM_START(39,25);
const int BoardWidget::BOARD_WIDTH;
const int BoardWidget::BOARD_HEIGHT;
const int BoardWidget::NO_PIECE;
const int BoardWidget::WHITE_PIECE;
const int BoardWidget::BLACK_PIECE;
const bool BoardWidget::WHITE_PLAYER;
const bool BoardWidget::BLACK_PLAYER;


BoardWidget::BoardWidget(QWidget *parent)
    : QWidget(parent),trackpos(28,28)
{
    setFixedSize(WIDGET_SIZE);
    setMouseTracking(true);
    initBoard();
}
void BoardWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);//创建一个QPainter对象,this表示是widget部件
    painter.fillRect(0,0,width(),height(),Qt::lightGray);//背景颜色
    for(int i=0;i<BOARD_WIDTH;i++)
    {
        painter.drawText(CLU_NUM_START+QPoint(i*CELL_SIZE.width(),0),QString::number(i+1));
    }
    for(int i=0;i<BOARD_HEIGHT;i++)
    {
        painter.drawText(ROW_NUM_START+QPoint(0,i*CELL_SIZE.height()),QString::number((i+1)));
    }
    //绘制棋盘格子
    for(int i=0;i<BOARD_WIDTH-1;i++)
    {
      for(int j=0;j<BOARD_HEIGHT-1;j++)
      {
          painter.drawRect(QRect(START_POS+QPoint(i*CELL_SIZE.width(),j*CELL_SIZE.height()),CELL_SIZE));
      }
    }
    painter.setPen(Qt::red);//添加画笔,设置画笔颜色
    QPoint poses[12]={
        trackpos+QPoint(0,8),
        trackpos,
        trackpos+QPoint(8,0),
        trackpos+QPoint(17,0),
        trackpos+QPoint(25,0),
        trackpos+QPoint(25,8),
        trackpos+QPoint(25,17),
        trackpos+QPoint(25,25),
        trackpos+QPoint(17,25),
        trackpos+QPoint(8,25),
        trackpos+QPoint(0,25),
        trackpos+QPoint(0,17)
    };
    painter.drawPolyline(poses,3);
    painter.drawPolyline(poses+3,3);
    painter.drawPolyline(poses+6,3);
    painter.drawPolyline(poses+9,3);

    painter.setPen(Qt::NoPen);//防止棋子的外轮廓边缘为红色
    //绘制天元四星标记
    QColor color=Qt::black;
    painter.setBrush(QBrush(color));
    painter.drawRoundRect(START_POS.x()+7*CELL_SIZE.width()-6,START_POS.y()+7*CELL_SIZE.height()-6,12,12,99,99);
    painter.drawRoundRect(START_POS.x()+3*CELL_SIZE.width()-4,START_POS.y()+3*CELL_SIZE.height()-4,8,8,99,99);
    painter.drawRoundRect(START_POS.x()+3*CELL_SIZE.width()-4,START_POS.y()+11*CELL_SIZE.height()-4,8,8,99,99);
    painter.drawRoundRect(START_POS.x()+11*CELL_SIZE.width()-4,START_POS.y()+3*CELL_SIZE.height()-4,8,8,99,99);
    painter.drawRoundRect(START_POS.x()+11*CELL_SIZE.width()-4,START_POS.y()+11*CELL_SIZE.height()-4,8,8,99,99);
    //绘制棋子
    for(int i=0;i<BOARD_WIDTH;i++)
    {
        for(int j=0;j<BOARD_HEIGHT;j++)
        {
            if(board[i][j]!=NO_PIECE)
            {
               QColor color=(board[i][j]==WHITE_PIECE)?Qt::white:Qt::black;
               painter.setBrush(QBrush(color));
               painter.drawEllipse(START_POS.x()-CELL_SIZE.width()/2+i*CELL_SIZE.width(),START_POS.y()-CELL_SIZE.height()/2+j*CELL_SIZE.height(),CELL_SIZE.width(),CELL_SIZE.height());
            }
        }
    }
    painter.setPen(Qt::red);
    if(!dropedPieces.isEmpty())
    {
        QPoint lastPos=dropedPieces.top();
        QPoint drawPos=QPoint(lastPos.x()*CELL_SIZE.width(),lastPos.y()*CELL_SIZE.height())+START_POS;
        painter.drawLine(drawPos+QPoint(0,5),drawPos+QPoint(0,-5));
        painter.drawLine(drawPos+QPoint(5,0),drawPos+QPoint(-5,0));
    }
    for(QPoint pos:winPoses)
    {
      QPoint drawPos=START_POS+QPoint(pos.x()*CELL_SIZE.width(),pos.y()*CELL_SIZE.height());
      painter.drawLine(drawPos+QPoint(0,5),drawPos+QPoint(0,-5));
      painter.drawLine(drawPos+QPoint(5,0),drawPos+QPoint(-5,0));
    }
}

void BoardWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(receivePlayers.contains(nextPlayer)&&!endGame)
    {
        QPoint pos=event->pos()-START_POS;
        int x=pos.x();
        int y=pos.y();
        int pieceX=x/CELL_SIZE.width();
        int pieceY=y/CELL_SIZE.height();
        int offsetX=x%CELL_SIZE.width();
        int offsetY=y%CELL_SIZE.height();
        if(offsetX>CELL_SIZE.width()/2)
        {
            pieceX++;
        }
        if(offsetY>CELL_SIZE.height()/2)
        {
            pieceY++;
        }
        downPiece(pieceX,pieceY);//确定鼠标落下的位置
    }
}

void BoardWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos=event->pos()-START_POS+QPoint(CELL_SIZE.width()/2,CELL_SIZE.height()/2);
    int x=pos.x();
    int y=pos.y();
    if(x<0||x>=CELL_SIZE.width()*BOARD_WIDTH||y<0||y>=CELL_SIZE.height()*BOARD_HEIGHT)
    {
        return;
    }
    int offsetX=x%CELL_SIZE.width();
    int offsetY=y%CELL_SIZE.height();
    setTrackPos(QPoint(x-offsetX,y-offsetY)+START_POS-QPoint(CELL_SIZE.width()/2,CELL_SIZE.height()/2));
}

//初始化棋盘控件
void BoardWidget::initBoard()
{
    receivePlayers<<WHITE_PLAYER<<BLACK_PLAYER;
    newGame();
}

void BoardWidget::undo(int steps)
{
 if(!endGame)
 {
    for(int i=0;i<steps;i++)
    {
        QPoint pos=dropedPieces.pop();
        board[pos.x()][pos.y()]=NO_PIECE;
    }
     update();
    if(steps==2)
    {
    nextPlayer=BLACK_PLAYER;
    }
    switchNextPlayer();
 }
}
//落子并判断游戏是否结束
void BoardWidget::downPiece(int x,int y)
{
    if(x>=0&&x<BOARD_WIDTH&&y>=0&&y<BOARD_HEIGHT&&board[x][y]==NO_PIECE)
    {
        dropedPieces.push(QPoint(x, y));
        board[x][y]=(nextPlayer==WHITE_PLAYER)?WHITE_PIECE:BLACK_PIECE;
        update();
        checkWinner();
        if(!endGame)
        {
            switchNextPlayer();
        }
    }
}

//设置鼠标在棋盘中的位置
void BoardWidget::setTrackPos(const QPoint &value)
{
    trackpos=value;
    update();
}

//获取棋盘信息
Board BoardWidget::getBoard()
{
    return board;
}
void BoardWidget::switchNextPlayer()
{
    nextPlayer=!nextPlayer;
    emit turnNextPlayer(nextPlayer);
}
void BoardWidget::checkWinner()
{
    bool fullPieces=true;
    for(int i=0;i<BOARD_WIDTH;i++)
    {
        for(int j=0;j<BOARD_HEIGHT;j++)
        {
            if(board[i][j]==NO_PIECE)
            {
                fullPieces=false;
            }
            if(board[i][j]!=NO_PIECE&&isFivePieceForm(i,j))
            {
                bool winner=(board[i][j]==WHITE_PIECE)?WHITE_PLAYER:BLACK_PLAYER;
                endGame=true;
                emit gameOver(winner);
            }
        }
    }
    if(fullPieces)
    {
        endGame=true;
        emit gameOver(2);//代表和棋
    }
}

bool BoardWidget::isFivePieceForm(int x, int y)
{
    return isVFivePieceForm(x,y)||isHFivePieceForm(x,y)||isFSFivePieceForm(x,y)||isBSFivePieceForm(x,y);
}

bool BoardWidget::isVFivePieceForm(int x, int y)
{
    int piece=board[x][y];
    for(int i=1;i<5;i++)
    {
        if(y+i>=BOARD_HEIGHT||board[x][y+i]!=piece)
        {
            return false;
        }
    }
    winPoses.clear();
    for(int i=0;i<5;i++)
    {
        winPoses.append(QPoint(x,y+i));
    }
    return true;
}
bool BoardWidget::isHFivePieceForm(int x, int y)
{
    int piece=board[x][y];
    for(int i=1;i<5;i++)
    {
        if(x+i>=BOARD_WIDTH||board[x+i][y]!=piece)
        {
            return false;
        }
    }
    winPoses.clear();
    for(int i=0;i<5;i++)
    {
        winPoses.append(QPoint(x+i,y));
    }
    return true;
}

bool BoardWidget::isFSFivePieceForm(int x, int y)
{
    int piece = board[x][y];
    for (int i = 1; i < 5; i++)
    {
        if (x + i >= BOARD_WIDTH || y - i < 0 || board[x + i][y - i] != piece)
        {
            return false;
        }
    }
    winPoses.clear();
    for (int i = 0; i < 5; i++)
    {
        winPoses.append(QPoint(x + i, y - i));
    }
    return true;
}

bool BoardWidget::isBSFivePieceForm(int x, int y)
{
    int piece = board[x][y];
    for (int i = 1; i < 5; i++)
    {
        if (x + i >= BOARD_WIDTH || y + i >= BOARD_HEIGHT || board[x + i][y + i] != piece)
        {
            return false;
        }
    }
    winPoses.clear();
    for (int i = 0; i < 5; i++)
    {
        winPoses.append(QPoint(x + i, y + i));
    }
    return true;
}


void BoardWidget::newGame()
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            board[i][j] = NO_PIECE;
        }
    }
    winPoses.clear();
    dropedPieces.clear();
    nextPlayer = BLACK_PLAYER;
    endGame = false;//用来判断是否可以落子
    update();
    emit turnNextPlayer(nextPlayer);
}

void BoardWidget::setReceivePlayers(const QSet<int> &value)
{
    receivePlayers=value;
}
