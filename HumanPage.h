#ifndef HRMPAGE_H
#define HRMPAGE_H

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

#define USER_ID 0
#define USER_NAME 1
#define USER_PASSWORD 2
#define USER_ROLE 3
#define USER_DATE 4

#define ROLE_ID 0
#define ROLE_NAME 1
#define ROLE_MARK 2

class HumanPage : public QWidget
{
    Q_OBJECT
public:
    explicit HumanPage(QWidget *parent = 0);

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
    StandardItemModel *m_users;
    StandardItemModel *m_roles;
    StandardSqlModel *sql_users;
    StandardSqlModel *sql_roles;
    QTableView *tab_users;
    QTableView *tab_iuser;
    QTableView *tab_roles;
    QTableView *tab_irole;
    QToolButton *btn_user;
    QToolButton *btn_role;
    QWidget *wiUsers;
    QWidget *wiRoles;
    QSqlDatabase db;
    ComboBoxDelegate *role_delegate;
};

#endif // HRMPAGE_H
