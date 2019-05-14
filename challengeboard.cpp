#include "challengeboard.h"
#include "ui_challengeboard.h"

ChallengeBoard::ChallengeBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChallengeBoard)
{
    ui->setupUi(this);
}

ChallengeBoard::~ChallengeBoard()
{
    delete ui;
}
