#include "SalesPage.h"

SalesPage::SalesPage(QWidget *parent) : QWidget(parent)
{
    initUI();
    initSql();
}


void SalesPage::initUI()
{
    this->setObjectName("SalesPage");
    tab_customs = new QTableView(this);
    tab_customs->setItemDelegate(new ReadOnlyDelegate);
    tab_customs->horizontalHeader()->setHighlightSections(false);
    tab_customs->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_customs,SIGNAL(clicked(QModelIndex)),this,SLOT(tabUserSync(QModelIndex)));

    QPushButton *user_snd = new QPushButton(this);
    user_snd->setFlat(true);
    user_snd->setMinimumSize(97,44);
    user_snd->setText(tr("刷新显示"));
    user_snd->setFocusPolicy(Qt::NoFocus);
    connect(user_snd,SIGNAL(clicked(bool)),this,SLOT(sndUser()));

    QGridLayout *susrLayout = new QGridLayout;
    susrLayout->addWidget(tab_customs,0,0,1,2);
    susrLayout->addWidget(user_snd,1,1);
    susrLayout->setColumnStretch(0,1);

    btn_customs = new QToolButton(this);//显示订单
    btn_customs->setIcon(QIcon(":/icons/left.png"));
    btn_customs->setIconSize(QSize(30,30));
    btn_customs->setFocusPolicy(Qt::NoFocus);
    btn_customs->setText(tr("客\n户\n管\n理"));
    btn_customs->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_customs,SIGNAL(clicked(bool)),this,SLOT(showTabUser()));

    QVBoxLayout *ubtnLayout = new QVBoxLayout;
    ubtnLayout->addWidget(btn_customs);
    ubtnLayout->addStretch();

    QStringList add_items;
    add_items << tr("编号") << tr("名称") << tr("销售") << tr("区域");
    m_customs = new StandardItemModel();
    QStringList users_header;
    users_header << tr("项目") << tr("参数");
    m_customs->setHorizontalHeaderLabels(users_header);
    for (int i=0; i < add_items.size(); i++) {
        m_customs->setItem(i,0,new QStandardItem(add_items.at(i)));
        m_customs->setItem(i,1,new QStandardItem(""));
    }
    area_delegate = new ComboBoxDelegate(this);
    sale_delegate = new ComboBoxDelegate(this);
    tab_icustom = new QTableView(this);
    tab_icustom->setModel(m_customs);
    tab_icustom->setColumnWidth(0,50);
    tab_icustom->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_icustom->setItemDelegateForColumn(0,new ReadOnlyDelegate);
    tab_icustom->setItemDelegateForRow(CUSTOM_ID, new ReadOnlyDelegate);
    tab_icustom->setItemDelegateForRow(CUSTOM_SALE, sale_delegate);
    tab_icustom->setItemDelegateForRow(CUSTOM_AREA, area_delegate);

    QPushButton *user_add = new QPushButton(this);
    user_add->setFlat(true);
    user_add->setMinimumSize(97,44);
    user_add->setText(tr("添加客户"));
    user_add->setFocusPolicy(Qt::NoFocus);
    connect(user_add,SIGNAL(clicked(bool)),this,SLOT(addUser()));
    QPushButton *user_del = new QPushButton(this);
    user_del->setFlat(true);
    user_del->setMinimumSize(97,44);
    user_del->setText(tr("删除客户"));
    user_del->setFocusPolicy(Qt::NoFocus);
    connect(user_del,SIGNAL(clicked(bool)),this,SLOT(delUser()));
    QPushButton *user_upt = new QPushButton(this);
    user_upt->setFlat(true);
    user_upt->setMinimumSize(97,44);
    user_upt->setText(tr("修改客户"));
    user_upt->setFocusPolicy(Qt::NoFocus);
    connect(user_upt,SIGNAL(clicked(bool)),this,SLOT(uptUser()));

    QGridLayout *iuserLayout = new QGridLayout;
    iuserLayout->addWidget(tab_icustom,0,0,1,4);
    iuserLayout->addWidget(user_add,1,1);
    iuserLayout->addWidget(user_del,1,2);
    iuserLayout->addWidget(user_upt,1,3);
    iuserLayout->setColumnStretch(0,1);
    iuserLayout->setMargin(0);

    wiCustoms = new QWidget(this);
    wiCustoms->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    wiCustoms->setLayout(iuserLayout);
    wiCustoms->hide();

    QHBoxLayout *userlayout = new QHBoxLayout;
    userlayout->addLayout(susrLayout);
    userlayout->addLayout(ubtnLayout);
    userlayout->addWidget(wiCustoms);
    userlayout->setStretch(0,1);

    QWidget *users = new QWidget(this);
    users->setLayout(userlayout);

    /**************************************************************************/
    tab_sales = new QTableView(this);
    tab_sales->setItemDelegate(new ReadOnlyDelegate);
    tab_sales->horizontalHeader()->setHighlightSections(false);
    tab_sales->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_sales,SIGNAL(clicked(QModelIndex)),this,SLOT(tabRoleSync(QModelIndex)));

    QPushButton *role_snd = new QPushButton(this);
    role_snd->setFlat(true);
    role_snd->setMinimumSize(97,44);
    role_snd->setText(tr("刷新显示"));
    role_snd->setFocusPolicy(Qt::NoFocus);
    connect(user_snd,SIGNAL(clicked(bool)),this,SLOT(sndRole()));

    QGridLayout *sroleLayout = new QGridLayout;
    sroleLayout->addWidget(tab_sales,0,0,1,2);
    sroleLayout->addWidget(role_snd,1,1);
    sroleLayout->setColumnStretch(0,1);

    btn_sales = new QToolButton(this);//显示订单
    btn_sales->setIcon(QIcon(":/icons/left.png"));
    btn_sales->setIconSize(QSize(30,30));
    btn_sales->setFocusPolicy(Qt::NoFocus);
    btn_sales->setText(tr("销\n售\n管\n理"));
    btn_sales->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_sales,SIGNAL(clicked(bool)),this,SLOT(showTabRole()));

    QVBoxLayout *rbtnsLayout = new QVBoxLayout;
    rbtnsLayout->addWidget(btn_sales);
    rbtnsLayout->addStretch();

    QStringList role_items;
    role_items << tr("编号") << tr("姓名") << tr("区域");
    m_sales = new StandardItemModel();
    QStringList roles_header;
    roles_header << tr("项目") << tr("参数");
    m_sales->setHorizontalHeaderLabels(roles_header);
    for (int i=0; i < role_items.size(); i++) {
        m_sales->setItem(i,0,new QStandardItem(role_items.at(i)));
        m_sales->setItem(i,1,new QStandardItem(""));
    }
    tab_isale = new QTableView(this);
    tab_isale->setModel(m_sales);
    tab_isale->setColumnWidth(0,50);
    tab_isale->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    QPushButton *role_add = new QPushButton(this);
    role_add->setFlat(true);
    role_add->setMinimumSize(97,44);
    role_add->setText(tr("添加销售"));
    role_add->setFocusPolicy(Qt::NoFocus);
    connect(role_add,SIGNAL(clicked(bool)),this,SLOT(addRole()));
    QPushButton *role_del = new QPushButton(this);
    role_del->setFlat(true);
    role_del->setMinimumSize(97,44);
    role_del->setText(tr("删除销售"));
    role_del->setFocusPolicy(Qt::NoFocus);
    connect(role_del,SIGNAL(clicked(bool)),this,SLOT(delRole()));
    QPushButton *role_upt = new QPushButton(this);
    role_upt->setFlat(true);
    role_upt->setMinimumSize(97,44);
    role_upt->setText(tr("修改销售"));
    role_upt->setFocusPolicy(Qt::NoFocus);
    connect(role_upt,SIGNAL(clicked(bool)),this,SLOT(uptRole()));

    QGridLayout *iroleLayout = new QGridLayout;
    iroleLayout->addWidget(tab_isale,0,0,1,4);
    iroleLayout->addWidget(role_add,1,1);
    iroleLayout->addWidget(role_del,1,2);
    iroleLayout->addWidget(role_upt,1,3);
    iroleLayout->setColumnStretch(0,1);
    iroleLayout->setMargin(0);

    wiSales = new QWidget(this);
    wiSales->setLayout(iroleLayout);
    wiSales->hide();

    QHBoxLayout *rolelayout = new QHBoxLayout;
    rolelayout->addLayout(sroleLayout);
    rolelayout->addLayout(rbtnsLayout);
    rolelayout->addWidget(wiSales);
    rolelayout->setStretch(0,1);

    QWidget *roles = new QWidget(this);
    roles->setLayout(rolelayout);

    /**************************************************************************/
    QSplitter *pSplitter = new QSplitter(this);
    pSplitter->addWidget(users);
    pSplitter->addWidget(roles);

    QVBoxLayout *splitter = new QVBoxLayout;
    splitter->addWidget(pSplitter);
    splitter->setMargin(0);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(splitter);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(group);
    this->setLayout(mainLayout);
}

