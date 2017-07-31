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
    QApplication::setApplicationName("供应链管理");
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
    QToolButton *title_works = new QToolButton(this);
    title_about->setObjectName("AboutPage");
    title_human->setObjectName("HumanPage");
    title_sales->setObjectName("SalesPage");
    title_order->setObjectName("OrderPage");
    title_prods->setObjectName("ProdsPage");
    title_purch->setObjectName("PurchPage");
    title_works->setObjectName("WorksPage");

    initToolButton(title_order);
    initToolButton(title_prods);
    initToolButton(title_purch);
    initToolButton(title_human);
    initToolButton(title_power);
    initToolButton(title_about);
    initToolButton(title_sales);
    initToolButton(title_works);

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
    title_works->setIcon(QIcon(":/icons/shop.png"));
    title_works->setText(tr("出料管理"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(title_order);
    layout->addWidget(title_prods);
    layout->addWidget(title_works);
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
    connect(this,SIGNAL(transmitJson(QJsonObject)),human,SLOT(recvNetJson(QJsonObject)));
    stack->addWidget(human);

    sales = new SalesPage(this);
    connect(sales,SIGNAL(sendJson(QJsonObject)),this,SIGNAL(sendJson(QJsonObject)));
    connect(this,SIGNAL(transmitJson(QJsonObject)),sales,SLOT(recvNetJson(QJsonObject)));
    stack->addWidget(sales);

    order = new OrderPage(this);
    connect(order,SIGNAL(sendJson(QJsonObject)),this,SIGNAL(sendJson(QJsonObject)));
    connect(this,SIGNAL(orderJson(QJsonObject)),order,SLOT(recvOrderJson(QJsonObject)));
    connect(this,SIGNAL(sendsJson(QJsonObject)),order,SLOT(recvSendsJson(QJsonObject)));
    stack->addWidget(order);

    prods = new ProdsPage(this);
    connect(prods,SIGNAL(sendJson(QJsonObject)),this,SIGNAL(sendJson(QJsonObject)));
    connect(this,SIGNAL(prodsJson(QJsonObject)),prods,SLOT(recvProdsJson(QJsonObject)));
    connect(this,SIGNAL(orderJson(QJsonObject)),prods,SLOT(recvOrderJson(QJsonObject)));
    stack->addWidget(prods);

    works = new WorksPage(this);
    connect(works,SIGNAL(sendJson(QJsonObject)),this,SIGNAL(sendJson(QJsonObject)));
    connect(this,SIGNAL(worksJson(QJsonObject)),works,SLOT(recvWorksJson(QJsonObject)));
    stack->addWidget(works);

    //    purch = new PurchPage(this);
    //    connect(purch,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    //    connect(this,SIGNAL(sendMsg(QUrl)),purch,SLOT(recvSocket(QUrl)));
    //    stack->addWidget(purch);
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
        qDebug() << "erp.db not exist";
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("erp.db");
    if (!db.open())
        qDebug() << "erp.db open fail";

    createTabRoles();
    createTabUsers();
    createTabSales();
    createTabCusts();
    createTabOrder();
    createTabSends();
    createTabPlans();
    createTabProds();
    createTabPurch();

    //    QSqlQuery query(db);
    //    QString cmd;



    //    query.exec("drop table erp_sales");
    //    query.exec("drop table erp_sales_log");

    //    cmd = "create table if not exists erp_sales(";//创建销售表
    //    cmd += "id integer primary key,";
    //    cmd += "logs_guid interger,";
    //    cmd += "logs_sign interger,";
    //    cmd += "sale_name text,";//销售名称
    //    cmd += "sale_area text)";//所属区域
    //    query.exec(cmd);

    //    query.exec("drop table erp_custs");
    //    query.exec("drop table erp_custs_log");

    //    cmd = "create table if not exists erp_custs(";//创建客户表
    //    cmd += "id integer primary key,";
    //    cmd += "logs_guid interger,";
    //    cmd += "logs_sign interger,";
    //    cmd += "cust_name text,";//客户名称
    //    cmd += "cust_sale text,";//销售名称
    //    cmd += "cust_area text)";//所属区域
    //    query.exec(cmd);

    //    query.exec("drop table erp_order");
    //    query.exec("drop table erp_order_log");

    //    cmd = "create table if not exists erp_order(";//创建订单表
    //    cmd += "id integer primary key,";
    //    cmd += "logs_guid interger,";
    //    cmd += "logs_sign interger,";
    //    cmd += "order_numb text,";//订单编号
    //    cmd += "order_date text,";//订单日期
    //    cmd += "order_area text,";//所属区域
    //    cmd += "order_sale text,";//销售经理
    //    cmd += "order_cust text,";//客户名称
    //    cmd += "order_view text,";//评审编号
    //    cmd += "order_quan text,";//订货数量
    //    cmd += "order_dead text,";//交货日期
    //    cmd += "order_mark text,";//备注内容
    //    cmd += "order_prod text,";//在产数量
    //    cmd += "order_stck text,";//入库数量
    //    cmd += "order_lnum text,";//未发数量
    //    cmd += "order_dnum text)";//发货数量
    //    query.exec(cmd);

    //    query.exec("drop table erp_sends");
    //    query.exec("drop table erp_sends_log");

    //    cmd = "create table if not exists erp_sends(";//创建发货表
    //    cmd += "id integer primary key,";
    //    cmd += "logs_guid interger,";
    //    cmd += "logs_sign interger,";
    //    cmd += "send_numb text,";//订单单号
    //    cmd += "send_view text,";//评审单号
    //    cmd += "send_mode text,";//发货方式
    //    cmd += "send_code text,";//货运单号
    //    cmd += "send_prce text,";//运费
    //    cmd += "send_mark text)";//货运单号
    //    query.exec(cmd);

    //    query.exec("drop table erp_prod");
    //    query.exec("drop table erp_prods");
    //    query.exec("drop table erp_prods_log");

    //    cmd = "create table if not exists erp_prod(";//创建生产表
    //    cmd += "prod_uuid integer primary key,";//生产ID
    //    cmd += "prod_guid interger,";//操作ID
    //    cmd += "prod_sign interger,";//操作标识
    //    cmd += "prod_numb text,";//生产单号
    //    cmd += "prod_quan text,";//生产数量
    //    cmd += "prod_type text,";//产品大类
    //    cmd += "prod_code text,";//产品编号
    //    cmd += "prod_name text,";//产品名称
    //    cmd += "prod_mode text,";//产品规格
    //    cmd += "prod_mnum text)";//仪表编号

    //    cmd = "create table if not exists erp_prods(";//创建生产详细表
    //    cmd += "prods_uuid integer primary key,";//生产详细ID
    //    cmd += "prods_guid interger,";//操作ID
    //    cmd += "prods_sign interger,";//操作标识
    //    cmd += "prods_puid interger";//生产ID
    //    cmd += "prods_ouid interger";//订单ID
    //    cmd += "prods_mnum text)";//仪表编号
    //    query.exec(cmd);

    //    query.exec("drop table erp_bills");
    //    query.exec("drop table erp_bills_log");

    //    cmd = "create table if not exists erp_bills(";
    //    cmd += "id integer primary key,";
    //    cmd += "logs_guid interger,";
    //    cmd += "logs_sign interger,";
    //    cmd += "bill_numb text,";//物料编号
    //    cmd += "bill_name text,";//物料名称
    //    cmd += "bill_type text,";//物料规格
    //    cmd += "bill_unit text,";//单位
    //    cmd += "bill_quan text,";//数量
    //    cmd += "bill_mark text)";//备注
    //    query.exec(cmd);

    //    cmd = "create table if not exists erp_bills_log(";
    //    cmd += "id integer primary key,";
    //    cmd += "logs_sign integer,";
    //    cmd += "tabs_guid integer,";
    //    cmd += "bill_numb text,";//物料编号
    //    cmd += "bill_name text,";//物料名称
    //    cmd += "bill_type text,";//物料规格
    //    cmd += "bill_unit text,";//单位
    //    cmd += "bill_quan text,";//数量
    //    cmd += "bill_mark text)";//备注
    //    query.exec(cmd);

    //    cmd = "create table if not exists erp_purch(";
    //    cmd += "id integer primary key,";
    //    cmd += "logs_guid interger,";
    //    cmd += "logs_sign interger,";
    //    cmd += "purch_numb text,";
    //    cmd += "purch_code text,";
    //    cmd += "purch_name text,";
    //    cmd += "purch_unit text,";
    //    cmd += "purch_lack text,";
    //    cmd += "purch_quan text,";
    //    cmd += "purch_date text,";
    //    cmd += "purch_bout text,";
    //    cmd += "purch_expt text,";
    //    cmd += "purch_real text,";
    //    cmd += "purch_come text,";
    //    cmd += "purch_oway text,";
    //    cmd += "purch_ofix text,";
    //    cmd += "purch_mark text)";
    //    query.exec(cmd);

    //    cmd = "create table if not exists erp_purch_log(";
    //    cmd += "id integer primary key,";
    //    cmd += "logs_sign integer,";
    //    cmd += "tabs_guid integer,";;
    //    cmd += "purch_numb text,";
    //    cmd += "purch_code text,";
    //    cmd += "purch_name text,";
    //    cmd += "purch_unit text,";
    //    cmd += "purch_lack text,";
    //    cmd += "purch_quan text,";
    //    cmd += "purch_date text,";
    //    cmd += "purch_bout text,";
    //    cmd += "purch_expt text,";
    //    cmd += "purch_real text,";
    //    cmd += "purch_come text,";
    //    cmd += "purch_oway text,";
    //    cmd += "purch_ofix text,";
    //    cmd += "purch_mark text)";
    //    query.exec(cmd);

    //    query.clear();
}

void MainScreen::recvNetJson(QJsonObject obj)
{
    QString cmd = obj.value("logs_cmmd").toString();
    if (cmd == "erp_login")
        emit loginJson(obj);
    if (cmd == "erp_roles")
        excuteCmdRoles(obj);
    if (cmd == "erp_users")
        excuteCmdUsers(obj);
    if (cmd == "erp_sales")
        excuteCmdSales(obj);
    if (cmd == "erp_custs")
        excuteCmdCusts(obj);
    if (cmd == "erp_order")
        emit orderJson(obj);
    if (cmd == "erp_sends")
        emit sendsJson(obj);
    if (cmd == "erp_prods")
        emit prodsJson(obj);

    emit transmitJson(obj);
}

void MainScreen::createTabRoles()
{
    QSqlQuery query(db);
    QString cmd;

    query.exec("drop table erp_roles");
    query.exec("drop table erp_roles_log");

    cmd = "create table if not exists erp_roles(";//创建角色表
    cmd += "role_uuid integer primary key,";//角色ID
    cmd += "role_guid interger,";//操作ID
    cmd += "role_sign interger,";//操作标识
    cmd += "role_name text,";//角色名称
    cmd += "role_mark text)";//角色备注
    if (!query.exec(cmd))
        qDebug() << "erp_roles create fail";

    cmd = "create table if not exists erp_roles_log(";//创建角色日志表
    cmd += "role_guid integer primary key,";//操作ID
    cmd += "role_sign integer,";//操作标识
    cmd += "role_uuid integer,";//角色ID
    cmd += "role_name text,";//角色名称
    cmd += "role_mark text)";//角色备注
    if (!query.exec(cmd))
        qDebug() << "erp_roles create fail";
    query.clear();
}

void MainScreen::excuteCmdRoles(QJsonObject obj)
{
    QSqlQuery query(db);
    qint64 logs_sign = obj.value("logs_sign").toDouble();
    qint64 logs_guid = obj.value("logs_guid").toDouble();
    qint64 tabs_guid = obj.value("tabs_guid").toDouble();

    switch (logs_sign) {
    case 0://查询
        break;
    case 1://增加
    case 3://修改
        query.prepare("replace into erp_roles values(?,?,?,?,?)");
        query.bindValue(0,tabs_guid);
        query.bindValue(1,logs_guid);
        query.bindValue(2,logs_sign);
        query.bindValue(3,obj.value("role_name").toString());
        query.bindValue(4,obj.value("role_mark").toString());
        query.exec();
        break;
    case 2://删除
        query.prepare("delete from erp_roles where role_uuid=:role_uuid");
        query.bindValue(":role_uuid",tabs_guid);
        query.exec();
    default:
        break;
    }
}

void MainScreen::createTabUsers()
{
    QSqlQuery query(db);
    QString cmd;

    query.exec("drop table erp_users");
    query.exec("drop table erp_users_log");

    cmd = "create table if not exists erp_users(";//创建用户表
    cmd += "user_uuid integer primary key,";//用户ID
    cmd += "user_guid interger,";//操作ID
    cmd += "user_sign interger,";//操作标识
    cmd += "user_name text,";//用户名称
    cmd += "user_pass text,";//用户密码
    cmd += "user_role interger,";//用户角色ID
    cmd += "user_date text)";//加入日期
    if (!query.exec(cmd))
        qDebug() << "erp_users create fail";

    cmd = "create table if not exists erp_users_log(";//创建用户日志表
    cmd += "user_guid integer primary key,";
    cmd += "user_sign integer,";
    cmd += "user_uuid integer,";
    cmd += "user_name text,";
    cmd += "user_pass text,";
    cmd += "user_role interger,";//用户角色ID
    cmd += "user_date text)";
    if (!query.exec(cmd))
        qDebug() << "erp_users create fail";

    query.clear();
}

void MainScreen::excuteCmdUsers(QJsonObject obj)
{
    QSqlQuery query(db);
    qint64 logs_sign = obj.value("logs_sign").toDouble();
    qint64 logs_guid = obj.value("logs_guid").toDouble();
    qint64 tabs_guid = obj.value("user_guid").toDouble();

    switch (logs_sign) {
    case 0://查询
        break;
    case 1://增加
    case 3://修改
        query.prepare("replace into erp_users values(?,?,?,?,?,?,?)");
        query.bindValue(0,tabs_guid);
        query.bindValue(1,logs_guid);
        query.bindValue(2,logs_sign);
        query.bindValue(3,obj.value("user_name").toString());
        query.bindValue(4,obj.value("user_pass").toString());
        query.bindValue(5,obj.value("user_role").toDouble());
        query.bindValue(6,obj.value("user_date").toString());
        query.exec();
        break;
    case 2://删除
        query.prepare("delete from erp_users where user_uuid=:user_uuid");
        query.bindValue(":user_uuid",tabs_guid);
        query.exec();
        break;
    default:
        break;
    }
    query.clear();
}

void MainScreen::createTabSales()
{
    QSqlQuery query(db);
    QString cmd;

    query.exec("drop table erp_sales");
    query.exec("drop table erp_sales_log");

    cmd = "create table if not exists erp_sales(";
    cmd += "sale_uuid integer primary key,";
    cmd += "sale_guid interger,";
    cmd += "sale_sign interger,";
    cmd += "sale_name text,";//销售名称
    cmd += "sale_area text)";//所属区域
    if (!query.exec(cmd))
        qDebug() << "erp_sales create fail";

    cmd = "create table if not exists erp_sales_log(";
    cmd += "sale_guid integer primary key,";
    cmd += "sale_sign integer,";
    cmd += "sale_uuid integer,";
    cmd += "sale_name text,";//销售名称
    cmd += "sale_area text)";//所属区域
    if (!query.exec(cmd))
        qDebug() << "erp_sales create fail";

    query.clear();
}

void MainScreen::excuteCmdSales(QJsonObject obj)
{
    QSqlQuery query(db);
    qint64 logs_sign = obj.value("logs_sign").toDouble();
    qint64 logs_guid = obj.value("logs_guid").toDouble();
    qint64 tabs_guid = obj.value("tabs_guid").toDouble();

    switch (logs_sign) {
    case 0://查询
        break;
    case 1://增加
    case 3://修改
        query.prepare("replace into erp_sales values(?,?,?,?,?)");
        query.bindValue(0,tabs_guid);
        query.bindValue(1,logs_guid);
        query.bindValue(2,logs_sign);
        query.bindValue(3,obj.value("sale_name").toString());
        query.bindValue(4,obj.value("sale_area").toString());
        query.exec();
        break;
    case 2://删除
        query.prepare("delete from erp_sales where id=:id");
        query.bindValue(":id",tabs_guid);
        query.exec();
    default:
        break;
    }
    query.clear();
}

void MainScreen::createTabCusts()
{
    QSqlQuery query(db);
    QString cmd;

    query.exec("drop table erp_custs");
    query.exec("drop table erp_custs_log");

    cmd = "create table if not exists erp_custs(";
    cmd += "cust_uuid integer primary key,";
    cmd += "cust_guid interger,";
    cmd += "cust_sign interger,";
    cmd += "cust_name text,";//客户名称
    cmd += "cust_sale interger)";//销售ID
    if (!query.exec(cmd))
        qDebug() << "erp_custs create fail";

    cmd = "create table if not exists erp_custs_log(";
    cmd += "cust_guid integer primary key,";
    cmd += "cust_sign integer,";
    cmd += "cust_uuid integer,";
    cmd += "cust_name text,";
    cmd += "cust_sale integer)";
    if (!query.exec(cmd))
        qDebug() << "erp_custs create fail";

    query.clear();
}

void MainScreen::excuteCmdCusts(QJsonObject obj)
{
    QSqlQuery query(db);
    qint64 logs_sign = obj.value("logs_sign").toDouble();
    qint64 logs_guid = obj.value("logs_guid").toDouble();
    qint64 tabs_guid = obj.value("tabs_guid").toDouble();

    switch (logs_sign) {
    case 0://查询
        break;
    case 1://增加
    case 3://修改
        query.prepare("replace into erp_custs values(?,?,?,?,?)");
        query.bindValue(0,tabs_guid);
        query.bindValue(1,logs_guid);
        query.bindValue(2,logs_sign);
        query.bindValue(3,obj.value("cust_name").toString());
        query.bindValue(4,obj.value("cust_sale").toDouble());
        query.exec();
        break;
    case 2://删除
        query.prepare("delete from erp_custs where cust_uuid=:cust_uuid");
        query.bindValue(":cust_uuid",tabs_guid);
        query.exec();
    default:
        break;
    }
    query.clear();
}

void MainScreen::createTabOrder()
{
    QSqlQuery query(db);
    QString cmd;

    query.exec("drop table erp_order");
    query.exec("drop table erp_order_log");

    cmd = "create table if not exists erp_order(";
    cmd += "ordr_uuid integer primary key,";
    cmd += "ordr_guid integer,";
    cmd += "ordr_sign integer,";
    cmd += "ordr_numb text,";//订单编号
    cmd += "ordr_date text,";//订单日期
    cmd += "ordr_area text,";//所属区域
    cmd += "ordr_sale text,";//销售经理
    cmd += "ordr_cust text,";//客户名称
    cmd += "ordr_view text,";//评审编号
    cmd += "ordr_quan text,";//订货数量
    cmd += "ordr_dead text,";//交货日期
    cmd += "ordr_mark text)";//备注内容
    if (!query.exec(cmd))
        qDebug() << "erp_order create fail";

    cmd = "create table if not exists erp_order_log(";
    cmd += "ordr_guid integer primary key,";
    cmd += "ordr_sign integer,";
    cmd += "ordr_uuid integer,";
    cmd += "ordr_numb text,";//订单编号
    cmd += "ordr_date text,";//订单日期
    cmd += "ordr_area text,";//所属区域
    cmd += "ordr_sale text,";//销售经理
    cmd += "ordr_cust text,";//客户名称
    cmd += "ordr_view text,";//评审编号
    cmd += "ordr_quan text,";//订货数量
    cmd += "ordr_dead text,";//交货日期
    cmd += "ordr_mark text)";//发货数量
    if (!query.exec(cmd))
        qDebug() << "erp_order create fail";
    query.clear();
}

void MainScreen::createTabSends()
{
    QSqlQuery query(db);
    QString cmd;

    query.exec("drop table erp_sends");
    query.exec("drop table erp_sends_log");

    cmd = "create table if not exists erp_sends(";
    cmd += "send_uuid integer primary key,";
    cmd += "send_guid interger,";
    cmd += "send_sign interger,";
    cmd += "send_numb text,";//订单单号
    cmd += "send_view text,";//评审单号
    cmd += "send_mode text,";//发货方式
    cmd += "send_code text,";//货运单号
    cmd += "send_prce text,";//运费
    cmd += "send_mark text)";//货运单号
    if (!query.exec(cmd))
        qDebug() << "erp_sends create fail";

    cmd = "create table if not exists erp_sends_log(";
    cmd += "send_guid integer primary key,";
    cmd += "send_sign integer,";
    cmd += "send_uuid integer,";
    cmd += "send_numb text,";//订单单号
    cmd += "send_view text,";//评审单号
    cmd += "send_mode text,";//发货方式
    cmd += "send_code text,";//货运单号
    cmd += "send_prce text)";//运费
    if (!query.exec(cmd))
        qDebug() << "erp_sends create fail";
}

void MainScreen::createTabPlans()
{
    QSqlQuery query(db);
    QString cmd;

    query.exec("drop table erp_plans");

    cmd = "create table if not exists erp_plans(";//创建生产表
    cmd += "plan_uuid integer primary key,";//生产计划ID
    cmd += "plan_guid integer,";//操作ID
    cmd += "plan_sign integer,";//操作标识
    cmd += "plan_numb text,";//生产单号
    cmd += "plan_quan text,";//生产数量
    cmd += "plan_type text,";//产品大类
    cmd += "plan_code text,";//产品编号
    cmd += "plan_name text,";//产品名称
    cmd += "plan_mode text,";//产品规格
    cmd += "plan_mnum text)";//仪表编号
    if (!query.exec(cmd))
        qDebug() << "erp_plans create fail";

    cmd = "create table if not exists erp_prods(";//创建生产详细表
    cmd += "prod_uuid integer primary key,";//生产详细ID
    cmd += "prod_guid integer,";//操作ID
    cmd += "prod_sign integer,";//操作标识
    cmd += "prod_puid integer,";//生产计划ID
    cmd += "prod_ouid integer,";//订单ID
    cmd += "prod_mnum text)";//仪表编号
    if (!query.exec(cmd))
        qDebug() << "erp_prods create fail";
}

void MainScreen::createTabProds()
{
    QSqlQuery query(db);
    QString cmd;

    query.exec("drop table erp_prods");

    cmd = "create table if not exists erp_prods(";//创建生产详细表
    cmd += "prod_uuid integer primary key,";//生产详细ID
    cmd += "prod_guid integer,";//操作ID
    cmd += "prod_sign integer,";//操作标识
    cmd += "prod_puid integer,";//生产计划ID
    cmd += "prod_ouid integer,";//订单ID
    cmd += "prod_mnum text)";//仪表编号
    if (!query.exec(cmd))
        qDebug() << "erp_prods create fail";
}

void MainScreen::createTabPurch()
{

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
