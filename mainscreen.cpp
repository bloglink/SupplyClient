#include "mainscreen.h"

MainScreen::MainScreen(QWidget *parent)
    : QMainWindow(parent)
{
    initUdp();
    initSql();
    initUI();
    preindex = 0;
}

MainScreen::~MainScreen()
{

}

int MainScreen::login()
{
    LoginPage login;
    connect(&login,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),&login,SLOT(recvSocket(QUrl)));
    return login.exec();
}

void MainScreen::initUI()
{
    //设置界面风格
    QFile file;
    QString qss;
    file.setFileName(":/skins/gn_bu.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    this->setWindowFlags(Qt::FramelessWindowHint);

    QApplication::setOrganizationName("AIP");
    QApplication::setApplicationName("QFramer");
    QApplication::setWindowIcon(QIcon(":/icons/link.ico"));
    QSystemTrayIcon *trayicon = new QSystemTrayIcon(QIcon(QString(":/icons/link.ico")), this);
    trayicon->setToolTip(QString(qApp->applicationName()));
    trayicon->show();

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
    connect(ext,SIGNAL(clicked(bool)),this,SLOT(animationHide()));

    QToolButton *app = new QToolButton(this);
    app->setIcon(QIcon(":/icons/app.png"));
    app->setFocusPolicy(Qt::NoFocus);
    app->setIconSize(QSize(25,25));
    app->resize(25,25);
    connect(app,SIGNAL(clicked(bool)),this,SLOT(swithMaxNormal()));

    QToolButton *min = new QToolButton(this);
    min->setIcon(QIcon(":/icons/minus.png"));
    min->setFocusPolicy(Qt::NoFocus);
    min->setIconSize(QSize(25,25));
    min->resize(25,25);
    connect(min,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));

    QHBoxLayout *title = new QHBoxLayout;
    title->addWidget(icon);
    title->addWidget(text);
    title->addStretch();
    title->addWidget(min);
    title->addWidget(app);
    title->addWidget(ext);
    title->setMargin(0);
    title->setSpacing(5);

    QToolButton *title_order = new QToolButton(this);
    QToolButton *title_prods = new QToolButton(this);
    QToolButton *title_purch = new QToolButton(this);
    QToolButton *title_human = new QToolButton(this);
    QToolButton *title_about = new QToolButton(this);
    QToolButton *title_sales = new QToolButton(this);
    QToolButton *title_power = new QToolButton(this);
    title_about->setObjectName("AboutPage");
    title_human->setObjectName("HumanPage");
    title_sales->setObjectName("SalesPage");
    title_order->setObjectName("OrderPage");
    title_prods->setObjectName("ProdsPage");
    title_purch->setObjectName("PurchPage");

    initToolButton(title_order);
    initToolButton(title_prods);
    initToolButton(title_purch);
    initToolButton(title_human);
    initToolButton(title_power);
    initToolButton(title_about);
    initToolButton(title_sales);

    title_order->setIcon(QIcon(":/icons/note.png"));
    title_order->setText(tr("订单管理"));
    title_prods->setIcon(QIcon(":/icons/industry.png"));
    title_prods->setText(tr("生产管理"));
    title_purch->setIcon(QIcon(":/icons/dollar.png"));
    title_purch->setText(tr("采购管理"));
    title_human->setIcon(QIcon(":/icons/user.png"));
    title_human->setText(tr("人员管理"));
    title_power->setIcon(QIcon(":/icons/stop.png"));
    title_power->setText(tr("权限管理"));
    title_sales->setIcon(QIcon(":/icons/man.png"));
    title_sales->setText(tr("销售管理"));
    title_about->setIcon(QIcon(":/icons/link.ico"));
    title_about->setText(tr("关于软件"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(title_order);
    layout->addWidget(title_prods);
    layout->addWidget(title_purch);
    layout->addWidget(title_human);
    layout->addWidget(title_sales);
    layout->addWidget(title_power);
    layout->addWidget(title_about);
    layout->addStretch();
    layout->setMargin(0);

    stack = new QStackedWidget(this);
    stack->addWidget(new AboutPage(this));

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addLayout(title);
    main_layout->addLayout(layout);
    main_layout->addWidget(stack);
    main_layout->setMargin(0);

    QWidget *desktop = new QWidget(this);
    desktop->setLayout(main_layout);
    setCentralWidget(desktop);
    this->resize(1280,900);

    human = new HumanPage(this);
    connect(human,SIGNAL(sendJson(QJsonObject)),this,SIGNAL(sendJson(QJsonObject)));
    connect(this,SIGNAL(sendMsg(QUrl)),human,SLOT(recvSocket(QUrl)));
    connect(this,SIGNAL(sendCommand(QString)),human,SLOT(recvCommand(QString)));
    stack->addWidget(human);

    sales = new SalesPage(this);
    connect(sales,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),sales,SLOT(recvSocket(QUrl)));
    stack->addWidget(sales);

    order = new OrderPage(this);
    connect(order,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),order,SLOT(recvSocket(QUrl)));
    stack->addWidget(order);

    prods = new ProdsPage(this);
    connect(prods,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),prods,SLOT(recvSocket(QUrl)));
    stack->addWidget(prods);

    purch = new PurchPage(this);
    connect(purch,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),purch,SLOT(recvSocket(QUrl)));
    stack->addWidget(purch);
}

