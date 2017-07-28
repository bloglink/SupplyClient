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

    tab_plan = new QTableView(this);
    tab_plan->setItemDelegate(new ReadOnlyDelegate);
    tab_plan->horizontalHeader()->setHighlightSections(false);
    tab_plan->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_plan,SIGNAL(clicked(QModelIndex)),this,SLOT(tabPlanSync(QModelIndex)));

    tab_prod = new QTableView(this);
    tab_prod->setItemDelegate(new ReadOnlyDelegate);
    tab_prod->horizontalHeader()->setHighlightSections(false);
    tab_prod->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_prod,SIGNAL(clicked(QModelIndex)),this,SLOT(tabProdSync(QModelIndex)));

    QPushButton *prod_update = new QPushButton(this);
    prod_update->setFlat(true);
    prod_update->setMinimumSize(97,44);
    prod_update->setText(tr("刷新显示"));
    prod_update->setFocusPolicy(Qt::NoFocus);
    connect(prod_update,SIGNAL(clicked(bool)),this,SLOT(updateProds()));

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
    pSpliter->addWidget(tab_plan);
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

    prod_items << tr("编号") << tr("记录") << tr("操作") << tr("订单单号")
               << tr("订单日期") << tr("客户名称") << tr("评审单号") << tr("发货日期")
               << tr("生产数量") << tr("联络单号") << tr("产品大类") << tr("产品编号")
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
    tab_iprod->setColumnWidth(0,100);
    tab_iprod->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_iprod->hideRow(PROD_ID);
    tab_iprod->hideRow(PROD_GUID);
    tab_iprod->hideRow(PROD_SIGN);

    QPushButton *prod_append = new QPushButton(this);
    prod_append->setFlat(true);
    prod_append->setMinimumSize(97,44);
    prod_append->setText(tr("添加计划"));
    prod_append->setFocusPolicy(Qt::NoFocus);
    connect(prod_append,SIGNAL(clicked(bool)),this,SLOT(appendProd()));
    QPushButton *prod_delete = new QPushButton(this);
    prod_delete->setFlat(true);
    prod_delete->setMinimumSize(97,44);
    prod_delete->setText(tr("删除计划"));
    prod_delete->setFocusPolicy(Qt::NoFocus);
    connect(prod_delete,SIGNAL(clicked(bool)),this,SLOT(deleteProd()));
    QPushButton *prod_change = new QPushButton(this);
    prod_change->setFlat(true);
    prod_change->setMinimumSize(97,44);
    prod_change->setText(tr("修改计划"));
    prod_change->setFocusPolicy(Qt::NoFocus);
    connect(prod_change,SIGNAL(clicked(bool)),this,SLOT(changeProd()));

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

    sql_plan = new StandardSqlModel(this,db);
    sql_plan->setTable("erp_order");
    QStringList order_items;
    order_items << tr("编号") << tr("记录") << tr("操作") << tr("订单单号")
                << tr("下单日期") << tr("所属区域") << tr("业务经理") << tr("客户名称")
                << tr("评审单号") << tr("订货数量") << tr("发货日期") << tr("备注内容")
                << tr("在产数量") << tr("入库数量") << tr("未发数量") << tr("发货数量");

    for (int i=0; i < order_items.size(); i++)
        sql_plan->setHeaderData(i, Qt::Horizontal, order_items.at(i));
    tab_plan->setModel(sql_plan);
    tab_plan->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_plan->hideColumn(ORDER_ID);
    tab_plan->hideColumn(ORDER_GUID);
    tab_plan->hideColumn(ORDER_SIGN);
    tab_plan->hideColumn(ORDER_AREA);
    tab_plan->hideColumn(ORDER_SALE);
    tab_plan->hideColumn(ORDER_STCK);
    tab_plan->hideColumn(ORDER_LNUM);
    tab_plan->hideColumn(ORDER_DNUM);

    sql_prod = new StandardSqlModel(this,db);
    sql_prod->setTable("erp_prods");
    for (int i=0; i < prod_items.size(); i++)
        sql_prod->setHeaderData(i, Qt::Horizontal, prod_items.at(i));
    tab_prod->setModel(sql_prod);
    tab_prod->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_prod->hideColumn(PROD_ID);
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
    for (int i=0; i < sql_plan->rowCount(); i++) {
        QString number = sql_plan->index(i,ORDER_NUMB).data().toString();
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
    sql_plan->select();
    for (int i=0; i < sql_plan->rowCount(); i++) {
        int quan = sql_plan->index(i,ORDER_QUAN).data().toInt();
        int prod = sql_plan->index(i,ORDER_PROD).data().toInt();
        int stck = sql_plan->index(i,ORDER_STCK).data().toInt();
        int dnum = sql_plan->index(i,ORDER_DNUM).data().toInt();
        if (quan == prod+stck+dnum)
            tab_plan->hideRow(i);
        else
            tab_plan->showRow(i);
    }
    sql_prod->select();
}