void SalesPage::initSql()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "erp_sales");
    db.setDatabaseName("erp.db");
    db.open();
    sql_customs = new StandardSqlModel(this,db);
    sql_customs->setTable("erp_customs");
    sql_customs->select();
    QStringList add_items;
    add_items << tr("编号") << tr("名称") << tr("销售") << tr("区域");
    for (int i=0; i < add_items.size(); i++)
        sql_customs->setHeaderData(i, Qt::Horizontal, add_items.at(i));
    tab_customs->setModel(sql_customs);
    tab_customs->setColumnWidth(CUSTOM_ID,50);
    tab_customs->horizontalHeader()->setSectionResizeMode(CUSTOM_NAME,QHeaderView::Stretch);
    tab_customs->horizontalHeader()->setSectionResizeMode(CUSTOM_SALE,QHeaderView::Stretch);
    tab_customs->horizontalHeader()->setSectionResizeMode(CUSTOM_AREA,QHeaderView::Stretch);

    sql_sales = new StandardSqlModel(this,db);
    sql_sales->setTable("erp_sales");
    sql_sales->select();
    QStringList role_items;
    role_items << tr("编号") << tr("姓名") << tr("区域");
    for (int i=0; i < role_items.size(); i++)
        sql_sales->setHeaderData(i, Qt::Horizontal, role_items.at(i));
    tab_sales->setModel(sql_sales);
    tab_sales->setColumnWidth(SALE_ID,50);
    tab_sales->horizontalHeader()->setSectionResizeMode(SALE_NAME,QHeaderView::Stretch);
    tab_sales->horizontalHeader()->setSectionResizeMode(SALE_AREA,QHeaderView::Stretch);
}

