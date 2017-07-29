#include "WorksPage.h"

WorksPage::WorksPage(QWidget *parent) : QWidget(parent)
{
    initUI();
    initSql();
}

WorksPage::~WorksPage()
{
    db.close();
}

void WorksPage::initUI()
{
    this->setObjectName("WorksPage");

    tab_prod = new QTableView(this);
    tab_prod->setItemDelegate(new ReadOnlyDelegate);
    tab_prod->horizontalHeader()->setHighlightSections(false);
    tab_prod->setSelectionBehavior(QAbstractItemView::SelectRows);
//    connect(tab_prod,SIGNAL(clicked(QModelIndex)),this,SLOT(tabCustSync(QModelIndex)));

    QPushButton *prod_update = new QPushButton(this);
    prod_update->setFlat(true);
    prod_update->setMinimumSize(97,44);
    prod_update->setText(tr("刷新显示"));
    prod_update->setFocusPolicy(Qt::NoFocus);
    connect(prod_update,SIGNAL(clicked(bool)),this,SLOT(updateCust()));

    QGridLayout *sprodLayout = new QGridLayout;
    sprodLayout->addWidget(tab_prod,0,0,1,2);
    sprodLayout->addWidget(prod_update,1,1);
    sprodLayout->setColumnStretch(0,1);

    QWidget *custs = new QWidget(this);
    custs->setLayout(sprodLayout);

    /**************************************************************************/
    tab_bills = new QTableView(this);
    tab_bills->setItemDelegate(new ReadOnlyDelegate);
    tab_bills->horizontalHeader()->setHighlightSections(false);
    tab_bills->setSelectionBehavior(QAbstractItemView::SelectRows);
//    connect(tab_bills,SIGNAL(clicked(QModelIndex)),this,SLOT(tabSaleSync(QModelIndex)));

    QPushButton *bill_update = new QPushButton(this);
    bill_update->setFlat(true);
    bill_update->setMinimumSize(97,44);
    bill_update->setText(tr("刷新显示"));
    bill_update->setFocusPolicy(Qt::NoFocus);
    connect(prod_update,SIGNAL(clicked(bool)),this,SLOT(updateSale()));

    QPushButton *bill_export = new QPushButton(this);
    bill_export->setFlat(true);
    bill_export->setMinimumSize(97,44);
    bill_export->setText(tr("最大出料"));
    bill_export->setFocusPolicy(Qt::NoFocus);
    connect(bill_export,SIGNAL(clicked(bool)),this,SLOT(updateSale()));

    QGridLayout *sbillLayout = new QGridLayout;
    sbillLayout->addWidget(tab_bills,0,0,1,4);
    sbillLayout->addWidget(bill_export,1,2);
    sbillLayout->addWidget(bill_update,1,3);
    sbillLayout->setColumnStretch(0,1);

    btn_bills = new QToolButton(this);//显示订单
    btn_bills->setIcon(QIcon(":/icons/left.png"));
    btn_bills->setIconSize(QSize(30,30));
    btn_bills->setFocusPolicy(Qt::NoFocus);
    btn_bills->setText(tr("物\n料\n管\n理"));
    btn_bills->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_bills,SIGNAL(clicked(bool)),this,SLOT(showTabSale()));

    QVBoxLayout *sbtnsLayout = new QVBoxLayout;
    sbtnsLayout->addWidget(btn_bills);
    sbtnsLayout->addStretch();

    bill_items << tr("编号") << tr("记录") << tr("操作") << tr("物料编号") << tr("物料名称")
                   << tr("物料规格") << tr("单位") << tr("数量") << tr("备注");
    m_bills = new StandardItemModel();
    QStringList sale_header;
    sale_header << tr("项目") << tr("参数");
    m_bills->setHorizontalHeaderLabels(sale_header);
    for (int i=0; i < bill_items.size(); i++) {
        m_bills->setItem(i,0,new QStandardItem(bill_items.at(i)));
        m_bills->setItem(i,1,new QStandardItem(""));
    }
    tab_ibill = new QTableView(this);
    tab_ibill->setModel(m_bills);
    tab_ibill->setColumnWidth(0,100);
    tab_ibill->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_ibill->hideRow(SALE_UUID);
    tab_ibill->hideRow(SALE_GUID);
    tab_ibill->hideRow(SALE_SIGN);

    QPushButton *sale_append = new QPushButton(this);
    sale_append->setFlat(true);
    sale_append->setMinimumSize(97,44);
    sale_append->setText(tr("添加物料"));
    sale_append->setFocusPolicy(Qt::NoFocus);
    connect(sale_append,SIGNAL(clicked(bool)),this,SLOT(appendSale()));
    QPushButton *sale_delete = new QPushButton(this);
    sale_delete->setFlat(true);
    sale_delete->setMinimumSize(97,44);
    sale_delete->setText(tr("删除物料"));
    sale_delete->setFocusPolicy(Qt::NoFocus);
    connect(sale_delete,SIGNAL(clicked(bool)),this,SLOT(deleteSale()));
    QPushButton *sale_change = new QPushButton(this);
    sale_change->setFlat(true);
    sale_change->setMinimumSize(97,44);
    sale_change->setText(tr("修改物料"));
    sale_change->setFocusPolicy(Qt::NoFocus);
    connect(sale_change,SIGNAL(clicked(bool)),this,SLOT(changeSale()));

    QGridLayout *isaleLayout = new QGridLayout;
    isaleLayout->addWidget(tab_ibill,0,0,1,4);
    isaleLayout->addWidget(sale_append,1,1);
    isaleLayout->addWidget(sale_delete,1,2);
    isaleLayout->addWidget(sale_change,1,3);
    isaleLayout->setColumnStretch(0,1);
    isaleLayout->setMargin(0);

    saleWidget = new QWidget(this);
    saleWidget->setLayout(isaleLayout);
    saleWidget->hide();

    QHBoxLayout *salelayout = new QHBoxLayout;
    salelayout->addLayout(sbillLayout);
    salelayout->addLayout(sbtnsLayout);
    salelayout->addWidget(saleWidget);
    salelayout->setStretch(0,1);

    QWidget *sales = new QWidget(this);
    sales->setLayout(salelayout);

    /**************************************************************************/
    QSplitter *pSplitter = new QSplitter(this);
    pSplitter->addWidget(custs);
    pSplitter->addWidget(sales);

    QVBoxLayout *splitter = new QVBoxLayout;
    splitter->addWidget(pSplitter);
    splitter->setMargin(0);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(splitter);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(group);
    this->setLayout(mainLayout);
}