void ProdsPage::appendProd()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_prods");
    obj.insert("logs_sign",1);
    obj.insert("prod_numb",m_prod->item(PROD_NUMB,1)->text());
    obj.insert("prod_date",m_prod->item(PROD_DATE,1)->text());
    obj.insert("prod_view",m_prod->item(PROD_VIEW,1)->text());
    obj.insert("prod_cust",m_prod->item(PROD_CUST,1)->text());
    obj.insert("prod_dead",m_prod->item(PROD_DEAD,1)->text());
    obj.insert("prod_quan",m_prod->item(PROD_QUAN,1)->text());
    obj.insert("prod_pnum",m_prod->item(PROD_PNUM,1)->text());
    obj.insert("prod_type",m_prod->item(PROD_TYPE,1)->text());
    obj.insert("prod_code",m_prod->item(PROD_CODE,1)->text());
    obj.insert("prod_name",m_prod->item(PROD_NAME,1)->text());
    obj.insert("prod_mode",m_prod->item(PROD_MODE,1)->text());
    obj.insert("prod_mnum",m_prod->item(PROD_MNUM,1)->text());
    emit sendJson(obj);
    //    int numb = m_prod->item(PROD_QUAN,1)->text().toInt();
    //    for (int k=0; k < numb; k++)
    //        emit sendJson(obj);
    for (int i=0; i < m_prod->rowCount(); i++)
        m_prod->item(i,1)->setText("");
}

void ProdsPage::deleteProd()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_prods");
    obj.insert("logs_sign",2);
    obj.insert("tabs_guid",m_prod->item(PROD_ID,1)->text().toDouble());
    obj.insert("prod_numb",m_prod->item(PROD_NUMB,1)->text());
    obj.insert("prod_date",m_prod->item(PROD_DATE,1)->text());
    obj.insert("prod_view",m_prod->item(PROD_VIEW,1)->text());
    obj.insert("prod_cust",m_prod->item(PROD_CUST,1)->text());
    obj.insert("prod_dead",m_prod->item(PROD_DEAD,1)->text());
    obj.insert("prod_quan",m_prod->item(PROD_QUAN,1)->text());
    obj.insert("prod_pnum",m_prod->item(PROD_PNUM,1)->text());
    obj.insert("prod_type",m_prod->item(PROD_TYPE,1)->text());
    obj.insert("prod_code",m_prod->item(PROD_CODE,1)->text());
    obj.insert("prod_name",m_prod->item(PROD_NAME,1)->text());
    obj.insert("prod_mode",m_prod->item(PROD_MODE,1)->text());
    obj.insert("prod_mnum",m_prod->item(PROD_MNUM,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_prod->rowCount(); i++)
        m_prod->item(i,1)->setText("");
}

void ProdsPage::changeProd()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_prods");
    obj.insert("logs_sign",3);
    obj.insert("tabs_guid",m_prod->item(PROD_ID,1)->text().toDouble());
    obj.insert("prod_numb",m_prod->item(PROD_NUMB,1)->text());
    obj.insert("prod_date",m_prod->item(PROD_DATE,1)->text());
    obj.insert("prod_view",m_prod->item(PROD_VIEW,1)->text());
    obj.insert("prod_cust",m_prod->item(PROD_CUST,1)->text());
    obj.insert("prod_dead",m_prod->item(PROD_DEAD,1)->text());
    obj.insert("prod_quan",m_prod->item(PROD_QUAN,1)->text());
    obj.insert("prod_pnum",m_prod->item(PROD_PNUM,1)->text());
    obj.insert("prod_type",m_prod->item(PROD_TYPE,1)->text());
    obj.insert("prod_code",m_prod->item(PROD_CODE,1)->text());
    obj.insert("prod_name",m_prod->item(PROD_NAME,1)->text());
    obj.insert("prod_mode",m_prod->item(PROD_MODE,1)->text());
    obj.insert("prod_mnum",m_prod->item(PROD_MNUM,1)->text());
    emit sendJson(obj);
}

