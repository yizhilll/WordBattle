#include "wordbattlewindow.h"
#include "ui_wordbattlewindow.h"

WordBattleWindow::WordBattleWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WordBattleWindow)
{
    ui->setupUi(this);
}

WordBattleWindow::~WordBattleWindow()
{
    delete ui;
}
