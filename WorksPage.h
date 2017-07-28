#ifndef WORKSPAGE_H
#define WORKSPAGE_H

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
#include <QSqlQuery>

#include "delegate.h"

#include "ProdsPage.h"
#include "SalesPage.h"

class WorksPage : public QWidget
{
    Q_OBJECT
public:
    explicit WorksPage(QWidget *parent = 0);
    ~WorksPage();

signals:
    void sendSocket(QUrl url);
    void sendJson(QJsonObject obj);
private slots:
    void initUI();
    void initSql();
    void initData();
    void showTabCust();
    void showTabSale();
    void tabCustSync(QModelIndex index);
    void tabSaleSync(QModelIndex index);
    void appendCust();
    void deleteCust();
    void changeCust();
    void updateCust();
    void appendSale();
    void deleteSale();
    void changeSale();
    void updateSale();
    void recvWorksJson(QJsonObject obj);
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_custs;
    StandardItemModel *m_bills;
    StandardSqlModel *sql_prod;
    StandardSqlModel *sql_bills;
    QTableView *tab_prod;
    QTableView *tab_icust;
    QTableView *tab_bills;
    QTableView *tab_ibill;
    QToolButton *btn_custs;
    QToolButton *btn_bills;
    QWidget *custWidget;
    QWidget *saleWidget;
    QSqlDatabase db;
    ComboBoxDelegate *area_delegate;
    ComboBoxDelegate *sale_delegate;
    QStringList prod_items;
    QStringList bill_items;
};

#endif // WORKSPAGE_H
