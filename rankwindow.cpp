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

}
