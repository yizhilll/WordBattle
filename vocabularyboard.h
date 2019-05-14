#ifndef VOCABULARYBOARD_H
#define VOCABULARYBOARD_H

#include <QWidget>
#include "clientend.h"
namespace Ui {
class VocabularyBoard;
}

class VocabularyBoard : public QWidget
{
    Q_OBJECT

public:
    explicit VocabularyBoard(QWidget *parent = nullptr, ClientAccess *CA=nullptr);
    ~VocabularyBoard();
     QTimer *timer;

private slots:
    void on_B_confirm_clicked();
    void on_input_returnPressed();
    void timeINC();

private:
    Ui::VocabularyBoard *ui;
    ClientAccess *CA;
    int playtime;
    void receiveHideAndStopTimer();

private slots:
    void receiveshow();
    void on_B_search_clicked();
    void on_B_logout_clicked();
    void on_B_switch_clicked();

signals:
    void logout();
    void showsearchwindow();

};

#endif // VOCABULARYBOARD_H
