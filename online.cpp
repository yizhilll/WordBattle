#include "online.h"
#include "ui_online.h"

online::online(QWidget *parent,ClientAccess* c) :
    QWidget(parent),
    ui(new Ui::online)
{
    ui->setupUi(this);
    ui->tips_skip->hide();
    ui->l_word->hide();
    ui->l_worddis->hide();
    ui->B_confirm->hide();
    ui->B_skip->hide();
    ui->input->hide();
    ui->tips->hide();
    currentround=0;
    this->CA=c;
    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeINC()));
}

//获取新的对战状态并且更新显示的比分，判定结果
void online::refreshStatus()
{
   qDebug()<<"begin refresh";
   QString searchType;
   if(CA->isAggressive)
       searchType="userA";
   else
       searchType="userB";
   this->r = CA->SearchTop("onbattle",searchType,1);
//   qDebug()<<"refresh status :score A B before judge " << scoreA<<":" <<scoreB;
   ui->l_twipoint->setText(QString("%1 : %2").arg(r[0][1]).arg(r[0][3]));
   onlineState=r[0][4].toInt();
   currentWord=r[0][6];
   //A 查看 是否对方有无强退
   if(CA->isAggressive &&-50==onlineState){
       CA->execSuperSQL(QString("DELETE FROM onbattle WHERE userA='%1'").arg(CA->user->getUsername()));
       this->hide();
       emit endGameAndBack();
   }
   //第五轮判定结果
   if(r[0][5].toInt()>=5){
       QMessageBox* msgb=new QMessageBox(this);
       msgb->setWindowTitle("对战结束通知");
       msgb->setStandardButtons(QMessageBox::Ok);
       if(CA->isAggressive && r[0][1].toInt()>r[0][3].toInt())
           msgb->setText("你赢辽！");
       else if(r[0][1].toInt()==r[0][3].toInt())
               msgb->setText("平局辽！");
       else
           msgb->setText("你输辽！");
       if(CA->isAggressive){
           CA->execSuperSQL(QString("DELETE FROM onbattle WHERE userA='%1'").arg(CA->user->getUsername()));
       }
       msgb->exec();
       this->hide();
       timer->stop();
       emit endGameAndBack();
   }
}


void online::timeINC()
{
    qDebug()<<"online::timeINC() running";
    switch(gameState){
    case -1:
        if(!ui->B_confirm->isHidden()){
            ui->B_confirm->hide();
            ui->B_skip->hide();
            ui->input->hide();
            ui->tips->hide();
            ui->input->setText("");
        }
        qDebug()<<"case -1";
        if(CA->isAggressive){
            currentround++;
            currentWord=CA->GetOneWord(CA->user->ch->wordLength(),"1");
            CA->execSuperSQL(QString("UPDATE onbattle SET state=0,currentword='%1',currentround=%3 WHERE userA='%2';").arg(currentWord).arg(CA->user->getUsername()).arg(currentround));
            gameState=0;
        }else{
            refreshStatus();
            gameState=onlineState;
            currentround=r[0][5].toInt();
        }
        playtime=0;
        timetohide=5;
        // 用户b更新为0
        break;
    //显示单词
    case 0:
        qDebug()<<"case 0";
        ui->l_word->setText(currentWord);
        ui->l_worddis->setText(QString("%1").arg(timetohide));
        if(ui->l_word->isHidden()){
            ui->l_word->show();
            ui->l_worddis->show();
            ui->tips_skip->show();
        }

        timetohide--;
        if(timetohide<0){
            gameState=20;
            ui->l_word->hide();
//            ui->l_worddis->hide();
            ui->l_worddis->setText(QString("%1").arg(playtime));
            ui->tips_skip->hide();
            ui->B_confirm->show();
            ui->B_skip->show();
            ui->input->show();
            ui->tips->show();
        }
        break;
    case 20://等待答题状态
        qDebug()<<"case 20";
        playtime++;
        ui->tips->setText("请输入单词后按回车键或确认来完成猜题");
        ui->l_worddis->setText(QString("%1").arg(31-playtime));
        if(playtime>=30){//超时了
            refreshStatus();
            endGame();
            gameState=21;
        }
        break;
    case 21://用户答题完毕查状态
        qDebug()<<"case 21";
        refreshStatus();
//        if()
        ui->tips->setText("作答环节结束，等待对方回答");
        endGame();

        break;
    default:
        break;
    }
}

