#ifndef ERPSQL_H
#define ERPSQL_H

#include <QFile>
#include <QDebug>
#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>

class ErpSql : public QObject
{
    Q_OBJECT
public:
    explicit ErpSql(QObject *parent = 0);

public:
    void initSql();
private:
    QSqlDatabase db;
};

#endif // ERPSQL_H
