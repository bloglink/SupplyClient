/*******************************************************************************
 * Copyright [2017]   <  青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.0.0.1
 * author:      zhaonanlin
 * brief:       供应链管理系统登录界面
*******************************************************************************/
#include "loginscreen.h"

LoginScreen::LoginScreen(QWidget *parent) : QDialog(parent)
{
    initUI();
//    initSql();
    initData();
}

LoginScreen::~LoginScreen()
{

}

void LoginScreen::initUI()
{
    this->setObjectName("loginscreen");
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

    QPushButton *lgn = new QPushButton(this);
    lgn->setText(tr("登录"));
    lgn->setMinimumHeight(35);
    lgn->setFocusPolicy(Qt::NoFocus);
    connect(lgn,SIGNAL(clicked(bool)),this,SLOT(login()));

    QPushButton *cls = new QPushButton(this);
    cls->setText(tr("退出"));
    cls->setMinimumHeight(35);
    cls->setFocusPolicy(Qt::NoFocus);
    connect(cls,SIGNAL(clicked(bool)),this,SLOT(close()));

    QGridLayout *btn = new QGridLayout;
    btn->addWidget(new QLabel(tr("服务器:")),0,0);
    btn->addWidget(new QLabel(tr("端  口:")),1,0);
    btn->addWidget(new QLabel(tr("操作员:")),2,0);
    btn->addWidget(new QLabel(tr("密  码:")),3,0);
    btn->addWidget(svr,0,1);
    btn->addWidget(prt,1,1);
    btn->addWidget(usr,2,1);
    btn->addWidget(pwd,3,1);
    btn->addWidget(lgn,2,2);
    btn->addWidget(cls,3,2);
    btn->setColumnStretch(0,1);
    btn->setColumnStretch(1,2);
    btn->setColumnStretch(2,1);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(btn);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(title);
    layout->addWidget(logo);
    layout->addWidget(group);
    layout->setMargin(0);
    layout->setSpacing(0);
    this->setLayout(layout);

    this->resize(500,360);
}

void LoginScreen::initSql()
{
    QFile file("erp.db");
    if (!file.exists()) {
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("erp.db");
    db.open();
}

void LoginScreen::initData()
{
    ini = new QSettings("conf.ini", QSettings::IniFormat);
    ini->setIniCodec("GB18030");
    ini->beginGroup("LOGIN");
    QStringList items;
    QByteArray byte_svr = "192.168.1.58";
    QByteArray save_svr = ini->value("svr", byte_svr.toBase64()).toByteArray();
    QByteArray byte_prt = "10000";
    QByteArray save_prt = ini->value("prt", byte_prt.toBase64()).toByteArray();
    QByteArray byte_usr = "root";
    QByteArray save_usr = ini->value("usr", byte_usr.toBase64()).toByteArray();
    items = QString(QByteArray::fromBase64(save_svr)).split("@");
    svr->addItems(items);
    items = QString(QByteArray::fromBase64(save_prt)).split("@");
    prt->addItems(items);
    items = QString(QByteArray::fromBase64(save_usr)).split("@");
    usr->addItems(items);
}

void LoginScreen::saveData()
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

void LoginScreen::login()
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

void LoginScreen::loginError()
{
    QMessageBox::warning(this,"",tr("登录超时"));
    QTimer *timer = new QTimer(this);
    timer->singleShot(100,this,SLOT(accept()));
}

void LoginScreen::recvSocket(QUrl url)
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