void ProdsPage::updateProds()
{
    QSqlQuery query(db);
    qint64 logs_guid = 0;
    QJsonObject obj;

    query.prepare("select max(logs_guid) from erp_prods");
    query.exec();
    query.next();
    logs_guid = query.value(0).toDouble();

    obj.insert("logs_cmmd","erp_prods");
    obj.insert("logs_guid",logs_guid);
    obj.insert("logs_sign",0);
    emit sendJson(obj);
    sql_prod->select();
}

void ProdsPage::updateOrder()
{
    QSqlQuery query(db);
    qint64 logs_guid = 0;
    QJsonObject obj;

    query.prepare("select max(logs_guid) from erp_order");
    query.exec();
    query.next();
    logs_guid = query.value(0).toDouble();

    obj.insert("logs_cmmd","erp_order");
    obj.insert("logs_guid",logs_guid);
    obj.insert("logs_sign",0);
    emit sendJson(obj);

    sql_plan->select();
}

void ProdsPage::tabPlanSync(QModelIndex index)
{
    int row = index.row();
    m_prod->item(PROD_NUMB,1)->setText(sql_plan->index(row,ORDER_NUMB).data().toString());
    m_prod->item(PROD_DATE,1)->setText(sql_plan->index(row,ORDER_DATE).data().toString());
    m_prod->item(PROD_CUST,1)->setText(sql_plan->index(row,ORDER_CUST).data().toString());
    m_prod->item(PROD_VIEW,1)->setText(sql_plan->index(row,ORDER_VIEW).data().toString());
    m_prod->item(PROD_DEAD,1)->setText(sql_plan->index(row,ORDER_DEAD).data().toString());
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
    int row = tab_plan->currentIndex().row();
    if (row < 0)
        return;
    QString view = sql_plan->index(row,ORDER_VIEW).data().toString();
    int quan = sql_plan->index(row,ORDER_QUAN).data().toInt();
    int prod = sql_plan->index(row,ORDER_PROD).data().toInt();
    int stck = sql_plan->index(row,ORDER_STCK).data().toInt();
    int dnum = sql_plan->index(row,ORDER_DNUM).data().toInt();

    for (int i=0; i < sql_prod->rowCount(); i++) {
        QString prod_view = sql_prod->index(i,PROD_VIEW).data().toString();
        QString numb = sql_prod->index(i,PROD_NUMB).data().toString();
        if (numb.isEmpty() && view == prod_view) {
            for (int k=1; k < PROD_QUAN; k++) {
                QJsonObject obj;
                obj.insert("logs_cmmd","erp_prods");
                obj.insert("logs_sign",3);
                obj.insert("tabs_guid",sql_prod->index(k,PROD_ID).data().toDouble());
                obj.insert("prod_numb",sql_plan->index(row,ORDER_NUMB).data().toString());
                obj.insert("prod_date",sql_plan->index(row,ORDER_DATE).data().toString());
                obj.insert("prod_view",sql_plan->index(row,ORDER_VIEW).data().toString());
                obj.insert("prod_cust",sql_plan->index(row,ORDER_CUST).data().toString());
                obj.insert("prod_sale",sql_plan->index(row,ORDER_SALE).data().toString());
                obj.insert("prod_area",sql_plan->index(row,ORDER_AREA).data().toString());
                obj.insert("prod_dead",sql_plan->index(row,ORDER_DEAD).data().toString());
                obj.insert("prod_need",sql_plan->index(row,ORDER_QUAN).data().toString());
                obj.insert("prod_quan",sql_prod->index(k,PROD_QUAN).data().toString());
                obj.insert("prod_pnum",sql_prod->index(k,PROD_PNUM).data().toString());
                obj.insert("prod_type",sql_prod->index(k,PROD_TYPE).data().toString());
                obj.insert("prod_code",sql_prod->index(k,PROD_CODE).data().toString());
                obj.insert("prod_name",sql_prod->index(k,PROD_NAME).data().toString());
                obj.insert("prod_mode",sql_prod->index(k,PROD_MODE).data().toString());
                obj.insert("prod_mnum",sql_prod->index(k,PROD_MNUM).data().toString());
                emit sendJson(obj);
            }
            prod++;
            if (quan == (prod+stck+dnum))
                break;
        }
    }
    //    sql_plan->setData(sql_plan->index(row,ORDER_PROD),QString::number(prod));
    //    sql_plan->submitAll();
    //    initData();
}

