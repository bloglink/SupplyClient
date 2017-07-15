#include "productionmanagement.h"

ProductionManagement::ProductionManagement(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void ProductionManagement::initUI()
{
    this->setObjectName("productionmanagement");

    QStringList prep_header;
    prep_header << tr("评审单号") << tr("订单数量") << tr("订单单号") << tr("录入日期")
                << tr("业务经理") << tr("客户名称") << tr("发货日期") << tr("订单状态");
    m_prep = new StandardItemModel();
    m_prep->setHorizontalHeaderLabels(prep_header);
    tab_prep = new QTableView(this);//订单显示
    tab_prep->setModel(m_prep);
    tab_prep->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_prep->horizontalHeader()->setHighlightSections(false);
    connect(tab_prep,SIGNAL(clicked(QModelIndex)),this,SLOT(tabPrepSync(QModelIndex)));

    QStringList order_header;
    order_header << tr("评审单号") << tr("订单数量") << tr("生产单号") << tr("产品大类")
                 << tr("产品编号") << tr("产品名称") << tr("产品规格")
                 << tr("订单单号") << tr("下单日期") << tr("业务经理") << tr("客户名称")
                 << tr("发货日期") << tr("生产状态") ;
    m_plan = new StandardItemModel();
    m_plan->setHorizontalHeaderLabels(order_header);
    tab_plan = new QTableView(this);//订单显示
    tab_plan->setModel(m_plan);
    tab_plan->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_plan->horizontalHeader()->setHighlightSections(false);
    tab_plan->hideColumn(PLAN_P_QUAN);

    QPushButton *order_qry = new QPushButton(this);
    order_qry->setFlat(true);
    order_qry->setMinimumSize(97,44);
    order_qry->setText(tr("生产查询"));
    order_qry->setFocusPolicy(Qt::NoFocus);
    connect(order_qry,SIGNAL(clicked(bool)),this,SLOT(refresh()));

    QPushButton *order_mat = new QPushButton(this);
    order_mat->setFlat(true);
    order_mat->setMinimumSize(97,44);
    order_mat->setText(tr("匹配生产"));
    order_mat->setFocusPolicy(Qt::NoFocus);
    connect(order_mat,SIGNAL(clicked(bool)),this,SLOT(matchPlan()));

    QPushButton *order_cnl = new QPushButton(this);
    order_cnl->setFlat(true);
    order_cnl->setMinimumSize(97,44);
    order_cnl->setText(tr("取消匹配"));
    order_cnl->setFocusPolicy(Qt::NoFocus);
    connect(order_cnl,SIGNAL(clicked(bool)),this,SLOT(matchCancel()));

    QHBoxLayout *datLayout = new QHBoxLayout;
    datLayout->addStretch();
    datLayout->addWidget(order_qry);
    datLayout->addWidget(order_mat);
    datLayout->addWidget(order_cnl);

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
    add_items << tr("评审单号") << tr("订单数量") << tr("生产单号") << tr("产品大类")
              << tr("产品编号") << tr("产品名称") << tr("产品规格");
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
    order_add->setText(tr("添加计划"));
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

void ProductionManagement::initPrep()
{
    m_prep->setRowCount(0);
    for (int i=0; i < json_prep.size(); i++) {
        QJsonObject obj = json_prep.at(i).toObject();
        QString ordernumber = obj.value("erp_ordernumber").toString();//订单单号
        QString orderdate = obj.value("erp_orderdate").toString();//订单日期
        QString ordersale = obj.value("erp_ordersale").toString();//业务经理
        QString customer = obj.value("erp_ordercustomer").toString();//客户名称
        int orderquantity = obj.value("erp_pas").toInt();//订单数量
        QString orderreview = obj.value("erp_orderreview").toString();//评审单号
        QString orderedate = obj.value("erp_orderedate").toString();//要求发货日期
        QString status = obj.value("erp_orderstat").toString();//订单状态
        m_prep->setItem(i, PLAN_O_ODDS, new QStandardItem(ordernumber));
        m_prep->setItem(i, PLAN_O_DATE, new QStandardItem(orderdate));
        m_prep->setItem(i, PLAN_O_SALE, new QStandardItem(ordersale));
        m_prep->setItem(i, PLAN_O_CSTM, new QStandardItem(customer));
        m_prep->setItem(i, PLAN_O_QUAN, new QStandardItem(QString::number(orderquantity)));
        m_prep->setItem(i, PLAN_O_VIEW, new QStandardItem(orderreview));
        m_prep->setItem(i, PLAN_O_EDAT, new QStandardItem(orderedate));
        m_prep->setItem(i, PLAN_O_STTS, new QStandardItem(status));
    }
}

void ProductionManagement::initPlan()
{
    m_plan->setRowCount(0);
    max_plan_id = 0;
    for (int i=0; i < json_plan.size(); i++) {
        QJsonObject obj = json_plan.at(i).toObject();
        int id   = obj.value("erp_productid").toInt();//ID
        QString ordernumber = obj.value("erp_ordernumber").toString();//订单单号
        QString orderdate = obj.value("erp_orderdate").toString();//订单日期
        QString ordersale = obj.value("erp_ordersale").toString();//业务经理
        QString customer = obj.value("erp_ordercustomer").toString();//客户名称

        QString orderedate = obj.value("erp_orderedate").toString();//要求发货日期
        QString status = obj.value("erp_orderstat").toString();//订单状态

        QString productcontact = obj.value("erp_productcontact").toString();//生产单号
        QString producttype = obj.value("erp_producttype").toString();//产品大类
        QString productcode = obj.value("erp_productcode").toString();//产品编号
        QString productname = obj.value("erp_productname").toString();//产品名称
        QString productmodel = obj.value("erp_productmodel").toString();//产品规格

        QString orderreview = obj.value("erp_orderreview").toString();//评审单号

        if (id > max_plan_id)
            max_plan_id = id;
        m_plan->setItem(i, PLAN_P_VIEW, new QStandardItem(orderreview));
        m_plan->setItem(i, PLAN_P_QUAN, new QStandardItem("1"));
        m_plan->setItem(i, PLAN_P_ODDM, new QStandardItem(productcontact));
        m_plan->setItem(i, PLAN_P_TYPE, new QStandardItem(producttype));
        m_plan->setItem(i, PLAN_P_CODE, new QStandardItem(productcode));
        m_plan->setItem(i, PLAN_P_NAME, new QStandardItem(productname));
        m_plan->setItem(i, PLAN_P_SPEC, new QStandardItem(productmodel));
        m_plan->setItem(i, PLAN_P_ODDS, new QStandardItem(ordernumber));
        m_plan->setItem(i, PLAN_P_DATE, new QStandardItem(orderdate));
        m_plan->setItem(i, PLAN_P_SALE, new QStandardItem(ordersale));
        m_plan->setItem(i, PLAN_P_CSTM, new QStandardItem(customer));
        m_plan->setItem(i, PLAN_P_EDAT, new QStandardItem(orderedate));
        m_plan->setItem(i, PLAN_P_STTS, new QStandardItem(status));
    }
}

void ProductionManagement::matchPlan()
{
    int row = tab_prep->currentIndex().row();
    QJsonArray array_upt;
    if (row >= 0) {
        for (int i=0; i < m_plan->rowCount(); i++) {
            if (!m_plan->item(i,PLAN_P_ODDS)->text().isEmpty())
                continue;
            QString orderreview = m_plan->item(i,PLAN_P_VIEW)->text();
            if (orderreview == m_prep->item(row,PLAN_O_VIEW)->text()) {
                int q = m_prep->item(row,PLAN_O_QUAN)->text().toInt();
                if (q == 0)
                    continue;
                q--;
                m_prep->item(row,PLAN_O_QUAN)->setText(QString::number(q));
                QJsonObject obj = json_plan.at(i).toObject();

                obj.insert("erp_ordernumber",m_prep->item(row,PLAN_O_ODDS)->text());
                obj.insert("erp_orderdate",m_prep->item(row,PLAN_O_DATE)->text());
                obj.insert("erp_ordersale",m_prep->item(row,PLAN_O_SALE)->text());
                obj.insert("erp_ordercustomer",m_prep->item(row,PLAN_O_CSTM)->text());
                obj.insert("erp_orderedate",m_prep->item(row,PLAN_O_EDAT)->text());
                obj.insert("erp_orderstat",tr("缺料"));

                array_upt.append(obj);
            }
        }
    } else {
        for (int i=0; i < m_plan->rowCount(); i++) {
            if (!m_plan->item(i,PLAN_P_ODDS)->text().isEmpty())
                continue;
            QString orderreview = m_plan->item(i,PLAN_P_VIEW)->text();
            for (int j=0; j < m_prep->rowCount(); j++) {
                if (orderreview == m_prep->item(j,PLAN_O_VIEW)->text()) {
                    int q = m_prep->item(j,PLAN_O_QUAN)->text().toInt();
                    if (q == 0)
                        continue;
                    q--;
                    m_prep->item(j,PLAN_O_QUAN)->setText(QString::number(q));
                    QJsonObject obj = json_plan.at(i).toObject();

                    obj.insert("erp_ordernumber",m_prep->item(j,PLAN_O_ODDS)->text());
                    obj.insert("erp_orderdate",m_prep->item(j,PLAN_O_DATE)->text());
                    obj.insert("erp_ordersale",m_prep->item(j,PLAN_O_SALE)->text());
                    obj.insert("erp_ordercustomer",m_prep->item(j,PLAN_O_CSTM)->text());
                    obj.insert("erp_orderedate",m_prep->item(j,PLAN_O_EDAT)->text());

                    array_upt.append(obj);
                    break;
                }
            }
        }
    }
    QJsonObject objs;
    objs.insert("upt",array_upt);

    QJsonArray array_upts;
    array_upts.append(objs);

    QByteArray order_byte = QJsonDocument(array_upts).toJson();

    QUrl url;
    url.setQuery("pdplan");
    url.setFragment(order_byte.toBase64());
    emit sendSocket(url);

    QJsonArray array_upt1;
    for (int i=0; i < m_prep->rowCount(); i++) {
        QJsonObject obj = json_prep.at(i).toObject();
        if (obj.value("erp_pas").toInt() != m_prep->item(i,PLAN_O_QUAN)->text().toInt()) {
            obj.insert("erp_pas",m_prep->item(i,PLAN_O_QUAN)->text().toInt());
            array_upt1.append(obj);
        }
    }
    QJsonObject objs1;
    objs1.insert("upt1",array_upt1);

    QJsonArray array_upts1;
    array_upts1.append(objs1);

    QByteArray order_byte1 = QJsonDocument(array_upts1).toJson();

    QUrl url1;
    url1.setQuery("order");
    url1.setFragment(order_byte1.toBase64());
    emit sendSocket(url1);

    refresh();
}

void ProductionManagement::matchCancel()
{
    int row = tab_plan->currentIndex().row();
    if (row < 0)
        return;

    QJsonObject obj = json_plan.at(row).toObject();

    obj.insert("erp_ordernumber","");
    obj.insert("erp_orderdate","");
    obj.insert("erp_ordersale","");
    obj.insert("erp_ordercustomer","");
    obj.insert("erp_orderedate","");
    QJsonArray array_upt;
    array_upt.append(obj);

    QJsonObject objs;
    objs.insert("upt",array_upt);

    QJsonArray array_upts;
    array_upts.append(objs);

    QByteArray order_byte = QJsonDocument(array_upts).toJson();

    QUrl url;
    url.setQuery("pdplan");
    url.setFragment(order_byte.toBase64());
    emit sendSocket(url);


    QString orderreview = m_plan->item(row,PLAN_P_ODDS)->text();
    int erp_pas = 0;

    for (int i=0; i < m_prep->rowCount(); i++) {
        if (orderreview == m_prep->item(i,PLAN_O_ODDS)->text()) {
            erp_pas = m_prep->item(i,PLAN_O_QUAN)->text().toInt();
            erp_pas++;
            m_prep->item(i,PLAN_O_QUAN)->setText(QString::number(erp_pas));
        }
    }
    if (erp_pas == 0)
        erp_pas = 1;
    QJsonObject obj1;
    obj1.insert("erp_ordernumber",orderreview);
    obj1.insert("erp_pas",erp_pas);
    QJsonArray array_upt1;
    array_upt1.append(obj1);
    QJsonObject objs1;
    objs1.insert("upt1",array_upt1);

    QJsonArray array_upts1;
    array_upts1.append(objs1);

    QByteArray order_byte1 = QJsonDocument(array_upts1).toJson();

    QUrl url1;
    url1.setQuery("order");
    url1.setFragment(order_byte1.toBase64());
    emit sendSocket(url1);
    refresh();
}

void ProductionManagement::addPlan()
{
    QJsonObject obj;
    obj.insert("erp_orderreview",m_add->item(PLAN_O_VIEW,1)->text());
    obj.insert("erp_productcontact",m_add->item(PLAN_O_ODDS,1)->text());
    obj.insert("erp_producttype",m_add->item(PLAN_P_TYPE,1)->text());
    obj.insert("erp_productcode",m_add->item(PLAN_P_CODE,1)->text());
    obj.insert("erp_productname",m_add->item(PLAN_P_NAME,1)->text());
    obj.insert("erp_productmodel",m_add->item(PLAN_P_SPEC,1)->text());

    QJsonArray array_add;
    int quan = m_add->item(PLAN_O_QUAN,1)->text().toInt();
    for (int i=0; i < quan; i++) {
        max_plan_id++;
        obj.insert("erp_productid",max_plan_id);
        array_add.append(obj);
    }

    QJsonObject objs;
    objs.insert("add",array_add);

    QJsonArray array_adds;
    array_adds.append(objs);
    array_adds.append(objs);

    QByteArray order_byte = QJsonDocument(array_adds).toJson();
    QUrl url;
    url.setQuery("pdplan");
    url.setFragment(order_byte.toBase64());

    emit sendSocket(url);

    refresh();
}

void ProductionManagement::delOrder()
{
    int row = tab_plan->currentIndex().row();
    m_plan->removeRow(row);

    QJsonArray array_add;
    array_add.append(json_prep.at(row));

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

void ProductionManagement::uptOrder()
{
    int row = tab_plan->currentIndex().row();
    QJsonObject obj = json_prep.at(row).toObject();
    obj.insert("erp_orderreview",m_add->item(PLAN_O_VIEW,1)->text());
    obj.insert("erp_pas",m_add->item(PLAN_O_QUAN,1)->text().toInt());

    obj.insert("erp_ordernumber",m_add->item(PLAN_O_ODDS,1)->text());
    obj.insert("erp_orderdate",m_add->item(PLAN_O_DATE,1)->text());
    obj.insert("erp_ordersale",m_add->item(PLAN_O_SALE,1)->text());
    obj.insert("erp_ordercustomer",m_add->item(PLAN_O_CSTM,1)->text());

    obj.insert("erp_orderedate",m_add->item(PLAN_O_EDAT,1)->text());
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

void ProductionManagement::sndOrder()
{
    //待添加
}

void ProductionManagement::tabPlanSync(QModelIndex index)
{
    QJsonObject obj = json_prep.at(index.row()).toObject();
    m_prep->item(PLAN_O_ODDS,1)->setText(obj.value("erp_ordernumber").toString());
    m_prep->item(PLAN_O_DATE,1)->setText(obj.value("erp_orderdate").toString());
    m_prep->item(PLAN_O_SALE,1)->setText(obj.value("erp_ordersale").toString());
    m_prep->item(PLAN_O_CSTM,1)->setText(obj.value("erp_ordercustomer").toString());
    m_prep->item(PLAN_O_QUAN,1)->setText(QString::number(obj.value("erp_pas").toInt()));
    m_prep->item(PLAN_O_VIEW,1)->setText(obj.value("erp_orderreview").toString());
    m_prep->item(PLAN_O_EDAT,1)->setText(obj.value("erp_orderedate").toString());
}

void ProductionManagement::tabPrepSync(QModelIndex index)
{
    QJsonObject obj = json_prep.at(index.row()).toObject();
    m_add->item(PLAN_O_VIEW,1)->setText(obj.value("erp_orderreview").toString());
    m_add->item(PLAN_O_QUAN,1)->setText(QString::number(obj.value("erp_pas").toInt()));
}

void ProductionManagement::recvSocket(QUrl url)
{
    QString cmd = url.query();
    QString usr = url.userName();
    if (usr != "productionmanagement")
        return;
    QByteArray byte = QByteArray::fromBase64(url.fragment().toUtf8());
    if (cmd == "pdprepinfo") {
        json_prep = QJsonDocument::fromJson(byte).array();
        initPrep();
    } else if (cmd == "pdplaninfo") {
        json_plan = QJsonDocument::fromJson(byte).array();
        initPlan();
    } else {
        qDebug() << "recv others" << url.toString();
    }
}

void ProductionManagement::showAdd()
{
    if (w_add->isHidden() && w_snd->isHidden()) {
        w_add->show();
        btn_add->setIcon(QIcon(":/icons/right.png"));
        btn_snd->setIcon(QIcon(":/icons/right.png"));
    } else {
        w_add->hide();
        w_snd->hide();
        btn_add->setIcon(QIcon(":/icons/left.png"));
        btn_snd->setIcon(QIcon(":/icons/left.png"));
    }
}

void ProductionManagement::showSnd()
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

void ProductionManagement::refresh()
{
    QUrl url;
    url.setQuery("pdprepinfo");
    emit sendSocket(url);
    url.setQuery("pdplaninfo");
    emit sendSocket(url);
}

void ProductionManagement::showEvent(QShowEvent *e)
{
    refresh();
    e->accept();
}
