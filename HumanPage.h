#ifndef HUMANPAGE_H
#define HUMANPAGE_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QGroupBox>
#include <QMessageBox>
#include <QTableView>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSplitter>
#include <QToolButton>

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QAbstractItemView>
#include <QSqlQueryModel>

#include "delegate.h"

#define USER_ID 0
#define USER_GUID 1
#define USER_SIGN 2
#define USER_NAME 3
#define USER_PASS 4
#define USER_ROLE 5
#define USER_DATE 6

#define ROLE_ID 0
#define ROLE_GUID 1
#define ROLE_SIGN 2
#define ROLE_NAME 3
#define ROLE_MARK 4

class HumanPage : public QWidget
{
    Q_OBJECT
public:
    explicit HumanPage(QWidget *parent = 0);
    ~HumanPage();

signals:
    void sendJson(QJsonObject obj);
    void updateSql(QString sql);
private slots:
    void initUI();
    void initSql();

    void appendUsers();
    void deleteUsers();
    void changeUsers();
    void appendRoles();
    void deleteRoles();
    void changeRoles();
    void showTabUser();
    void showTabRole();
    void tabUserSync(QModelIndex index);
    void tabRoleSync(QModelIndex index);
    void recvNetJson(QJsonObject obj);
    void recvAppShow(QString win);
private:
    StandardItemModel *m_users;
    StandardItemModel *m_roles;
    SqlQueryModel *sql_users;
    SqlQueryModel *sql_roles;
    QTableView *tab_users;
    QTableView *tab_iuser;
    QTableView *tab_roles;
    QTableView *tab_irole;
    QToolButton *btn_user;
    QToolButton *btn_role;
    QWidget *userWidget;
    QWidget *roleWidget;
    QSqlDatabase db;
    ComboBoxDelegate *role_delegate;

    QStringList user_items;
    QStringList role_items;
};

#endif // HUMANPAGE_H