void MainScreen::initUdp()
{
    udp.initSocket(10000);
    connect(this,SIGNAL(sendJson(QJsonObject)),&udp,SLOT(recvAppJson(QJsonObject)));
    connect(&udp,SIGNAL(sendJson(QJsonObject)),this,SLOT(recvNetJson(QJsonObject)));
}

void MainScreen::initSql()
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

    QString cmd = "create table if not exists erp_customs(";
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

    //    query.exec("drop table erp_orders");

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
    cmd += "order_prod text,";
    cmd += "order_stck text,";
    cmd += "order_dnum text)";
    query.exec(cmd);

    //    query.exec("drop table erp_prods");

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
    cmd += "prod_mode text,";
    cmd += "prod_mnum text,";
    cmd += "prod_stck text)";
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

void MainScreen::swithMaxNormal()
{
    if(isMaximized())
        this->showNormal();
    else
        this->showMaximized();
}

void MainScreen::initToolButton(QToolButton *btn)
{
    btn->setFixedSize(90,80);
    btn->setIconSize(QSize(50,60));
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(readToolButton()));
}
void MainScreen::readToolButton()
{
    stackChange(QObject::sender()->objectName().toUtf8());
}

void MainScreen::stackChange(QByteArray win)
{
    for (int i=0; i < stack->count(); i++) {
        if (stack->widget(i)->objectName() == win) {
            if (preindex == i)
                break;
            stack->setCurrentIndex(i);
            cloudAntimation();
            preindex = i;
            break;
        }
    }
}

void MainScreen::animationHide()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    connect(animation, SIGNAL(finished()), this, SLOT(animationClose()));
    animation->setDuration(500);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
}

void MainScreen::animationClose()
{
    this->hide();
    QTimer *timer = new QTimer(this);
    timer->singleShot(50,this,SLOT(close()));
}

void MainScreen::recvSocket(QUrl url)
{
    QString cmd = url.query();
    emit sendMsg(url);
    if (cmd == "action" || cmd == "error") {
        url.setUserName("loginscreen");
        emit sendMsg(url);
    } else if (cmd == "userinfo") {
        url.setUserName("usermanagerment");
        emit sendMsg(url);
    } else if (cmd == "roleinfo") {
        url.setUserName("usermanagerment");
        emit sendMsg(url);
        url.setUserName("rolemanagerment");
        emit sendMsg(url);
    } else if (cmd == "orderinfo") {
        url.setUserName("ordermanagement");
        emit sendMsg(url);
    } else if (cmd == "saleinfo" || cmd == "customerinfo") {
        url.setUserName("ordermanagement");
        emit sendMsg(url);
    } else if (cmd == "pdprepinfo" || cmd == "pdplaninfo") {
        url.setUserName("productionmanagement");
        emit sendMsg(url);
    } else if (cmd == "lackinfo") {
        url.setUserName("lackmanagement");
        emit sendMsg(url);
    } else if (cmd == "buyinfo") {
        url.setUserName("purchasemanagement");
        emit sendMsg(url);
    } else {
        qDebug() << "recv others" << url.toString();
    }
}

void MainScreen::recvNetJson(QJsonObject obj)
{
    QString cmd = obj.value("logs_cmmd").toString();
    if (cmd == "erp_roles")
        roleCommand(obj);
    if (cmd == "erp_users")
        userCommand(obj);
}

