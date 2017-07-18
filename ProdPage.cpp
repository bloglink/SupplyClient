#include "ProdPage.h"

ProdPage::ProdPage(QWidget *parent) : QWidget(parent)
{
    initUI();
    initSql();
}


ProdPage::~ProdPage()
{
    db.close();
}

void ProdPage::initUI()
{
    this->setObjectName("ProdPage");

    tab_order = new QTableView(this);
    tab_order->setItemDelegate(new ReadOnlyDelegate);
    tab_order->horizontalHeader()->setHighlightSections(false);
    tab_order->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_order,SIGNAL(clicked(QModelIndex)),this,SLOT(tabSync(QModelIndex)));

    QPushButton *user_snd = new QPushButton(this);
    user_snd->setFlat(true);
    user_snd->setMinimumSize(97,44);
    user_snd->setText(tr("刷新显示"));
    user_snd->setFocusPolicy(Qt::NoFocus);
    connect(user_snd,SIGNAL(clicked(bool)),this,SLOT(sndOrder()));

    QGridLayout *susrLayout = new QGridLayout;
    susrLayout->addWidget(tab_order,0,0,1,2);
    susrLayout->addWidget(user_snd,1,1);
    susrLayout->setColumnStretch(0,1);

    btn_add = new QToolButton(this);//显示订单
    btn_add->setIcon(QIcon(":/icons/left.png"));
    btn_add->setIconSize(QSize(30,30));
    btn_add->setFocusPolicy(Qt::NoFocus);
    btn_add->setText(tr("订\n单\n管\n理"));
    btn_add->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_add,SIGNAL(clicked(bool)),this,SLOT(showTabOrder()));

    QVBoxLayout *ubtnLayout = new QVBoxLayout;
    ubtnLayout->addWidget(btn_add);
    ubtnLayout->addStretch();

    QStringList add_items;
    add_items << tr("编号") << tr("评审单号") << tr("客户名称") << tr("销售经理")
              << tr("所属区域") << tr("订货数量") << tr("发货日期")
              << tr("库存数量") << tr("在产数量") << tr("缺料数量") << tr("发货数量");
    m_order = new StandardItemModel();
    QStringList users_header;
    users_header << tr("项目") << tr("参数");
    m_order->setHorizontalHeaderLabels(users_header);
    for (int i=0; i < add_items.size(); i++) {
        m_order->setItem(i,0,new QStandardItem(add_items.at(i)));
        m_order->setItem(i,1,new QStandardItem(""));
    }
    sale_delegate = new ComboBoxDelegate;
    area_delegate = new ComboBoxDelegate;
    customer = new ComboBoxDelegate;
    tab_iorder = new QTableView(this);
    tab_iorder->setModel(m_order);
    tab_iorder->setColumnWidth(0,100);
    tab_iorder->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_iorder->setItemDelegateForColumn(0,new ReadOnlyDelegate);
    tab_iorder->setItemDelegateForRow(ORDER_CUSTOM,customer);
    tab_iorder->setItemDelegateForRow(ORDER_AREA,area_delegate);
    tab_iorder->setItemDelegateForRow(ORDER_SALE,sale_delegate);
    tab_iorder->setItemDelegateForRow(ORDER_DATE,new DateEditDelegate);

    QPushButton *user_add = new QPushButton(this);
    user_add->setFlat(true);
    user_add->setMinimumSize(97,44);
    user_add->setText(tr("添加订单"));
    user_add->setFocusPolicy(Qt::NoFocus);
    connect(user_add,SIGNAL(clicked(bool)),this,SLOT(addOrder()));
    QPushButton *user_del = new QPushButton(this);
    user_del->setFlat(true);
    user_del->setMinimumSize(97,44);
    user_del->setText(tr("删除订单"));
    user_del->setFocusPolicy(Qt::NoFocus);
    connect(user_del,SIGNAL(clicked(bool)),this,SLOT(delOrder()));
    QPushButton *user_upt = new QPushButton(this);
    user_upt->setFlat(true);
    user_upt->setMinimumSize(97,44);
    user_upt->setText(tr("修改订单"));
    user_upt->setFocusPolicy(Qt::NoFocus);
    connect(user_upt,SIGNAL(clicked(bool)),this,SLOT(uptOrder()));

    QGridLayout *iuserLayout = new QGridLayout;
    iuserLayout->addWidget(tab_iorder,0,0,1,4);
    iuserLayout->addWidget(user_add,1,1);
    iuserLayout->addWidget(user_del,1,2);
    iuserLayout->addWidget(user_upt,1,3);
    iuserLayout->setColumnStretch(0,1);
    iuserLayout->setMargin(0);

    wiOrders = new QWidget(this);
    wiOrders->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    wiOrders->setLayout(iuserLayout);
    wiOrders->hide();

    QHBoxLayout *userlayout = new QHBoxLayout;
    userlayout->addLayout(susrLayout);
    userlayout->addLayout(ubtnLayout);
    userlayout->addWidget(wiOrders);
    userlayout->setStretch(0,1);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(userlayout);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(group);
    this->setLayout(mainLayout);
}

