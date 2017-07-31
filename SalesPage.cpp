#include "SalesPage.h"

SalesPage::SalesPage(QWidget *parent) : QWidget(parent)
{
    initUI();
    initSql();
}

SalesPage::~SalesPage()
{
    db.close();
}

void SalesPage::initUI()
{
    this->setObjectName("SalesPage");

    tab_custs = new QTableView(this);
    tab_custs->setItemDelegate(new ReadOnlyDelegate);
    tab_custs->horizontalHeader()->setHighlightSections(false);
    tab_custs->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_custs,SIGNAL(clicked(QModelIndex)),this,SLOT(tabCustSync(QModelIndex)));

    QPushButton *cust_update = new QPushButton(this);
    cust_update->setFlat(true);
    cust_update->setMinimumSize(97,44);
    cust_update->setText(tr("刷新显示"));
    cust_update->setFocusPolicy(Qt::NoFocus);
    connect(cust_update,SIGNAL(clicked(bool)),this,SLOT(updateCusts()));

    QGridLayout *scustLayout = new QGridLayout;
    scustLayout->addWidget(tab_custs,0,0,1,2);
    scustLayout->addWidget(cust_update,1,1);
    scustLayout->setColumnStretch(0,1);

    btn_custs = new QToolButton(this);
    btn_custs->setIcon(QIcon(":/icons/left.png"));
    btn_custs->setIconSize(QSize(30,30));
    btn_custs->setFocusPolicy(Qt::NoFocus);
    btn_custs->setText(tr("客\n户\n管\n理"));
    btn_custs->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_custs,SIGNAL(clicked(bool)),this,SLOT(showTabCust()));

    QVBoxLayout *ubtnsLayout = new QVBoxLayout;
    ubtnsLayout->addWidget(btn_custs);
    ubtnsLayout->addStretch();

    cust_items << tr("编号") << tr("记录") << tr("操作")
               << tr("客户名称") << tr("销售姓名") << tr("所属区域");
    m_custs = new StandardItemModel();
    QStringList cust_header;
    cust_header << tr("项目") << tr("参数");
    m_custs->setHorizontalHeaderLabels(cust_header);
    for (int i=0; i < cust_items.size(); i++) {
        m_custs->setItem(i,0,new QStandardItem(cust_items.at(i)));
        m_custs->setItem(i,1,new QStandardItem(""));
    }
    area_delegate = new ComboBoxDelegate(this);
    sale_delegate = new ComboBoxDelegate(this);
    tab_icust = new QTableView(this);
    tab_icust->setModel(m_custs);
    tab_icust->verticalHeader()->setVisible(false);
    tab_icust->setColumnWidth(0,100);
    tab_icust->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_icust->setItemDelegateForColumn(0,new ReadOnlyDelegate);
    tab_icust->setItemDelegateForRow(CUSTS_SALE, sale_delegate);
    tab_icust->setItemDelegateForRow(CUSTS_AREA, area_delegate);
    tab_icust->hideRow(CUSTS_UUID);
    tab_icust->hideRow(CUSTS_GUID);
    tab_icust->hideRow(CUSTS_SIGN);
    connect(m_custs,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(tabCustSyncExp(QStandardItem*)));

    QPushButton *cust_append = new QPushButton(this);
    cust_append->setFlat(true);
    cust_append->setMinimumSize(97,44);
    cust_append->setText(tr("添加客户"));
    cust_append->setFocusPolicy(Qt::NoFocus);
    connect(cust_append,SIGNAL(clicked(bool)),this,SLOT(appendCusts()));
    QPushButton *cust_delete = new QPushButton(this);
    cust_delete->setFlat(true);
    cust_delete->setMinimumSize(97,44);
    cust_delete->setText(tr("删除客户"));
    cust_delete->setFocusPolicy(Qt::NoFocus);
    connect(cust_delete,SIGNAL(clicked(bool)),this,SLOT(deleteCusts()));
    QPushButton *cust_change = new QPushButton(this);
    cust_change->setFlat(true);
    cust_change->setMinimumSize(97,44);
    cust_change->setText(tr("修改客户"));
    cust_change->setFocusPolicy(Qt::NoFocus);
    connect(cust_change,SIGNAL(clicked(bool)),this,SLOT(changeCusts()));

    QGridLayout *icustLayout = new QGridLayout;
    icustLayout->addWidget(tab_icust,0,0,1,4);
    icustLayout->addWidget(cust_append,1,1);
    icustLayout->addWidget(cust_delete,1,2);
    icustLayout->addWidget(cust_change,1,3);
    icustLayout->setColumnStretch(0,1);
    icustLayout->setMargin(0);

    custWidget = new QWidget(this);
    custWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    custWidget->setLayout(icustLayout);
    custWidget->hide();

    QHBoxLayout *custlayout = new QHBoxLayout;
    custlayout->addLayout(scustLayout);
    custlayout->addLayout(ubtnsLayout);
    custlayout->addWidget(custWidget);
    custlayout->setStretch(0,1);

    QWidget *custs = new QWidget(this);
    custs->setLayout(custlayout);

    /**************************************************************************/
    tab_sales = new QTableView(this);
    tab_sales->setItemDelegate(new ReadOnlyDelegate);
    tab_sales->horizontalHeader()->setHighlightSections(false);
    tab_sales->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_sales,SIGNAL(clicked(QModelIndex)),this,SLOT(tabSaleSync(QModelIndex)));

    QPushButton *sale_update = new QPushButton(this);
    sale_update->setFlat(true);
    sale_update->setMinimumSize(97,44);
    sale_update->setText(tr("刷新显示"));
    sale_update->setFocusPolicy(Qt::NoFocus);
    connect(cust_update,SIGNAL(clicked(bool)),this,SLOT(updateSales()));

    QGridLayout *ssaleLayout = new QGridLayout;
    ssaleLayout->addWidget(tab_sales,0,0,1,2);
    ssaleLayout->addWidget(sale_update,1,1);
    ssaleLayout->setColumnStretch(0,1);

    btn_sales = new QToolButton(this);//显示订单
    btn_sales->setIcon(QIcon(":/icons/left.png"));
    btn_sales->setIconSize(QSize(30,30));
    btn_sales->setFocusPolicy(Qt::NoFocus);
    btn_sales->setText(tr("销\n售\n管\n理"));
    btn_sales->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_sales,SIGNAL(clicked(bool)),this,SLOT(showTabSale()));

    QVBoxLayout *sbtnsLayout = new QVBoxLayout;
    sbtnsLayout->addWidget(btn_sales);
    sbtnsLayout->addStretch();

    sale_items << tr("编号") << tr("记录") << tr("操作") << tr("销售姓名") << tr("所属区域");
    m_sales = new StandardItemModel();
    QStringList sale_header;
    sale_header << tr("项目") << tr("参数");
    m_sales->setHorizontalHeaderLabels(sale_header);
    for (int i=0; i < sale_items.size(); i++) {
        m_sales->setItem(i,0,new QStandardItem(sale_items.at(i)));
        m_sales->setItem(i,1,new QStandardItem(""));
    }
    tab_isale = new QTableView(this);
    tab_isale->setModel(m_sales);
    tab_isale->verticalHeader()->setVisible(false);
    tab_isale->setColumnWidth(0,100);
    tab_isale->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_isale->hideRow(SALES_UUID);
    tab_isale->hideRow(SALES_GUID);
    tab_isale->hideRow(SALES_SIGN);

    QPushButton *sale_append = new QPushButton(this);
    sale_append->setFlat(true);
    sale_append->setMinimumSize(97,44);
    sale_append->setText(tr("添加销售"));
    sale_append->setFocusPolicy(Qt::NoFocus);
    connect(sale_append,SIGNAL(clicked(bool)),this,SLOT(appendSales()));
    QPushButton *sale_delete = new QPushButton(this);
    sale_delete->setFlat(true);
    sale_delete->setMinimumSize(97,44);
    sale_delete->setText(tr("删除销售"));
    sale_delete->setFocusPolicy(Qt::NoFocus);
    connect(sale_delete,SIGNAL(clicked(bool)),this,SLOT(deleteSales()));
    QPushButton *sale_change = new QPushButton(this);
    sale_change->setFlat(true);
    sale_change->setMinimumSize(97,44);
    sale_change->setText(tr("修改销售"));
    sale_change->setFocusPolicy(Qt::NoFocus);
    connect(sale_change,SIGNAL(clicked(bool)),this,SLOT(changeSales()));

    QGridLayout *isaleLayout = new QGridLayout;
    isaleLayout->addWidget(tab_isale,0,0,1,4);
    isaleLayout->addWidget(sale_append,1,1);
    isaleLayout->addWidget(sale_delete,1,2);
    isaleLayout->addWidget(sale_change,1,3);
    isaleLayout->setColumnStretch(0,1);
    isaleLayout->setMargin(0);

    saleWidget = new QWidget(this);
    saleWidget->setLayout(isaleLayout);
    saleWidget->hide();

    QHBoxLayout *salelayout = new QHBoxLayout;
    salelayout->addLayout(ssaleLayout);
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

