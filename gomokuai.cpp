#include "gomokuai.h"
#include <QDebug>
#include <cmath>
#include <iostream>
#include <vector>
const int GomokuAi::AI_PLAYER;
GomokuAi::GomokuAi(QObject *parent) : QObject(parent)
{
}
QPoint GomokuAi::searchbestPos(Board board)
{
 if(board[7][7]==BoardWidget::NO_PIECE)
 {
     return QPoint(7,7);
 }
 else
 {
 int alpha=SCORE_MIN;
 int beta=SCORE_MAX;
 int depth=6;
 return search(board,depth,alpha,beta);
 }
}


//判断空位置附近是否有棋子
bool GomokuAi::hasNeighbor(Board &board,int x,int y,int radius)
{
 int startx=x-radius;
 int endx=x+radius;
 int starty=y-radius;
 int endy=y+radius;
 for(int i=startx;i<=endx;i++)
 {
     for(int j=starty;j<=endy;j++)
     {
       if(i>=0&&i<BoardWidget::BOARD_WIDTH&&j>=0&&j<BoardWidget::BOARD_HEIGHT)
       {
           if(board[i][j]!=BoardWidget::NO_PIECE)
           {
               //std::cout<<i<<","<<j<<","<<board[i][j]<<std::endl;
               return true;
           }
       }
     }
 }
 return false;
}

