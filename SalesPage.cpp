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
    connect(cust_update,SIGNAL(clicked(bool)),this,SLOT(updateCust()));

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

    cust_items << tr("编号") << tr("名称") << tr("销售") << tr("区域");
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
    tab_icust->setColumnWidth(0,50);
    tab_icust->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_icust->setItemDelegateForColumn(0,new ReadOnlyDelegate);
    tab_icust->setItemDelegateForRow(CUST_ID, new ReadOnlyDelegate);
    tab_icust->setItemDelegateForRow(CUST_SALE, sale_delegate);
    tab_icust->setItemDelegateForRow(CUST_AREA, area_delegate);
    tab_icust->hideRow(CUST_ID);

    QPushButton *cust_append = new QPushButton(this);
    cust_append->setFlat(true);
    cust_append->setMinimumSize(97,44);
    cust_append->setText(tr("添加客户"));
    cust_append->setFocusPolicy(Qt::NoFocus);
    connect(cust_append,SIGNAL(clicked(bool)),this,SLOT(appendCust()));
    QPushButton *cust_delete = new QPushButton(this);
    cust_delete->setFlat(true);
    cust_delete->setMinimumSize(97,44);
    cust_delete->setText(tr("删除客户"));
    cust_delete->setFocusPolicy(Qt::NoFocus);
    connect(cust_delete,SIGNAL(clicked(bool)),this,SLOT(deleteCust()));
    QPushButton *cust_change = new QPushButton(this);
    cust_change->setFlat(true);
    cust_change->setMinimumSize(97,44);
    cust_change->setText(tr("修改客户"));
    cust_change->setFocusPolicy(Qt::NoFocus);
    connect(cust_change,SIGNAL(clicked(bool)),this,SLOT(changeCust()));

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
    connect(cust_update,SIGNAL(clicked(bool)),this,SLOT(updateSale()));

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

    sale_items << tr("编号") << tr("记录") << tr("操作") << tr("姓名") << tr("区域");
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
    tab_isale->setColumnWidth(0,50);
    tab_isale->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_isale->hideRow(SALE_ID);
    tab_isale->hideRow(SALE_GUID);
    tab_isale->hideRow(SALE_SIGN);

    QPushButton *sale_append = new QPushButton(this);
    sale_append->setFlat(true);
    sale_append->setMinimumSize(97,44);
    sale_append->setText(tr("添加销售"));
    sale_append->setFocusPolicy(Qt::NoFocus);
    connect(sale_append,SIGNAL(clicked(bool)),this,SLOT(appendSale()));
    QPushButton *sale_delete = new QPushButton(this);
    sale_delete->setFlat(true);
    sale_delete->setMinimumSize(97,44);
    sale_delete->setText(tr("删除销售"));
    sale_delete->setFocusPolicy(Qt::NoFocus);
    connect(sale_delete,SIGNAL(clicked(bool)),this,SLOT(deleteSale()));
    QPushButton *sale_change = new QPushButton(this);
    sale_change->setFlat(true);
    sale_change->setMinimumSize(97,44);
    sale_change->setText(tr("修改销售"));
    sale_change->setFocusPolicy(Qt::NoFocus);
    connect(sale_change,SIGNAL(clicked(bool)),this,SLOT(changeSale()));

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

    sql_custs = new StandardSqlModel(this,db);
    sql_custs->setTable("erp_custs");
    sql_custs->select();

    for (int i=0; i < cust_items.size(); i++)
        sql_custs->setHeaderData(i, Qt::Horizontal, cust_items.at(i));
    tab_custs->setModel(sql_custs);
    tab_custs->setColumnWidth(CUST_ID,50);
    tab_custs->horizontalHeader()->setSectionResizeMode(CUST_NAME,QHeaderView::Stretch);
    tab_custs->horizontalHeader()->setSectionResizeMode(CUST_SALE,QHeaderView::Stretch);
    tab_custs->horizontalHeader()->setSectionResizeMode(CUST_AREA,QHeaderView::Stretch);
    tab_custs->hideColumn(CUST_ID);

    sql_sales = new StandardSqlModel(this,db);
    sql_sales->setTable("erp_sales");
    sql_sales->select();

    for (int i=0; i < sale_items.size(); i++)
        sql_sales->setHeaderData(i, Qt::Horizontal, sale_items.at(i));
    tab_sales->setModel(sql_sales);
    tab_sales->setColumnWidth(SALE_ID,50);
    tab_sales->horizontalHeader()->setSectionResizeMode(SALE_NAME,QHeaderView::Stretch);
    tab_sales->horizontalHeader()->setSectionResizeMode(SALE_AREA,QHeaderView::Stretch);
    tab_sales->hideColumn(SALE_ID);
}

