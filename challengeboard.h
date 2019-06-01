#ifndef CHALLENGEBOARD_H
#define CHALLENGEBOARD_H

#include <QWidget>
#include "clientend.h"
namespace Ui {
class ChallengeBoard;
}

class ChallengeBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChallengeBoard(QWidget *parent = nullptr, ClientAccess *CA=nullptr);
    ~ChallengeBoard();
    QTimer *timer;


private:
    Ui::ChallengeBoard *ui;
    ClientAccess *CA;
    bool isOnlineMode;
    bool isAggressive;
    void refreshStatus();

    int playtime;
    void receiveHideAndStopTimer();//TODO: UPDATE AND SAVE USER INFORMATION
    int howmuchleft;//how much word left for current stage
    int timetohide;//how much time left to hide the current word
    int chanceLeft;//failure tolerance

    QString currentWord;
    void gameControl(int state);//control the game logic

private slots:
    void setup();
    void receiveshow();
    void timeINC();
    void on_B_logout_clicked();//save user info to db
    void on_B_switch_clicked();
    void on_B_search_clicked();
    void on_B_start_clicked();

    void on_B_skip_clicked();

    void on_B_confirm_clicked();

    void on_input_returnPressed();


    void on_tips_skip_clicked();

    void on_B_save_clicked();

signals:
    void logout();
    void showvocabularyboard();
    void showsearchwindow();
};

#endif // CHALLENGEBOARD_H
