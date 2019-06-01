#ifndef ONLINE_H
#define ONLINE_H

#include <QWidget>
#include <clientend.h>
#include <QMessageBox>
namespace Ui {
class online;
}

class online : public QWidget
{
    Q_OBJECT

public:
    explicit online(QWidget *parent = nullptr,ClientAccess *c=nullptr);
    ~online();

    ClientAccess *CA;
    void refreshStatus();

    int playtime;//单词用时
    int timetohide;//how much time left to hide the current word
    int chanceLeft;//failure tolerance

    QList<QList<QString>> r;
    QString currentWord;
//    void gameControl(int state);//control the game logic
    int gameState;
    int onlineState;
    QTimer *timer;
    int currentround;

    void judge(int timeA,int timeB);
    void endGame();

private slots:
    void timeINC();
    void receiveshow();
    void on_B_skip_clicked();

    void on_B_confirm_clicked();

    void on_B_start_clicked();

    void on_input_returnPressed();

signals:
    void endGameAndBack();
private:
    Ui::online *ui;
};

#endif // ONLINE_H
