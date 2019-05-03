#ifndef RANKWINDOW_H
#define RANKWINDOW_H

#include <QWidget>
#include "clientend.h"
namespace Ui {
class RankWindow;
}

class RankWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RankWindow(QWidget *parent = nullptr, ClientAccess *CA=nullptr);
    ~RankWindow();

    bool showChallengerRank(QString type,QString attribute,int num);
    bool showVocabulorRank(QString type,QString attribute,int num);
private:
    Ui::RankWindow *ui;
    ClientAccess *CA;

private slots:
    void receivelogin();

    void on_Button_logout_clicked();

    void on_Button_switch_clicked();

    void on_BS_u_clicked();

    void on_BS_s_clicked();

    void on_Button_check_clicked();

    void on_BS_l_clicked();

    void on_BS_t_clicked();

signals:
//    void awshow();
    void logout();
    void quit();

};

#endif // RANKWINDOW_H