//返回当前棋盘上的所有空位置，并且空位置附近有棋子
std::vector<QPoint> GomokuAi::genMove(Board &board,int mypiece)
{
    //int oppopiece=(mypiece==BoardWidget::WHITE_PIECE)?BoardWidget::BLACK_PIECE:BoardWidget::WHITE_PIECE;
    //优先考虑连五、活四、双三、冲四、活三情况
    //std::vector<QPoint>emptyPos;
    std::vector<QPoint>five;//连五
    std::vector<QPoint>ai_livefour;
    std::vector<QPoint>hum_livefour;//活四
    std::vector<QPoint>ai_twothrees;
    std::vector<QPoint>hum_twothrees;//双三
    std::vector<QPoint>ai_sleepfour;
    std::vector<QPoint>hum_sleepfour;//冲四
    std::vector<QPoint>ai_onethree;
    std::vector<QPoint>hum_onethree;//活三
    std::vector<QPoint>ai_two;
    std::vector<QPoint>hum_two;
    std::vector<QPoint>neighbor;
    //emptyPos.clear();
    five.clear();
    ai_livefour.clear();
    hum_livefour.clear();//活四
    ai_twothrees.clear();
    hum_twothrees.clear();//双三
    ai_sleepfour.clear();
    hum_sleepfour.clear();//冲四
    ai_onethree.clear();
    hum_onethree.clear();//活三
    ai_two.clear();
    hum_two.clear();
    neighbor.clear();
    int radius=2;
    for(int i=0;i<BoardWidget::BOARD_WIDTH;i++)
    {
        for(int j=0;j<BoardWidget::BOARD_HEIGHT;j++)
        {
            if(board[i][j]==BoardWidget::NO_PIECE&&hasNeighbor(board,i,j,radius))
            {
                QPoint p=QPoint(i,j);
                int score_ai=evaluatePointScore(board,i,j, BoardWidget::BLACK_PIECE);
                int score_hum=evaluatePointScore(board,i,j, BoardWidget::WHITE_PIECE);
                if(score_ai>=livefive)
                {
                    five.push_back(p);
                }
                else if(score_hum>=livefive)
                {
                    five.push_back(p);
                }
                else if(score_ai>=livefour)
                {
                    ai_livefour.push_back(p);
                }
                else if(score_hum>=livefour)
                {
                    hum_livefour.push_back(p);
                }
                else if(score_ai>=twothrees)
                {
                    ai_twothrees.push_back(p);
                }
                else if(score_ai>=twothrees)
                {
                    hum_twothrees.push_back(p);
                }
                else if(score_ai>=sleepfour)
                {
                    ai_sleepfour.push_back(p);
                }
                else if(score_hum>=sleepfour)
                {
                    hum_sleepfour.push_back(p);
                }
                else if(score_ai>=livethree)
                {
                    ai_onethree.push_back(p);
                }
                else if(score_hum>=livethree)
                {
                    hum_onethree.push_back(p);
                }
                else if(score_ai>=sleeptwo)
                {
                    ai_two.push_back(p);
                }
                else if(score_hum>=sleeptwo)
                {
                    hum_two.push_back(p);
                }
                else
                {
                    neighbor.push_back(p);
                }
            }
        }
    }
    //对特殊情况的判断
    //如果有连五，直接返回
    if(five.size()>0)
    {
        return five;
    }
    //如果己方有活四，直接返回
    if(mypiece==BoardWidget::BLACK_PIECE&&ai_livefour.size()>0)
    {
        return ai_livefour;
    }
    if(mypiece==BoardWidget::WHITE_PIECE&&hum_livefour.size()>0)
    {
        return hum_livefour;
    }
    //对方有活四，己方没有冲四，考虑下在对方活四的位置
    if(mypiece==BoardWidget::BLACK_PIECE&&hum_livefour.size()>0&&ai_sleepfour.size()==0)
    {
        return hum_livefour;
    }
    if(mypiece==BoardWidget::WHITE_PIECE&&ai_livefour.size()>0&&hum_livefour.size()==0)
    {
        return ai_livefour;
    }
    //对方有活四，自己有冲四，则都考虑
    std::vector<QPoint>fours;
    fours.clear();
    if(mypiece==BoardWidget::BLACK_PIECE)
    {
        fours.insert(fours.end(),ai_livefour.begin(),ai_livefour.end());
        fours.insert(fours.end(),hum_livefour.begin(),hum_livefour.end());
    }
    else
    {
        fours.insert(fours.end(),hum_livefour.begin(),hum_livefour.end());
        fours.insert(fours.end(),ai_livefour.begin(),ai_livefour.end());
    }
    if(mypiece==BoardWidget::BLACK_PIECE)
    {
        fours.insert(fours.end(),ai_sleepfour.begin(),ai_sleepfour.end());
        fours.insert(fours.end(),hum_sleepfour.begin(),hum_sleepfour.end());
    }
    else
    {
        fours.insert(fours.end(),hum_sleepfour.begin(),hum_sleepfour.end());
        fours.insert(fours.end(),ai_sleepfour.begin(),ai_sleepfour.end());
    }
    if(fours.size()>0)
    {
        return fours;
    }
    //考虑双三、眠四、活三
    std::vector<QPoint>emptyPos;
    emptyPos.clear();
    if(mypiece==BoardWidget::BLACK_PIECE)
    {
        emptyPos.insert(emptyPos.end(),ai_twothrees.begin(),ai_twothrees.end());
        emptyPos.insert(emptyPos.end(),hum_twothrees.begin(),hum_twothrees.end());
        emptyPos.insert(emptyPos.end(),ai_sleepfour.begin(),ai_sleepfour.end());
        emptyPos.insert(emptyPos.end(),hum_sleepfour.begin(),hum_sleepfour.end());
        emptyPos.insert(emptyPos.end(),ai_onethree.begin(),ai_onethree.end());
        emptyPos.insert(emptyPos.end(),hum_onethree.begin(),hum_onethree.end());
    }
    if(mypiece==BoardWidget::WHITE_PIECE)
    {
        emptyPos.insert(emptyPos.end(),hum_twothrees.begin(),hum_twothrees.end());
        emptyPos.insert(emptyPos.end(),ai_twothrees.begin(),ai_twothrees.end());
        emptyPos.insert(emptyPos.end(),hum_sleepfour.begin(),hum_sleepfour.end());
        emptyPos.insert(emptyPos.end(),ai_sleepfour.begin(),ai_sleepfour.end());
        emptyPos.insert(emptyPos.end(),hum_onethree.begin(),hum_onethree.end());
        emptyPos.insert(emptyPos.end(),ai_onethree.begin(),ai_onethree.end());
    }
    //存在双三、返回
    if(ai_twothrees.size()>0||hum_twothrees.size()>0)
    {
        return emptyPos;
    }
    //考虑两个相连的情况
    std::vector<QPoint>two;
    two.clear();
    if(mypiece==BoardWidget::BLACK_PIECE)
    {
        two.insert(two.end(),ai_two.begin(),ai_two.end());
        two.insert(two.end(),hum_two.begin(),hum_two.end());
    }
    else
    {
        two.insert(two.end(),hum_two.begin(),hum_two.end());
        two.insert(two.end(),ai_two.begin(),ai_two.end());
    }
    //需要对剩余位置排序，并确定返回容器的最大长度，去除评分比较低的位置

    two.insert(two.end(),neighbor.begin(),neighbor.end());
    return two;
}
int GomokuAi::evaluatePointScore(Board board, int x, int y, int mypiece)
{
    reset(count);
    board[x][y]=mypiece;
    evaluateFourDirect(x,y,board);
    int score=getPointScore(count);
    board[x][y]=BoardWidget::NO_PIECE;
    return score;
}
int GomokuAi::getPointScore(double *count)
{
    int score=0;
    if(count[LIVE_FIVE]>0)
    {
        return livefive;
    }
    if(count[LIVE_FOUR]>0)
    {
        return livefour;
    }
    if(count[LIVE_THREE]>1)
    {
        return twothrees;
    }
    if(count[CHONG_FOUR]>0)
    {
        score+=sleepfour*count[CHONG_FOUR];
    }
    if(count[LIVE_THREE]>0)
    {
        score+=livethree*count[LIVE_THREE];
    }
    if(count[SLEEP_THREE]>0)
    {
        score+=sleepthree*count[SLEEP_THREE];
    }
    if(count[LIVE_TWO]>0)
    {
        score+=livetwo*count[LIVE_TWO];
    }
    if(count[SLEEP_TWO]>0)
    {
        score+=sleeptwo*count[SLEEP_TWO];
    }
    return score;
}
QPoint GomokuAi::search(Board &board,int depth,int alpha,int beta)
{
 int bestscore=SCORE_MIN;
 std::vector<QPoint>points=genMove(board,BoardWidget::BLACK_PIECE);
 for(unsigned int i=0;i<points.size();i++)
 {
  QPoint p=points[i];
  //std::cout<<points.length()<<std::endl;
  board[p.x()][p.y()]=BoardWidget::BLACK_PIECE;//AI落子
  int score=minSearch(board,depth-1,beta,alpha);//找最大值
  if(score>bestscore)
  {
      bestscore=score;
      bestPos=p;
  }
  board[p.x()][p.y()]=BoardWidget::NO_PIECE;//把尝试下的子移除
 }
 return bestPos;
}
int GomokuAi::minSearch(Board &board,int depth,int alpha,int beta)
{
 int score=evaluate(board,BoardWidget::BLACK_PIECE);//返回对当前局势的评分
 if(depth<=0||abs(score)>=729000)
 {
     return score;
 }
 int bestscore=SCORE_MAX;
 std::vector<QPoint> points=genMove(board,BoardWidget::WHITE_PIECE);
 for(unsigned int i=0;i<points.size();i++)
 {
  QPoint p=points[i];
  board[p.x()][p.y()]=BoardWidget::WHITE_PIECE;
  //score=maxSearch(board,depth-1,bestscore/*<beta?bestscore:beta*/,alpha);
  //score=maxSearch(board,depth-1,bestscore<beta?bestscore:beta,alpha);
  score=maxSearch(board,depth-1,bestscore<alpha?bestscore:alpha, beta);
  board[p.x()][p.y()]=BoardWidget::NO_PIECE;
  if(score<bestscore)
  {
      bestscore=score;
  }
  if(score<beta)
  {
      break;
  }
 }
return bestscore;//最小值，代表对于ai来说最不利的位置
}

