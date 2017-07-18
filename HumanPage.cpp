#include "HumanPage.h"

HumanPage::HumanPage(QWidget *parent) : QWidget(parent)
{
    initUI();
    initSql();
}

void HumanPage::initUI()
{
    this->setObjectName("humanpage");
    tab_users = new QTableView(this);
    tab_users->setItemDelegate(new ReadOnlyDelegate);
    tab_users->horizontalHeader()->setHighlightSections(false);
    tab_users->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_users,SIGNAL(clicked(QModelIndex)),this,SLOT(tabUserSync(QModelIndex)));

    QPushButton *user_snd = new QPushButton(this);
    user_snd->setFlat(true);
    user_snd->setMinimumSize(97,44);
    user_snd->setText(tr("刷新显示"));
    user_snd->setFocusPolicy(Qt::NoFocus);
    connect(user_snd,SIGNAL(clicked(bool)),this,SLOT(sndUser()));

    QGridLayout *susrLayout = new QGridLayout;
    susrLayout->addWidget(tab_users,0,0,1,2);
    susrLayout->addWidget(user_snd,1,1);
    susrLayout->setColumnStretch(0,1);

    btn_user = new QToolButton(this);//显示订单
    btn_user->setIcon(QIcon(":/icons/left.png"));
    btn_user->setIconSize(QSize(30,30));
    btn_user->setFocusPolicy(Qt::NoFocus);
    btn_user->setText(tr("用\n户\n管\n理"));
    btn_user->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_user,SIGNAL(clicked(bool)),this,SLOT(showTabUser()));

    QVBoxLayout *ubtnLayout = new QVBoxLayout;
    ubtnLayout->addWidget(btn_user);
    ubtnLayout->addStretch();

    QStringList add_items;
    add_items << tr("编号") << tr("姓名") << tr("密码") << tr("角色") << tr("日期");
    m_users = new StandardItemModel();
    QStringList users_header;
    users_header << tr("项目") << tr("参数");
    m_users->setHorizontalHeaderLabels(users_header);
    for (int i=0; i < add_items.size(); i++) {
        m_users->setItem(i,0,new QStandardItem(add_items.at(i)));
        m_users->setItem(i,1,new QStandardItem(""));
    }
    role_delegate = new ComboBoxDelegate(this);
    tab_iuser = new QTableView(this);
    tab_iuser->setModel(m_users);
    tab_iuser->setColumnWidth(0,50);
    tab_iuser->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_iuser->setItemDelegateForColumn(0,new ReadOnlyDelegate);
    tab_iuser->setItemDelegateForRow(USER_ID, new ReadOnlyDelegate);
    tab_iuser->setItemDelegateForRow(USER_ROLE, role_delegate);
    tab_iuser->setItemDelegateForRow(USER_DATE, new DateEditDelegate);

    QPushButton *user_add = new QPushButton(this);
    user_add->setFlat(true);
    user_add->setMinimumSize(97,44);
    user_add->setText(tr("添加用户"));
    user_add->setFocusPolicy(Qt::NoFocus);
    connect(user_add,SIGNAL(clicked(bool)),this,SLOT(addUser()));
    QPushButton *user_del = new QPushButton(this);
    user_del->setFlat(true);
    user_del->setMinimumSize(97,44);
    user_del->setText(tr("删除用户"));
    user_del->setFocusPolicy(Qt::NoFocus);
    connect(user_del,SIGNAL(clicked(bool)),this,SLOT(delUser()));
    QPushButton *user_upt = new QPushButton(this);
    user_upt->setFlat(true);
    user_upt->setMinimumSize(97,44);
    user_upt->setText(tr("修改用户"));
    user_upt->setFocusPolicy(Qt::NoFocus);
    connect(user_upt,SIGNAL(clicked(bool)),this,SLOT(uptUser()));

    QGridLayout *iuserLayout = new QGridLayout;
    iuserLayout->addWidget(tab_iuser,0,0,1,4);
    iuserLayout->addWidget(user_add,1,1);
    iuserLayout->addWidget(user_del,1,2);
    iuserLayout->addWidget(user_upt,1,3);
    iuserLayout->setColumnStretch(0,1);
    iuserLayout->setMargin(0);

    wiUsers = new QWidget(this);
    wiUsers->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    wiUsers->setLayout(iuserLayout);
    wiUsers->hide();

    QHBoxLayout *userlayout = new QHBoxLayout;
    userlayout->addLayout(susrLayout);
    userlayout->addLayout(ubtnLayout);
    userlayout->addWidget(wiUsers);
    userlayout->setStretch(0,1);

    QWidget *users = new QWidget(this);
    users->setLayout(userlayout);

    /**************************************************************************/
    tab_roles = new QTableView(this);
    tab_roles->setItemDelegate(new ReadOnlyDelegate);
    tab_roles->horizontalHeader()->setHighlightSections(false);
    tab_roles->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_roles,SIGNAL(clicked(QModelIndex)),this,SLOT(tabRoleSync(QModelIndex)));

    QPushButton *role_snd = new QPushButton(this);
    role_snd->setFlat(true);
    role_snd->setMinimumSize(97,44);
    role_snd->setText(tr("刷新显示"));
    role_snd->setFocusPolicy(Qt::NoFocus);
    connect(user_snd,SIGNAL(clicked(bool)),this,SLOT(sndRole()));

    QGridLayout *sroleLayout = new QGridLayout;
    sroleLayout->addWidget(tab_roles,0,0,1,2);
    sroleLayout->addWidget(role_snd,1,1);
    sroleLayout->setColumnStretch(0,1);

    btn_role = new QToolButton(this);//显示订单
    btn_role->setIcon(QIcon(":/icons/left.png"));
    btn_role->setIconSize(QSize(30,30));
    btn_role->setFocusPolicy(Qt::NoFocus);
    btn_role->setText(tr("角\n色\n管\n理"));
    btn_role->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_role,SIGNAL(clicked(bool)),this,SLOT(showTabRole()));

    QVBoxLayout *rbtnsLayout = new QVBoxLayout;
    rbtnsLayout->addWidget(btn_role);
    rbtnsLayout->addStretch();

    QStringList role_items;
    role_items << tr("编号") << tr("角色") << tr("备注");
    m_roles = new StandardItemModel();
    QStringList roles_header;
    roles_header << tr("项目") << tr("参数");
    m_roles->setHorizontalHeaderLabels(roles_header);
    for (int i=0; i < role_items.size(); i++) {
        m_roles->setItem(i,0,new QStandardItem(role_items.at(i)));
        m_roles->setItem(i,1,new QStandardItem(""));
    }
    tab_irole = new QTableView(this);
    tab_irole->setModel(m_roles);
    tab_irole->setColumnWidth(0,50);
    tab_irole->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    QPushButton *role_add = new QPushButton(this);
    role_add->setFlat(true);
    role_add->setMinimumSize(97,44);
    role_add->setText(tr("添加角色"));
    role_add->setFocusPolicy(Qt::NoFocus);
    connect(role_add,SIGNAL(clicked(bool)),this,SLOT(addRole()));
    QPushButton *role_del = new QPushButton(this);
    role_del->setFlat(true);
    role_del->setMinimumSize(97,44);
    role_del->setText(tr("删除角色"));
    role_del->setFocusPolicy(Qt::NoFocus);
    connect(role_del,SIGNAL(clicked(bool)),this,SLOT(delRole()));
    QPushButton *role_upt = new QPushButton(this);
    role_upt->setFlat(true);
    role_upt->setMinimumSize(97,44);
    role_upt->setText(tr("修改角色"));
    role_upt->setFocusPolicy(Qt::NoFocus);
    connect(role_upt,SIGNAL(clicked(bool)),this,SLOT(uptRole()));

    QGridLayout *iroleLayout = new QGridLayout;
    iroleLayout->addWidget(tab_irole,0,0,1,4);
    iroleLayout->addWidget(role_add,1,1);
    iroleLayout->addWidget(role_del,1,2);
    iroleLayout->addWidget(role_upt,1,3);
    iroleLayout->setColumnStretch(0,1);
    iroleLayout->setMargin(0);

    wiRoles = new QWidget(this);
    wiRoles->setLayout(iroleLayout);
    wiRoles->hide();

    QHBoxLayout *rolelayout = new QHBoxLayout;
    rolelayout->addLayout(sroleLayout);
    rolelayout->addLayout(rbtnsLayout);
    rolelayout->addWidget(wiRoles);
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

