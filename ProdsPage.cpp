#include "ProdsPage.h"

ProdsPage::ProdsPage(QWidget *parent) : QWidget(parent)
{
    initUI();
    initSql();
}

ProdsPage::~ProdsPage()
{
    db.close();
}

void ProdsPage::initUI()
{
    this->setObjectName("ProdsPage");

    tab_order = new QTableView(this);
    tab_order->setItemDelegate(new ReadOnlyDelegate);
    tab_order->horizontalHeader()->setHighlightSections(false);
    tab_order->setSelectionBehavior(QAbstractItemView::SelectRows);
//    connect(tab_plans,SIGNAL(clicked(QModelIndex)),this,SLOT(tabPlanSync(QModelIndex)));

    tab_prods = new QTableView(this);
    tab_prods->setItemDelegate(new ReadOnlyDelegate);
    tab_prods->horizontalHeader()->setHighlightSections(false);
    tab_prods->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_prods,SIGNAL(clicked(QModelIndex)),this,SLOT(tabProdSync(QModelIndex)));

    QPushButton *prod_update = new QPushButton(this);
    prod_update->setFlat(true);
    prod_update->setMinimumSize(97,44);
    prod_update->setText(tr("刷新显示"));
    prod_update->setFocusPolicy(Qt::NoFocus);
    //    connect(prod_update,SIGNAL(clicked(bool)),this,SLOT(updateProds()));

    QPushButton *prod_match = new QPushButton(this);
    prod_match->setFlat(true);
    prod_match->setMinimumSize(97,44);
    prod_match->setText(tr("匹配生产"));
    prod_match->setFocusPolicy(Qt::NoFocus);
    connect(prod_match,SIGNAL(clicked(bool)),this,SLOT(matchProds()));

    QPushButton *prod_cancel = new QPushButton(this);
    prod_cancel->setFlat(true);
    prod_cancel->setMinimumSize(97,44);
    prod_cancel->setText(tr("取消匹配"));
    prod_cancel->setFocusPolicy(Qt::NoFocus);
    connect(prod_cancel,SIGNAL(clicked(bool)),this,SLOT(matchCancel()));

    QSplitter *pSpliter = new QSplitter(Qt::Vertical);
    pSpliter->addWidget(tab_order);
    pSpliter->addWidget(tab_prods);
    pSpliter->setStretchFactor(0,2);
    pSpliter->setStretchFactor(1,8);

    QGridLayout *sprodLayout = new QGridLayout;
    sprodLayout->addWidget(pSpliter,0,0,1,4);
    sprodLayout->addWidget(prod_update,1,1);
    sprodLayout->addWidget(prod_match,1,2);
    sprodLayout->addWidget(prod_cancel,1,3);
    sprodLayout->setColumnStretch(0,1);
    sprodLayout->setMargin(0);

    btn_prods = new QToolButton(this);//显示订单
    btn_prods->setIcon(QIcon(":/icons/left.png"));
    btn_prods->setIconSize(QSize(30,30));
    btn_prods->setFocusPolicy(Qt::NoFocus);
    btn_prods->setText(tr("生\n产\n管\n理"));
    btn_prods->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_prods,SIGNAL(clicked(bool)),this,SLOT(showTabProd()));

    QVBoxLayout *pbtnsLayout = new QVBoxLayout;
    pbtnsLayout->addWidget(btn_prods);
    pbtnsLayout->addStretch();

    prod_items << tr("编号") << tr("记录") << tr("操作") << tr("联络单号")
               << tr("生产数量") << tr("产品大类") << tr("产品编号")
               << tr("产品名称") << tr("产品规格") << tr("仪表编码");

    m_prod = new StandardItemModel();
    QStringList prod_header;
    prod_header << tr("项目") << tr("参数");
    m_prod->setHorizontalHeaderLabels(prod_header);
    for (int i=0; i < prod_items.size(); i++) {
        m_prod->setItem(i,0,new QStandardItem(prod_items.at(i)));
        m_prod->setItem(i,1,new QStandardItem(""));
    }
    tab_iprod = new QTableView(this);
    tab_iprod->setModel(m_prod);
    tab_iprod->verticalHeader()->setVisible(false);
    tab_iprod->setColumnWidth(0,100);
    tab_iprod->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_iprod->hideRow(PRODS_UUID);
    tab_iprod->hideRow(PRODS_GUID);
    tab_iprod->hideRow(PRODS_SIGN);

    QPushButton *prod_append = new QPushButton(this);
    prod_append->setFlat(true);
    prod_append->setMinimumSize(97,44);
    prod_append->setText(tr("添加计划"));
    prod_append->setFocusPolicy(Qt::NoFocus);
    connect(prod_append,SIGNAL(clicked(bool)),this,SLOT(appendPlans()));
    QPushButton *prod_delete = new QPushButton(this);
    prod_delete->setFlat(true);
    prod_delete->setMinimumSize(97,44);
    prod_delete->setText(tr("删除计划"));
    prod_delete->setFocusPolicy(Qt::NoFocus);
    connect(prod_delete,SIGNAL(clicked(bool)),this,SLOT(deletePlans()));
    QPushButton *prod_change = new QPushButton(this);
    prod_change->setFlat(true);
    prod_change->setMinimumSize(97,44);
    prod_change->setText(tr("修改计划"));
    prod_change->setFocusPolicy(Qt::NoFocus);
    connect(prod_change,SIGNAL(clicked(bool)),this,SLOT(changePlans()));

    QGridLayout *iprodLayout = new QGridLayout;
    iprodLayout->addWidget(tab_iprod,0,0,1,4);
    iprodLayout->addWidget(prod_append,1,1);
    iprodLayout->addWidget(prod_delete,1,2);
    iprodLayout->addWidget(prod_change,1,3);
    iprodLayout->setColumnStretch(0,1);
    iprodLayout->setMargin(0);

    prodWidget = new QWidget(this);
    prodWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    prodWidget->setLayout(iprodLayout);
    prodWidget->hide();

    QHBoxLayout *prodLayout = new QHBoxLayout;
    prodLayout->addLayout(sprodLayout);
    prodLayout->addLayout(pbtnsLayout);
    prodLayout->addWidget(prodWidget);
    prodLayout->setStretch(0,1);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(prodLayout);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(group);
    this->setLayout(mainLayout);
}

