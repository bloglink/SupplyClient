#include "purchasemanagement.h"

PurchaseManagement::PurchaseManagement(QWidget *parent) : QWidget(parent)
{
initUI();
}

void PurchaseManagement::initUI()
{
    this->setObjectName("purchasemanagement");

    QStringList prep_header;
    prep_header << tr("评审单号") << tr("订单单号") << tr("下单日期") << tr("客户名称")
                << tr("发货日期") << tr("生产状态");
    m_prep = new StandardItemModel();
    m_prep->setHorizontalHeaderLabels(prep_header);
    tab_prep = new QTableView(this);//订单显示
    tab_prep->setModel(m_prep);
    tab_prep->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_prep->horizontalHeader()->setHighlightSections(false);
    connect(tab_prep,SIGNAL(clicked(QModelIndex)),this,SLOT(tabPrepSync(QModelIndex)));

    QStringList order_header;
    order_header << tr("客户名称") << tr("计划单号") << tr("物料编码") << tr("物料名称")
                 << tr("产品规格") << tr("物料单位") << tr("缺料数量") << tr("订货数量")
                 << tr("订货时间") << tr("外出采购") << tr("预计日期") << tr("实际日期")
                 << tr("到货数量") << tr("在途数量") << tr("返修数量") << tr("备注");
    m_plan = new StandardItemModel();
    m_plan->setHorizontalHeaderLabels(order_header);
    tab_plan = new QTableView(this);//订单显示
    tab_plan->setModel(m_plan);
    tab_plan->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_plan->horizontalHeader()->setHighlightSections(false);
    connect(tab_plan,SIGNAL(clicked(QModelIndex)),this,SLOT(tabPlanSync(QModelIndex)));

    QPushButton *order_qry = new QPushButton(this);
    order_qry->setFlat(true);
    order_qry->setMinimumSize(97,44);
    order_qry->setText(tr("缺料查询"));
    order_qry->setFocusPolicy(Qt::NoFocus);
    connect(order_qry,SIGNAL(clicked(bool)),this,SLOT(refresh()));

    QHBoxLayout *datLayout = new QHBoxLayout;
    datLayout->addStretch();
    datLayout->addWidget(order_qry);

    QSplitter *pSpliter = new QSplitter(Qt::Vertical);
    pSpliter->addWidget(tab_prep);
    pSpliter->addWidget(tab_plan);
    pSpliter->setStretchFactor(0,2);
    pSpliter->setStretchFactor(1,8);

    QWidget *w_order = new QWidget(this);//订单显示页面
    QVBoxLayout *ord_layout = new QVBoxLayout;
    ord_layout->addWidget(pSpliter);
    ord_layout->addLayout(datLayout);
    w_order->setLayout(ord_layout);

    btn_add = new QToolButton(this);//显示订单
    btn_add->setIcon(QIcon(":/icons/left.png"));
    btn_add->setText(tr("添\n加\n采\n购"));
    btn_add->setIconSize(QSize(30,30));
    btn_add->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn_add->setFocusPolicy(Qt::NoFocus);
    connect(btn_add,SIGNAL(clicked(bool)),this,SLOT(showAdd()));

    btn_snd = new QToolButton(this);//显示发货
    btn_snd->setIcon(QIcon(":/icons/left.png"));
    btn_snd->setIconSize(QSize(30,30));
    btn_snd->setFocusPolicy(Qt::NoFocus);
    btn_snd->setText(tr("修\n改\n状\n态"));
    btn_snd->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_snd,SIGNAL(clicked(bool)),this,SLOT(showSnd()));

    QVBoxLayout *extLayout = new QVBoxLayout;//隐藏/显示按钮布局
    extLayout->addWidget(btn_add);
    extLayout->addWidget(btn_snd);
    extLayout->addStretch();

    QStringList add_items;
    add_items << tr("客户名称") << tr("计划单号") << tr("物料编码") << tr("物料名称")
              << tr("产品规格") << tr("物料单位") << tr("缺料数量") << tr("订货数量");
    QStringList add_header;
    add_header << tr("项目") << tr("参数");
    m_add = new StandardItemModel();
    m_add->setHorizontalHeaderLabels(add_header);
    for (int i=0; i < add_items.size(); i++) {
        m_add->setItem(i,0,new QStandardItem(add_items.at(i)));
        m_add->setItem(i,1,new QStandardItem(""));
    }

    tab_add = new QTableView(this);
    tab_add->setModel(m_add);
    tab_add->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_add->horizontalHeader()->setHighlightSections(false);
    tab_add->setItemDelegateForColumn(0, new ReadOnlyDelegate);

    QPushButton *order_add = new QPushButton(this);
    order_add->setFlat(true);
    order_add->setMinimumSize(97,44);
    order_add->setText(tr("添加采购"));
    order_add->setFocusPolicy(Qt::NoFocus);
    connect(order_add,SIGNAL(clicked(bool)),this,SLOT(addPlan()));

    QHBoxLayout *btnoLayout = new QHBoxLayout;
    btnoLayout->addStretch();
    btnoLayout->addWidget(order_add);

    w_add = new QWidget(this);
    QVBoxLayout *add_layout = new QVBoxLayout;
    w_add->setLayout(add_layout);
    add_layout->addWidget(tab_add);
    add_layout->addLayout(btnoLayout);

    QStringList snd_items;
    snd_items << tr("客户名称") << tr("计划单号") << tr("物料编码") << tr("物料名称")
              << tr("产品规格") << tr("物料单位") << tr("缺料数量") << tr("订货数量")
              << tr("订货时间") << tr("外出采购") << tr("预计日期") << tr("实际日期")
              << tr("到货数量") << tr("在途数量") << tr("返修数量") << tr("备注");
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
    order_snd->setText(tr("修改状态"));
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

