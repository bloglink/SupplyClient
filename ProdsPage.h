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

#define PROD_UUID 0x00 //生产ID
#define PROD_GUID 0x01 //操作ID
#define PROD_SIGN 0x02 //操作标识
#define PROD_NUMB 0x03 //生产单号
#define PROD_QUAN 0x04 //生产数量
#define PROD_TYPE 0x05 //产品大类
#define PROD_CODE 0x06 //产品编号
#define PROD_NAME 0x07 //产品名称
#define PROD_MODE 0x08 //产品规格
#define PROD_MNUM 0x09 //仪表编号


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
    void updateProds();
    void updateOrder();
    void tabPlanSync(QModelIndex index);
    void tabProdSync(QModelIndex index);
    void matchProds();
    void matchCancel();
    void recvProdsJson(QJsonObject obj);
    void recvOrderJson(QJsonObject obj);
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_prod;
    StandardSqlModel *sql_plan;
    SqlQueryModel *sql_prod;
    QTableView *tab_order;
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