void SalesPage::initSql()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "erp_sales");
    db.setDatabaseName("erp.db");
    db.open();

    QString cmd = "select custs_uuid,custs_guid,custs_sign,custs_name,sales_name,sales_area ";
    cmd += "from erp_custs,erp_sales where custs_sale=sales_uuid";

    sql_custs = new SqlQueryModel(this);
    sql_custs->setQuery(cmd, db);

    for (int i=0; i < cust_items.size(); i++)
        sql_custs->setHeaderData(i, Qt::Horizontal, cust_items.at(i));
    tab_custs->setModel(sql_custs);
    tab_custs->horizontalHeader()->setSectionResizeMode(CUSTS_NAME,QHeaderView::Stretch);
    tab_custs->horizontalHeader()->setSectionResizeMode(CUSTS_SALE,QHeaderView::Stretch);
    tab_custs->horizontalHeader()->setSectionResizeMode(CUSTS_AREA,QHeaderView::Stretch);
    tab_custs->hideColumn(CUSTS_UUID);
    tab_custs->hideColumn(CUSTS_GUID);
    tab_custs->hideColumn(CUSTS_SIGN);

    sql_sales = new StandardSqlModel(this,db);
    sql_sales->setTable("erp_sales");
    sql_sales->select();

    for (int i=0; i < sale_items.size(); i++)
        sql_sales->setHeaderData(i, Qt::Horizontal, sale_items.at(i));
    tab_sales->setModel(sql_sales);
    tab_sales->horizontalHeader()->setSectionResizeMode(SALES_NAME,QHeaderView::Stretch);
    tab_sales->horizontalHeader()->setSectionResizeMode(SALES_AREA,QHeaderView::Stretch);
    tab_sales->hideColumn(SALES_UUID);
    tab_sales->hideColumn(SALES_GUID);
    tab_sales->hideColumn(SALES_SIGN);
}

