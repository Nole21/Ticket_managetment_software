#include "login.h"
#include "ui_mainwindow.h"


login::login(QWidget *parent)
    : QMainWindow(parent)
{

    this->resize(400,200);
    this->user->resize(200,40);
    this->password->resize(200,40);
    this->user->move(100,10);
    this->password->move(100,60);
    this->user->setPlaceholderText("Input user name");
    this->password->setPlaceholderText("Input password");
    this->password->setEchoMode(QLineEdit::Password);
    this->l->move(80,150);
    this->l->resize(80,30);
    this->exit->move(240,150);
    this->exit->resize(80,30);

    connect(this->l,&QPushButton::clicked,[=](){
        QString usr = this->user->text();
        QString pw = this->password->text();
        if(verify(usr,pw)){
            this->user->clear();
            this->password->clear();
            work* workspace = new work(this);
            workspace->show();
            this->close();
        }
        else{
            QMessageBox::warning(NULL, "warning", "User name or password error", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
    });
    connect(this->exit,&QPushButton::clicked,[=](){
        this->user->clear();
        this->password->clear();
        this->close();
    }); 
}


bool login::verify(QString a, QString b){
    if(a==this->real_user&&b==this->real_pw) return true;
    else return false;
}


work::work(QWidget *parent)
    : QMainWindow(parent)
{
    add->resize(50,20);
    del->resize(50,20);
    sub->resize(50,20);
    add->move(0,0);
    del->move(48,0);
    sub->move(96,0);
    all->resize(50,20);
    all->move(144,0);

    // layout 布局
    this->resize(800,600);
    this->find->resize(100,50);
    this->find->move(600,350);
    this->back->resize(100,50);
    this->back->move(600,460);
    this->flight->resize(300,30);
    this->flight->move(150,360);
    this->year->resize(80,30);
    this->year->move(100,470);
    this->month->resize(80,30);
    this->month->move(255,470);
    this->day->resize(80,30);
    this->day->move(410,470);

    this->flight->setPlaceholderText("Input flight number");
    this->year->setPlaceholderText("year");
    this->month->setPlaceholderText("month");
    this->day->setPlaceholderText("day");

    // button configuration
    for(int i=2000;i<=2023;i++){
        QString y = QString::number(i);
        this->year->addItem(y);
    }
    for(int i=1;i<=12;i++){
        QString s = QString::number(i);
        month->addItem(s);
    }


    // Access the table
    db.setHostName("localhost");
    db.setDatabaseName("flight");
    db.setUserName("sa");
    db.setPassword("123");
    bool ok = db.open();
    if(!ok){
        QMessageBox::warning(NULL, "warning", "Fail to connect the database", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }


    model->setTable("final_flight");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0,Qt::Horizontal,tr("航班号"));
    model->setHeaderData(1,Qt::Horizontal,tr("起点"));
    model->setHeaderData(2,Qt::Horizontal,tr("终点"));
    model->setHeaderData(3,Qt::Horizontal,tr("日期"));
    model->setHeaderData(4,Qt::Horizontal,tr("起飞时刻"));
    model->setHeaderData(5,Qt::Horizontal,tr("到达时刻"));
    model->setHeaderData(6,Qt::Horizontal,tr("剩余座位数"));
    model->setHeaderData(7,Qt::Horizontal,tr("票价"));
    model->setHeaderData(8,Qt::Horizontal,tr("折扣票价"));
    model->setHeaderData(9,Qt::Horizontal,tr("折扣率"));
    model->setHeaderData(10,Qt::Horizontal,tr("航班所属公司"));





    // View the table
    view->setModel(model);
    view->move(0,this->add->height()+1);
    view->resize(800,300);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    for(int i=0;i<11;i++){
        view->setColumnWidth(i,80);
    }
    view->show();


    // connnect slots
    connect(year,&QComboBox::currentTextChanged,[=](){
        update_days();
    });

    connect(month,&QComboBox::currentTextChanged,[=](){
        update_days();
    });
    connect(this->back,&QPushButton::clicked,[=](){
        this->close();
    });
    connect(this->sub,&QPushButton::clicked,[=](){
        model->submitAll();
        model->select();
    });

    // add function
    connect(add,&QPushButton::clicked,[=](){
        // insert an empty row
        model->insertRow(0);
    });


    // delete a certain row
    connect(del,&QPushButton::clicked,[=](){
        QModelIndex index=view->currentIndex();
        qDebug()<<index.row();
        model->removeRow(index.row());
        //model->submit();
    });

    connect(all,&QPushButton::clicked,[=](){
        model->setTable("final_flight");
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();
        view->setModel(model);
        view->move(0,this->add->height()+1);
        view->resize(800,300);
        view->setSelectionMode(QAbstractItemView::SingleSelection);
        for(int i=0;i<11;i++){
            view->setColumnWidth(i,80);
        }
        view->show();
    });


    // look up function
    connect(find,&QPushButton::clicked,[=](){
       QString flight_num = this->flight->text();
       QString y = this->year->currentText();
       QString m = this->month->currentText();
       if(m.toInt()<10) m = '0'+m;
       QString d = this->day->currentText();
       if(d.toInt()<10) d = '0'+d;
       QString date = y+'-'+m+'-'+d;
       if(y==""||m==""||d=="") model->setFilter(QString("航班号='%1'").arg(flight_num));
       else model->setFilter(QString("航班号='%1' and 日期='%2'").arg(flight_num).arg(date));
       if(!model->select()){
            QMessageBox::warning(NULL, "warning", "No relevant info found!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       }
       view->setModel(model);
       view->move(0,this->add->height()+1);
       view->resize(800,300);
       for(int i=0;i<11;i++){
           view->setColumnWidth(i,80);
       }
       view->show();
    });
}

void work::update_days(){
    QString y = this->year->currentText();
    QString m = this->month->currentText();
    int ny = y.toInt();
    int nm = m.toInt();
    this->day->clear();
    for(int i=1;i<=days[nm];i++){
          QString j = QString::number(i);
          this->day->addItem(j);
    }
    if(nm==2){
        if((ny%4==0&&ny%100!=0) || ny%400==0) this->day->addItem("29");
    }
}


common::common(QWidget *parent)
    : QMainWindow(parent)
{
    // layout 布局
    this->resize(800,600);
    this->find->resize(100,50);
    this->find->move(600,350);
    this->back->resize(100,50);
    this->back->move(600,460);
    this->flight->resize(300,30);
    this->flight->move(150,360);
    this->year->resize(80,30);
    this->year->move(100,470);
    this->month->resize(80,30);
    this->month->move(255,470);
    this->day->resize(80,30);
    this->day->move(410,470);

    this->flight->setPlaceholderText("Input flight number");
    this->year->setPlaceholderText("year");
    this->month->setPlaceholderText("month");
    this->day->setPlaceholderText("day");

    // button configuration
    for(int i=2000;i<=2023;i++){
        QString y = QString::number(i);
        this->year->addItem(y);
    }
    for(int i=1;i<=12;i++){
        QString s = QString::number(i);
        month->addItem(s);
    }

    // connect to the database
    // Access the table
    db.setHostName("localhost");
    db.setDatabaseName("flight");
    db.setUserName("sa");
    db.setPassword("123");
    bool ok = db.open();
    if(!ok){
        QMessageBox::warning(NULL, "warning", "Fail to connect the database", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }


    model->setTable("final_flight");
    model->setHeaderData(0,Qt::Horizontal,tr("航班号"));
    model->setHeaderData(1,Qt::Horizontal,tr("起点"));
    model->setHeaderData(2,Qt::Horizontal,tr("终点"));
    model->setHeaderData(3,Qt::Horizontal,tr("日期"));
    model->setHeaderData(4,Qt::Horizontal,tr("起飞时刻"));
    model->setHeaderData(5,Qt::Horizontal,tr("到达时刻"));
    model->setHeaderData(6,Qt::Horizontal,tr("剩余座位数"));
    model->setHeaderData(7,Qt::Horizontal,tr("票价"));
    model->setHeaderData(8,Qt::Horizontal,tr("折扣票价"));
    model->setHeaderData(9,Qt::Horizontal,tr("折扣率"));
    model->setHeaderData(10,Qt::Horizontal,tr("航班所属公司"));


    // View the table
    view->setModel(model);
    view->move(0,0);
    view->resize(800,300);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    for(int i=0;i<11;i++){
        view->setColumnWidth(i,80);
    }

    connect(this->find,&QPushButton::clicked,[=](){
        QString flight_num = this->flight->text();
        QString y = this->year->currentText();
        QString m = this->month->currentText();
        if(m.toInt()<10) m = '0'+m;
        QString d = this->day->currentText();
        if(d.toInt()<10) d = '0'+d;
        QString date = y+'-'+m+'-'+d;
        if(y==""||m==""||d=="") model->setFilter(QString("航班号='%1'").arg(flight_num));
        else model->setFilter(QString("航班号='%1' and 日期='%2'").arg(flight_num).arg(date));
        if(!model->select()){
             QMessageBox::warning(NULL, "warning", "No relevant info found!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
        view->show();
     });

    connect(back,&QPushButton::clicked,[=](){
        view->close();
        this->close();
    });

    // connnect slots
    connect(year,&QComboBox::currentTextChanged,[=](){
        update_days();
    });

    connect(month,&QComboBox::currentTextChanged,[=](){
        update_days();
    });
}

void common::update_days(){
    QString y = this->year->currentText();
    QString m = this->month->currentText();
    int ny = y.toInt();
    int nm = m.toInt();
    this->day->clear();
    for(int i=1;i<=days[nm];i++){
          QString j = QString::number(i);
          this->day->addItem(j);
    }
    if(nm==2){
        if((ny%4==0&&ny%100!=0) || ny%400==0) this->day->addItem("29");
    }

}






















