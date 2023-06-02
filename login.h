#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QPushButton>
#include <QMessageBox>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMenuBar>
#include <QMenu>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QSqlRecord>
#include <QTableView>
#include <QAbstractItemView>
#include <QList>


// 管理员登录界面
class login: public QMainWindow
{
    Q_OBJECT

public: 
    login(QWidget *parent = nullptr);
    bool verify(QString ,QString );
    QPushButton* l = new QPushButton("Login",this);
    QPushButton* exit = new QPushButton("Exit",this);
    QLineEdit* user = new QLineEdit(this);
    QLineEdit* password = new QLineEdit(this);

     QString real_user = "1";
     QString real_pw = "1";


};

// 管理员工作界面
// 提供增删该查服务
class work: public QMainWindow{
    Q_OBJECT

public:
    int days[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    work(QWidget *parent = nullptr);
    void update_days();
    QPushButton* find = new QPushButton("Look Up",this);
    QPushButton* back = new QPushButton("Back",this);
    QLineEdit* flight = new QLineEdit(this);
    QComboBox* year = new QComboBox(this);
    QComboBox* month = new QComboBox(this);
    QComboBox* day = new QComboBox(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    QSqlTableModel* model = new QSqlTableModel(this,db);
    QTableView* view = new QTableView(this);
    QPushButton* all = new QPushButton("all",this);
    QPushButton* add = new QPushButton("add",this);
    QPushButton* del = new QPushButton("delete",this);
    QPushButton* sub = new QPushButton("submit",this);
};

// 普通查询服务界面
class common:public QMainWindow{
    Q_OBJECT
public:
    int days[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    common(QWidget *parent = nullptr);
    void update_days();
    QPushButton* find = new QPushButton("Look Up",this);
    QPushButton* back = new QPushButton("Back",this);
    QLineEdit* flight = new QLineEdit(this);
    QComboBox* year = new QComboBox(this);
    QComboBox* month = new QComboBox(this);
    QComboBox* day = new QComboBox(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    QSqlTableModel* model = new QSqlTableModel(this,db);
    QTableView* view = new QTableView(this);
};

#endif // LOGIN_H