void PurchaseManagement::initPlan()
{
    m_plan->setRowCount(0);
    max_plan_id = 0;
    for (int i=0; i < json_plan.size(); i++) {
        QJsonObject obj = json_plan.at(i).toObject();
        int id   = obj.value("erp_productid").toInt();//ID
        QString customer = obj.value("erp_ordercustomer").toString();//客户名称
        QString plannumber = obj.value("erp_plannumber").toString();//计划单号
        QString lacknumber = obj.value("erp_lacknumber").toString();//物料编号
        QString lackname = obj.value("erp_lackname").toString();//物料名称
        QString lacktype = obj.value("erp_lacktype").toString();//物料规格
        QString lackunit = obj.value("erp_lackunit").toString();//物料单位
        QString lacknum = QString::number(obj.value("erp_lacknum").toInt());//缺料数量
        QString lackenum = QString::number(obj.value("erp_lackenum").toInt());//订货数量
        QString lackdate = obj.value("erp_lackdate").toString();//录入日期

        QString lackmask = obj.value("erp_lackmask").toString();//物料备注
        //        QString producttype = obj.value("erp_ordernumber").toString();//订单号
        //        QString productcode = obj.value("erp_orderreview").toString();//评审单号
        //        QString productname = obj.value("erp_lackstat").toString();//缺料状态

        //        QString productmodel = obj.value("erp_productmodel").toString();//产品规格
        //        QString orderreview = obj.value("erp_orderreview").toString();//评审单号

        if (id > max_plan_id)
            max_plan_id = id;
        m_plan->setItem(i, PLAN_B_CSTM, new QStandardItem(customer));
        m_plan->setItem(i, PLAN_B_CODE, new QStandardItem(plannumber));
        m_plan->setItem(i, PLAN_B_PCOD, new QStandardItem(lacknumber));
        m_plan->setItem(i, PLAN_B_NAME, new QStandardItem(lackname));
        m_plan->setItem(i, PLAN_B_TYPE, new QStandardItem(lacktype));
        m_plan->setItem(i, PLAN_B_UNIT, new QStandardItem(lackunit));
        m_plan->setItem(i, PLAN_B_QUAN, new QStandardItem(lacknum));
        m_plan->setItem(i, PLAN_B_OQUA, new QStandardItem(lackenum));
        m_plan->setItem(i, PLAN_B_DATE, new QStandardItem(lackdate));
        //        m_plan->setItem(i, PLAN_B_IOUT, new QStandardItem(orderdate));
        //        m_plan->setItem(i, PLAN_B_PDAT, new QStandardItem(ordersale));
        //        m_plan->setItem(i, PLAN_B_EDAT, new QStandardItem(ordersale));
        //        m_plan->setItem(i, PLAN_B_TQUA, new QStandardItem(customer));
        //        m_plan->setItem(i, PLAN_B_PQUA, new QStandardItem(orderedate));
        //        m_plan->setItem(i, PLAN_B_BQUA, new QStandardItem(status));
        m_plan->setItem(i, PLAN_B_MARK, new QStandardItem(lackmask));
    }
}