void SalesPage::initData()
{
    sql_customs->select();
    sql_sales->select();
}

void SalesPage::showTabUser()
{
    if (wiCustoms->isHidden()) {
        wiCustoms->show();
        btn_customs->setIcon(QIcon(":/icons/right.png"));
    } else {
        wiCustoms->hide();
        btn_customs->setIcon(QIcon(":/icons/left.png"));
    }
    QStringList sales_head;
    for (int i=0; i < sql_sales->rowCount(); i++)
        sales_head.append(sql_sales->data(sql_sales->index(i,SALE_NAME)).toString());
    sale_delegate->setItemHeaders(sales_head);

    QStringList areas_head;
    for (int i=0; i < sql_sales->rowCount(); i++)
        areas_head.append(sql_sales->data(sql_sales->index(i,SALE_AREA)).toString());
    area_delegate->setItemHeaders(areas_head);
}

void SalesPage::showTabRole()
{
    if (wiSales->isHidden()) {
        wiSales->show();
        btn_sales->setIcon(QIcon(":/icons/right.png"));
    } else {
        wiSales->hide();
        btn_sales->setIcon(QIcon(":/icons/left.png"));
    }
}

void SalesPage::tabUserSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_customs->rowCount(); i++) {
        m_customs->item(i,1)->setText(sql_customs->index(row,i).data().toString());
    }
}

void SalesPage::tabRoleSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_sales->rowCount(); i++) {
        m_sales->item(i,1)->setText(sql_sales->index(row,i).data().toString());
    }
}

void SalesPage::addUser()
{
    int row = sql_customs->rowCount();
    sql_customs->insertRow(row);
    QString area;
    QString name = m_customs->item(CUSTOM_SALE,1)->text();
    for (int i=0; i < sql_sales->rowCount(); i++) {
        if (name == sql_sales->index(i,SALE_NAME).data().toString()) {
            area = sql_sales->index(i,SALE_AREA).data().toString();
            break;
        }
    }
    m_customs->item(CUSTOM_AREA,1)->setText(area);
    for (int i=1; i < m_customs->rowCount(); i++)
        sql_customs->setData(sql_customs->index(row,i),m_customs->item(i,1)->text());
    sql_customs->submitAll();
}

void SalesPage::delUser()
{
    int row = tab_customs->currentIndex().row();
    sql_customs->removeRow(row);
    sql_customs->submitAll();
    sql_customs->select();
}

void SalesPage::uptUser()
{
    int row = tab_customs->currentIndex().row();
    for (int i=1; i < m_customs->rowCount(); i++)
        sql_customs->setData(sql_customs->index(row,i),m_customs->item(i,1)->text());
    sql_customs->submitAll();
}

void SalesPage::sndUser()
{
    sql_customs->select();
}

void SalesPage::addRole()
{
    int row = sql_sales->rowCount();
    sql_sales->insertRow(row);
    for (int i=1; i < m_sales->rowCount(); i++)
        sql_sales->setData(sql_sales->index(row,i),m_sales->item(i,1)->text());
    sql_sales->submitAll();
}

void SalesPage::delRole()
{
    int row = tab_sales->currentIndex().row();
    sql_sales->removeRow(row);
    sql_sales->submitAll();
    sql_sales->select();
}

void SalesPage::uptRole()
{
    int row = tab_sales->currentIndex().row();
    for (int i=1; i < m_sales->rowCount(); i++)
        sql_sales->setData(sql_sales->index(row,i),m_sales->item(i,1)->text());
    sql_sales->submitAll();
}

void SalesPage::sndRole()
{
    sql_sales->select();
}

void SalesPage::recvSocket(QUrl url)
{
    QString cmd = url.query();
    QString usr = url.userName();
    if (usr != "Users")
        return;
    QByteArray byte = QByteArray::fromBase64(url.fragment().toUtf8());
    if (cmd == "userinfo") {
        qDebug() << "recv userinfo" << QJsonDocument::fromJson(byte).array();
        initData();
    } else if (cmd == "roleinfo") {
        qDebug() << "recv roleinfo" << QJsonDocument::fromJson(byte).array();
        initData();
    } else {
        qDebug() << "recv others" << url.toString();
    }
}

void SalesPage::showEvent(QShowEvent *e)
{
    QUrl url;
    url.setQuery("userinfo");
    sendSocket(url);

    url.setQuery("roleinfo");
    sendSocket(url);
    e->accept();
}
