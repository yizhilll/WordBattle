#include "challengeboard.h"
#include "ui_challengeboard.h"

ChallengeBoard::ChallengeBoard(QWidget *parent, ClientAccess *CA) :
    QWidget(parent),
    ui(new Ui::ChallengeBoard)
{
    ui->setupUi(this);
    this->CA=CA;
    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeINC()));
    this->playtime=0;
    this->currentWord=QString("Hilarious");
    ui->input->hide();
    ui->tips->hide();
    ui->B_confirm->hide();
    ui->B_skip->hide();
    ui->l_word->hide();
    ui->tips_skip->hide();
    ui->l_worddis->hide();
    isOnlineMode=false;
    isAggressive=false;
}


ChallengeBoard::~ChallengeBoard()
{
    delete ui;
}

void ChallengeBoard::setup(void)
{
    this->howmuchleft=CA->user->ch->nStageWord();
    this->timetohide=CA->user->ch->WordShowTime(-1);
    this->chanceLeft=3;
}

void ChallengeBoard::refreshStatus()
{
    ui->l_hi->setText(QString("%1，你好！").arg(CA->user->getUsername()));
    ui->l_stage->setText(QString("你已经闯到第%1关,还差%2个词过关").arg(CA->user->ch->Stage()).arg(howmuchleft));
    ui->l_level->setText(QString("你的闯关者等级为%1级").arg(CA->user->ch->Level()));
    ui->l_exp->setText(QString("目前经验进度为：%1/%2").arg(CA->user->ch->Exp()).arg(CA->user->ch->expToNext()));
}

void ChallengeBoard::gameControl(int state)
{
    //start from stretch
    switch (state) {
    case 1://initialization
        timer->start(1*1000);   
    case 2://grab a new word
        this->currentWord = CA->GetOneWord(CA->user->ch->wordLength(),"1");//fetch new word
        this->timetohide = CA->user->ch->WordShowTime(-1);//reset the countdown
        ui->l_word->setText(currentWord);
        ui->l_worddis->setText(QString("%1").arg(timetohide));
        ui->l_word->show();
        ui->l_worddis->show();
        ui->tips_skip->show();
        break;
    case 3://check showtime count down expire
        if(timetohide>0){
            --timetohide;
            ui->l_worddis->setText(QString("%1").arg(timetohide));
            break;
        }//if count down end then display
    case 4://then to wait for answer
        if(ui->input->isHidden()){
            ui->l_word->hide();
            ui->tips_skip->hide();
            ui->l_worddis->hide();
            ui->input->setText("");
            ui->tips->setText("请输入单词后按回车键或确认来完成猜题");
            ui->input->show();
            ui->tips->show();
            ui->B_confirm->show();
            ui->B_skip->show();
        }
        break;
    case 5://check the answer (signal from confirm)
        if(ui->input->text().toUpper()==currentWord.toUpper()){
            howmuchleft--;
            chanceLeft=3;
            ui->input->hide();
            ui->tips->hide();
            ui->B_confirm->hide();
            ui->B_skip->hide();
            //TODO: if online case?
            CA->user->ch->addExp(currentWord,"local");

            if(howmuchleft==0){
                CA->user->ch->stageUp(); //go to next stage
                howmuchleft=CA->user->ch->nStageWord(); //RESET HOWMUCHLEFT
            }
            gameControl(2);//start a new word
        }else{
            chanceLeft--;
            if(chanceLeft>0){
                ui->tips->setText(QString("记错啦，还有%1次机会！").arg(chanceLeft));
            }else{
                ui->tips->setText(QString("本关卡重置"));
                howmuchleft=CA->user->ch->nStageWord(); //RESET HOWMUCHLEFT
                ui->input->hide();
                ui->tips->hide();
                ui->B_confirm->hide();
                ui->B_skip->hide();
                gameControl(2);//start a new word
            }
        }
        break;
    case 6://skip to next word
        ui->input->hide();
        ui->tips->hide();
        ui->B_confirm->hide();
        ui->B_skip->hide();
        gameControl(2);
        break;
    case 10://stop game mode
        timer->stop();
        ui->input->hide();
        ui->tips->hide();
        ui->B_confirm->hide();
        ui->B_skip->hide();
        ui->l_word->hide();
        ui->tips_skip->hide();
        ui->l_worddis->hide();
        break;
    default:
        break;
    }
}

void ChallengeBoard::timeINC()
{
    playtime++;
    gameControl(3);
    ui->l_time->setText(QString("%1 secs").arg(playtime));
}
void ChallengeBoard::receiveHideAndStopTimer()
{
    timer->stop();
    this->hide();
    ui->B_start->setText("开始");
    CA->AddTime((int)(playtime/60),"vocabulor");
    playtime=0;
    ui->l_time->setText(QString("%1 secs").arg(playtime));
    CA->SaveUserToDatabase();
}
void ChallengeBoard::receiveshow()
{
    //TODO: SHOW USER INFORMATION
    this->show();
    refreshStatus();
}


void ChallengeBoard::on_B_logout_clicked()
{
    receiveHideAndStopTimer();
    CA->DestoryUserWLogOut();
    emit logout();
}

void ChallengeBoard::on_B_switch_clicked()
{
    receiveHideAndStopTimer();
    emit showvocabularyboard();
}

void ChallengeBoard::on_B_search_clicked()
{
    receiveHideAndStopTimer();
    emit showsearchwindow();

}


void ChallengeBoard::on_B_start_clicked()
{
    if("开始"==ui->B_start->text()){
        ui->B_start->setText("暂停");
        gameControl(1);

    }else if("暂停"==ui->B_start->text()){
        CA->SaveUserToDatabase();
        ui->B_start->setText("开始");
        gameControl(10);
    }
}

void ChallengeBoard::on_B_skip_clicked()
{
    gameControl(6);
}

void ChallengeBoard::on_B_confirm_clicked()
{
    gameControl(5);
    refreshStatus();
}

void ChallengeBoard::on_input_returnPressed()
{
    on_B_confirm_clicked();
}

void ChallengeBoard::on_tips_skip_clicked()
{
    timetohide=1;
}

void ChallengeBoard::on_B_save_clicked()
{
    CA->SaveUserToDatabase();
}
