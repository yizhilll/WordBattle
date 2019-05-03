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
private:
       QSqlDatabase udb;
};

#endif // CLIENTEND_H
