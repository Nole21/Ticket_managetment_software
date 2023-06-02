#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // button
    // ***************************************************************************
    QPushButton* management = new QPushButton("Ticket Management",this);
    QPushButton* lookup = new QPushButton("Ticket Lookup",this);
    QLabel* prompt = new QLabel("Welcome to the airplane ticket management system",this);
    int height = this->height();
    int width = this->width();
    management->move(width/2-250,height/2+100);
    lookup->move(width/2+50,height/2+100);
    management->resize(200,50);
    lookup->resize(200,50);
    prompt->resize(700,50);
    prompt->move(60,100);
    QFont ft;
    ft.setPointSize(20);
    prompt->setFont(ft);
    // ***************************************************************************


    connect(management,&QPushButton::clicked,[=](){
        this->loginscene->show();
    });

    connect(lookup,&QPushButton::clicked,[=](){
        this->com->show();
    });
}


MainWindow::~MainWindow()
{
    delete ui;
}

