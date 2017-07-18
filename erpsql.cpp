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
    db = QSqlDatabase::addDatabase("QSQLITE");
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
}

