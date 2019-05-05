#ifndef CLIENTEND_H
#define CLIENTEND_H
#include <QWidget>
#include <QString>
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlRecord>

#include <QCryptographicHash>
#include <QRegExpValidator>

#include <QTime>



class UserInfo
{
public:
    QString Username(){return username;}
    int Stage(){return stage;}
    int Level(){return level;}
    int Exp(){return exp;}
    int Accumulate(){return accumulate;}
    QString setUsername(QString x="no_one"){return username=x;}
    int setStage(int x=1){return stage=x;}
    int setLevel(int x=1){return level=x;}
    int setExp(int x=0){return exp=x;}
    int setAccumulate(int x=0){return accumulate=x;}

private:
    QString username;
    int stage;//stage for challenger; word number for examiner
    int level;
    int exp;
    int accumulate;//play time
};

class UserControl
{
public:
    UserControl();
    ~UserControl();
private:
    QString current_user_name;

};

class ClientAccess
{
public:
    ClientAccess();
    ~ClientAccess();

    bool Register(QString username,QString password);
    int LoginValid(QString username,QString password);
    QList<QList<QString>> SearchTop(QString type="Challenger",QString attribute="c_stage",int num=10);
    QList<QList<QString>> CheckUser(QString type, QString u,QString s,QString l,QString t);

    QString releaseDebug;

private:
       QSqlDatabase udb;
};

#endif // CLIENTEND_H
