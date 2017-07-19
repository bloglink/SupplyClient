#include "HumanPage.h"

HumanPage::HumanPage(QWidget *parent) : QWidget(parent)
{
    initUI();
    initSql();
}

HumanPage::~HumanPage()
{
    db.close();
}

void HumanPage::initUI()
{
    this->setObjectName("HumanPage");
    tab_users = new QTableView(this);
    tab_users->setItemDelegate(new ReadOnlyDelegate);
    tab_users->horizontalHeader()->setHighlightSections(false);
    tab_users->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_users,SIGNAL(clicked(QModelIndex)),this,SLOT(tabUserSync(QModelIndex)));

    QPushButton *user_update = new QPushButton(this);
    user_update->setFlat(true);
    user_update->setMinimumSize(97,44);
    user_update->setText(tr("刷新显示"));
    user_update->setFocusPolicy(Qt::NoFocus);
    connect(user_update,SIGNAL(clicked(bool)),this,SLOT(updateUser()));

    QGridLayout *suserLayout = new QGridLayout;
    suserLayout->addWidget(tab_users,0,0,1,2);
    suserLayout->addWidget(user_update,1,1);
    suserLayout->setColumnStretch(0,1);

    btn_user = new QToolButton(this);
    btn_user->setIcon(QIcon(":/icons/left.png"));
    btn_user->setIconSize(QSize(30,30));
    btn_user->setFocusPolicy(Qt::NoFocus);
    btn_user->setText(tr("用\n户\n管\n理"));
    btn_user->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_user,SIGNAL(clicked(bool)),this,SLOT(showTabUser()));

    QVBoxLayout *ubtnsLayout = new QVBoxLayout;
    ubtnsLayout->addWidget(btn_user);
    ubtnsLayout->addStretch();

    user_items << tr("编号") << tr("姓名") << tr("密码") << tr("角色") << tr("日期");
    m_users = new StandardItemModel();
    QStringList user_header;
    user_header << tr("项目") << tr("参数");
    m_users->setHorizontalHeaderLabels(user_header);
    for (int i=0; i < user_items.size(); i++) {
        m_users->setItem(i,0,new QStandardItem(user_items.at(i)));
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

    QPushButton *user_append = new QPushButton(this);
    user_append->setFlat(true);
    user_append->setMinimumSize(97,44);
    user_append->setText(tr("添加用户"));
    user_append->setFocusPolicy(Qt::NoFocus);
    connect(user_append,SIGNAL(clicked(bool)),this,SLOT(appendUser()));
    QPushButton *user_delete = new QPushButton(this);
    user_delete->setFlat(true);
    user_delete->setMinimumSize(97,44);
    user_delete->setText(tr("删除用户"));
    user_delete->setFocusPolicy(Qt::NoFocus);
    connect(user_delete,SIGNAL(clicked(bool)),this,SLOT(deleteUser()));
    QPushButton *user_change = new QPushButton(this);
    user_change->setFlat(true);
    user_change->setMinimumSize(97,44);
    user_change->setText(tr("修改用户"));
    user_change->setFocusPolicy(Qt::NoFocus);
    connect(user_change,SIGNAL(clicked(bool)),this,SLOT(changeUser()));

    QGridLayout *iuserLayout = new QGridLayout;
    iuserLayout->addWidget(tab_iuser,0,0,1,4);
    iuserLayout->addWidget(user_append,1,1);
    iuserLayout->addWidget(user_delete,1,2);
    iuserLayout->addWidget(user_change,1,3);
    iuserLayout->setColumnStretch(0,1);
    iuserLayout->setMargin(0);

    userWidget = new QWidget(this);
    userWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    userWidget->setLayout(iuserLayout);
    userWidget->hide();

    QHBoxLayout *userlayout = new QHBoxLayout;
    userlayout->addLayout(suserLayout);
    userlayout->addLayout(ubtnsLayout);
    userlayout->addWidget(userWidget);
    userlayout->setStretch(0,1);

    QWidget *users = new QWidget(this);
    users->setLayout(userlayout);

    /**************************************************************************/
    tab_roles = new QTableView(this);
    tab_roles->setItemDelegate(new ReadOnlyDelegate);
    tab_roles->horizontalHeader()->setHighlightSections(false);
    tab_roles->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_roles,SIGNAL(clicked(QModelIndex)),this,SLOT(tabRoleSync(QModelIndex)));

    QPushButton *role_update = new QPushButton(this);
    role_update->setFlat(true);
    role_update->setMinimumSize(97,44);
    role_update->setText(tr("刷新显示"));
    role_update->setFocusPolicy(Qt::NoFocus);
    connect(user_update,SIGNAL(clicked(bool)),this,SLOT(updateRole()));

    QGridLayout *sroleLayout = new QGridLayout;
    sroleLayout->addWidget(tab_roles,0,0,1,2);
    sroleLayout->addWidget(role_update,1,1);
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

    role_items << tr("编号") << tr("角色") << tr("备注");
    m_roles = new StandardItemModel();
    QStringList role_header;
    role_header << tr("项目") << tr("参数");
    m_roles->setHorizontalHeaderLabels(role_header);
    for (int i=0; i < role_items.size(); i++) {
        m_roles->setItem(i,0,new QStandardItem(role_items.at(i)));
        m_roles->setItem(i,1,new QStandardItem(""));
    }
    tab_irole = new QTableView(this);
    tab_irole->setModel(m_roles);
    tab_irole->setColumnWidth(0,50);
    tab_irole->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

    QPushButton *role_append = new QPushButton(this);
    role_append->setFlat(true);
    role_append->setMinimumSize(97,44);
    role_append->setText(tr("添加角色"));
    role_append->setFocusPolicy(Qt::NoFocus);
    connect(role_append,SIGNAL(clicked(bool)),this,SLOT(appendRole()));
    QPushButton *role_delete = new QPushButton(this);
    role_delete->setFlat(true);
    role_delete->setMinimumSize(97,44);
    role_delete->setText(tr("删除角色"));
    role_delete->setFocusPolicy(Qt::NoFocus);
    connect(role_delete,SIGNAL(clicked(bool)),this,SLOT(deleteRole()));
    QPushButton *role_change = new QPushButton(this);
    role_change->setFlat(true);
    role_change->setMinimumSize(97,44);
    role_change->setText(tr("修改角色"));
    role_change->setFocusPolicy(Qt::NoFocus);
    connect(role_change,SIGNAL(clicked(bool)),this,SLOT(changeRole()));

    QGridLayout *iroleLayout = new QGridLayout;
    iroleLayout->addWidget(tab_irole,0,0,1,4);
    iroleLayout->addWidget(role_append,1,1);
    iroleLayout->addWidget(role_delete,1,2);
    iroleLayout->addWidget(role_change,1,3);
    iroleLayout->setColumnStretch(0,1);
    iroleLayout->setMargin(0);

    roleWidget = new QWidget(this);
    roleWidget->setLayout(iroleLayout);
    roleWidget->hide();

    QHBoxLayout *rolelayout = new QHBoxLayout;
    rolelayout->addLayout(sroleLayout);
    rolelayout->addLayout(rbtnsLayout);
    rolelayout->addWidget(roleWidget);
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

    for (int i=0; i < user_items.size(); i++)
        sql_users->setHeaderData(i, Qt::Horizontal, user_items.at(i));
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
    if (userWidget->isHidden()) {
        userWidget->show();
        btn_user->setIcon(QIcon(":/icons/right.png"));
    } else {
        userWidget->hide();
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
    if (roleWidget->isHidden()) {
        roleWidget->show();
        btn_role->setIcon(QIcon(":/icons/right.png"));
    } else {
        roleWidget->hide();
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

void HumanPage::appendUser()
{
    int row = sql_users->rowCount();
    sql_users->insertRow(row);
    for (int i=1; i < m_users->rowCount(); i++)
        sql_users->setData(sql_users->index(row,i),m_users->item(i,1)->text());
    sql_users->submitAll();
}

void HumanPage::deleteUser()
{
    int row = tab_users->currentIndex().row();
    sql_users->removeRow(row);
    sql_users->submitAll();
    sql_users->select();
}

void HumanPage::changeUser()
{
    int row = tab_users->currentIndex().row();
    for (int i=1; i < m_users->rowCount(); i++)
        sql_users->setData(sql_users->index(row,i),m_users->item(i,1)->text());
    sql_users->submitAll();
}

void HumanPage::updateUser()
{
    sql_users->select();
}

void HumanPage::appendRole()
{
    int row = sql_roles->rowCount();
    sql_roles->insertRow(row);
    for (int i=1; i < m_roles->rowCount(); i++)
        sql_roles->setData(sql_roles->index(row,i),m_roles->item(i,1)->text());
    sql_roles->submitAll();
}

void HumanPage::deleteRole()
{
    int row = tab_roles->currentIndex().row();
    sql_roles->removeRow(row);
    sql_roles->submitAll();
    sql_roles->select();
}

void HumanPage::changeRole()
{
    int row = tab_roles->currentIndex().row();
    for (int i=1; i < m_roles->rowCount(); i++)
        sql_roles->setData(sql_roles->index(row,i),m_roles->item(i,1)->text());
    sql_roles->submitAll();
}

void HumanPage::updateRole()
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
    e->accept();
}
