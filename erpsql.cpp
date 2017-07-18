#include "erpsql.h"

ErpSql::ErpSql(QObject *parent) : QObject(parent)
{

}

void ErpSql::initSql()
{
    QFile file("erp.db");
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    db = QSqlDatabase::addDatabase("QSQLITE","login");
    db.setDatabaseName("erp.db");
    db.open();

    QSqlQuery query(db);
    QString cmd = "create table if not exists erp_users(";
    cmd += "user_id integer primary key,";
    cmd += "user_name text,";
    cmd += "user_password text,";
    cmd += "user_role text,";
    cmd += "user_date text)";
    query.exec(cmd);

    cmd = "create table if not exists erp_roles(";
    cmd += "role_id integer primary key,";
    cmd += "role_name text,";
    cmd += "role_mark text)";
    query.exec(cmd);

    cmd = "create table if not exists erp_customs(";
    cmd += "custom_id integer primary key,";
    cmd += "custom_name text,";
    cmd += "custom_sale text,";
    cmd += "custom_area text)";
    query.exec(cmd);

    cmd = "create table if not exists erp_sales(";
    cmd += "sale_id integer primary key,";
    cmd += "sale_name text,";
    cmd += "sale_area text)";
    query.exec(cmd);

    cmd = "create table if not exists erp_orders(";
    cmd += "order_id integer primary key,";
    cmd += "order_number text,";
    cmd += "order_custom text,";
    cmd += "order_sale text,";
    cmd += "order_area text,";
    cmd += "order_quantity text,";
    cmd += "order_date text,";
    cmd += "order_stock text,";
    cmd += "order_produce text,";
    cmd += "order_lack text,";
    cmd += "order_delivery text)";
    query.exec(cmd);
}