void PurchaseManagement::addPlan()
{
    QJsonObject obj;
    max_plan_id++;
    obj.insert("erp_productid",max_plan_id);
    obj.insert("erp_ordercustomer",m_add->item(PLAN_B_CSTM,1)->text());
    obj.insert("erp_plannumber",m_add->item(PLAN_B_CODE,1)->text());
    obj.insert("erp_lacknumber",m_add->item(PLAN_B_PCOD,1)->text());
    obj.insert("erp_lackname",m_add->item(PLAN_B_NAME,1)->text());
    obj.insert("erp_lacktype",m_add->item(PLAN_B_TYPE,1)->text());
    obj.insert("erp_lackunit",m_add->item(PLAN_B_UNIT,1)->text());
    obj.insert("erp_lacknum",m_add->item(PLAN_B_QUAN,1)->text().toInt());
    obj.insert("erp_lackenum",m_add->item(PLAN_B_OQUA,1)->text().toInt());
    QJsonArray array_add;
    array_add.append(obj);

    QJsonObject objs;
    objs.insert("add",array_add);

    QJsonArray array_adds;
    array_adds.append(objs);

    QByteArray order_byte = QJsonDocument(array_adds).toJson();
    QUrl url;
    url.setQuery("buyplan");
    url.setFragment(order_byte.toBase64());

    emit sendSocket(url);

    refresh();
}

void PurchaseManagement::delOrder()
{
    //    int row = tab_plan->currentIndex().row();
    //    m_plan->removeRow(row);

    //    QJsonArray array_add;
    //    array_add.append(json_prep.at(row));

    //    QJsonObject objs;
    //    objs.insert("del",array_add);

    //    QJsonArray array_adds;
    //    array_adds.append(objs);

    //    QByteArray order_byte = QJsonDocument(array_adds).toJson();
    //    QUrl url;
    //    url.setQuery("order");
    //    url.setFragment(order_byte.toBase64());
    //    emit sendSocket(url);

    //    refresh();
}

void PurchaseManagement::uptOrder()
{
    //    int row = tab_plan->currentIndex().row();
    //    QJsonObject obj = json_prep.at(row).toObject();
    //    obj.insert("erp_orderreview",m_add->item(PLAN_O_VIEW,1)->text());
    //    obj.insert("erp_pas",m_add->item(PLAN_O_QUAN,1)->text().toInt());

    //    obj.insert("erp_ordernumber",m_add->item(PLAN_O_ODDS,1)->text());
    //    obj.insert("erp_orderdate",m_add->item(PLAN_O_DATE,1)->text());
    //    obj.insert("erp_ordersale",m_add->item(PLAN_O_SALE,1)->text());
    //    obj.insert("erp_ordercustomer",m_add->item(PLAN_O_CSTM,1)->text());

    //    obj.insert("erp_orderedate",m_add->item(PLAN_O_EDAT,1)->text());
    //    QJsonArray array_add;
    //    array_add.append(obj);

    //    QJsonObject objs;
    //    objs.insert("upt",array_add);

    //    QJsonArray array_adds;
    //    array_adds.append(objs);

    //    QByteArray order_byte = QJsonDocument(array_adds).toJson();
    //    QUrl url;
    //    url.setQuery("order");
    //    url.setFragment(order_byte.toBase64());
    //    emit sendSocket(url);

    //    refresh();
}

