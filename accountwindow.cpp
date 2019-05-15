#include "accountwindow.h"
#include "ui_accountwindow.h"

AccountWindow::AccountWindow(QWidget *parent,ClientAccess *CA) :
    QWidget(parent),
    ui(new Ui::AccountWindow)
{
    ui->setupUi(this);
    this->CA=CA;

    ui->Text_ac->setToolTip(QString("仅限1-16位字母和数字"));
    ui->Text_pd->setToolTip(QString("任意1-256位字符串"));
    ui->Text_pd->setEchoMode(QLineEdit::Password);

    QRegExp regx_ac("^[0-9|a-z|A-Z|^\s]{1,16}");
    QRegExp regx_pd("^(.)*$");
    ui->Text_ac->setMaxLength(16);
    ui->Text_pd->setMaxLength(256);
    vld_ac = new QRegExpValidator(regx_ac, ui->Text_ac);
    vld_pd = new QRegExpValidator(regx_pd, ui->Text_pd);
    ui->Text_ac->setValidator(vld_ac);
    ui->Text_pd->setValidator(vld_pd);

    ui->superSQL->hide();
    ui->superEXEC->hide();
}

AccountWindow::~AccountWindow()
{
    delete ui;
}

void AccountWindow::on_Button_Login_clicked()
{

    int result_vld = CA->LoginValid(ui->Text_ac->text(),ui->Text_pd->text());
    switch(result_vld){
    case 0:
        ui->Message_tip->setText("找不到该用户名");
//        ui->Message_tip->setText(CA->releaseDebug);
        break;
    case 1:
        ui->Message_tip->setText("密码错误，请重新输入");
        break;
    case 2:
        ui->Message_tip->setText("登录成功，现在进入...");
        this->hide();
        emit showrankwindow();
        break;
    default:
        break;
    }


}

void AccountWindow::on_Button_Register_clicked()
{
    if("superuser"==ui->Text_ac->text() && "sudo"==ui->Text_pd->text() ){
        ui->superSQL->show();
        ui->superEXEC->show();
        return ;
    }
//    qDebug()<<ui->Text_ac->text();
//    qDebug()<<ui->Text_pd->text();
    bool ok=ui->Text_ac->hasAcceptableInput() & ui->Text_pd->hasAcceptableInput();
    if(ok&CA->Register(ui->Text_ac->text(),ui->Text_pd->text())){
        qDebug()<<"Registeration succeed";
        ui->Message_tip->setText("注册成功，请登录");
    }else{
        qDebug()<<"Registeration fail";
        ui->Message_tip->setText("注册失败，用户可能已存在");
    }

}

void AccountWindow::receiveshow()
{
    ui->Message_tip->setText("");
    ui->Text_ac->setText("");
    ui->Text_pd->setText("");
    this->show();
}

void AccountWindow::on_superEXEC_clicked()
{
    QString text=ui->superSQL->toPlainText();
    QStringList magic=text.split("|",QString::KeepEmptyParts,Qt::CaseInsensitive);
    QString response;
    if("loadfile"==magic.at(0)){
        response=CA->loadCSV(magic.at(1));
    }else if("sql"==magic.at(0)){
        response=CA->execSuperSQL(magic.at(1));
    }else{
        response="no such command:\n";
        response.append(text);
    }

    ui->Message_debug->setText(response);
}