void ProdsPage::initSql()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "erp_prods");
    db.setDatabaseName("erp.db");
    db.open();

    sql_query = new SqlQueryModel(this);

    sql_order = new SqlQueryModel(this);
    updateOrder();

    order_items << tr("编号") << tr("记录") << tr("操作") << tr("订单单号")
                << tr("下单日期") << tr("客户名称")
                << tr("评审单号") << tr("订货数量") << tr("发货日期") << tr("备注内容");
    for (int i=0; i < order_items.size(); i++)
        sql_order->setHeaderData(i, Qt::Horizontal, order_items.at(i));
    tab_order->setModel(sql_order);
    tab_order->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_order->hideColumn(ORDER_UUID);
    tab_order->hideColumn(ORDER_GUID);
    tab_order->hideColumn(ORDER_SIGN);

    prod_items.clear();
    prod_items << tr("编号") << tr("记录") << tr("操作")
               << tr("订单单号") << tr("下单日期") << tr("客户名称")<< tr("评审单号") << tr("发货日期")
               << tr("联络单号") << tr("产品大类") << tr("产品编号")
               << tr("产品名称") << tr("产品规格") << tr("仪表编码");

    sql_prods = new SqlQueryModel(this);
    updateProds();
    for (int i=0; i < prod_items.size(); i++)
        sql_prods->setHeaderData(i, Qt::Horizontal, prod_items.at(i));
    tab_prods->setModel(sql_prods);
    tab_prods->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_prods->hideColumn(PRODS_UUID);
    tab_prods->hideColumn(PRODS_GUID);
    tab_prods->hideColumn(PRODS_SIGN);
}
void ProdsPage::showTabProd()
{
    if (prodWidget->isHidden()) {
        prodWidget->show();
        btn_prods->setIcon(QIcon(":/icons/right.png"));
    } else {
        prodWidget->hide();
        btn_prods->setIcon(QIcon(":/icons/left.png"));
    }

    for (int i=0; i < m_prod->rowCount(); i++) {
        m_prod->item(i,1)->setText("");
    }
}

