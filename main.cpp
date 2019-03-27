#include "wordbattlewindow.h"
#include <QApplication>//所有的QT class都在这儿

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WordBattleWindow w;
    w.show();

    return a.exec();
}