//用户A完成答题,上传以后调用，根据时间更新比分
void online::judge(int timeA,int timeB){
    int scoreA=r[0][1].toInt();
    int scoreB=r[0][3].toInt();
    qDebug()<<"score A B before judge " << scoreA<<":" <<scoreB;
    if(timeA<timeB){
        scoreA++;
    }else if(timeA>timeB){
        scoreB++;
    }else{
        scoreA++;
        scoreB++;
    }
    qDebug()<<"score A B after judge " << scoreA<<":" <<scoreB;
    if(CA->isAggressive){
        CA->execSuperSQL(QString("UPDATE onbattle SET scoreA=%1,scoreB=%2 WHERE userA='%3';").arg(scoreA).arg(scoreB).arg(CA->user->getUsername()));
    }else{
        CA->execSuperSQL(QString("UPDATE onbattle SET scoreA=%1,scoreB=%2 WHERE userB='%3';").arg(scoreA).arg(scoreB).arg(CA->user->getUsername()));
    }
}
//必须先refresh再用
//用户完成答题，并上传提交答题时间以后调用，根据onlinestate状态和用户身份来更新状态
void online::endGame(){
    //如果是A
    if(CA->isAggressive){
        if(0==onlineState){//告诉B，A已经答题完成
            CA->execSuperSQL(QString("UPDATE onbattle SET state=2,timeA=%2 WHERE userA='%1';").arg(CA->user->getUsername()).arg(playtime));
        }
        else if(4==onlineState){//如果B在等A，告诉B已经两人都已经完成，并且judge谁得分
            judge(playtime,r[0][8].toInt());
            CA->execSuperSQL(QString("UPDATE onbattle SET state=8,timeA=%2 WHERE userA='%1';").arg(CA->user->getUsername()).arg(playtime));
            refreshStatus();
        }
        else if(10==onlineState){//B已经知道结果，进入新的一局或者删除记录
            CA->execSuperSQL(QString("UPDATE onbattle SET state=-1 WHERE userA='%1';").arg(CA->user->getUsername()));
            gameState=-1;
            if(currentround>=5){//如果是最后一局
                refreshStatus();
//                timer->stop();
//                this->hide();
//                emit endGameAndBack();
            }
        }else{
            qDebug()<<"error online state at online::endGame()";
        }
    }else{

        if(0==onlineState||2==onlineState){//A是否在等B无所谓
            CA->execSuperSQL(QString("UPDATE onbattle SET state=4,timeB=%2 WHERE userB='%1';").arg(CA->user->getUsername()).arg(playtime));
        }else if(8==onlineState){//A已经判定完当前局，B通知A已经读到
            CA->execSuperSQL(QString("UPDATE onbattle SET state=10 WHERE userB='%1';").arg(CA->user->getUsername()));
            gameState=-1;
            if(currentround>=5){//如果是最后一局
                refreshStatus();
//                timer->stop();
//                this->hide();
//                emit endGameAndBack();
            }
        }else if(-1==onlineState){
            gameState=-1;
        }
    }
}

void online::receiveshow()
{
    ui->l_username->setText(CA->user->getUsername());
    this->show();
    qDebug()<<"here online::receiveshow";
    if(CA->isAggressive){
        ui->l_twoname->setText(QString("%1 VS %2").arg(CA->user->getUsername()).arg(CA->opposer));
    }else{
        ui->l_twoname->setText(QString("%1 VS %2").arg(CA->opposer).arg(CA->user->getUsername()));
    }
    refreshStatus();//小心会导致退出?
    qDebug()<<"here 2";
    gameState=onlineState;
    qDebug()<<"here 3";
    timer->start(1000);
}
void online::on_B_skip_clicked()
{
    playtime=30;
}

void online::on_B_confirm_clicked()
{
    if(ui->input->text()==currentWord){
        gameState=21;
        ui->tips->setText("答对了，等待对方回答");
    }else{
        ui->tips->setText("答错了，请重试");
    }
}
online::~online()
{
    delete ui;
}

void online::on_B_start_clicked()
{
    if(CA->isAggressive)
        CA->execSuperSQL(QString("DELETE FROM onbattle WHERE userA='%1';").arg(CA->user->getUsername()));
    else{
        CA->execSuperSQL(QString("UPDATE onbattle SET state=-50 WHERE userB='%1';").arg(CA->user->getUsername()));
    }
    this->hide();
    emit endGameAndBack();
}

void online::on_input_returnPressed()
{
    on_B_confirm_clicked();
}