void WorksPage::initSql()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "erp_works");
    db.setDatabaseName("erp.db");
    db.open();

    prod_items << tr("编号") << tr("记录") << tr("操作") << tr("订单单号")
               << tr("订单日期") << tr("客户名称") << tr("评审单号") << tr("发货日期")
               << tr("生产数量") << tr("联络单号") << tr("产品大类") << tr("产品编号")
               << tr("产品名称") << tr("产品规格") << tr("仪表编码");

    sql_prod = new StandardSqlModel(this,db);
    sql_prod->setTable("erp_prods");
    for (int i=0; i < prod_items.size(); i++)
        sql_prod->setHeaderData(i, Qt::Horizontal, prod_items.at(i));
    tab_prod->setModel(sql_prod);
    tab_prod->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_prod->hideColumn(PROD_UUID);
    tab_prod->hideColumn(PROD_GUID);
    tab_prod->hideColumn(PROD_SIGN);
    tab_prod->hideColumn(PROD_TYPE);
    tab_prod->hideColumn(PROD_NAME);
    tab_prod->hideColumn(PROD_MODE);
    tab_prod->hideColumn(PROD_MNUM);

    sql_bills = new StandardSqlModel(this,db);
    sql_bills->setTable("erp_bills");
    sql_bills->select();

    for (int i=0; i < bill_items.size(); i++)
        sql_bills->setHeaderData(i, Qt::Horizontal, bill_items.at(i));
    tab_bills->setModel(sql_bills);
    tab_bills->setColumnWidth(SALE_UUID,50);
    tab_bills->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_bills->hideColumn(SALE_UUID);
    tab_bills->hideColumn(SALE_GUID);
    tab_bills->hideColumn(SALE_SIGN);
}

void WorksPage::initData()
{
    sql_prod->select();
    sql_bills->select();
}

void WorksPage::showTabCust()
{
    if (custWidget->isHidden()) {
        custWidget->show();
        btn_custs->setIcon(QIcon(":/icons/right.png"));
    } else {
        custWidget->hide();
        btn_custs->setIcon(QIcon(":/icons/left.png"));
    }
    QStringList sales_head;
    for (int i=0; i < sql_bills->rowCount(); i++)
        sales_head.append(sql_bills->data(sql_bills->index(i,SALE_NAME)).toString());
    sale_delegate->setItemHeaders(sales_head);

    QStringList areas_head;
    for (int i=0; i < sql_bills->rowCount(); i++)
        areas_head.append(sql_bills->data(sql_bills->index(i,SALE_AREA)).toString());
    area_delegate->setItemHeaders(areas_head);
}

void WorksPage::showTabSale()
{
    if (saleWidget->isHidden()) {
        saleWidget->show();
        btn_bills->setIcon(QIcon(":/icons/right.png"));
    } else {
        saleWidget->hide();
        btn_bills->setIcon(QIcon(":/icons/left.png"));
    }
}

void WorksPage::tabCustSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_custs->rowCount(); i++) {
        m_custs->item(i,1)->setText(sql_prod->index(row,i).data().toString());
    }
}