void MainScreen::roleCommand(QJsonObject obj)
{
    QSqlQuery query(db);
    qint64 logs_sign = obj.value("logs_sign").toDouble();
    qint64 logs_guid = obj.value("logs_guid").toDouble();
    qint64 tabs_guid = obj.value("tabs_guid").toDouble();

    query.prepare("select count(*) from erp_roles_log where id=:id");
    query.bindValue(":id",logs_guid);
    query.exec();
    query.next();
    if (query.value(0).toInt() > 0)
        return;

    switch (logs_sign) {
    case 0://查询
        logs_guid = tabs_guid;
        if (logs_guid == 0xffffffff) {
            qDebug() << "blank";
            query.prepare("select * from erp_roles_log");
        } else {
            query.prepare("select * from erp_roles_log where id>:id");
            query.bindValue(":id",logs_guid);
        }
        query.exec();
        while (query.next()) {
            QJsonObject sned_obj;
            sned_obj.insert("sendto",obj.value("sender").toString());
            sned_obj.insert("logs_cmmd","erp_roles");
            sned_obj.insert("logs_guid",query.value(0).toDouble());
            sned_obj.insert("logs_sign",query.value(1).toDouble());
            sned_obj.insert("tabs_guid",query.value(2).toDouble());
            sned_obj.insert("role_name",query.value(3).toString());
            sned_obj.insert("role_mark",query.value(4).toString());
            emit sendJson(sned_obj);
            qDebug() << "send" << sned_obj;
        }
        return;
        break;
    case 1://增加
        tabs_guid = logs_guid;
        query.prepare("insert into erp_roles values(?,?,?)");
        query.bindValue(0,tabs_guid);
        query.bindValue(1,obj.value("role_name").toString());
        query.bindValue(2,obj.value("role_mark").toString());
        query.exec();
        break;
    case 2://删除
        query.prepare("delete from erp_roles where id=:id");
        query.bindValue(":id",tabs_guid);
        query.exec();
        break;
    case 3://修改
        query.prepare("update erp_roles set role_name=:1,role_mark=:2 where id=:3");
        query.bindValue(":1",obj.value("role_name").toString());
        query.bindValue(":2",obj.value("role_mark").toString());
        query.bindValue(":3",tabs_guid);
        query.exec();
        break;
    default:
        break;
    }
    query.prepare("insert into erp_roles_log values(?,?,?,?,?)");
    query.bindValue(0,logs_guid);
    query.bindValue(1,logs_sign);
    query.bindValue(2,tabs_guid);
    query.bindValue(3,obj.value("role_name").toString());
    query.bindValue(4,obj.value("role_mark").toString());
    query.exec();
    emit sendCommand("update");
}

void MainScreen::userCommand(QJsonObject obj)
{
    QSqlQuery query(db);
    qint64 logs_sign = obj.value("logs_sign").toDouble();
    qint64 logs_guid = obj.value("logs_guid").toDouble();
    qint64 tabs_guid = obj.value("tabs_guid").toDouble();

    query.prepare("select count(*) from erp_users_log where id=:id");
    query.bindValue(":id",logs_guid);
    query.exec();
    query.next();
    if (query.value(0).toInt() > 0)
        return;
    QString cmd;

    switch (logs_sign) {
    case 0://查询
        logs_guid = tabs_guid;
        if (logs_guid == 0xffffffff) {
            qDebug() << "blank";
            query.prepare("select * from erp_users_log");
        } else {
            query.prepare("select * from erp_users_log where id>:id");
            query.bindValue(":id",logs_guid);
        }
        query.exec();
        while (query.next()) {
            QJsonObject sned_obj;
            sned_obj.insert("sendto",obj.value("sender").toString());
            sned_obj.insert("logs_cmmd","erp_users");
            sned_obj.insert("logs_guid",query.value(0).toDouble());
            sned_obj.insert("logs_sign",query.value(1).toDouble());
            sned_obj.insert("tabs_guid",query.value(2).toDouble());
            sned_obj.insert("user_name",query.value(3).toString());
            sned_obj.insert("user_pass",query.value(4).toString());
            sned_obj.insert("user_role",query.value(5).toString());
            sned_obj.insert("user_date",query.value(6).toString());
            emit sendJson(sned_obj);
        }
        return;
        break;
    case 1://增加
        tabs_guid = logs_guid;
        query.prepare("insert into erp_users values(?,?,?,?,?)");
        query.bindValue(0,tabs_guid);
        query.bindValue(1,obj.value("user_name").toString());
        query.bindValue(2,obj.value("user_pass").toString());
        query.bindValue(3,obj.value("user_role").toString());
        query.bindValue(4,obj.value("user_date").toString());
        query.exec();
        break;
    case 2://删除
        query.prepare("delete from erp_users where id=:id");
        query.bindValue(":id",tabs_guid);
        query.exec();
        break;
    case 3://修改
        cmd += "update erp_users set ";
        cmd += "user_name=:user_name,";
        cmd += "user_pass=:user_pass,";
        cmd += "user_role=:user_role,";
        cmd += "user_date=:user_date ";
        cmd += "where id=:tabs_guid";
        query.prepare(cmd);
        query.bindValue(":user_name",obj.value("user_name").toString());
        query.bindValue(":user_pass",obj.value("user_pass").toString());
        query.bindValue(":user_role",obj.value("user_role").toString());
        query.bindValue(":user_date",obj.value("user_date").toString());
        query.bindValue(":tabs_guid",tabs_guid);
        qDebug() << query.exec() << query.lastError();
        break;
    default:
        break;
    }
    query.prepare("insert into erp_users_log values(?,?,?,?,?,?,?)");
    query.bindValue(0,logs_guid);
    query.bindValue(1,logs_sign);
    query.bindValue(2,tabs_guid);
    query.bindValue(3,obj.value("user_name").toString());
    query.bindValue(4,obj.value("user_pass").toString());
    query.bindValue(5,obj.value("user_role").toString());
    query.bindValue(6,obj.value("user_date").toString());
    query.exec();
    emit sendCommand("update");
}

