#ifndef PURCHPAGE_H
#define PURCHPAGE_H

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

#include "ProdsPage.h"

class PurchPage : public QWidget
{
    Q_OBJECT
public:
    explicit PurchPage(QWidget *parent = 0);
    ~PurchPage();

signals:
    void sendSocket(QUrl url);
private slots:
    void initUI();
    void initSql();
    void initData();
    void showTabProd();
    void appendPurch();
    void deletePurch();
    void changePurch();
    void updatePurch();
    void tabPurchSync(QModelIndex index);
    void recvSocket(QUrl url);
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_pruch;
    StandardSqlModel *sql_purch;
    QTableView *tab_purch;
    QTableView *tab_ipurch;
    QWidget *pruchWidget;
    QToolButton *btn_purch;
    QSqlDatabase db;
    ComboBoxDelegate *sale_delegate;
    ComboBoxDelegate *area_delegate;
    ComboBoxDelegate *cust_delegate;
    QStringList purch_items;
};

#endif // PURCHPAGE_H
