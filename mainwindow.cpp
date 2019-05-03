#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent, ClientAccess *CA) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->CA=CA;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receivelogin()
{
    this->show();
}
