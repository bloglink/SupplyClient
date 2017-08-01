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
    //    connect(tab_plan,SIGNAL(clicked(QModelIndex)),this,SLOT(tabPlanSync(QModelIndex)));

    tab_prod = new QTableView(this);
    tab_prod->setItemDelegate(new ReadOnlyDelegate);
    tab_prod->horizontalHeader()->setHighlightSections(false);
    tab_prod->setSelectionBehavior(QAbstractItemView::SelectRows);
//    connect(tab_prod,SIGNAL(clicked(QModelIndex)),this,SLOT(tabProdSync(QModelIndex)));

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
    pSpliter->addWidget(tab_prod);
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
    tab_iprod->hideRow(PROD_UUID);
    tab_iprod->hideRow(PROD_GUID);
    tab_iprod->hideRow(PROD_SIGN);

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

    sql_order = new SqlQueryModel(this);
    QString cmd = "select prods_uuid,prods_guid,prods_sign,";
    cmd += "order_numb,order_date,custs_name,order_view,order_dead,";
    cmd += "plans_numb,plans_type,plans_code,plans_name,plans_mode,plans_mnum ";
    cmd += "from erp_plans,erp_prods,erp_order,erp_custs ";
    cmd += "where prods_puid=plans_uuid and order_cust=custs_uuid or prods_puid=0" ;
    sql_order->setQuery(cmd,db);

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

    sql_prod = new SqlQueryModel(this);
    cmd = "select prods_uuid,prods_guid,prods_sign,";
    cmd += "order_numb,order_date,order_cust,order_view,order_dead,";
    cmd += "plans_numb,plans_type,plans_code,plans_name,plans_mode,plans_mnum ";
    cmd += "from erp_plans,erp_prods,erp_order ";
    cmd += "where prods_puid=plans_uuid";
    sql_prod->setQuery(cmd,db);
    for (int i=0; i < prod_items.size(); i++)
        sql_prod->setHeaderData(i, Qt::Horizontal, prod_items.at(i));
    tab_prod->setModel(sql_prod);
    tab_prod->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_prod->hideColumn(PROD_UUID);
    tab_prod->hideColumn(PROD_GUID);
    tab_prod->hideColumn(PROD_SIGN);
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
    obj.insert("plans_numb",m_prod->item(PROD_NUMB,1)->text());
    obj.insert("plans_quan",m_prod->item(PROD_QUAN,1)->text());
    obj.insert("plans_type",m_prod->item(PROD_TYPE,1)->text());
    obj.insert("plans_code",m_prod->item(PROD_CODE,1)->text());
    obj.insert("plans_name",m_prod->item(PROD_NAME,1)->text());
    obj.insert("plans_mode",m_prod->item(PROD_MODE,1)->text());
    obj.insert("plans_mnum",m_prod->item(PROD_MNUM,1)->text());
    emit sendJson(obj);

    emit updateSql("erp_plans");
    emit updateSql("erp_prods");
}

void ProdsPage::deletePlans()
{
    QJsonObject obj;
    obj.insert("command","erp_plans");
    obj.insert("plans_sign",2);
    obj.insert("tabs_guid",m_prod->item(PROD_UUID,1)->text().toDouble());
    obj.insert("prod_numb",m_prod->item(PROD_NUMB,1)->text());
    obj.insert("prod_quan",m_prod->item(PROD_QUAN,1)->text());
    obj.insert("prod_type",m_prod->item(PROD_TYPE,1)->text());
    obj.insert("prod_code",m_prod->item(PROD_CODE,1)->text());
    obj.insert("prod_name",m_prod->item(PROD_NAME,1)->text());
    obj.insert("prod_mode",m_prod->item(PROD_MODE,1)->text());
    obj.insert("prod_mnum",m_prod->item(PROD_MNUM,1)->text());
    emit sendJson(obj);

    emit updateSql("erp_plans");
    emit updateSql("erp_prods");
}

void ProdsPage::changePlans()
{
    QJsonObject obj;
    obj.insert("command","erp_plans");
    obj.insert("plans_sign",3);
    obj.insert("tabs_guid",m_prod->item(PROD_UUID,1)->text().toDouble());
    obj.insert("prod_numb",m_prod->item(PROD_NUMB,1)->text());
    obj.insert("prod_quan",m_prod->item(PROD_QUAN,1)->text());
    obj.insert("prod_type",m_prod->item(PROD_TYPE,1)->text());
    obj.insert("prod_code",m_prod->item(PROD_CODE,1)->text());
    obj.insert("prod_name",m_prod->item(PROD_NAME,1)->text());
    obj.insert("prod_mode",m_prod->item(PROD_MODE,1)->text());
    obj.insert("prod_mnum",m_prod->item(PROD_MNUM,1)->text());
    emit sendJson(obj);

    emit updateSql("erp_plans");
    emit updateSql("erp_prods");
}

void ProdsPage::tabPlanSync(QModelIndex index)
{

}

void ProdsPage::tabProdSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_prod->rowCount(); i++) {
        m_prod->item(i,1)->setText(sql_prod->index(row,i).data().toString());
    }
}