int GomokuAi::maxSearch(Board &board,int depth,int alpha,int beta)
{
  int score=evaluate(board,BoardWidget::WHITE_PIECE);
  if(depth<=0||abs(score)>=729000)
  {
      return score;
  }
  int bestscore=SCORE_MIN;
  std::vector<QPoint> points=genMove(board,BoardWidget::BLACK_PIECE);
  for(unsigned int i=0;i<points.size();i++)
  {
      QPoint p=points[i];
      board[p.x()][p.y()]=BoardWidget::BLACK_PIECE;
     //score=minSearch(board,depth-1,beta,bestscore/*>alpha?bestscore:alpha*/);
      //score=minSearch(board,depth-1,beta,bestscore>alpha?bestscore:alpha);
      score=minSearch(board,depth-1,alpha,bestscore>beta?bestscore:beta);
      board[p.x()][p.y()]=BoardWidget::NO_PIECE;
      if(score>bestscore)
      {
          bestscore=score;
      }
      if(score>alpha)
      {
          break;
      }
  }
 return bestscore;//最大值，代表对于ai来说最有利的位置
}
void GomokuAi::copycount(double *array,bool isPiece)
{
    for(int i=0;i<8;i++)
    {
        if(isPiece)
        {
            mcount[i]+=array[i];
        }
        else
        {
            ocount[i]+=array[i];
        }
    }
}

