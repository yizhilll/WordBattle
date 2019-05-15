#include "mainwindow.h"
#include <QApplication>

#include "accountwindow.h"
#include "rankwindow.h"
#include "clientend.h"
#include "vocabularyboard.h"
#include "challengeboard.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ClientAccess CA;

    MainWindow w(nullptr, &CA);
    AccountWindow aw(nullptr, &CA);
    RankWindow rw(nullptr,&CA);
    VocabularyBoard vb(nullptr,&CA);
    ChallengeBoard cb(nullptr,&CA);

    //信号与槽之间进行关联
    QObject::connect(&aw,SIGNAL(showrankwindow()),&rw,SLOT(receivelogin()));

    QObject::connect(&rw,SIGNAL(logout()),&aw,SLOT(receiveshow()));
    QObject::connect(&rw,SIGNAL(showvocaboard()),&vb,SLOT(receiveshow()));

    QObject::connect(&vb,SIGNAL(logout()),&aw,SLOT(receiveshow()));
    QObject::connect(&vb,SIGNAL(showsearchwindow()),&rw,SLOT(receiveshow()));
    QObject::connect(&vb,SIGNAL(showchanllengeboard()),&cb,SLOT(receiveshow()));

    QObject::connect(&cb,SIGNAL(logout()),&aw,SLOT(receiveshow()));
    QObject::connect(&cb,SIGNAL(showsearchwindow()),&rw,SLOT(receiveshow()));
    QObject::connect(&cb,SIGNAL(showvocabularyboard()),&vb,SLOT(receiveshow()));

    aw.show();

//    w.show();

    return a.exec();
}