void SalesPage::initData()
{
    sql_custs->select();
    sql_sales->select();
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
        sales_head.append(sql_sales->data(sql_sales->index(i,SALE_NAME)).toString());
    sale_delegate->setItemHeaders(sales_head);

    QStringList areas_head;
    for (int i=0; i < sql_sales->rowCount(); i++)
        areas_head.append(sql_sales->data(sql_sales->index(i,SALE_AREA)).toString());
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

void SalesPage::appendCust()
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

void SalesPage::deleteCust()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_custs");
    obj.insert("logs_sign",2);
    obj.insert("tabs_guid",m_custs->item(CUST_ID,1)->text().toDouble());
    obj.insert("cust_name",m_custs->item(CUST_NAME,1)->text());
    obj.insert("cust_sale",m_custs->item(CUST_SALE,1)->text());
    obj.insert("cust_area",m_custs->item(CUST_AREA,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_custs->rowCount(); i++)
        m_custs->item(i,1)->setText("");
}

void SalesPage::changeCust()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_custs");
    obj.insert("logs_sign",3);
    obj.insert("tabs_guid",m_custs->item(CUST_ID,1)->text().toDouble());
    obj.insert("cust_name",m_custs->item(CUST_NAME,1)->text());
    obj.insert("cust_sale",m_custs->item(CUST_SALE,1)->text());
    obj.insert("cust_area",m_custs->item(CUST_AREA,1)->text());
    emit sendJson(obj);
}

void SalesPage::updateCust()
{
    sql_custs->select();
}

void SalesPage::appendSale()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_sales");
    obj.insert("logs_sign",1);
    obj.insert("sale_name",m_sales->item(SALE_NAME,1)->text());
    obj.insert("sale_area",m_sales->item(SALE_AREA,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_sales->rowCount(); i++)
        m_sales->item(i,1)->setText("");
}

void SalesPage::deleteSale()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_sales");
    obj.insert("logs_sign",2);
    obj.insert("tabs_guid",m_sales->item(SALE_ID,1)->text().toDouble());
    obj.insert("sale_name",m_sales->item(SALE_NAME,1)->text());
    obj.insert("sale_area",m_sales->item(SALE_AREA,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_sales->rowCount(); i++)
        m_sales->item(i,1)->setText("");
}

void SalesPage::changeSale()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_sales");
    obj.insert("logs_sign",3);
    obj.insert("tabs_guid",m_sales->item(SALE_ID,1)->text().toDouble());
    obj.insert("sale_name",m_sales->item(SALE_NAME,1)->text());
    obj.insert("sale_area",m_sales->item(SALE_AREA,1)->text());
    emit sendJson(obj);
}

void SalesPage::updateSale()
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

    sql_sales->select();
}

void SalesPage::recvSocket(QUrl url)
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

void SalesPage::recvSalesJson(QJsonObject obj)
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
    sql_sales->select();
}

void SalesPage::recvCustsJson(QJsonObject obj)
{
    QSqlQuery query(db);
    qint64 logs_sign = obj.value("logs_sign").toDouble();
    qint64 logs_guid = obj.value("logs_guid").toDouble();
    qint64 tabs_guid = obj.value("tabs_guid").toDouble();

    query.prepare("select count(*) from erp_custs_log where id=:id");
    query.bindValue(":id",logs_guid);
    query.exec();
    query.next();
    if (query.value(0).toInt() > 0)
        return;
    QString cmd;

    switch (logs_sign) {
    case 0://查询
        logs_guid = tabs_guid;
        if (logs_guid == 0xffffffff) {
            qDebug() << "blank";
            query.prepare("select * from erp_custs_log");
        } else {
            query.prepare("select * from erp_custs_log where id>:id");
            query.bindValue(":id",logs_guid);
        }
        query.exec();
        while (query.next()) {
            QJsonObject sned_obj;
            sned_obj.insert("sendto",obj.value("sender").toString());
            sned_obj.insert("logs_cmmd","erp_custs");
            sned_obj.insert("logs_guid",query.value(0).toDouble());
            sned_obj.insert("logs_sign",query.value(1).toDouble());
            sned_obj.insert("tabs_guid",query.value(2).toDouble());
            sned_obj.insert("cust_name",query.value(3).toString());
            sned_obj.insert("cust_sale",query.value(4).toString());
            sned_obj.insert("cust_area",query.value(5).toString());
            emit sendJson(sned_obj);
        }
        return;
        break;
    case 1://增加
        tabs_guid = logs_guid;
        query.prepare("insert into erp_custs values(?,?,?,?)");
        query.bindValue(0,tabs_guid);
        query.bindValue(1,obj.value("cust_name").toString());
        query.bindValue(2,obj.value("cust_sale").toString());
        query.bindValue(3,obj.value("cust_area").toString());
        query.exec();
        break;
    case 2://删除
        query.prepare("delete from erp_custs where id=:id");
        query.bindValue(":id",tabs_guid);
        query.exec();
        break;
    case 3://修改
        cmd += "update erp_custs set ";
        cmd += "cust_name=:cust_name,";
        cmd += "cust_sale=:cust_sale,";
        cmd += "cust_area=:cust_area ";
        cmd += "where id=:tabs_guid";
        query.prepare(cmd);
        query.bindValue(":cust_name",obj.value("cust_name").toString());
        query.bindValue(":cust_sale",obj.value("cust_sale").toString());
        query.bindValue(":cust_area",obj.value("cust_area").toString());
        query.bindValue(":tabs_guid",tabs_guid);
        query.exec();
        break;
    default:
        break;
    }
    query.prepare("insert into erp_custs_log values(?,?,?,?,?,?)");
    query.bindValue(0,logs_guid);
    query.bindValue(1,logs_sign);
    query.bindValue(2,tabs_guid);
    query.bindValue(3,obj.value("cust_name").toString());
    query.bindValue(4,obj.value("cust_sale").toString());
    query.bindValue(5,obj.value("cust_area").toString());
    query.exec();
    sql_custs->select();
}

void SalesPage::showEvent(QShowEvent *e)
{
    updateSale();
    updateCust();
    e->accept();
}
