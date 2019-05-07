#include "mainwindow.h"
#include <QApplication>

#include "accountwindow.h"
#include "rankwindow.h"
#include "clientend.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ClientAccess CA;

    MainWindow w(nullptr, &CA);
    AccountWindow aw(nullptr, &CA);
    RankWindow rw(nullptr,&CA);

    //信号与槽之间进行关联
    QObject::connect(&aw,SIGNAL(showrankwindow()),&rw,SLOT(receivelogin()));
    QObject::connect(&rw,SIGNAL(logout()),&aw,SLOT(receiveshow()));
    aw.show();

//    w.show();

    return a.exec();
}
