#ifndef WORDBATTLEWINDOW_H
#define WORDBATTLEWINDOW_H

#include <QMainWindow>

namespace Ui {
class WordBattleWindow;
}

class WordBattleWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WordBattleWindow(QWidget *parent = 0);
    ~WordBattleWindow();

private:
    Ui::WordBattleWindow *ui;
};

#endif // WORDBATTLEWINDOW_H