void GomokuAi::reset(double *array)
{
    for(int i=0;i<8;i++)
    {
        array[i]=0;
    }
}
//对棋局进行打分，针对某一个棋手，返回AI得分-人得分
int GomokuAi::evaluate(Board &board,int mypiece)
{
 //memcpy(destinboard,board,sizeof(destinboard));
 //double scoreAi=0;
 //double scoreHuman=0;
 reset(mcount);
 reset(ocount);
 bool isPiece;
 if(mypiece==BoardWidget::BLACK_PIECE)
 {
     isPiece=true;
 }
 else
 {
     isPiece=false;
 }

 for(int i=0;i<BoardWidget::BOARD_WIDTH;i++)
 {
     for(int j=0;j<BoardWidget::BOARD_HEIGHT;j++)
     {
         if(board[i][j]!=BoardWidget::NO_PIECE)
         {
          if(board[i][j]==BoardWidget::BLACK_PIECE)
          {
           evaluateFourDirect(i,j,board);
           copycount(count,isPiece);
           reset(count);
          }
         else
          {
           evaluateFourDirect(i,j,board);
           copycount(count,!isPiece);
           reset(count);
          }
        }
     }
 }
if(isPiece)
 {
  return getScore();
 }
 else
 {
  return -1*getScore();
 }
}