void ProdsPage::autoNumber()
{
    QString dat = QDate::currentDate().toString("yyyyMMdd");
    int max = 0;
    for (int i=0; i < sql_order->rowCount(); i++) {
        QString number = sql_order->index(i,ORDER_NUMB).data().toString();
        if (number.contains(dat)) {
            int num = number.remove(dat).toInt();
            if (max < num)
                max = num;
        }
    }
    QString odds = QString("%1%2").arg(dat).arg(max+1,3,10,QLatin1Char('0'));
    m_prod->item(ORDER_NUMB,1)->setText(odds);
}

void ProdsPage::initData()
{
    //    sql_order->select();
    for (int i=0; i < sql_order->rowCount(); i++) {
        int quan = sql_order->index(i,ORDER_QUAN).data().toInt();
        int prod = sql_order->index(i,ORDER_PROD).data().toInt();
        int stck = sql_order->index(i,ORDER_STCK).data().toInt();
        int dnum = sql_order->index(i,ORDER_DNUM).data().toInt();
        if (quan == prod+stck+dnum)
            tab_order->hideRow(i);
        else
            tab_order->showRow(i);
    }
    //    sql_prod->select();
}

void ProdsPage::appendPlans()
{
    QJsonObject obj;
    obj.insert("command","erp_plans");
    obj.insert("plans_sign",1);
    obj.insert("plans_numb",m_prod->item(PRODS_NUMB,1)->text());
    obj.insert("plans_quan",m_prod->item(PRODS_QUAN,1)->text());
    obj.insert("plans_type",m_prod->item(PRODS_TYPE,1)->text());
    obj.insert("plans_code",m_prod->item(PRODS_CODE,1)->text());
    obj.insert("plans_name",m_prod->item(PRODS_NAME,1)->text());
    obj.insert("plans_mode",m_prod->item(PRODS_MODE,1)->text());
    obj.insert("plans_mnum",m_prod->item(PRODS_MNUM,1)->text());
    emit sendJson(obj);

    emit updateSql("erp_plans");
    emit updateSql("erp_prods");
}

void ProdsPage::deletePlans()
{
    QJsonObject obj;
    obj.insert("command","erp_plans");
    obj.insert("plans_sign",2);
    obj.insert("plans_uuid",m_prod->item(PRODS_UUID,1)->text().toDouble());
    obj.insert("plans_numb",m_prod->item(PRODS_NUMB,1)->text());
    obj.insert("plans_quan",m_prod->item(PRODS_QUAN,1)->text());
    obj.insert("plans_type",m_prod->item(PRODS_TYPE,1)->text());
    obj.insert("plans_code",m_prod->item(PRODS_CODE,1)->text());
    obj.insert("plans_name",m_prod->item(PRODS_NAME,1)->text());
    obj.insert("plans_mode",m_prod->item(PRODS_MODE,1)->text());
    obj.insert("plans_mnum",m_prod->item(PRODS_MNUM,1)->text());
    emit sendJson(obj);

    emit updateSql("erp_plans");
    emit updateSql("erp_prods");
}

void ProdsPage::changePlans()
{
    QJsonObject obj;
    obj.insert("command","erp_plans");
    obj.insert("plans_sign",3);
    obj.insert("plans_uuid",m_prod->item(PRODS_UUID,1)->text().toDouble());
    obj.insert("plans_numb",m_prod->item(PRODS_NUMB,1)->text());
    obj.insert("plans_quan",m_prod->item(PRODS_QUAN,1)->text());
    obj.insert("plans_type",m_prod->item(PRODS_TYPE,1)->text());
    obj.insert("plans_code",m_prod->item(PRODS_CODE,1)->text());
    obj.insert("plans_name",m_prod->item(PRODS_NAME,1)->text());
    obj.insert("plans_mode",m_prod->item(PRODS_MODE,1)->text());
    obj.insert("plans_mnum",m_prod->item(PRODS_MNUM,1)->text());
    emit sendJson(obj);

    emit updateSql("erp_plans");
    emit updateSql("erp_prods");
}

void ProdsPage::tabPlanSync(QModelIndex index)
{

}

