#include "gamewidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//定义并创建应用程序
    GameWidget w;
    w.show();
    return a.exec();//应用程序运行
}