void ProdsPage::matchCancel()
{
    int row = tab_prod->currentIndex().row();
    if (row < 0)
        return;
    QString numb = sql_prod->index(row,PROD_NUMB).data().toString();
    for (int i=0; i < sql_plan->rowCount(); i++) {
        QString plan_numb = sql_plan->index(i,PROD_NUMB).data().toString();
        if (numb == plan_numb) {
            int prod = sql_plan->index(i,ORDER_PROD).data().toInt();
            prod--;
            sql_plan->setData(sql_plan->index(i,ORDER_PROD),QString::number(prod));
            sql_plan->submitAll();
            break;
        }
    }
    for (int k=1; k < PROD_QUAN; k++) {
        if (k == PROD_VIEW)
            continue;
        sql_prod->setData(sql_prod->index(row,k),"");
        sql_prod->submitAll();
    }
    initData();
}

void ProdsPage::recvProdsJson(QJsonObject obj)
{
    QSqlQuery query(db);
    qint64 logs_sign = obj.value("logs_sign").toDouble();
    qint64 logs_guid = obj.value("logs_guid").toDouble();
    qint64 tabs_guid = obj.value("tabs_guid").toDouble();

    switch (logs_sign) {
    case 0://查询
        updateProds();
        return;
        break;
    case 1://增加
    case 3://修改
        query.prepare("replace into erp_prods values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        query.bindValue(PROD_ID,tabs_guid);
        query.bindValue(PROD_GUID,logs_guid);
        query.bindValue(PROD_SIGN,logs_sign);
        query.bindValue(PROD_NUMB,obj.value("prod_numb").toString());
        query.bindValue(PROD_DATE,obj.value("prod_date").toString());
        query.bindValue(PROD_VIEW,obj.value("prod_view").toString());
        query.bindValue(PROD_CUST,obj.value("prod_cust").toString());
        query.bindValue(PROD_DEAD,obj.value("prod_dead").toString());
        query.bindValue(PROD_QUAN,obj.value("prod_quan").toString());
        query.bindValue(PROD_PNUM,obj.value("prod_pnum").toString());
        query.bindValue(PROD_TYPE,obj.value("prod_type").toString());
        query.bindValue(PROD_CODE,obj.value("prod_code").toString());
        query.bindValue(PROD_NAME,obj.value("prod_name").toString());
        query.bindValue(PROD_MODE,obj.value("prod_mode").toString());
        query.bindValue(PROD_MNUM,obj.value("prod_mnum").toString());
        query.exec();
        break;
    case 2://删除
        query.prepare("delete from erp_prods where id=:id");
        query.bindValue(":id",tabs_guid);
        query.exec();
    default:
        break;
    }
    sql_plan->select();
    for (int i=0; i < sql_plan->rowCount(); i++) {
        int quan = sql_plan->index(i,ORDER_QUAN).data().toInt();
        int prod = sql_plan->index(i,ORDER_PROD).data().toInt();
        int stck = sql_plan->index(i,ORDER_STCK).data().toInt();
        int dnum = sql_plan->index(i,ORDER_DNUM).data().toInt();
        if (quan == prod+stck+dnum)
            tab_plan->hideRow(i);
        else
            tab_plan->showRow(i);
    }
    sql_prod->select();
}

void ProdsPage::recvOrderJson(QJsonObject )
{
    sql_plan->select();
}

void ProdsPage::showEvent(QShowEvent *e)
{
    updateOrder();
    updateProds();

    e->accept();
}
