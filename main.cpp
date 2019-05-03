#include "mainwindow.h"
#include <QApplication>

#include "accountwindow.h"
#include "rankwindow.h"
#include "clientend.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UserControl user;
    ClientAccess CA;

    MainWindow w(nullptr, &CA);
    AccountWindow aw(nullptr, &CA);

    //信号与槽之间进行关联
    QObject::connect(&aw,SIGNAL(showmainwindow()),&w,SLOT(receivelogin()));

    aw.show();

//    w.show();

    return a.exec();
}