void SalesPage::initData()
{
    //    sql_custs->select();
    //    sql_sales->select();
}

void SalesPage::showTabCust()
{
    if (custWidget->isHidden()) {
        custWidget->show();
        btn_custs->setIcon(QIcon(":/icons/right.png"));
    } else {
        custWidget->hide();
        btn_custs->setIcon(QIcon(":/icons/left.png"));
    }
    QStringList sales_head;
    for (int i=0; i < sql_sales->rowCount(); i++)
        sales_head.append(sql_sales->data(sql_sales->index(i,SALES_NAME)).toString());
    sale_delegate->setItemHeaders(sales_head);

    QStringList areas_head;
    for (int i=0; i < sql_sales->rowCount(); i++)
        areas_head.append(sql_sales->data(sql_sales->index(i,SALES_AREA)).toString());
    area_delegate->setItemHeaders(areas_head);
}

void SalesPage::showTabSale()
{
    if (saleWidget->isHidden()) {
        saleWidget->show();
        btn_sales->setIcon(QIcon(":/icons/right.png"));
    } else {
        saleWidget->hide();
        btn_sales->setIcon(QIcon(":/icons/left.png"));
    }
}

void SalesPage::tabCustSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_custs->rowCount(); i++) {
        m_custs->item(i,1)->setText(sql_custs->index(row,i).data().toString());
    }
}

void SalesPage::tabSaleSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_sales->rowCount(); i++) {
        m_sales->item(i,1)->setText(sql_sales->index(row,i).data().toString());
    }
}

void SalesPage::tabCustSyncExp(QStandardItem *item)
{
    int row = item->row();
    if (row == CUSTS_AREA) { //区域完成自动填充销售
        QString name;
        QString area = m_custs->item(CUSTS_AREA,1)->text();
        for (int i=0; i < sql_sales->rowCount(); i++) {
            if (area == sql_sales->data(sql_sales->index(i,SALES_AREA)).toString())
                name = sql_sales->data(sql_sales->index(i,SALES_NAME)).toString();
        }
        m_custs->item(CUSTS_SALE,1)->setText(name);
    }
}

void SalesPage::appendCusts()
{
    this->setFocus(); //完成输入


    if (m_custs->item(CUSTS_NAME,1)->text().isEmpty()) {
        QMessageBox::warning(this,"",tr("请输入姓名"));
        return;
    }
    if (m_custs->item(CUSTS_AREA,1)->text().isEmpty()) {
        QMessageBox::warning(this,"",tr("请输入区域"));
        return;
    }

    qint64 uuid = 0;
    QString area = m_custs->item(CUSTS_AREA,1)->text();
    sql_sales->select();
    for (int i=0; i < sql_sales->rowCount(); i++) {
        if (area == sql_sales->data(sql_sales->index(i,SALES_AREA)).toString()) {
            uuid = sql_sales->data(sql_sales->index(i,SALES_UUID)).toDouble();
            break;
        }
    }

    QJsonObject obj;
    obj.insert("command","erp_custs");
    obj.insert("custs_sign",1);
    obj.insert("custs_name",m_custs->item(CUSTS_NAME,1)->text());
    obj.insert("custs_sale",uuid);
    emit sendJson(obj);

    updateCusts();
}