void ProdsPage::matchProds()
{
    //    int row = tab_order->currentIndex().row();
    //    if (row < 0)
    //        return;
    //    QString view = sql_plan->index(row,ORDER_VIEW).data().toString();
    //    int quan = sql_plan->index(row,ORDER_QUAN).data().toInt();
    //    int prod = sql_plan->index(row,ORDER_PROD).data().toInt();
    //    int stck = sql_plan->index(row,ORDER_STCK).data().toInt();
    //    int dnum = sql_plan->index(row,ORDER_DNUM).data().toInt();

    //    for (int i=0; i < sql_prod->rowCount(); i++) {
    //        QString prod_view = sql_prod->index(i,PROD_VIEW).data().toString();
    //        QString numb = sql_prod->index(i,PROD_NUMB).data().toString();
    //        if (numb.isEmpty() && view == prod_view) {
    //            for (int k=1; k < PROD_QUAN; k++) {
    //                QJsonObject obj;
    //                obj.insert("command","erp_prods");
    //                obj.insert("plans_sign",3);
    //                obj.insert("tabs_guid",sql_prod->index(k,PROD_UUID).data().toDouble());
    //                obj.insert("prod_numb",sql_plan->index(row,ORDER_NUMB).data().toString());
    //                obj.insert("prod_date",sql_plan->index(row,ORDER_DATE).data().toString());
    //                obj.insert("prod_view",sql_plan->index(row,ORDER_VIEW).data().toString());
    //                obj.insert("prod_cust",sql_plan->index(row,ORDER_CUST).data().toString());
    //                obj.insert("prod_sale",sql_plan->index(row,ORDER_SALE).data().toString());
    //                obj.insert("prod_area",sql_plan->index(row,ORDER_AREA).data().toString());
    //                obj.insert("prod_dead",sql_plan->index(row,ORDER_DEAD).data().toString());
    //                obj.insert("prod_need",sql_plan->index(row,ORDER_QUAN).data().toString());
    //                obj.insert("prod_quan",sql_prod->index(k,PROD_QUAN).data().toString());
    //                obj.insert("prod_pnum",sql_prod->index(k,PROD_PNUM).data().toString());
    //                obj.insert("prod_type",sql_prod->index(k,PROD_TYPE).data().toString());
    //                obj.insert("prod_code",sql_prod->index(k,PROD_CODE).data().toString());
    //                obj.insert("prod_name",sql_prod->index(k,PROD_NAME).data().toString());
    //                obj.insert("prod_mode",sql_prod->index(k,PROD_MODE).data().toString());
    //                obj.insert("prod_mnum",sql_prod->index(k,PROD_MNUM).data().toString());
    //                emit sendJson(obj);
    //            }
    //            prod++;
    //            if (quan == (prod+stck+dnum))
    //                break;
    //        }
    //    }
    //    sql_plan->setData(sql_plan->index(row,ORDER_PROD),QString::number(prod));
    //    sql_plan->submitAll();
    //    initData();
}

void ProdsPage::matchCancel()
{
    //    int row = tab_prod->currentIndex().row();
    //    if (row < 0)
    //        return;
    //    QString numb = sql_prod->index(row,PROD_NUMB).data().toString();
    //    for (int i=0; i < sql_plan->rowCount(); i++) {
    //        QString plan_numb = sql_plan->index(i,PROD_NUMB).data().toString();
    //        if (numb == plan_numb) {
    //            int prod = sql_plan->index(i,ORDER_PROD).data().toInt();
    //            prod--;
    //            sql_plan->setData(sql_plan->index(i,ORDER_PROD),QString::number(prod));
    //            sql_plan->submitAll();
    //            break;
    //        }
    //    }
    //    for (int k=1; k < PROD_QUAN; k++) {
    //        if (k == PROD_VIEW)
    //            continue;
    //        sql_prod->setData(sql_prod->index(row,k),"");
    ////        sql_prod->submitAll();
    //    }
    //    initData();
}

void ProdsPage::recvNetJson(QJsonObject obj)
{
    QString cmd = obj.value("command").toString();
    if (cmd == "erp_order") {
        cmd = "select order_uuid,order_guid,order_sign,order_numb,order_date,";
        cmd += "custs_name,";
        cmd += "order_view,order_quan,order_dead,order_mark ";
        cmd += "from erp_order,erp_custs,erp_sales ";
        cmd += "where order_cust=custs_uuid and custs_sale=sales_uuid";
        sql_order->setQuery(cmd,db);
    }
    if (cmd == "erp_prods") {
        cmd = "select prods_uuid,prods_guid,prods_sign,";
        cmd += "order_numb,order_date,custs_name,order_view,order_dead,";
        cmd += "plans_numb,plans_type,plans_code,plans_name,plans_mode,prods_mnum ";
        cmd += "from erp_plans,erp_prods,erp_order,erp_custs ";
        cmd += "where prods_puid=plans_uuid and order_cust=custs_uuid";
        sql_prod->setQuery(cmd,db);
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

