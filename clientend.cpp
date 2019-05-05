#include "clientend.h"

//-------------------------------------
UserControl::UserControl()
{
//    this->current_user_name=QString("no_one");
}
UserControl::~UserControl()
{
//    this->current_user_name=QString("no_one");
}

//-------------------------------------
ClientAccess::ClientAccess()
{
    //需要进行判断默认的连接名是否存在，
    //如果不存在才使用addDatabase()方法，如果存在则使用database()方法
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        udb = QSqlDatabase::database("qt_sql_default_connection");
    }else{
        udb = QSqlDatabase::addDatabase("QMYSQL");
    }
    udb.setHostName("127.0.0.1");
    udb.setPort(3306);
    udb.setUserName("root");
    udb.setPassword("000000");
    udb.setDatabaseName("wordbattle");

    if (!udb.isValid()){
        qDebug() << udb.lastError().text();
        return;
    }
    udb.open();
    QString query = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = 'wordbattle'";
    QSqlQuery q = udb.exec(query);
    if(q.size()==0){
        //创建数据库
        udb.exec("CREATE DATABASE IF NOT EXISTS wordbattle");
    }


    if(udb.lastError().isValid()){
        qDebug() << udb.lastError();
        qDebug() << "Create/Connect database failed.";
        return;
    }else{
        qDebug() << "Create/Connect database successfully.";
    }

    //test table existence
    udb.exec("CREATE TABLE IF NOT EXISTS users \
            (username VARCHAR(128) PRIMARY KEY,\
            password VARCHAR(1024),\
            c_stage INT DEFAULT 1, c_level INT DEFAULT 1, c_exp INT DEFAULT 0, c_time INT DEFAULT 0,\
            v_stage INT DEFAULT 1, v_level INT DEFAULT 1, v_exp INT DEFAULT 0, v_number INT DEFAULT 0);");
   if(udb.lastError().isValid()){
       qDebug() << udb.lastError();
       qDebug() << "Create table failed.";
       return;
    }else{
       qDebug()<<"Create table succeed.";
    }
}

bool ClientAccess::Register(QString username,QString password)
{
    QString h_password=QString(QCryptographicHash::hash(password.toLatin1(),QCryptographicHash::Sha3_256));
    QString qinsert;
    qinsert = QString("INSERT INTO users (username,password)\
                       values ('%1','%2');").arg(username).arg(h_password);
    udb.exec(qinsert);
    if(udb.lastError().isValid()){
        qDebug()<<"user register failed.";
        return false;
    }else{
        qDebug()<<"user register succeed.";
        return true;
    }
}

int ClientAccess::LoginValid(QString username, QString password)
{
    QSqlQuery query=udb.exec(QString("SELECT * FROM users WHERE username='%1'").arg(username));//
    //find user
    query.next();
    if(query.value(0).toString()==username){
        qDebug()<<"user found";
    }else {
        qDebug()<< "no such user";
//        this->releaseDebug = QString("%1").arg(query.lastError().text());//.arg(query.value(0).toString())
        return 0;
    }
    //match password
    QString h_password=QString(QCryptographicHash::hash(password.toLatin1(),QCryptographicHash::Sha3_256));

    if(h_password ==query.value(1).toString()){
        qDebug()<<"password match";
        return 2;
    }else{
        return 1;
    }
}

QList<QList<QString>> ClientAccess::SearchTop(QString type,QString attribute,int num)
{
    qDebug()<<"running searchtop";
    QList<QList<QString>> r;
    QString a1,a2,a3;
    if("Challenger"==type){
        a1=QString("c_stage");
        a2=QString("c_level");
        a3=QString("c_time");
    }else{
        a1=QString("v_stage");
        a2=QString("v_level");
        a3=QString("v_number");
    }
    QString q=QString("SELECT username, %1, %2, %3 \
                    FROM users\
                    ORDER BY %4 DESC\
                    LIMIT %5;")\
            .arg(a1).arg(a2).arg(a3).arg(attribute).arg(num);
    QSqlQuery query=udb.exec(q);
    qDebug()<<query.lastError();

    while(query.next()){
        QList<QString> tem;
        for(int j=0;j<4;j++){
            tem.append(query.value(j).toString());
        }
        r.append(tem);
        qDebug()<<tem;
    }
    return r;
}

QList<QList<QString>> ClientAccess::CheckUser(QString type, QString u, QString s, QString l,QString t)
{
    QList<QString> keywords({u,s,l,t});
    QList<QString> attributes;
    QList<QList<QString>> r;
    attributes.append(QString("username"));
    if("Challenger"==type){
        attributes.append(QString("c_stage"));
        attributes.append(QString("c_level"));
        attributes.append(QString("c_time"));
    }else{
        attributes.append(QString("v_stage"));
        attributes.append(QString("v_level"));
        attributes.append(QString("v_number"));
    }
    int nonEmpty=0;
    QString predicate("WHERE ");
    for(int i=0;i<keywords.count();i++){
        if(!keywords.at(i).isEmpty()){
            nonEmpty=nonEmpty+1;
            if(0==i){//username is string
                predicate=predicate+QString("username like '%%1%' ").arg(keywords.at(i));
            }else{
                switch (nonEmpty) {
                case 0:
                    break;
                case 1:
                    predicate=predicate+QString("%1 = %2 ").arg(attributes.at(i)).arg(keywords.at(i));
                    break;
                default:
                    predicate=predicate+QString("AND %1 = %2 ").arg(attributes.at(i)).arg(keywords.at(i));
                    break;
                }
            }
        }
    }
    QString qselect(QString("SELECT %1, %2, %3, %4 FROM users ")\
                    .arg(attributes.at(0)).arg(attributes.at(1))\
                    .arg(attributes.at(2)).arg(attributes.at(3)));
    qDebug()<<"executing: "<<qselect+predicate;
    QSqlQuery query=udb.exec(qselect+predicate);
    qDebug()<<query.lastError();

    while(query.next()){
        QList<QString> tem;
        for(int j=0;j<4;j++){
            tem.append(query.value(j).toString());
        }
        r.append(tem);
        qDebug()<<tem;
    }
    return r;
}

ClientAccess::~ClientAccess()
{

}
