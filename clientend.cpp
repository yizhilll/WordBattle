#include "clientend.h"
//-------------------------------------
int uChallenger::WordShowTime(int type)
{
    int r=5;//default show time 5s
    if(type==1){
        r=5;
    }
    return r;
}
int uChallenger::nStageWord(void)
{
    int r= 3 + 2* (int)(Stage()/3);
    return r;
}
int uChallenger::addExp(QString winword, QString type)
{
    //TODO if("local"==type){}else{}
    int reward=(int)winword.size()*(Stage()*0.5+ Level() +Accumulate()*0.01)*4;
    reward = reward>0.35*expToNext()? 0.35*expToNext():reward;
    int exp= reward+(int)0.05* expToNext();
    if(exp+Exp()>expToNext()){
        levelUp();
        setExp(exp+Exp()-expToNext());
        return 1;//level up
    }else{
        setExp(exp+Exp());
        return 0;//not level up
    }
}
int uChallenger::wordLength(void)
{
    int len= 3+(int)Stage()*0.2;
    return len>7? 7:len;
}
//-------------------------------------
int uVocabulor::addOneWord(QString word)
{
    setStage(Stage()+1);
    setExp(Exp()+ word.size());
}

int uVocabulor::addExp(QString winword)
{
    int exp= (int)winword.size()*(Stage()*0.01+ Level() +Accumulate()*0.01)*10;
    if(exp+Exp()>expToNext()){
        levelUp();
        setExp(exp+Exp()-expToNext());
        return 1;//level up
    }else{
        setExp(exp+Exp());
        return 0;//not level up
    }
}
//-------------------------------------
UserControl::UserControl(QString un,int cstage,int clevel,int cexp,int cacc,int vstage,int vlevel,int vexp,int vacc)
{
    this->ch=new uChallenger;
    this->vo=new uVocabulor;
    this->username=un;
    ch->setStage(cstage);
    ch->setLevel(clevel);
    ch->setExp(cexp);
    ch->setAccumulate(cacc);
    vo->setStage(vstage);
    vo->setLevel(vlevel);
    vo->setExp(vexp);
    vo->setAccumulate(vacc);
}
UserControl::~UserControl()
{

    delete ch;
    delete vo;
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
            //test table existence
    udb.exec("CREATE TABLE IF NOT EXISTS words \
            (word VARCHAR(512) PRIMARY KEY,\
            length INT NOT NULL, username VARCHAR(256), wdate DATE DEFAULT '2000-01-01');");
   if(udb.lastError().isValid()){
       qDebug() << udb.lastError();
       qDebug() << "Create table failed.";
       return;
    }else{
       qDebug()<<"Load or create table succeed.";
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
    QSqlQuery query=udb.exec(QString("SELECT * FROM users WHERE username='%1' LIMIT 1").arg(username));//
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
        //load user information to local client
        this->user=new UserControl(username,
                                   query.value(2).toInt(),query.value(3).toInt(),query.value(4).toInt(),query.value(5).toInt(),
                                   query.value(6).toInt(),query.value(7).toInt(),query.value(8).toInt(),query.value(9).toInt());
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
void ClientAccess::AddTime(int increase,QString type)
{
    if("challenger"==type){
        user->ch->setAccumulate(increase+user->vo->Accumulate());
    }else if("vocabulor"==type){
        user->vo->setAccumulate(increase+user->vo->Accumulate());
    }else{
        qDebug()<<"wrong user type!";
    }
}


int ClientAccess::AddWord(QString word,QString isDaddy)
{
    /* return 1 succeed
     * return 0 exists or other database error
     * return -1 spelling error
     */
    //TODO: spelling check
    QString un;
    //if it's the user adding
    if("superuser"==isDaddy){
        un="superuser";
    }else{
        un=this->user->getUsername();
    }
    //to see if it's been create by the super user
    QString squery = QString("SELECT * FROM words WHERE word = '%1' AND username not like 'superuser'").arg(word);
    QSqlQuery query(udb);
    query.exec(squery);
    if(query.next()){
        qDebug()<<QString("add new word failed, %1 has been created by other user").arg(word);
        return 0;
    }
    squery = QString("INSERT INTO words(word,length,username,wdate)\
                    VALUES('%1',%2,'%3','%4')").arg(word).arg(word.size()).arg(un).arg(QDate::currentDate().toString(QString("yyyy-MM-dd")));
    query.exec(squery);
    if(query.lastError().isValid()){
        qDebug()<<query.lastError();
        qDebug()<<"add new word failed.";
        return 0;
    }else{
        qDebug()<<"add new word succeed.";
        if(isDaddy!="superuser"){
            user->vo->addOneWord(word);
        }
        return 1;
    }
}

QString ClientAccess::GetOneWord(int len,QString start)
{
    QString condition="";
    if(len!=-1){
        condition+= QString("WHERE length >= %1" ).arg(len);
    }

    //a slow implementation   WHERE
    QString query = QString("SELECT *\
                            FROM words %1\
                            ORDER BY RAND()\
                            LIMIT 1").arg(condition);
    QSqlQuery q = udb.exec(query);


    QString r=QString("emm no done");
    if(!q.lastError().isValid() && q.next()){
        for(int j=0;j<4;j++){
            qDebug()<<q.value(j).toString();
        }
        r=q.value(0).toString();
    }else{
        qDebug()<<"fail to fetch a word";
        qDebug() << q.lastError().text();
    }
    return r;
}
void ClientAccess::SaveUserToDatabase()
{
    qDebug()<<"start to save user status to database";
    QSqlQuery query(udb);
    QString q=QString("UPDATE users \
                       SET\
                       c_stage= %1, c_level= %2, c_exp= %3, c_time =%4, v_stage= %5, v_level= %6, v_exp= %7, v_number=%8\
                       WHERE username = '%9';")\
                       .arg(user->ch->Stage()).arg(user->ch->Level()).arg(user->ch->Exp()).arg(user->ch->Accumulate())\
                       .arg(user->vo->Stage()).arg(user->vo->Level()).arg(user->vo->Exp()).arg(user->vo->Accumulate())\
                       .arg(user->getUsername());
    query.exec(q);
    if(query.lastError().isValid()){
        qDebug()<<query.lastError();
    }
}
void ClientAccess::DestoryUserWLogOut()
{
    //SAVE BEFORE DELETE
    SaveUserToDatabase();
    delete this->user;
}

QString ClientAccess::execSuperSQL(QString query)
{
    QSqlQuery q= udb.exec(query);
    if(q.lastError().isValid()){
        return q.lastError().text();
    }else{
        return QString("SQL executed successfully");
    }
}
QString ClientAccess::loadCSV(QString filename)
{
    int succount=0;
    int failcount=0;
    QString response;
    //default file is "TOEFL核心词汇21天突破.CSV"
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {

        int lineindex = 0;                     // file line counter
        QTextStream in(&file);                 // read to text stream

        while (!in.atEnd()) {

            // read one line from textstream(separated by "\n")
            QString fileLine = in.readLine();

            // parse the read line into separate pieces(tokens) with "," as the delimiter
            QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);
            qDebug()<<lineToken;
            if(0==AddWord(lineToken.at(0),"superuser")){
                failcount++;
            }else{
                succount++;
            }
            lineindex++;
        }
        file.close();
        response=QString("load csv file successfully\nsucceed in %1 words\nfail at %2 words").arg(succount).arg(failcount);
    }else{
        response=QString("open csv file fail");
        qDebug()<<response;
    }
    return response;
}

ClientAccess::~ClientAccess()
{
    delete user;
}
