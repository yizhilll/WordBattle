#include "mainwindow.h"
#include <QApplication>

#include "accountwindow.h"
#include "rankwindow.h"
#include "clientend.h"
#include "vocabularyboard.h"
#include "challengeboard.h"
#include "online.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ClientAccess CA;

    MainWindow w(nullptr, &CA);
    AccountWindow aw(nullptr, &CA);
    RankWindow rw(nullptr,&CA);
    VocabularyBoard vb(nullptr,&CA);
    ChallengeBoard cb(nullptr,&CA);
    qDebug()<<"here0";
    online olcb(nullptr,&CA);

    //信号与槽之间进行关联
    QObject::connect(&aw,SIGNAL(showrankwindow()),&rw,SLOT(receivelogin()));
    QObject::connect(&aw,SIGNAL(showchanllengeboard()),&cb,SLOT(receiveshow()));
    QObject::connect(&aw,SIGNAL(cbsetup()),&cb,SLOT(setup()));

    QObject::connect(&rw,SIGNAL(logout()),&aw,SLOT(receiveshow()));
    QObject::connect(&rw,SIGNAL(showvocaboard()),&vb,SLOT(receiveshow()));

    QObject::connect(&vb,SIGNAL(logout()),&aw,SLOT(receiveshow()));
    QObject::connect(&vb,SIGNAL(showsearchwindow()),&rw,SLOT(receiveshow()));
    QObject::connect(&vb,SIGNAL(showchanllengeboard()),&cb,SLOT(receiveshow()));

    QObject::connect(&cb,SIGNAL(logout()),&aw,SLOT(receiveshow()));
    QObject::connect(&cb,SIGNAL(showsearchwindow()),&rw,SLOT(receiveshow()));
    QObject::connect(&cb,SIGNAL(showvocabularyboard()),&vb,SLOT(receiveshow()));

    qDebug()<<"here1";
    QObject::connect(&rw,SIGNAL(showonbt()),&olcb,SLOT(receiveshow()));
    qDebug()<<"here2";
    QObject::connect(&olcb,SIGNAL(endGameAndBack()),&rw,SLOT(restartTimer()));
qDebug()<<"here3";
    aw.show();

//    w.show();

    return a.exec();
}
