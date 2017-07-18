#ifndef SALESPAGE_H
#define SALESPAGE_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QGroupBox>
#include <QTableView>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSplitter>
#include <QToolButton>

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QAbstractItemView>

#include "delegate.h"

#define CUSTOM_ID 0
#define CUSTOM_NAME 1
#define CUSTOM_SALE 2
#define CUSTOM_AREA 3

#define SALE_ID 0
#define SALE_NAME 1
#define SALE_AREA 2

class SalesPage : public QWidget
{
    Q_OBJECT
public:
    explicit SalesPage(QWidget *parent = 0);
    ~SalesPage();

signals:
    void sendSocket(QUrl url);
private slots:
    void initUI();
    void initSql();
    void initData();
    void showTabUser();
    void showTabRole();
    void tabUserSync(QModelIndex index);
    void tabRoleSync(QModelIndex index);
    void addUser();
    void delUser();
    void uptUser();
    void sndUser();
    void addRole();
    void delRole();
    void uptRole();
    void sndRole();
    void recvSocket(QUrl url);
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_customs;
    StandardItemModel *m_sales;
    StandardSqlModel *sql_customs;
    StandardSqlModel *sql_sales;
    QTableView *tab_customs;
    QTableView *tab_icustom;
    QTableView *tab_sales;
    QTableView *tab_isale;
    QToolButton *btn_customs;
    QToolButton *btn_sales;
    QWidget *wiCustoms;
    QWidget *wiSales;
    QSqlDatabase db;
    ComboBoxDelegate *area_delegate;
    ComboBoxDelegate *sale_delegate;
};

#endif // SALESPAGE_H
