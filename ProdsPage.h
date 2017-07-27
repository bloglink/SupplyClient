#ifndef PRODSPAGE_H
#define PRODSPAGE_H

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

#include <QSplitter>

#include "delegate.h"

#include "OrderPage.h"

#define PROD_ID 0
#define PROD_GUID 1
#define PROD_SIGN 2
#define PROD_NUMB 3
#define PROD_DATE 4
#define PROD_VIEW 5
#define PROD_CUST 6
#define PROD_SALE 7
#define PROD_AREA 8
#define PROD_DEAD 9
#define PROD_NEED 10
#define PROD_QUAN 11
#define PROD_PNUM 12
#define PROD_TYPE 13
#define PROD_CODE 14
#define PROD_NAME 15
#define PROD_MODE 16
#define PROD_MNUM 17
#define PROD_STCK 18

class ProdsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ProdsPage(QWidget *parent = 0);
    ~ProdsPage();

signals:
    void sendSocket(QUrl url);
    void sendJson(QJsonObject obj);
private slots:
    void initUI();
    void initSql();
    void initData();
    void showTabProd();
    void autoNumber();
    void appendProd();
    void deleteProd();
    void changeProd();
    void updateProd();
    void tabPlanSync(QModelIndex index);
    void tabProdSync(QModelIndex index);
    void matchProds();
    void matchCancel();
    void recvSocket(QUrl url);
    void recvProdsJson(QJsonObject obj);
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_prod;
    StandardSqlModel *sql_plan;
    StandardSqlModel *sql_prod;
    QTableView *tab_plan;
    QTableView *tab_prod;
    QTableView *tab_iprod;
    QWidget *prodWidget;
    QToolButton *btn_prods;
    QSqlDatabase db;
    ComboBoxDelegate *sale_delegate;
    ComboBoxDelegate *area_delegate;
    ComboBoxDelegate *cust_delegate;
    QStringList prod_items;
};

#endif // PRODSPAGE_H
