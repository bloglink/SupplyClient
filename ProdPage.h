#ifndef PRODPAGE_H
#define PRODPAGE_H

#include <QDate>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QGroupBox>
#include <QDateEdit>
#include <QTableView>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QToolButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "delegate.h"

#define ORDER_ID 0
#define ORDER_NUMBER 1
#define ORDER_CUSTOM 2
#define ORDER_SALE 3
#define ORDER_AREA 4
#define ORDER_QUANTITY 5
#define ORDER_DATE 6
#define ORDER_STOCK 7
#define ORDER_PRODUCE 8
#define ORDER_LACK 9
#define ORDER_DELIVERY 10

class ProdPage : public QWidget
{
    Q_OBJECT
public:
    explicit ProdPage(QWidget *parent = 0);
    ~ProdPage();

signals:
    void sendSocket(QUrl url);
private slots:
    void initUI();
    void initSql();
    void initData();
    void showTabOrder();
    void autoNumber();
    void addOrder();
    void delOrder();
    void uptOrder();
    void sndOrder();
    void tabSync(QModelIndex index);
    void recvSocket(QUrl url);

    void showSnd();
    void refresh();
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_show;
    StandardItemModel *m_order;
    StandardItemModel *m_snd;
    StandardSqlModel *sql_order;
    StandardSqlModel *sql_customs;
    StandardSqlModel *sql_sales;

    QTableView *tab_order;
    QTableView *tab_iorder;

    QWidget *wiOrders;

    QToolButton *btn_add;

    QJsonArray json_show;

    QTableView *tab_add;


    QJsonArray json_sale;
    QJsonArray json_customer;
    QDateEdit *s_date;
    QDateEdit *e_date;



    QTableView *tab_snd;
    QToolButton *btn_snd;
    QWidget *w_snd;

    int max_order_id;
    QComboBox *status;

    QSqlDatabase db;

    ComboBoxDelegate *sale_delegate;
    ComboBoxDelegate *area_delegate;
    ComboBoxDelegate *customer;
};

#endif // PRODPAGE_H