void ProdPage::initSql()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "erp_orders");
    db.setDatabaseName("erp.db");
    db.open();
    sql_order = new StandardSqlModel(this,db);
    sql_order->setTable("erp_orders");
    sql_order->select();
    QStringList add_items;
    add_items << tr("编号") << tr("评审单号") << tr("客户名称") << tr("销售经理")
              << tr("所属区域") << tr("订货数量") << tr("发货日期")
              << tr("库存数量") << tr("在产数量") << tr("缺料数量") << tr("发货数量");
    for (int i=0; i < add_items.size(); i++)
        sql_order->setHeaderData(i, Qt::Horizontal, add_items.at(i));
    tab_order->setModel(sql_order);
    tab_order->setColumnWidth(ORDER_ID,50);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_NUMBER,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_CUSTOM,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_SALE,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_AREA,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_QUANTITY,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_DATE,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_STOCK,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_PRODUCE,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_LACK,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_DELIVERY,QHeaderView::Stretch);

    sql_customs = new StandardSqlModel(this,db);
    sql_customs->setTable("erp_customs");
    sql_customs->select();

    sql_sales = new StandardSqlModel(this,db);
    sql_sales->setTable("erp_sales");
    sql_sales->select();

}
void ProdPage::showTabOrder()
{
    if (wiOrders->isHidden()) {
        wiOrders->show();
        btn_add->setIcon(QIcon(":/icons/right.png"));
    } else {
        wiOrders->hide();
        btn_add->setIcon(QIcon(":/icons/left.png"));
    }

    QStringList sales_head;
    for (int i=0; i < sql_sales->rowCount(); i++)
        sales_head.append(sql_sales->data(sql_sales->index(i,1)).toString());
    sale_delegate->setItemHeaders(sales_head);

    QStringList areas_head;
    for (int i=0; i < sql_sales->rowCount(); i++)
        areas_head.append(sql_sales->data(sql_sales->index(i,2)).toString());
    area_delegate->setItemHeaders(areas_head);

    QStringList customs_head;
    for (int i=0; i < sql_customs->rowCount(); i++)
        customs_head.append(sql_customs->data(sql_customs->index(i,1)).toString());
    customer->setItemHeaders(customs_head);

    autoNumber();
}

void ProdPage::autoNumber()
{
    QString dat = QDate::currentDate().toString("yyyyMMdd");
    int max = 0;
    for (int i=0; i < sql_order->rowCount(); i++) {
        QString number = sql_order->index(i,ORDER_NUMBER).data().toString();
        if (number.contains(dat)) {
            int num = number.remove(dat).toInt();
            if (max < num)
                max = num;
        }
    }
    QString odds = QString("%1%2").arg(dat).arg(max+1,3,10,QLatin1Char('0'));
    m_order->item(ORDER_NUMBER,1)->setText(odds);
}

void ProdPage::initData()
{

}

void ProdPage::addOrder()
{
    autoNumber();
    int row = sql_order->rowCount();
    sql_order->insertRow(row);
    for (int i=1; i < m_order->rowCount(); i++)
        sql_order->setData(sql_order->index(row,i),m_order->item(i,1)->text());
    sql_order->submitAll();
}

void ProdPage::delOrder()
{
    int row = tab_order->currentIndex().row();
    sql_order->removeRow(row);
    sql_order->submitAll();
    sql_order->select();
}

void ProdPage::uptOrder()
{
    int row = tab_order->currentIndex().row();
    for (int i=1; i < m_order->rowCount(); i++)
        sql_order->setData(sql_order->index(row,i),m_order->item(i,1)->text());
    sql_order->submitAll();
}

void ProdPage::sndOrder()
{
    sql_order->select();
}

void ProdPage::tabSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_order->rowCount(); i++) {
        m_order->item(i,1)->setText(sql_order->index(row,i).data().toString());
    }
}

void ProdPage::recvSocket(QUrl url)
{
    QString cmd = url.query();
    QString usr = url.userName();
    if (usr != "ProdPage")
        return;
    QByteArray byte = QByteArray::fromBase64(url.fragment().toUtf8());
    if (cmd == "orderinfo") {
        json_show = QJsonDocument::fromJson(byte).array();
        initData();
    } else if (cmd == "saleinfo") {
        json_sale = QJsonDocument::fromJson(byte).array();
        QStringList items;
        for (int i=0; i < json_sale.size(); i++) {
            QJsonObject obj = json_sale.at(i).toObject();
            items.append(obj.value("erp_solename").toString());
        }
        sale_delegate->setItemHeaders(items);
    } else if (cmd == "customerinfo") {
        json_customer = QJsonDocument::fromJson(byte).array();
        QStringList items;
        for (int i=0; i < json_customer.size(); i++) {
            QJsonObject obj = json_customer.at(i).toObject();
            items.append(obj.value("erp_solename").toString());
        }
        customer->setItemHeaders(items);
    } else if (cmd == "pmstayinfo") {
        qDebug() << QJsonDocument::fromJson(byte).array();
    } else {
        qDebug() << "recv others" << url.toString();
    }
}



void ProdPage::showSnd()
{
    if (wiOrders->isHidden() && w_snd->isHidden()) {
        w_snd->show();
        btn_add->setIcon(QIcon(":/icons/right.png"));
        btn_snd->setIcon(QIcon(":/icons/right.png"));
    } else {
        wiOrders->hide();
        w_snd->hide();
        btn_add->setIcon(QIcon(":/icons/left.png"));
        btn_snd->setIcon(QIcon(":/icons/left.png"));
    }
}

void ProdPage::refresh()
{
    m_show->setRowCount(0);
    QUrl url;
    url.setQuery("orderinfo");
    QJsonObject obj;
    obj.insert("odstarttime",s_date->date().toString("yyyyMMdd"));
    obj.insert("odendtime",e_date->date().toString("yyyyMMdd"));
    QByteArray msg = QJsonDocument(obj).toJson();
    url.setFragment(msg.toBase64());
    emit sendSocket(url);
}

void ProdPage::showEvent(QShowEvent *e)
{
    //    refresh();
    e->accept();
}