void MainScreen::cloudAntimation()
{
    QLabel* circle = new QLabel(stack->currentWidget());
    QLabel* line = new QLabel(this);
    line->setObjectName(QString("AntimationLine"));
    line->resize(0, 2);
    line->show();

    circle->setPixmap(stack->widget(preindex)->grab());

    circle->show();
    circle->resize(stack->currentWidget()->size());
    QPropertyAnimation *animation = new QPropertyAnimation(circle, "geometry");

    animation->setDuration(1000);
    animation->setStartValue(circle->geometry());

    QPropertyAnimation* animation_line = new QPropertyAnimation(line, "size");
    animation_line->setDuration(1000);
    animation_line->setEasingCurve(QEasingCurve::OutCubic);

    animation->setEndValue(QRect(-3, 0, 0, circle->height()));
    line->move(stack->x(), stack->y() - 2);
    animation_line->setStartValue(QSize(0, 2));
    animation_line->setEndValue(QSize(stack->width(), 2));

    animation->setEasingCurve(QEasingCurve::OutCubic);

    QPropertyAnimation* animation_opacity = new QPropertyAnimation(circle, "windowOpacity");
    animation_opacity->setDuration(1000);
    animation_opacity->setStartValue(1);
    animation_opacity->setEndValue(0);
    animation_opacity->setEasingCurve(QEasingCurve::OutCubic);

    QParallelAnimationGroup *group = new QParallelAnimationGroup;

    connect(group,SIGNAL(finished()), circle, SLOT(hide()));
    connect(group,SIGNAL(finished()), circle, SLOT(deleteLater()));
    connect(group,SIGNAL(finished()), line, SLOT(deleteLater()));
    connect(group,SIGNAL(finished()), group, SLOT(deleteLater()));
    connect(group,SIGNAL(finished()), animation, SLOT(deleteLater()));
    connect(group,SIGNAL(finished()), animation_opacity, SLOT(deleteLater()));
    connect(group,SIGNAL(finished()), animation_line, SLOT(deleteLater()));
    group->addAnimation(animation);
    group->addAnimation(animation_opacity);
    group->addAnimation(animation_line);
    group->start();
}

void MainScreen::mousePressEvent(QMouseEvent *e)
{
    if(e->button() & Qt::LeftButton) {
        if(e->y() < this->height() and e->x() > this->width() - 120) {
            leftbuttonpressed = false;
        } else {
            dragPosition = e->globalPos() - frameGeometry().topLeft();
            leftbuttonpressed = true;
        }
    }
    e->accept();
}

void MainScreen::mouseReleaseEvent(QMouseEvent *e)
{
    leftbuttonpressed = false;
    e->accept();
}

void MainScreen::mouseMoveEvent(QMouseEvent *e)
{
    if(isMaximized()) {
        e->ignore();
    } else {
        if(e->y() < this->height() and e->x() > this->width() - 120) {
            e->ignore();
        } else {
            if(leftbuttonpressed) {
                move(e->globalPos() - dragPosition);
                e->accept();
            }
        }
    }
}
