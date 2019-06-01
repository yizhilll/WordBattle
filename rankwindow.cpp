#include "rankwindow.h"
#include "ui_rankwindow.h"

RankWindow::RankWindow(QWidget *parent, ClientAccess *CA) :
    QWidget(parent),
    ui(new Ui::RankWindow)
{
    ui->setupUi(this);
    this->CA =CA;
    ui->RankTable->setRowCount(10);
    ui->RankTable->setColumnCount(4);
    onlineState=-1;
    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeINC()));
    msg=new QMessageBox(this);
    msg->setWindowTitle("对战请求");
    msg->setText("XXX 发起对战请求，是否接受？");
    msg->setStandardButtons(QMessageBox::Yes| QMessageBox::No);//对话框上包含的按钮


}

RankWindow::~RankWindow()
{
    delete ui;
}

bool RankWindow::showChallengerRank(QString type,QString attribute,int num)
{
    ui->RankTable->clearContents();
    ui->label_rank->setText("闯关者排行");
    QStringList h_header;
    h_header<<"用户名"<<"闯关数"<<"等级"<<"游戏时间（分钟）";
    ui->RankTable->setHorizontalHeaderLabels(h_header);

    QList<QList<QString>> r = CA->SearchTop(type,attribute,num);
    for(int i=0;i<r.count();i++){
        for(int j=0;j<4;j++){
            ui->RankTable->setItem(i,j,new QTableWidgetItem(r[i][j]));
        }
    }
//    return true;
}
bool RankWindow::showVocabulorRank(QString type,QString attribute,int num)
{
    ui->RankTable->clearContents();
    ui->label_rank->setText("出题者排行");
    QStringList h_header;
    h_header<<"用户名"<<"出题数"<<"等级"<<"游戏时间（分钟）";
    ui->RankTable->setHorizontalHeaderLabels(h_header);

    QList<QList<QString>> r = CA->SearchTop(type,attribute,num);
    for(int i=0;i<r.count();i++){
        for(int j=0;j<4;j++){
            ui->RankTable->setItem(i,j,new QTableWidgetItem(r[i][j]));
        }
    }
//    return true;
}

void RankWindow::receivelogin()
{
    //ui->label_rank->setText("闯关者排行");
    showChallengerRank("Challenger","c_stage",10);
    this->show();
}
void RankWindow::on_Button_logout_clicked()
{
    this->hide();
    emit logout();
    CA->DestoryUserWLogOut();
}
void RankWindow::receiveshow()
{
    this->show();
}
void RankWindow::on_Button_game_clicked()
{
    this->hide();
    emit showvocaboard();
}

void RankWindow::on_Button_switch_clicked()
{
    if("闯关者排行"==ui->label_rank->text()){
        showVocabulorRank("Vocabulor","v_stage",10);
        ui->label_rank->setText("出题者排行");
        ui->BS_s->setText("按题数排序");
    }else{
        showChallengerRank("Challenger","c_stage",10);
        ui->label_rank->setText("闯关者排行");
        ui->BS_s->setText("按关数排序");
    }
}

void RankWindow::on_BS_u_clicked()
{
    if("闯关者排行"==ui->label_rank->text()){
        showChallengerRank("Challenger","username",10);
    }else{
        showVocabulorRank("Vocabulor","username",10);
    }
}

void RankWindow::on_BS_s_clicked()
{
    if("闯关者排行"==ui->label_rank->text()){
        showChallengerRank("Challenger","c_stage",10);
    }else{
        showVocabulorRank("Vocabulor","v_stage",10);
    }
}

void RankWindow::on_BS_l_clicked()
{
    if("闯关者排行"==ui->label_rank->text()){
        showChallengerRank("Challenger","c_level",10);
    }else{
        showVocabulorRank("Vocabulor","v_level",10);
    }
}


void RankWindow::on_BS_t_clicked()
{
    if("闯关者排行"==ui->label_rank->text()){
        showChallengerRank("Challenger","c_time",10);
    }else{
        showVocabulorRank("Vocabulor","v_number",10);
    }
}

void RankWindow::on_Button_check_clicked()
{
    QString u= ui->lineEdit_u->text();
    QString s= ui->lineEdit_s->text();
    QString l= ui->lineEdit_l->text();
    QString t= ui->lineEdit_t->text();
    QString checktype=("闯关者排行"==ui->label_rank->text())? "Challenger":"Vocabulor";
    QList<QList<QString>> r=CA->CheckUser(checktype,u,s,l,t);
    ui->RankTable->clearContents();
    for(int i=0;i<r.count();i++){
        for(int j=0;j<4;j++){
            ui->RankTable->setItem(i,j,new QTableWidgetItem(r[i][j]));
        }
    }
}





