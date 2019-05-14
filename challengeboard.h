#ifndef CHALLENGEBOARD_H
#define CHALLENGEBOARD_H

#include <QWidget>

namespace Ui {
class ChallengeBoard;
}

class ChallengeBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChallengeBoard(QWidget *parent = nullptr);
    ~ChallengeBoard();

private:
    Ui::ChallengeBoard *ui;
};

#endif // CHALLENGEBOARD_H
