#ifndef ACCOUNTWINDOW_H
#define ACCOUNTWINDOW_H

#include <QWidget>
#include "clientend.h"

namespace Ui {
class AccountWindow;
}

class AccountWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AccountWindow(QWidget *parent = nullptr, ClientAccess *CA=nullptr);
    ~AccountWindow();

private slots:
    void on_Button_Login_clicked();
    void on_Button_Register_clicked();
    void receiveshow();

    void on_superEXEC_clicked();

signals:
    void showrankwindow();
    void showchanllengeboard();
    void cbsetup();
    void quit();

private:
    Ui::AccountWindow *ui;
    ClientAccess *CA;
    QValidator *vld_pd;
    QValidator *vld_ac;
};

#endif // ACCOUNTWINDOW_H
