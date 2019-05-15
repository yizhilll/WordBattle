#include "vocabularyboard.h"
#include "ui_vocabularyboard.h"

VocabularyBoard::VocabularyBoard(QWidget *parent, ClientAccess *CA) :
    QWidget(parent),
    ui(new Ui::VocabularyBoard)
{
    ui->setupUi(this);
    this->CA=CA;
    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeINC()));
    this->playtime=0;
}

VocabularyBoard::~VocabularyBoard()
{
    delete ui;
    delete timer;
}

void VocabularyBoard::on_B_confirm_clicked()
{
     QString strIN=ui->input->text();
     if(1==CA->AddWord(strIN)){
         ui->input->setText("");
         ui->tips->setText("添加成功！再接再厉！");
     //TODO: spelling check
     }else{
         ui->tips->setText("该词语已经存在，请再尝试");
     }
}

void VocabularyBoard::on_input_returnPressed()
{
    this->on_B_confirm_clicked();
}
void VocabularyBoard::timeINC()
{
    playtime++;
    ui->l_time->setText(QString("%1 secs").arg(playtime));
}

void VocabularyBoard::receiveshow()
{
    //TODO: SHOW USER INFORMATION
    this->show();
    timer->start(1*1000);
}

void VocabularyBoard::receiveHideAndStopTimer()
{
    timer->stop();
    this->hide();
    CA->AddTime((int)(playtime/60),"vocabulor");
    playtime=0;
    ui->l_time->setText(QString("%1 secs").arg(playtime));
}

void VocabularyBoard::on_B_switch_clicked()
{
    receiveHideAndStopTimer();
    emit showchanllengeboard();
}

void VocabularyBoard::on_B_search_clicked()
{
    receiveHideAndStopTimer();
    emit showsearchwindow();
}

void VocabularyBoard::on_B_logout_clicked()
{
    receiveHideAndStopTimer();
    CA->DestoryUserWLogOut();
    emit logout();
}