void HumanPage::initSql()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "erp_human");
    db.setDatabaseName("erp.db");
    db.open();
    sql_users = new StandardSqlModel(this,db);
    sql_users->setTable("erp_users");
    sql_users->select();
    QStringList add_items;
    add_items << tr("编号") << tr("姓名") << tr("密码") << tr("角色") << tr("日期");
    for (int i=0; i < add_items.size(); i++)
        sql_users->setHeaderData(i, Qt::Horizontal, add_items.at(i));
    tab_users->setModel(sql_users);
    tab_users->setColumnWidth(USER_ID,50);
    tab_users->horizontalHeader()->setSectionResizeMode(USER_NAME,QHeaderView::Stretch);
    tab_users->horizontalHeader()->setSectionResizeMode(USER_PASSWORD,QHeaderView::Stretch);
    tab_users->horizontalHeader()->setSectionResizeMode(USER_ROLE,QHeaderView::Stretch);
    tab_users->horizontalHeader()->setSectionResizeMode(USER_DATE,QHeaderView::Stretch);
    tab_users->hideColumn(USER_PASSWORD);

    sql_roles = new StandardSqlModel(this,db);
    sql_roles->setTable("erp_roles");
    sql_roles->select();
    QStringList role_items;
    role_items << tr("编号") << tr("角色") << tr("备注");
    for (int i=0; i < role_items.size(); i++)
        sql_roles->setHeaderData(i, Qt::Horizontal, role_items.at(i));
    tab_roles->setModel(sql_roles);
    tab_roles->setColumnWidth(ROLE_ID,50);
    tab_roles->setColumnWidth(ROLE_NAME,100);
    tab_roles->horizontalHeader()->setSectionResizeMode(ROLE_MARK,QHeaderView::Stretch);
}