void WorksPage::tabSaleSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_bills->rowCount(); i++) {
        m_bills->item(i,1)->setText(sql_bills->index(row,i).data().toString());
    }
}

void WorksPage::appendCust()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_custs");
    obj.insert("logs_sign",1);
    obj.insert("cust_name",m_custs->item(CUST_NAME,1)->text());
    obj.insert("cust_sale",m_custs->item(CUST_SALE,1)->text());
    obj.insert("cust_area",m_custs->item(CUST_AREA,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_custs->rowCount(); i++)
        m_custs->item(i,1)->setText("");
}

void WorksPage::deleteCust()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_custs");
    obj.insert("logs_sign",2);
    obj.insert("tabs_guid",m_custs->item(CUST_UUID,1)->text().toDouble());
    obj.insert("cust_name",m_custs->item(CUST_NAME,1)->text());
    obj.insert("cust_sale",m_custs->item(CUST_SALE,1)->text());
    obj.insert("cust_area",m_custs->item(CUST_AREA,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_custs->rowCount(); i++)
        m_custs->item(i,1)->setText("");
}

void WorksPage::changeCust()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_custs");
    obj.insert("logs_sign",3);
    obj.insert("tabs_guid",m_custs->item(CUST_UUID,1)->text().toDouble());
    obj.insert("cust_name",m_custs->item(CUST_NAME,1)->text());
    obj.insert("cust_sale",m_custs->item(CUST_SALE,1)->text());
    obj.insert("cust_area",m_custs->item(CUST_AREA,1)->text());
    emit sendJson(obj);
}

void WorksPage::updateCust()
{
    QSqlQuery query(db);
    qint64 logs_guid = 0;
    QJsonObject obj;

    query.prepare("select max(logs_guid) from erp_custs");
    query.exec();
    query.next();
    logs_guid = query.value(0).toDouble();

    obj.insert("logs_cmmd","erp_custs");
    obj.insert("logs_guid",logs_guid);
    obj.insert("logs_sign",0);
    emit sendJson(obj);

    sql_prod->select();
}

void WorksPage::appendSale()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_sales");
    obj.insert("logs_sign",1);
    obj.insert("sale_name",m_bills->item(SALE_NAME,1)->text());
    obj.insert("sale_area",m_bills->item(SALE_AREA,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_bills->rowCount(); i++)
        m_bills->item(i,1)->setText("");
}

void WorksPage::deleteSale()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_sales");
    obj.insert("logs_sign",2);
    obj.insert("tabs_guid",m_bills->item(SALE_UUID,1)->text().toDouble());
    obj.insert("sale_name",m_bills->item(SALE_NAME,1)->text());
    obj.insert("sale_area",m_bills->item(SALE_AREA,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_bills->rowCount(); i++)
        m_bills->item(i,1)->setText("");
}

void WorksPage::changeSale()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_sales");
    obj.insert("logs_sign",3);
    obj.insert("tabs_guid",m_bills->item(SALE_UUID,1)->text().toDouble());
    obj.insert("sale_name",m_bills->item(SALE_NAME,1)->text());
    obj.insert("sale_area",m_bills->item(SALE_AREA,1)->text());
    emit sendJson(obj);
}

void WorksPage::updateSale()
{
    QSqlQuery query(db);
    qint64 logs_guid = 0;
    QJsonObject obj;

    query.prepare("select max(logs_guid) from erp_sales");
    query.exec();
    query.next();
    logs_guid = query.value(0).toDouble();

    obj.insert("logs_cmmd","erp_sales");
    obj.insert("logs_guid",logs_guid);
    obj.insert("logs_sign",0);
    emit sendJson(obj);

    sql_bills->select();
}

void WorksPage::recvWorksJson(QJsonObject obj)
{
    QSqlQuery query(db);
    qint64 logs_sign = obj.value("logs_sign").toDouble();
    qint64 logs_guid = obj.value("logs_guid").toDouble();
    qint64 tabs_guid = obj.value("tabs_guid").toDouble();

    switch (logs_sign) {
    case 0://查询
        updateSale();
        return;
        break;
    case 1://增加
    case 3://修改
        query.prepare("replace into erp_sales values(?,?,?,?,?)");
        query.bindValue(0,tabs_guid);
        query.bindValue(1,logs_guid);
        query.bindValue(2,logs_sign);
        query.bindValue(3,obj.value("sale_name").toString());
        query.bindValue(4,obj.value("sale_area").toString());
        query.exec();
        break;
    case 2://删除
        query.prepare("delete from erp_sales where id=:id");
        query.bindValue(":id",tabs_guid);
        query.exec();
    default:
        break;
    }
    sql_bills->select();
}

void WorksPage::showEvent(QShowEvent *e)
{
    updateSale();
    updateCust();
    e->accept();
}
