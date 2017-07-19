/*******************************************************************************
 * Copyright [2017]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       供应链管理系统登录界面
*******************************************************************************/
#include "LoginPage.h"

LoginPage::LoginPage(QWidget *parent) : QDialog(parent)
{
    initUI();
    initSql();
    initData();
}

LoginPage::~LoginPage()
{

}

void LoginPage::initUI()
{
    //设置界面风格
    QFile file;
    QString qss;
    file.setFileName(":/skins/gn_bu.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    this->setObjectName("LoginPage");
    this->setWindowFlags(Qt::FramelessWindowHint);

    QLabel *icon  = new QLabel(this);
    icon->setPixmap(QPixmap(":/icons/link.ico"));
    icon->setScaledContents(true);
    icon->setMaximumSize(25,25);
    QLabel *text = new QLabel(tr("青岛艾普供应链管理系统"));
    QToolButton *ext = new QToolButton(this);
    ext->setIcon(QIcon(":/icons/close.png"));
    ext->setFocusPolicy(Qt::NoFocus);
    ext->setIconSize(QSize(25,25));
    ext->resize(25,25);
    connect(ext,SIGNAL(clicked(bool)),this,SLOT(close()));

    QHBoxLayout *title = new QHBoxLayout;
    title->addWidget(icon);
    title->addWidget(text);
    title->addStretch();
    title->addWidget(ext);
    title->setMargin(0);
    title->setSpacing(5);

    QLabel *logo = new QLabel(this);
    logo->setPixmap(QPixmap(":/images/aip.png"));
    logo->setScaledContents(true);
    logo->setMaximumSize(480,130);

    svr = new QComboBox(this);
    svr->setEditable(true);
    svr->setMinimumHeight(35);
    svr->setView(new QListView);

    prt = new QComboBox(this);
    prt->setEditable(true);
    prt->setMinimumHeight(35);
    prt->setView(new QListView);

    usr = new QComboBox(this);
    usr->setEditable(true);
    usr->setMinimumHeight(35);
    usr->setView(new QListView);

    pwd = new QLineEdit(this);
    pwd->setMinimumHeight(35);
    pwd->setEchoMode(QLineEdit::Password);

    QPushButton *btn_login = new QPushButton(this);
    btn_login->setText(tr("登录"));
    btn_login->setMinimumHeight(35);
    btn_login->setFocusPolicy(Qt::NoFocus);
    connect(btn_login,SIGNAL(clicked(bool)),this,SLOT(login()));

    QPushButton *btn_close = new QPushButton(this);
    btn_close->setText(tr("退出"));
    btn_close->setMinimumHeight(35);
    btn_close->setFocusPolicy(Qt::NoFocus);
    connect(btn_close,SIGNAL(clicked(bool)),this,SLOT(close()));

    QGridLayout *btnsLayout = new QGridLayout;
    btnsLayout->addWidget(new QLabel(tr("服务器:")),0,0);
    btnsLayout->addWidget(new QLabel(tr("端  口:")),1,0);
    btnsLayout->addWidget(new QLabel(tr("操作员:")),2,0);
    btnsLayout->addWidget(new QLabel(tr("密  码:")),3,0);
    btnsLayout->addWidget(svr,0,1);
    btnsLayout->addWidget(prt,1,1);
    btnsLayout->addWidget(usr,2,1);
    btnsLayout->addWidget(pwd,3,1);
    btnsLayout->addWidget(btn_login,2,2);
    btnsLayout->addWidget(btn_close,3,2);
    btnsLayout->setColumnStretch(0,1);
    btnsLayout->setColumnStretch(1,2);
    btnsLayout->setColumnStretch(2,1);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(btnsLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(title);
    layout->addWidget(logo);
    layout->addWidget(group);
    layout->setMargin(0);
    layout->setSpacing(0);
    this->setLayout(layout);

    this->resize(500,360);
}

void LoginPage::initSql()
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
    cmd += "order_numb text,";
    cmd += "order_date text,";
    cmd += "order_view text,";
    cmd += "order_cust text,";
    cmd += "order_sale text,";
    cmd += "order_area text,";
    cmd += "order_dead text,";
    cmd += "order_quan text,";
    cmd += "order_stck text,";
    cmd += "order_prod text,";
    cmd += "order_lack text,";
    cmd += "order_dnum text)";
    query.exec(cmd);

    cmd = "create table if not exists erp_prods(";
    cmd += "prod_id integer primary key,";
    cmd += "prod_numb text,";
    cmd += "prod_date text,";
    cmd += "prod_view text,";
    cmd += "prod_cust text,";
    cmd += "prod_sale text,";
    cmd += "prod_area text,";
    cmd += "prod_dead text,";
    cmd += "prod_need text,";
    cmd += "prod_quan text,";
    cmd += "prod_pnum text,";
    cmd += "prod_type text,";
    cmd += "prod_code text,";
    cmd += "prod_name text,";
    cmd += "prod_mode text)";
    query.exec(cmd);

    cmd = "create table if not exists erp_purchs(";
    cmd += "purch_id integer primary key,";
    cmd += "purch_numb text,";
    cmd += "purch_code text,";
    cmd += "purch_name text,";
    cmd += "purch_unit text,";
    cmd += "purch_lack text,";
    cmd += "purch_quan text,";
    cmd += "purch_date text,";
    cmd += "purch_bout text,";
    cmd += "purch_expt text,";
    cmd += "purch_real text,";
    cmd += "purch_come text,";
    cmd += "purch_oway text,";
    cmd += "purch_ofix text,";
    cmd += "purch_mark text)";
    query.exec(cmd);
}

void LoginPage::initData()
{
    ini = new QSettings("conf.ini", QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("LOGIN");
    QStringList items;
    QByteArray byte_svr = "192.168.1.58";
    QByteArray save_svr = ini->value("svr", byte_svr.toBase64()).toByteArray();
    QByteArray byte_prt = "10000";
    QByteArray save_prt = ini->value("prt", byte_prt.toBase64()).toByteArray();
    QByteArray byte_usr = "admin";
    QByteArray save_usr = ini->value("usr", byte_usr.toBase64()).toByteArray();
    items = QString(QByteArray::fromBase64(save_svr)).split("@");
    svr->addItems(items);
    items = QString(QByteArray::fromBase64(save_prt)).split("@");
    prt->addItems(items);
    items = QString(QByteArray::fromBase64(save_usr)).split("@");
    usr->addItems(items);
}

void LoginPage::saveData()
{
    QStringList items;
    for (int i=0; i < svr->count(); i++)
        items.append(svr->itemText(i));
    if (!items.contains(svr->currentText()))
        items.append(svr->currentText());
    if (items.size() > SAVE_MAX) //最多存储5条
        items.removeFirst();
    QByteArray save_svr = QString(items.join("@")).toUtf8();
    ini->setValue("svr", save_svr.toBase64());
    items.clear();

    for (int i=0; i < prt->count(); i++)
        items.append(prt->itemText(i));
    if (!items.contains(prt->currentText()))
        items.append(prt->currentText());
    if (items.size() > SAVE_MAX) //最多存储5条
        items.removeFirst();
    QByteArray save_prt = QString(items.join("@")).toUtf8();
    ini->setValue("prt", save_prt.toBase64());
    items.clear();

    for (int i=0; i < usr->count(); i++)
        items.append(usr->itemText(i));
    if (!items.contains(usr->currentText()))
        items.append(usr->currentText());
    if (items.size() > SAVE_MAX) //最多存储5条
        items.removeFirst();
    QByteArray save_usr = QString(items.join("@")).toUtf8();
    ini->setValue("usr", save_usr.toBase64());
    items.clear();
}

void LoginPage::login()
{
    QUrl url;
    url.setUserName(usr->currentText());
    url.setPassword(pwd->text());
    url.setHost(svr->currentText());
    url.setPort(10000);
    url.setQuery("login");
    emit sendSocket(url);

    QTimer *timer = new QTimer(this);
    timer->singleShot(200,this,SLOT(loginError()));

    saveData();
}

void LoginPage::loginError()
{
    QMessageBox::warning(this,"",tr("登录超时"));
    QTimer *timer = new QTimer(this);
    timer->singleShot(100,this,SLOT(accept()));
}

void LoginPage::recvSocket(QUrl url)
{
    QString cmd = url.query();
    QString usr = url.userName();
    if (usr != "loginscreen")
        return;
    if (cmd == "action")
        this->accept();
    if (cmd == "error")
        QMessageBox::warning(this,"",tr("登录失败"));
}
/*********************************END OF FILE**********************************/
