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
#include <QTimer>

#include <QFile>



class UserInfo
{
public:
//    QString Username(){return username;}
    int Stage(){return stage;}
    int Level(){return level;}
    int Exp(){return exp;}
    int Accumulate(){return accumulate;}
//    QString setUsername(QString x="no_one"){return username=x;}
    int setStage(int x=1){return stage=x;}
    int setLevel(int x=1){return level=x;}
    int setExp(int x=0){return exp=x;}
    int setAccumulate(int x=0){return accumulate=x;}

private:
//    QString username;
    int stage;//stage for challenger; word number for examiner
    int level;
    int exp;
    int accumulate;//play time
};

class uChallenger:public UserInfo
{
public:
    int cLevel();//?
    void stageUp(void){setStage(Stage()+1);}//the user will go to the next stage
    int WordShowTime(int type);//return the duration(sec) a word should show depending on something
    int nStageWord(void);//return the number of words this stage need to challenge
};

class uVocabulor:public UserInfo
{
public:
    int vLevel();
    int addOneWord(QString word);//add exp and records to the user when a word is successfully added
};


class UserControl
{
public:
    UserControl(QString un,int cstage,int clevel,int cexp,int cacc,int vstage,int vlevel,int vexp,int vacc);
    QString getUsername(void){return this->username;}
    void setUsername(QString un){this->username=un;}
    ~UserControl();
    uChallenger *ch;
    uVocabulor  *vo;
private:
    QString username;
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

    void AddTime(int increase,QString type);//add time with minute unit
    int AddWord(QString word,QString isDaddy="notsuperuser");//add the word to database
    QString GetOneWord(int len,QString start);//randomly get one word with a specific length and starting character
    void DestoryUserWLogOut();//save the user status to the database and exit.

    QString execSuperSQL(QString query);
    QString loadCSV(QString filename="TOEFL核心词汇21天突破.CSV");

    UserControl *user;
    QString releaseDebug;

private:

    QSqlDatabase udb;
};

#endif // CLIENTEND_H
