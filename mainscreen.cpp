#include "mainscreen.h"

MainScreen::MainScreen(QWidget *parent)
    : QMainWindow(parent)
{
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
    connect(&login,SIGNAL(sendJson(QJsonObject)),this,SLOT(initUdp(QJsonObject)));
    connect(this,SIGNAL(loginJson(QJsonObject)),&login,SLOT(recvLoginJson(QJsonObject)));
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
    title_sales->setText(tr("客户档案"));
    title_about->setIcon(QIcon(":/icons/link.ico"));
    title_about->setText(tr("关于软件"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(title_order);
    layout->addWidget(title_prods);
    layout->addWidget(title_purch);
    layout->addWidget(title_human);
    layout->addWidget(title_power);
    layout->addWidget(title_sales);
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
    connect(this,SIGNAL(rolesJson(QJsonObject)),human,SLOT(recvRolesJson(QJsonObject)));
    connect(this,SIGNAL(usersJson(QJsonObject)),human,SLOT(recvUsersJson(QJsonObject)));
    stack->addWidget(human);

    sales = new SalesPage(this);
    connect(sales,SIGNAL(sendJson(QJsonObject)),this,SIGNAL(sendJson(QJsonObject)));
    connect(this,SIGNAL(salesJson(QJsonObject)),sales,SLOT(recvSalesJson(QJsonObject)));
    connect(this,SIGNAL(custsJson(QJsonObject)),sales,SLOT(recvCustsJson(QJsonObject)));
    stack->addWidget(sales);

    order = new OrderPage(this);
    connect(order,SIGNAL(sendJson(QJsonObject)),this,SIGNAL(sendJson(QJsonObject)));
    connect(this,SIGNAL(orderJson(QJsonObject)),order,SLOT(recvOrderJson(QJsonObject)));
    stack->addWidget(order);

    prods = new ProdsPage(this);
    connect(prods,SIGNAL(sendJson(QJsonObject)),this,SIGNAL(sendJson(QJsonObject)));
    connect(this,SIGNAL(prodsJson(QJsonObject)),prods,SLOT(recvProdsJson(QJsonObject)));
    stack->addWidget(prods);

    purch = new PurchPage(this);
    connect(purch,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),purch,SLOT(recvSocket(QUrl)));
    stack->addWidget(purch);
}

void MainScreen::initUdp(QJsonObject obj)
{
    QString host = obj.value("host_addr").toString();
    quint16 port = obj.value("host_port").toString().toInt();
    udp.initSocket(host,port);
    connect(this,SIGNAL(sendJson(QJsonObject)),&udp,SLOT(recvAppJson(QJsonObject)));
    connect(&udp,SIGNAL(sendJson(QJsonObject)),this,SLOT(recvNetJson(QJsonObject)));
    emit sendJson(obj);
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
    QString cmd;

    query.exec("drop table erp_roles");
    query.exec("drop table erp_roles_log");

    cmd = "create table if not exists erp_roles(";//创建角色表
    cmd += "id integer primary key,";
    cmd += "logs_guid interger,";
    cmd += "logs_sign interger,";
    cmd += "role_name text,";//角色名称
    cmd += "role_mark text)";//角色备注
    query.exec(cmd);

    cmd = "create table if not exists erp_roles_log(";//创建角色日志表
    cmd += "id integer primary key,";
    cmd += "logs_sign integer,";//操作标识
    cmd += "tabs_guid integer,";//角色ID
    cmd += "role_name text,";
    cmd += "role_mark text)";
    query.exec(cmd);

    query.exec("drop table erp_users");
    query.exec("drop table erp_users_log");

    cmd = "create table if not exists erp_users(";//创建用户表
    cmd += "id integer primary key,";
    cmd += "logs_guid interger,";
    cmd += "logs_sign interger,";
    cmd += "user_name text,";//用户名称
    cmd += "user_pass text,";//用户密码
    cmd += "user_role text,";//用户角色
    cmd += "user_date text)";//加入日期
    query.exec(cmd);

    cmd = "create table if not exists erp_users_log(";//创建用户日志表
    cmd += "id integer primary key,";
    cmd += "logs_sign integer,";
    cmd += "tabs_guid integer,";
    cmd += "user_name text,";
    cmd += "user_pass text,";
    cmd += "user_role text,";
    cmd += "user_date text)";
    query.exec(cmd);

    query.exec("drop table erp_sales");
    query.exec("drop table erp_sales_log");

    cmd = "create table if not exists erp_sales(";
    cmd += "id integer primary key,";
    cmd += "logs_guid interger,";
    cmd += "logs_sign interger,";
    cmd += "sale_name text,";//销售名称
    cmd += "sale_area text)";//所属区域
    query.exec(cmd);

    cmd = "create table if not exists erp_sales_log(";
    cmd += "id integer primary key,";
    cmd += "logs_sign integer,";
    cmd += "tabs_guid integer,";
    cmd += "sale_name text,";
    cmd += "sale_area text)";
    query.exec(cmd);

    query.exec("drop table erp_custs");
    query.exec("drop table erp_custs_log");

    cmd = "create table if not exists erp_custs(";
    cmd += "id integer primary key,";
    cmd += "logs_guid interger,";
    cmd += "logs_sign interger,";
    cmd += "cust_name text,";//客户名称
    cmd += "cust_sale text,";//销售名称
    cmd += "cust_area text)";//所属区域
    query.exec(cmd);

    cmd = "create table if not exists erp_custs_log(";
    cmd += "id integer primary key,";
    cmd += "logs_sign integer,";
    cmd += "tabs_guid integer,";
    cmd += "cust_name text,";
    cmd += "cust_sale text,";
    cmd += "cust_area text)";
    query.exec(cmd);

    query.exec("drop table erp_orders");
    query.exec("drop table erp_orders_log");

    cmd = "create table if not exists erp_orders(";
    cmd += "id integer primary key,";
    cmd += "logs_guid interger,";
    cmd += "logs_sign interger,";
    cmd += "order_numb text,";//订单编号
    cmd += "order_date text,";//订单日期
    cmd += "order_view text,";//评审编号
    cmd += "order_cust text,";//客户名称
    cmd += "order_sale text,";//销售经理
    cmd += "order_area text,";//所属区域
    cmd += "order_dead text,";//交货日期
    cmd += "order_quan text,";//订货数量
    cmd += "order_prod text,";//在产数量
    cmd += "order_stck text,";//入库数量
    cmd += "order_dnum text)";//发货数量
    query.exec(cmd);

    cmd = "create table if not exists erp_orders_log(";
    cmd += "id integer primary key,";
    cmd += "logs_sign integer,";
    cmd += "tabs_guid integer,";
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

    query.exec("drop table erp_prods");
    query.exec("drop table erp_prods_log");

    cmd = "create table if not exists erp_prods(";
    cmd += "id integer primary key,";
    cmd += "logs_guid interger,";
    cmd += "logs_sign interger,";
    cmd += "prod_numb text,";//订单编号
    cmd += "prod_date text,";//订单日期
    cmd += "prod_view text,";//评审编号
    cmd += "prod_cust text,";//客户名称
    cmd += "prod_sale text,";//销售名称
    cmd += "prod_area text,";//所属区域
    cmd += "prod_dead text,";//交货日期
    cmd += "prod_need text,";//订货数量
    cmd += "prod_quan text,";//在产数量
    cmd += "prod_pnum text,";//生产单号
    cmd += "prod_type text,";//产品大类
    cmd += "prod_code text,";//产品编号
    cmd += "prod_name text,";//产品名称
    cmd += "prod_mode text,";//产品规格
    cmd += "prod_mnum text,";//仪表编号
    cmd += "prod_stck text)";//入库标志
    query.exec(cmd);

    cmd = "create table if not exists erp_prods_log(";
    cmd += "id integer primary key,";
    cmd += "logs_sign integer,";
    cmd += "tabs_guid integer,";
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
    cmd += "id integer primary key,";
    cmd += "logs_guid interger,";
    cmd += "logs_sign interger,";
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

    cmd = "create table if not exists erp_purchs_log(";
    cmd += "id integer primary key,";
    cmd += "logs_sign integer,";
    cmd += "tabs_guid integer,";;
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
    if (cmd == "erp_login")
        emit loginJson(obj);
    if (cmd == "erp_roles")
        emit rolesJson(obj);
    if (cmd == "erp_users")
        emit usersJson(obj);
    if (cmd == "erp_sales")
        emit salesJson(obj);
    if (cmd == "erp_custs")
        emit custsJson(obj);
    if (cmd == "erp_orders")
        emit orderJson(obj);
    if (cmd == "erp_prods")
        emit prodsJson(obj);
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