void PurchaseManagement::sndOrder()
{
    //待添加
}

void PurchaseManagement::tabPlanSync(QModelIndex index)
{
    QJsonObject obj = json_plan.at(index.row()).toObject();
    m_add->item(PLAN_B_CSTM,1)->setText(obj.value("erp_ordercustomer").toString());
    m_add->item(PLAN_B_CODE,1)->setText(obj.value("erp_plannumber").toString());
    m_add->item(PLAN_B_PCOD,1)->setText(obj.value("erp_lacknumber").toString());
    m_add->item(PLAN_B_NAME,1)->setText(obj.value("erp_lackname").toString());
    m_add->item(PLAN_B_TYPE,1)->setText(obj.value("erp_lacktype").toString());
    m_add->item(PLAN_B_UNIT,1)->setText(obj.value("erp_lackunit").toString());
    m_add->item(PLAN_B_QUAN,1)->setText(QString::number(obj.value("erp_lacknum").toInt()));
    m_add->item(PLAN_B_OQUA,1)->setText(QString::number(obj.value("erp_lackenum").toInt()));

    m_snd->item(PLAN_B_CSTM,1)->setText(obj.value("erp_ordercustomer").toString());
    m_snd->item(PLAN_B_CODE,1)->setText(obj.value("erp_plannumber").toString());
    m_snd->item(PLAN_B_PCOD,1)->setText(obj.value("erp_lacknumber").toString());
    m_snd->item(PLAN_B_NAME,1)->setText(obj.value("erp_lackname").toString());
    m_snd->item(PLAN_B_TYPE,1)->setText(obj.value("erp_lacktype").toString());
    m_snd->item(PLAN_B_UNIT,1)->setText(obj.value("erp_lackunit").toString());
    m_snd->item(PLAN_B_QUAN,1)->setText(QString::number(obj.value("erp_lacknum").toInt()));
    m_snd->item(PLAN_B_OQUA,1)->setText(QString::number(obj.value("erp_lackenum").toInt()));
}

void PurchaseManagement::tabPrepSync(QModelIndex index)
{

}

void PurchaseManagement::recvSocket(QUrl url)
{
    QString cmd = url.query();
    QString usr = url.userName();
    if (usr != "purchasemanagement")
        return;
    QByteArray byte = QByteArray::fromBase64(url.fragment().toUtf8());
    if (cmd == "buyinfo") {
        json_plan = QJsonDocument::fromJson(byte).array();
        qDebug() << json_plan;
        initPlan();
    } else {
        qDebug() << "recv others" << url.toString();
    }
}

void PurchaseManagement::showAdd()
{
    if (w_add->isHidden()) {
        w_add->show();
        w_snd->hide();
        btn_add->setIcon(QIcon(":/icons/right.png"));
        btn_snd->setIcon(QIcon(":/icons/right.png"));
    } else {
        w_add->hide();
        w_snd->hide();
        btn_add->setIcon(QIcon(":/icons/left.png"));
        btn_snd->setIcon(QIcon(":/icons/left.png"));
    }
}

void PurchaseManagement::showSnd()
{
    if (w_snd->isHidden()) {
        w_add->hide();
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

void PurchaseManagement::refresh()
{
    QUrl url;
    url.setQuery("buyinfo");
    emit sendSocket(url);
}

void PurchaseManagement::showEvent(QShowEvent *e)
{
    refresh();
    e->accept();
}