void SalesPage::deleteCusts()
{
    this->setFocus(); //完成输入

    qint64 uuid = 0;
    QString area = m_custs->item(CUSTS_AREA,1)->text();
    sql_sales->select();
    for (int i=0; i < sql_sales->rowCount(); i++) {
        if (area == sql_sales->data(sql_sales->index(i,SALES_AREA)).toString()) {
            uuid = sql_sales->data(sql_sales->index(i,SALES_UUID)).toDouble();
            break;
        }
    }

    QJsonObject obj;
    obj.insert("command","erp_custs");
    obj.insert("custs_sign",2);
    obj.insert("custs_uuid",m_custs->item(CUSTS_UUID,1)->text().toDouble());
    obj.insert("custs_name",m_custs->item(CUSTS_NAME,1)->text());
    obj.insert("custs_sale",uuid);
    emit sendJson(obj);

    updateCusts();
}

void SalesPage::changeCusts()
{
    this->setFocus(); //完成输入

    qint64 uuid = 0;
    QString area = m_custs->item(CUSTS_AREA,1)->text();
    for (int i=0; i < sql_sales->rowCount(); i++) {
        if (area == sql_sales->data(sql_sales->index(i,SALES_AREA)).toString()) {
            uuid = sql_sales->data(sql_sales->index(i,SALES_UUID)).toDouble();
            break;
        }
    }

    QJsonObject obj;
    obj.insert("command","erp_custs");
    obj.insert("custs_sign",3);
    obj.insert("custs_uuid",m_custs->item(CUSTS_UUID,1)->text().toDouble());
    obj.insert("custs_name",m_custs->item(CUSTS_NAME,1)->text());
    obj.insert("custs_sale",uuid);
    emit sendJson(obj);

    updateCusts();
}

void SalesPage::updateCusts()
{
    QSqlQuery query(db);
    qint64 guid = 0;
    QJsonObject obj;

    query.prepare("select max(custs_guid) from erp_custs");
    query.exec();
    if (query.next())
        guid = query.value(0).toDouble();

    obj.insert("command","erp_custs");
    obj.insert("custs_guid",guid);
    obj.insert("custs_sign",0);
    emit sendJson(obj);
}

void SalesPage::appendSales()
{
    this->setFocus(); //完成输入

    QJsonObject obj;
    obj.insert("command","erp_sales");
    obj.insert("sales_sign",1);
    obj.insert("sales_name",m_sales->item(SALES_NAME,1)->text());
    obj.insert("sales_area",m_sales->item(SALES_AREA,1)->text());
    emit sendJson(obj);

    updateSales();
}

void SalesPage::deleteSales()
{
    this->setFocus(); //完成输入

    QJsonObject obj;
    obj.insert("command","erp_sales");
    obj.insert("sales_sign",2);
    obj.insert("sales_uuid",m_sales->item(SALES_UUID,1)->text().toDouble());
    obj.insert("sales_name",m_sales->item(SALES_NAME,1)->text());
    obj.insert("sales_area",m_sales->item(SALES_AREA,1)->text());
    emit sendJson(obj);

    updateSales();
}

void SalesPage::changeSales()
{
    this->setFocus(); //完成输入

    QJsonObject obj;
    obj.insert("command","erp_sales");
    obj.insert("sales_sign",3);
    obj.insert("sales_uuid",m_sales->item(SALES_UUID,1)->text().toDouble());
    obj.insert("sales_name",m_sales->item(SALES_NAME,1)->text());
    obj.insert("sales_area",m_sales->item(SALES_AREA,1)->text());
    emit sendJson(obj);

    updateSales();
}

void SalesPage::updateSales()
{
    QSqlQuery query(db);
    qint64 guid = 0;
    QJsonObject obj;

    query.prepare("select max(sales_guid) from erp_sales");
    query.exec();
    if (query.next())
        guid = query.value(0).toDouble();

    obj.insert("command","erp_sales");
    obj.insert("sales_guid",guid);
    obj.insert("sales_sign",0);
    emit sendJson(obj);

    sql_sales->select();
}

void SalesPage::recvNetJson(QJsonObject obj)
{
    QString cmd = obj.value("command").toString();
    if (cmd == "erp_sales")
        sql_sales->select();
    if (cmd == "erp_custs") {
        cmd = "select custs_uuid,custs_guid,custs_sign,custs_name,sales_name,sales_area ";
        cmd += "from erp_custs,erp_sales where custs_sale = sales_uuid";
        sql_custs->setQuery(cmd);
    }
}

void SalesPage::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    updateSales();
    updateCusts();
}
