#ifndef RANKWINDOW_H
#define RANKWINDOW_H

#include <QWidget>
#include <QMessageBox>
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

    int onlineState;//记录状态，-1离线，0 上线扫描有无挑战，1 发起挑战等待接受，2进入对战停止扫描
    QTimer *timer;
    QMessageBox *msg;
private:
    Ui::RankWindow *ui;
    ClientAccess *CA;


private slots:
    void receivelogin();
    void receiveshow();

    void on_Button_logout_clicked();

    void on_Button_switch_clicked();

    void on_BS_u_clicked();

    void on_BS_s_clicked();

    void on_Button_check_clicked();

    void on_BS_l_clicked();

    void on_BS_t_clicked();

    void on_Button_game_clicked();

    void on_B_online_check_clicked();

    void on_B_online_clicked();

    void on_B_offline_clicked();

    void on_B_challenge_clicked();

    void timeINC();
    void restartTimer();

signals:
//    void awshow();
    void showvocaboard();
    void logout();
    void quit();
    void showonbt();

};

#endif // RANKWINDOW_H