//判断一条线上棋子形成的棋型
int GomokuAi::analysisLine(int *line,int mypiece,int oppopiece)
{
 int nopiece=BoardWidget::NO_PIECE;
 int right_idx=4;
 int left_idx=4;
 while(right_idx<8)
 {
   if(line[right_idx+1]!=mypiece)
   break;
   right_idx++;
 }
 while(left_idx>0)
 {
     if(line[left_idx-1]!=mypiece)
     break;
     left_idx--;
 }
 int right_range=right_idx;
 int left_range=left_idx;
 while(right_range<8)
 {
     if(line[right_range+1]==oppopiece)
         break;
     right_range++;
 }
 while(left_range>0)
 {
     if(line[left_range-1]==oppopiece)
         break;
     left_range--;
 }
int chess_range=right_range-left_range+1;
if(chess_range<5)
 {
   return NONE;
 }
int my_range=right_idx-left_idx+1;
if(my_range==5)
{
  count[LIVE_FIVE]+=1.0/5;
}

//M:己方棋子，P:对方棋子，X:空
//LIVE_FOUR:XMMMMX
//CHONG_FOUR:PMMMMX,XMMMMP
if(my_range==4)
{
    if(line[right_idx+1]==nopiece&&line[left_idx-1]==nopiece)
    {

        count[LIVE_FOUR]+=1.0/4;
    }
    if((line[right_idx+1]==nopiece&&line[left_idx-1]==oppopiece)||(line[right_idx+1]==oppopiece&&line[left_idx-1]==nopiece))
    {
        count[CHONG_FOUR]+=1.0/4;
    }
}

//CHONG_FOUR:MXMMM,MMMXM
//LIVE_THREE:XMMMXX,XXMMMX
//SLEEP_THREE:PMMMX,XMMMP,PXMMMXP
if(my_range==3)
{
    //MXMMM
    if(line[left_idx-1]==nopiece&&line[left_idx-2]==mypiece)
    {
        count[CHONG_FOUR]+=1.0/3;
    }
    //MMMXM
    if(line[right_idx+1]==nopiece&&line[right_idx+2]==mypiece)
    {
       count[CHONG_FOUR]+=1.0/3;
    }
    //XMMMXX,XXMMMX,PXMMMXP
    if(line[left_idx-1]==nopiece&&line[right_idx+1]==nopiece)
    {
        if(chess_range>5)
        {
            count[LIVE_THREE]+=1.0/3;
        }
        else
        {
            count[SLEEP_THREE]+=1.0/3;
        }
    }
    //PMMMX,XMMMP
    if((line[left_idx-1]==nopiece&&line[right_idx+1]==oppopiece)||(line[left_idx-1]==oppopiece&&line[right_idx+1]==mypiece))
    {
      count[SLEEP_THREE]+=1.0/3;
    }
}

//CHONG_FOUR:MMXMM
//LIVE_THREE:XMXMMX,XMMXMX
//SLEEP_THREE:PMXMMX,XMXMMP,PMMXMX,XMMXMP
//LIVE_TWO:XMMX
//SLEEP_TWO:PMMX,XMMP
if(my_range==2)
{
    //MMXMM,XMMXMX,PMMXMX,XMMXMP
   if(line[right_idx+1]==nopiece&&line[right_idx+2]==mypiece)
   {
       if(line[right_idx+3]==mypiece)
       {
           count[CHONG_FOUR]+=1.0/2;
       }
       else if(line[right_idx+3]==oppopiece)
       {
           count[SLEEP_THREE]+=1.0/2;
       }
       else
       {
           if(line[left_idx-1]==nopiece)
           {
             count[LIVE_THREE]+=1.0/2;
           }
           if(line[left_idx-1]==oppopiece)
           {
               count[LIVE_THREE]+=1.0/2;
           }
       }
   }

   //XMXMMX,XMXMMP,PMXMMX
   else if(line[left_idx-1]==nopiece&&line[left_idx-2]==mypiece)
   {
       if(line[left_idx-3]==oppopiece)
       {
           count[SLEEP_THREE]+=1.0/2;
       }
       if(line[left_idx-3]==nopiece)
       {
           if(line[right_idx+1]==nopiece)
           {
               count[LIVE_THREE]+=1.0/2;
           }
           if(line[right_idx+1]==oppopiece)
           {
              count[SLEEP_THREE]+=1.0/2;
           }
       }
   }

   //XMMX,PMMX,XMMP
   else
   {
       if(line[left_idx-1]==nopiece&&line[right_idx+1]==nopiece)
       {
           count[LIVE_TWO]+=1.0/2;
       }
       else
       {
           count[SLEEP_TWO]+=1.0/2;
       }
   }
}

//LIVE_TWO:XMXMX,XMXXMX
//SLEEP_TWO:PMXMX,XMXMP
if(my_range==1)
{
    //XMXMP
    if(line[left_idx-1]==nopiece&&line[left_idx-2]==mypiece&&line[left_idx-3]==nopiece)
    {
        if(line[right_idx+1]==nopiece)
        {
            count[LIVE_TWO]+=1;
        }
        if(line[right_idx+1]==oppopiece)
        {
            count[SLEEP_TWO]+=1;
        }
    }
    if(line[right_idx+1]==nopiece&&line[right_idx+1]==nopiece)
    {
        count[LIVE_TWO]+=1;
    }
    if(line[right_idx+1]==nopiece&&line[left_idx-1]==oppopiece)
    {
        count[SLEEP_TWO]+=1;
    }
}
return NONE;
}