void RankWindow::on_B_online_check_clicked()
{
    ui->RankTable->clearContents();
    ui->label_rank->setText("闯关者排行");
    QStringList h_header;
    h_header<<"在线用户名"<<"闯关数"<<"等级"<<"游戏时间（分钟）";
    ui->RankTable->setHorizontalHeaderLabels(h_header);

    QList<QList<QString>> r = CA->SearchTop("OnlineUser","c_stage",50);
    for(int i=0;i<r.count();i++){
        for(int j=0;j<4;j++){
            ui->RankTable->setItem(i,j,new QTableWidgetItem(r[i][j]));
        }
    }
}

void RankWindow::on_B_online_clicked()
{
    CA->getOnlineOffline("getOnline");
    onlineState=0;
    timer->start(2*1000);
}
void RankWindow::on_B_offline_clicked()
{
    CA->getOnlineOffline("getOffline");
    onlineState=-1;
    timer->stop();
}
void RankWindow::on_B_challenge_clicked()
{
    qDebug()<<"click challenge";
    QString opposer=ui->L_challenge->text();
    QString query=QString("INSERT INTO onbattle (userA,userB,scoreA,state) VALUES('%1','%2',0,-1);").arg(CA->user->getUsername()).arg(opposer);
    qDebug()<<"processing "<<query;
    CA->execSuperSQL(query);
    onlineState=1;
}

void RankWindow::timeINC()
{
    QList<QList<QString>> r;
    //记录状态，-1离线，0 上线扫描有无挑战，1 发起挑战查看是否被接受，2进入对战停止扫描
    switch(onlineState){
    case 0:
        timer->stop();
        r = CA->SearchTop("userB",ui->L_challenge->text(),50);
        for(int i=0;i<r.count();i++){
            if(CA->user->getUsername()==r[i][2] && "-1" == r[i][3].toLatin1()){
                msg->setText(QString("%1 发起对战请求，是否接受？").arg(r[i][0]));
                if(QMessageBox::Yes==msg->exec()){
                    CA->execSuperSQL(QString("UPDATE onbattle SET scoreB=0 WHERE userB='%1';").arg(CA->user->getUsername()));
                    onlineState=2;
                    CA->isAggressive=false;
                    CA->opposer=QString(r[i][0]);
                    break;
                }else{
                    CA->execSuperSQL(QString("UPDATE onbattle SET scoreA=-1 WHERE userB='%1';").arg(CA->user->getUsername()));
                }
            }
        }
        timer->start(2*1000);
        break;
    case 1:
        qDebug()<<"online state 1";
        timer->stop();
        r = CA->SearchTop("userA",ui->L_challenge->text(),50);
        for(int i=0;i<r.count();i++){
            if(CA->user->getUsername()==r[i][0] && "-1" == r[i][1].toLatin1()){
                msg->setText(QString("你已经被 %1 拒绝，是否继续尝试？").arg(r[i][2]));
                if(QMessageBox::Yes==msg->exec()){
                    CA->execSuperSQL(QString("UPDATE onbattle SET scoreA=0 WHERE userA='%1';").arg(CA->user->getUsername()));
                    break;
                }else{
                    CA->execSuperSQL(QString("DELETE FROM onbattle WHERE userA='%1';").arg(CA->user->getUsername()));
                    onlineState=0;
                }
            }else if(CA->user->getUsername()==r[i][0] && "0" == r[i][3].toLatin1()){
                onlineState=2;
                CA->isAggressive=true;
                CA->opposer=QString(r[i][2]);
            }
        }
        timer->start(2000);
        break;
    case 2:
        //TODO 对局
        timer->stop();
        qDebug()<<"here 1";
        this->hide();
        qDebug()<<"here 2";
        emit showonbt();
        qDebug()<<"here 3";
        break;
    case -1:
        qDebug()<<"-1 离线状态下仍然运行 INC";
        break;
    default:
        ui->label_rank->setText("出现错误的onlineState");
        break;
    }
}

void RankWindow::restartTimer()
{
    this->show();
    onlineState=0;
    timer->start(2000);

}
