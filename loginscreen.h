#ifndef CLOGIN_H
#define CLOGIN_H

#include <QUrl>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QDialog>
#include <QMenu>
#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QToolButton>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QSizePolicy>

#include <QLineEdit>
#include <QAbstractItemView>
#include <QListView>
#include <QMessageBox>
#include <QSettings>
#include <QSqlDatabase>

#define SAVE_MAX 5

class LoginScreen : public QDialog
{
    Q_OBJECT
public:
    explicit LoginScreen(QWidget *parent = 0);
    ~LoginScreen();

signals:
    void sendSocket(QUrl url);
private slots:
    void initUI();
    void initSql();
    void initData();
    void saveData();
    void login();
    void loginError();
    void recvSocket(QUrl url);
private:
    QSqlDatabase db;
    QComboBox *svr;
    QComboBox *prt;
    QComboBox *usr;
    QLineEdit *pwd;
    QSettings *ini;
};

#endif // CLOGIN_H
