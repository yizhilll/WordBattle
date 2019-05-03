#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clientend.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, ClientAccess *CA=nullptr);
    ~MainWindow();

private slots:
    void receivelogin();

signals:
//    void awshow();
    void quit();

private:
    Ui::MainWindow *ui;
    ClientAccess *CA;
};

#endif // MAINWINDOW_H
