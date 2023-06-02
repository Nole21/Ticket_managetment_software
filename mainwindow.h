#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include "login.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    login* loginscene = new login(this);
    work* workscene = new work(this);
    common* com = new common(this);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