void ProdsPage::tabProdSync(QModelIndex index)
{
    int prods_row = index.row();
    qint64 prods_uuid = sql_prods->data(sql_prods->index(prods_row,PRODS_UUID)).toDouble();
    QString cmd = tr("select prods_puid from erp_prods where prods_uuid=%1").arg(prods_uuid);
    sql_query->setQuery(cmd,db);
    qint64 plans_uuid = sql_query->data(sql_query->index(0,0)).toDouble();
    cmd = tr("select * from erp_plans where plans_uuid=%1").arg(plans_uuid);
    sql_query->setQuery(cmd,db);
    for (int i=0; i < m_prod->rowCount(); i++)
        m_prod->item(i,1)->setText(sql_query->index(0,i).data().toString());
}

void ProdsPage::matchProds()
{
    int order_row = tab_order->currentIndex().row();
    if (order_row < 0)
        return;
    int prods_row = tab_prods->currentIndex().row();
    if (prods_row < 0)
        return;
    qint64 order_uuid = sql_order->data(sql_order->index(order_row,ORDER_UUID)).toDouble();
    qint64 prods_uuid = sql_prods->data(sql_prods->index(prods_row,PRODS_UUID)).toDouble();

    QString cmd = tr("select prods_puid,prods_mnum from erp_prods where prods_uuid=%1").arg(prods_uuid);

    sql_query->setQuery(cmd,db);

    QJsonObject obj;
    obj.insert("command","erp_prods");
    obj.insert("prods_sign",3);
    obj.insert("prods_uuid",prods_uuid);
    obj.insert("prods_puid",sql_query->data(sql_query->index(0,0)).toDouble());
    obj.insert("prods_ouid",order_uuid);
    obj.insert("prods_mnum",sql_query->data(sql_query->index(0,1)).toString());

    emit sendJson(obj);

    emit updateSql("erp_plans");
    emit updateSql("erp_prods");
}

void ProdsPage::matchCancel()
{
    int prods_row = tab_prods->currentIndex().row();
    if (prods_row < 0)
        return;
    qint64 prods_uuid = sql_prods->data(sql_prods->index(prods_row,PRODS_UUID)).toDouble();

    QString cmd = tr("select prods_puid,prods_mnum from erp_prods where prods_uuid=%1").arg(prods_uuid);

    sql_query->setQuery(cmd,db);

    QJsonObject obj;
    obj.insert("command","erp_prods");
    obj.insert("prods_sign",3);
    obj.insert("prods_uuid",prods_uuid);
    obj.insert("prods_puid",sql_query->data(sql_query->index(0,0)).toDouble());
    obj.insert("prods_ouid",0);
    obj.insert("prods_mnum",sql_query->data(sql_query->index(0,1)).toString());

    emit sendJson(obj);

    emit updateSql("erp_plans");
    emit updateSql("erp_prods");
}

void ProdsPage::recvNetJson(QJsonObject obj)
{
    QString cmd = obj.value("command").toString();
    if (cmd == "erp_order") {
        updateOrder();
    }
    if (cmd == "erp_prods") {
        updateProds();
    }
}

void ProdsPage::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    emit updateSql("erp_custs");
    emit updateSql("erp_sales");
    emit updateSql("erp_order");
    emit updateSql("erp_plans");
    emit updateSql("erp_prods");
}

void ProdsPage::updateOrder()
{
    QString cmd = "select order_uuid,order_guid,order_sign,order_numb,order_date,";
    cmd += "custs_name,order_view,order_quan,order_dead,order_mark ";
    cmd += "from erp_order left join erp_custs on order_cust=custs_uuid ";
    cmd += "left join erp_sales on custs_sale=sales_uuid";
    sql_order->setQuery(cmd,db);
}

void ProdsPage::updateProds()
{
    QString cmd = "select prods_uuid,prods_guid,prods_sign,";
    cmd += "order_numb,order_date,custs_name,order_view,order_dead,";
    cmd += "plans_numb,plans_type,plans_code,plans_name,plans_mode,prods_mnum ";
    cmd += "from erp_prods left join erp_plans on prods_puid=plans_uuid ";
    cmd += "left join erp_order on prods_ouid=order_uuid ";
    cmd += "left join erp_custs on order_cust=custs_uuid";

    sql_prods->setQuery(cmd,db);
}