//对一个棋子的四个方向进行检查
void GomokuAi::evaluateFourDirect(int posx,int posy,Board &board)
{
  int line[9];//一条线上的9个棋子
  int mypiece=board[posx][posy];//己方棋子
  int oppopiece=(mypiece==BoardWidget::WHITE_PIECE)?BoardWidget::BLACK_PIECE:BoardWidget::WHITE_PIECE;//对方棋子
 //水平方向
  for(int i=0;i<9;i++)
  {
     int tmpx=posx-4+i;
     if(tmpx<0||tmpx>=BoardWidget::BOARD_WIDTH)
     {
        line[i]=oppopiece;
     }
     else
     {
       line[i]=board[tmpx][posy];
     }
  }
  analysisLine(line,mypiece,oppopiece);
 //竖直方向
 for(int i=0;i<9;i++)
 {
    int tmpy=posy-4+i;
    if(tmpy<0||tmpy>=BoardWidget::BOARD_HEIGHT)
    {
       line[i]=oppopiece;
    }
    else
    {
        line[i]=board[posx][tmpy];
    }
 }
 analysisLine(line,mypiece,oppopiece);
 //左下至右上方向
 for(int i=0;i<9;i++)
 {
     int tmpx=posx-4+i;
     int tmpy=posy+4-i;
     if(tmpx<0||tmpx>=BoardWidget::BOARD_WIDTH||tmpy<0||tmpy>=BoardWidget::BOARD_HEIGHT)
     {
         line[i]=oppopiece;
     }
     else
     {
         line[i]=board[tmpx][tmpy];
     }

 }
 analysisLine(line,mypiece,oppopiece);
 //左上至右下方向
 for(int i=0;i<9;i++)
 {
     int tmpx=posx-4+i;
     int tmpy=posy-4+i;
     if(tmpx<0||tmpx>=BoardWidget::BOARD_WIDTH||tmpy<0||tmpy>=BoardWidget::BOARD_HEIGHT)
     {
         line[i]=oppopiece;
     }
     else
     {
         line[i]=board[tmpx][tmpy];
     }
 }
 analysisLine(line,mypiece,oppopiece);
}

//对黑棋和白棋进行评分，单个棋子的评分
int GomokuAi::getScore()
{
 int mscore=0;
 int oscore=0;
 //己方连五
 if(mcount[LIVE_FIVE]>0)
 {
     mscore+=729000;
 }

 //对方连五
 if(ocount[LIVE_FIVE]>0)
 {
    oscore+=729000;
 }

 //己方的两个冲四相当于一个活四
 if(mcount[CHONG_FOUR]>=2)
 {
     mcount[LIVE_FOUR]+=1;
 }

 //对方活四
 if(ocount[LIVE_FOUR]>0)
 {
     oscore+=243000;
 }

 //对方冲四
 if(ocount[CHONG_FOUR]>0)
 {
     oscore+=81000;
 }

 //己方活四
 if(mcount[LIVE_FOUR]>0)
 {
     mscore+=27000;
 }

 //己方冲四和活三
 if(mcount[CHONG_FOUR]>0&&mcount[LIVE_THREE]>0)
 {
    mscore+=9000;
 }

 //对方活三，己方没有冲四
 if(ocount[LIVE_THREE]>0&&mcount[CHONG_FOUR]==0)
 {
     oscore+=3000;
 }

 //己方活三至少两个，对方无活三和眠三
 if(mcount[LIVE_THREE]>1&&ocount[LIVE_THREE]==0&&ocount[SLEEP_THREE]==0)
 {
     mscore+=1000;
 }

//累计加分
 if(mcount[CHONG_FOUR]>0)
 {
     mscore+=600;
 }
 if(mcount[LIVE_THREE]>1)
 {
     mscore+=300;
 }
 else if(mcount[SLEEP_THREE]>0)
 {
     mscore+=60;
 }

 if(ocount[LIVE_THREE]>1)
 {
     oscore+=200;
 }
 else if(mcount[SLEEP_THREE]>0)
 {
     oscore+=50;
 }
 if(mcount[LIVE_TWO]>0)
 {
     mscore+=mcount[LIVE_TWO]*3;
 }
 if(ocount[LIVE_TWO]>0)
 {
     oscore+=ocount[LIVE_TWO]*3;
 }
 if(mcount[SLEEP_TWO]>0)
 {
     mscore+=mcount[SLEEP_TWO]*1;
 }
 if(ocount[SLEEP_TWO]>0)
 {
     oscore+=ocount[SLEEP_TWO]*1;
 }
 return mscore-oscore;
}