void HumanPage::initData()
{
    sql_users->select();
    sql_roles->select();
}

void HumanPage::showTabUser()
{
    if (wiUsers->isHidden()) {
        wiUsers->show();
        btn_user->setIcon(QIcon(":/icons/right.png"));
    } else {
        wiUsers->hide();
        btn_user->setIcon(QIcon(":/icons/left.png"));
    }
    m_users->item(USER_DATE,1)->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    QStringList head;
    for (int i=0; i < sql_roles->rowCount(); i++)
        head.append(sql_roles->data(sql_roles->index(i,ROLE_NAME)).toString());
    role_delegate->setItemHeaders(head);
}

void HumanPage::showTabRole()
{
    if (wiRoles->isHidden()) {
        wiRoles->show();
        btn_role->setIcon(QIcon(":/icons/right.png"));
    } else {
        wiRoles->hide();
        btn_role->setIcon(QIcon(":/icons/left.png"));
    }
}

void HumanPage::tabUserSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_users->rowCount(); i++) {
        m_users->item(i,1)->setText(sql_users->index(row,i).data().toString());
    }
}

void HumanPage::tabRoleSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_roles->rowCount(); i++) {
        m_roles->item(i,1)->setText(sql_roles->index(row,i).data().toString());
    }
}

void HumanPage::addUser()
{
    int row = sql_users->rowCount();
    sql_users->insertRow(row);
    for (int i=1; i < m_users->rowCount(); i++)
        sql_users->setData(sql_users->index(row,i),m_users->item(i,1)->text());
    sql_users->submitAll();
}

void HumanPage::delUser()
{
    int row = tab_users->currentIndex().row();
    sql_users->removeRow(row);
    sql_users->submitAll();
    sql_users->select();
}

void HumanPage::uptUser()
{
    int row = tab_users->currentIndex().row();
    for (int i=1; i < m_users->rowCount(); i++)
        sql_users->setData(sql_users->index(row,i),m_users->item(i,1)->text());
    sql_users->submitAll();
}

void HumanPage::sndUser()
{
    sql_users->select();
}

void HumanPage::addRole()
{
    int row = sql_roles->rowCount();
    sql_roles->insertRow(row);
    for (int i=1; i < m_roles->rowCount(); i++)
        sql_roles->setData(sql_roles->index(row,i),m_roles->item(i,1)->text());
    sql_roles->submitAll();
}

void HumanPage::delRole()
{
    int row = tab_roles->currentIndex().row();
    sql_roles->removeRow(row);
    sql_roles->submitAll();
    sql_roles->select();
}

void HumanPage::uptRole()
{
    int row = tab_roles->currentIndex().row();
    for (int i=1; i < m_roles->rowCount(); i++)
        sql_roles->setData(sql_roles->index(row,i),m_roles->item(i,1)->text());
    sql_roles->submitAll();
}

void HumanPage::sndRole()
{
    sql_roles->select();
}

void HumanPage::recvSocket(QUrl url)
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

void HumanPage::showEvent(QShowEvent *e)
{
    QUrl url;
    url.setQuery("salesinfo");
    emit sendSocket(url);

    e->accept();
}
