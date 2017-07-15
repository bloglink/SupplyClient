#include "ordermanagement.h"

OrderManagement::OrderManagement(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void OrderManagement::initUI()
{
    this->setObjectName("ordermanagement");

    QStringList order_header;
    order_header << tr("订单单号") << tr("录入日期") << tr("业务经理") << tr("客户名称")
                 << tr("订单数量") << tr("评审单号") << tr("要求发货日期") << tr("订单状态");
    m_show = new StandardItemModel();
    m_show->setHorizontalHeaderLabels(order_header);
    tab_show = new QTableView(this);//订单显示
    tab_show->setModel(m_show);
    tab_show->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_show->horizontalHeader()->setHighlightSections(false);
    tab_show->setItemDelegateForColumn(ORDER_ODDS, new ReadOnlyDelegate);
    connect(tab_show,SIGNAL(clicked(QModelIndex)),this,SLOT(tabSync(QModelIndex)));

    s_date = new QDateEdit(this);//订单日期输入
    s_date->setMinimumSize(112,44);
    s_date->setCalendarPopup(true);
    s_date->setDate(QDate::currentDate());
    s_date->setDisplayFormat("yyyy-MM-dd");

    e_date = new QDateEdit(this);//订单日期输入
    e_date->setMinimumSize(112,44);
    e_date->setCalendarPopup(true);
    e_date->setDate(QDate::currentDate());
    e_date->setDisplayFormat("yyyy-MM-dd");

    status = new QComboBox(this);//订单状态输入
    status->addItem(tr("商务"));
    status->setMinimumSize(97,44);
    status->setView(new QListView);

    QPushButton *order_qry = new QPushButton(this);
    order_qry->setFlat(true);
    order_qry->setMinimumSize(97,44);
    order_qry->setText(tr("查询订单"));
    order_qry->setFocusPolicy(Qt::NoFocus);
    connect(order_qry,SIGNAL(clicked(bool)),this,SLOT(refresh()));

    QHBoxLayout *datLayout = new QHBoxLayout;
    datLayout->addStretch();
    datLayout->addWidget(new QLabel(tr("日期范围:")));
    datLayout->addWidget(s_date);
    datLayout->addWidget(new QLabel("-"));
    datLayout->addWidget(e_date);
    datLayout->addWidget(status);
    datLayout->addWidget(order_qry);

    QWidget *w_order = new QWidget(this);//订单显示页面
    QVBoxLayout *ord_layout = new QVBoxLayout;
    ord_layout->addWidget(tab_show);
    ord_layout->addLayout(datLayout);
    w_order->setLayout(ord_layout);

    btn_add = new QToolButton(this);//显示订单
    btn_add->setIcon(QIcon(":/icons/left.png"));
    btn_add->setIconSize(QSize(30,30));
    btn_add->setFocusPolicy(Qt::NoFocus);
    connect(btn_add,SIGNAL(clicked(bool)),this,SLOT(showAdd()));

    btn_snd = new QToolButton(this);//显示发货
    btn_snd->setIcon(QIcon(":/icons/left.png"));
    btn_snd->setIconSize(QSize(30,30));
    btn_snd->setFocusPolicy(Qt::NoFocus);
    connect(btn_snd,SIGNAL(clicked(bool)),this,SLOT(showSnd()));

    QVBoxLayout *extLayout = new QVBoxLayout;//隐藏/显示按钮布局
    extLayout->addWidget(btn_add);
    extLayout->addWidget(btn_snd);
    extLayout->addStretch();

    QStringList add_items;
    add_items << tr("订单单号") << tr("录入日期") << tr("业务经理") << tr("客户名称")
              << tr("订单数量") << tr("评审单号") << tr("要求发货日期");
    QStringList add_header;
    add_header << tr("项目") << tr("参数");
    m_add = new StandardItemModel();
    m_add->setHorizontalHeaderLabels(add_header);
    for (int i=0; i < add_items.size(); i++) {
        m_add->setItem(i,0,new QStandardItem(add_items.at(i)));
        m_add->setItem(i,1,new QStandardItem(""));
    }
    sales = new ComboBoxDelegate(this);
    customer = new ComboBoxDelegate(this);
    tab_add = new QTableView(this);
    tab_add->setModel(m_add);
    tab_add->setColumnWidth(ORDER_ODDS,100);
    tab_add->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_add->horizontalHeader()->setHighlightSections(false);
    tab_add->setItemDelegateForRow(ORDER_DATE,new DateEditDelegate);
    tab_add->setItemDelegateForRow(ORDER_SALE,sales);
    tab_add->setItemDelegateForRow(ORDER_CSTM,customer);
    tab_add->setItemDelegateForRow(ORDER_EDAT,new DateEditDelegate);
    tab_add->setItemDelegateForColumn(ORDER_ODDS, new ReadOnlyDelegate);

    QPushButton *order_add = new QPushButton(this);
    order_add->setFlat(true);
    order_add->setMinimumSize(97,44);
    order_add->setText(tr("添加订单"));
    order_add->setFocusPolicy(Qt::NoFocus);
    connect(order_add,SIGNAL(clicked(bool)),this,SLOT(addOrder()));

    QPushButton *order_del = new QPushButton(this);
    order_del->setFlat(true);
    order_del->setMinimumSize(97,44);
    order_del->setText(tr("删除订单"));
    order_del->setFocusPolicy(Qt::NoFocus);
    connect(order_del,SIGNAL(clicked(bool)),this,SLOT(delOrder()));

    QPushButton *order_upt = new QPushButton(this);
    order_upt->setFlat(true);
    order_upt->setMinimumSize(97,44);
    order_upt->setText(tr("修改订单"));
    order_upt->setFocusPolicy(Qt::NoFocus);
    connect(order_upt,SIGNAL(clicked(bool)),this,SLOT(uptOrder()));

    QHBoxLayout *btnoLayout = new QHBoxLayout;
    btnoLayout->addStretch();
    btnoLayout->addWidget(order_add);
    btnoLayout->addWidget(order_del);
    btnoLayout->addWidget(order_upt);

    w_add = new QWidget(this);
    QVBoxLayout *add_layout = new QVBoxLayout;
    w_add->setLayout(add_layout);
    add_layout->addWidget(tab_add);
    add_layout->addLayout(btnoLayout);

    QStringList snd_items;
    snd_items << tr("订单单号") << tr("发货日期") << tr("客户名称") << tr("评审单号")
              << tr("发货方式") << tr("发货单号") << tr("快递费");
    QStringList snd_header;
    snd_header << tr("项目") << tr("参数");
    m_snd = new StandardItemModel();
    m_snd->setHorizontalHeaderLabels(snd_header);
    for (int i=0; i < snd_items.size(); i++) {
        m_snd->setItem(i,0,new QStandardItem(snd_items.at(i)));
        m_snd->setItem(i,1,new QStandardItem(""));
    }
    tab_snd = new QTableView(this);
    tab_snd->setModel(m_snd);
    tab_snd->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_snd->horizontalHeader()->setHighlightSections(false);

    QPushButton *order_snd = new QPushButton(this);
    order_snd->setFlat(true);
    order_snd->setMinimumSize(97,44);
    order_snd->setText(tr("添加发货"));
    order_snd->setFocusPolicy(Qt::NoFocus);
    connect(order_snd,SIGNAL(clicked(bool)),this,SLOT(sndOrder()));

    QHBoxLayout *btns_Layout = new QHBoxLayout;
    btns_Layout->addStretch();
    btns_Layout->addWidget(order_snd);

    w_snd = new QWidget(this);
    QVBoxLayout *snd_layout = new QVBoxLayout;
    w_snd->setLayout(snd_layout);
    snd_layout->addWidget(tab_snd);
    snd_layout->addLayout(btns_Layout);

    QHBoxLayout *tabLayout = new QHBoxLayout;
    tabLayout->addWidget(w_order);
    tabLayout->addLayout(extLayout);
    tabLayout->addWidget(w_add);
    tabLayout->addWidget(w_snd);
    tabLayout->setMargin(0);
    tabLayout->setStretch(0,1);
    tabLayout->setSpacing(0);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(tabLayout);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(group);
    this->setLayout(mainLayout);

    w_add->hide();
    w_snd->hide();
}

void OrderManagement::initData()
{
    max_order_id = 0;
    for (int i=0; i < json_show.size(); i++) {
        QJsonObject obj = json_show.at(i).toObject();
        int id   = obj.value("erp_orderscid").toInt();//ID
        QString ordernumber = obj.value("erp_ordernumber").toString();//订单单号
        QString orderdate = obj.value("erp_orderdate").toString();//订单日期
        QString ordersale = obj.value("erp_ordersale").toString();//业务经理
        QString customer = obj.value("erp_ordercustomer").toString();//客户名称
        int orderquantity = obj.value("erp_orderquantity").toInt();//订单数量
        QString orderreview = obj.value("erp_orderreview").toString();//评审单号
        QString orderedate = obj.value("erp_orderedate").toString();//要求发货日期
        QString status = obj.value("erp_orderstat").toString();//订单状态
        if (id > max_order_id)
            max_order_id = id;
        m_show->setItem(i, ORDER_ODDS, new QStandardItem(ordernumber));
        m_show->setItem(i, ORDER_DATE, new QStandardItem(orderdate));
        m_show->setItem(i, ORDER_SALE, new QStandardItem(ordersale));
        m_show->setItem(i, ORDER_CSTM, new QStandardItem(customer));
        m_show->setItem(i, ORDER_QUAN, new QStandardItem(QString::number(orderquantity)));
        m_show->setItem(i, ORDER_VIEW, new QStandardItem(orderreview));
        m_show->setItem(i, ORDER_EDAT, new QStandardItem(orderedate));
        m_show->setItem(i, ORDER_STTS, new QStandardItem(status));
    }
}

void OrderManagement::addOrder()
{
    max_order_id++;
    QJsonObject obj;
    obj.insert("erp_orderscid",max_order_id);
    obj.insert("erp_ordernumber",m_add->item(ORDER_ODDS,1)->text());
    obj.insert("erp_orderdate",m_add->item(ORDER_DATE,1)->text());
    obj.insert("erp_ordersale",m_add->item(ORDER_SALE,1)->text());
    obj.insert("erp_ordercustomer",m_add->item(ORDER_CSTM,1)->text());
    obj.insert("erp_orderquantity",m_add->item(ORDER_QUAN,1)->text().toInt());
    obj.insert("erp_orderreview",m_add->item(ORDER_VIEW,1)->text());
    obj.insert("erp_orderedate",m_add->item(ORDER_EDAT,1)->text());
    QJsonArray array_add;
    array_add.append(obj);

    QJsonObject objs;
    objs.insert("add",array_add);

    QJsonArray array_adds;
    array_adds.append(objs);

    QByteArray order_byte = QJsonDocument(array_adds).toJson();
    QUrl url;
    url.setQuery("order");
    url.setFragment(order_byte.toBase64());
    emit sendSocket(url);

    refresh();
}

void OrderManagement::delOrder()
{
    int row = tab_show->currentIndex().row();
    m_show->removeRow(row);

    QJsonArray array_add;
    array_add.append(json_show.at(row));

    QJsonObject objs;
    objs.insert("del",array_add);

    QJsonArray array_adds;
    array_adds.append(objs);

    QByteArray order_byte = QJsonDocument(array_adds).toJson();
    QUrl url;
    url.setQuery("order");
    url.setFragment(order_byte.toBase64());
    emit sendSocket(url);

    refresh();
}

void OrderManagement::uptOrder()
{
    int row = tab_show->currentIndex().row();
    QJsonObject obj = json_show.at(row).toObject();
    obj.insert("erp_ordernumber",m_add->item(ORDER_ODDS,1)->text());
    obj.insert("erp_orderdate",m_add->item(ORDER_DATE,1)->text());
    obj.insert("erp_ordersale",m_add->item(ORDER_SALE,1)->text());
    obj.insert("erp_ordercustomer",m_add->item(ORDER_CSTM,1)->text());
    obj.insert("erp_orderquantity",m_add->item(ORDER_QUAN,1)->text().toInt());
    obj.insert("erp_orderreview",m_add->item(ORDER_VIEW,1)->text());
    obj.insert("erp_orderedate",m_add->item(ORDER_EDAT,1)->text());
    QJsonArray array_add;
    array_add.append(obj);

    QJsonObject objs;
    objs.insert("upt",array_add);

    QJsonArray array_adds;
    array_adds.append(objs);

    QByteArray order_byte = QJsonDocument(array_adds).toJson();
    QUrl url;
    url.setQuery("order");
    url.setFragment(order_byte.toBase64());
    emit sendSocket(url);

    refresh();
}

void OrderManagement::sndOrder()
{
    //待添加
}

void OrderManagement::tabSync(QModelIndex index)
{
    QJsonObject obj = json_show.at(index.row()).toObject();
    m_add->item(ORDER_ODDS,1)->setText(obj.value("erp_ordernumber").toString());
    m_add->item(ORDER_DATE,1)->setText(obj.value("erp_orderdate").toString());
    m_add->item(ORDER_SALE,1)->setText(obj.value("erp_ordersale").toString());
    m_add->item(ORDER_CSTM,1)->setText(obj.value("erp_ordercustomer").toString());
    m_add->item(ORDER_QUAN,1)->setText(QString::number(obj.value("erp_orderquantity").toInt()));
    m_add->item(ORDER_VIEW,1)->setText(obj.value("erp_orderreview").toString());
    m_add->item(ORDER_EDAT,1)->setText(obj.value("erp_orderedate").toString());
}

void OrderManagement::recvSocket(QUrl url)
{
    QString cmd = url.query();
    QString usr = url.userName();
    if (usr != "ordermanagement")
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
        sales->setItemHeaders(items);
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

void OrderManagement::showAdd()
{
    if (w_add->isHidden() && w_snd->isHidden()) {
        w_add->show();
        btn_add->setIcon(QIcon(":/icons/right.png"));
        btn_snd->setIcon(QIcon(":/icons/right.png"));
        QString dat = QDate::currentDate().toString("yyyyMMdd");
        int max = 0;
        for (int i=0; i < json_show.size(); i++) {
            QJsonObject obj = json_show.at(i).toObject();
            QString odd = obj.value("erp_ordernumber").toString();
            if (odd.contains(dat)) {
                int num = odd.remove(dat).toInt();
                if (max < num)
                    max = num;
            }
        }
        QString odds = QString("%1%2").arg(dat).arg(max+1,3,10,QLatin1Char('0'));
        m_add->item(ORDER_ODDS,1)->setText(odds);
        m_add->item(ORDER_DATE,1)->setText(dat);

        if (json_customer.isEmpty()) {
            QUrl url;
            url.setQuery("customerinfo");
            emit sendSocket(url);
        }
        if (json_sale.isEmpty()) {
            QUrl url;
            url.setQuery("saleinfo");
            emit sendSocket(url);
        }
    } else {
        w_add->hide();
        w_snd->hide();
        btn_add->setIcon(QIcon(":/icons/left.png"));
        btn_snd->setIcon(QIcon(":/icons/left.png"));
    }
}

void OrderManagement::showSnd()
{
    if (w_add->isHidden() && w_snd->isHidden()) {
        w_snd->show();
        btn_add->setIcon(QIcon(":/icons/right.png"));
        btn_snd->setIcon(QIcon(":/icons/right.png"));
    } else {
        w_add->hide();
        w_snd->hide();
        btn_add->setIcon(QIcon(":/icons/left.png"));
        btn_snd->setIcon(QIcon(":/icons/left.png"));
    }
}

void OrderManagement::refresh()
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

void OrderManagement::showEvent(QShowEvent *e)
{
    refresh();
    e->accept();
}

