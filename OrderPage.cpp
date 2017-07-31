#include "OrderPage.h"

OrderPage::OrderPage(QWidget *parent) : QWidget(parent)
{
    initUI();
    initSql();
}

OrderPage::~OrderPage()
{
    db.close();
}

void OrderPage::initUI()
{
    this->setObjectName("OrderPage");

    tab_order = new QTableView(this);
    tab_order->setItemDelegate(new ReadOnlyDelegate);
    tab_order->horizontalHeader()->setHighlightSections(false);
    tab_order->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_order,SIGNAL(clicked(QModelIndex)),this,SLOT(tabOrderSync(QModelIndex)));

    tab_sends = new QTableView(this);
    tab_sends->setItemDelegate(new ReadOnlyDelegate);
    tab_sends->horizontalHeader()->setHighlightSections(false);
    tab_sends->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_sends,SIGNAL(clicked(QModelIndex)),this,SLOT(tabSendsSync(QModelIndex)));

    QPushButton *order_update = new QPushButton(this);
    order_update->setFlat(true);
    order_update->setMinimumSize(97,44);
    order_update->setText(tr("刷新显示"));
    order_update->setFocusPolicy(Qt::NoFocus);
    connect(order_update,SIGNAL(clicked(bool)),this,SLOT(initOrder()));

    QSplitter *pSpliter = new QSplitter(Qt::Vertical);
    pSpliter->addWidget(tab_order);
    pSpliter->addWidget(tab_sends);
    pSpliter->setStretchFactor(0,8);
    pSpliter->setStretchFactor(1,2);

    QGridLayout *sorderLayout = new QGridLayout;
    sorderLayout->addWidget(pSpliter,0,0,1,2);
    sorderLayout->addWidget(order_update,1,1);
    sorderLayout->setColumnStretch(0,1);

    btn_order = new QToolButton(this);
    btn_order->setIcon(QIcon(":/icons/left.png"));
    btn_order->setIconSize(QSize(30,30));
    btn_order->setFocusPolicy(Qt::NoFocus);
    btn_order->setText(tr("订\n单\n管\n理"));
    btn_order->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_order,SIGNAL(clicked(bool)),this,SLOT(showTabOrder()));

    btn_sends = new QToolButton(this);
    btn_sends->setIcon(QIcon(":/icons/left.png"));
    btn_sends->setIconSize(QSize(30,30));
    btn_sends->setFocusPolicy(Qt::NoFocus);
    btn_sends->setText(tr("发\n货\n管\n理"));
    btn_sends->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_sends,SIGNAL(clicked(bool)),this,SLOT(showTabSends()));

    QVBoxLayout *obtnsLayout = new QVBoxLayout;
    obtnsLayout->addWidget(btn_order);
    obtnsLayout->addWidget(btn_sends);
    obtnsLayout->addStretch();

    order_items << tr("编号") << tr("记录") << tr("操作") << tr("订单单号")
                << tr("下单日期") << tr("所属区域") << tr("业务经理") << tr("客户名称")
                << tr("评审单号") << tr("订货数量") << tr("发货日期") << tr("备注内容");
    m_order = new StandardItemModel();
    QStringList order_header;
    order_header << tr("项目") << tr("参数");
    m_order->setHorizontalHeaderLabels(order_header);
    for (int i=0; i < order_items.size(); i++) {
        m_order->setItem(i,0,new QStandardItem(order_items.at(i)));
        m_order->setItem(i,1,new QStandardItem(""));
    }
    sale_delegate = new ComboBoxDelegate;
    area_delegate = new ComboBoxDelegate;
    cust_delegate = new ComboBoxDelegate;
    tab_iorder = new QTableView(this);
    tab_iorder->setModel(m_order);
    tab_iorder->verticalHeader()->setVisible(false);
    tab_iorder->setColumnWidth(0,100);
    tab_iorder->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_iorder->setItemDelegateForColumn(0,new ReadOnlyDelegate);
    tab_iorder->setItemDelegateForRow(ORDER_DATE,new DateEditDelegate);
    tab_iorder->setItemDelegateForRow(ORDER_CUST,cust_delegate);
    tab_iorder->setItemDelegateForRow(ORDER_AREA,area_delegate);
    tab_iorder->setItemDelegateForRow(ORDER_SALE,sale_delegate);
    tab_iorder->setItemDelegateForRow(ORDER_DEAD,new DateEditDelegate);
    tab_iorder->hideRow(ORDER_UUID);
    tab_iorder->hideRow(ORDER_GUID);
    tab_iorder->hideRow(ORDER_SIGN);
    tab_iorder->hideRow(ORDER_PROD);
    tab_iorder->hideRow(ORDER_STCK);
    tab_iorder->hideRow(ORDER_LNUM);
    tab_iorder->hideRow(ORDER_DNUM);

    QPushButton *order_append = new QPushButton(this);
    order_append->setFlat(true);
    order_append->setMinimumSize(97,44);
    order_append->setText(tr("添加订单"));
    order_append->setFocusPolicy(Qt::NoFocus);
    connect(order_append,SIGNAL(clicked(bool)),this,SLOT(appendOrder()));
    QPushButton *order_delete = new QPushButton(this);
    order_delete->setFlat(true);
    order_delete->setMinimumSize(97,44);
    order_delete->setText(tr("删除订单"));
    order_delete->setFocusPolicy(Qt::NoFocus);
    connect(order_delete,SIGNAL(clicked(bool)),this,SLOT(deleteOrder()));
    QPushButton *order_change = new QPushButton(this);
    order_change->setFlat(true);
    order_change->setMinimumSize(97,44);
    order_change->setText(tr("修改订单"));
    order_change->setFocusPolicy(Qt::NoFocus);
    connect(order_change,SIGNAL(clicked(bool)),this,SLOT(changeOrder()));

    QGridLayout *iorderLayout = new QGridLayout;
    iorderLayout->addWidget(tab_iorder,0,0,1,4);
    iorderLayout->addWidget(order_append,1,1);
    iorderLayout->addWidget(order_delete,1,2);
    iorderLayout->addWidget(order_change,1,3);
    iorderLayout->setColumnStretch(0,1);
    iorderLayout->setMargin(0);

    orderWidget = new QWidget(this);
    orderWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    orderWidget->setLayout(iorderLayout);
    orderWidget->hide();
    ///////////////////////////////////////////////////////////////////////////
    sends_items << tr("编号") << tr("记录") << tr("操作") << tr("订单单号") << tr("发货日期")
                << tr("客户名称") << tr("发货方式") << tr("货运单号") << tr("运费") << tr("发货数量") << tr("备注内容");

    m_sends = new StandardItemModel();
    QStringList sends_header;
    sends_header << tr("项目") << tr("参数");
    m_sends->setHorizontalHeaderLabels(sends_header);
    for (int i=0; i < sends_items.size(); i++) {
        m_sends->setItem(i,0,new QStandardItem(sends_items.at(i)));
        m_sends->setItem(i,1,new QStandardItem(""));
    }
    tab_isend = new QTableView(this);
    tab_isend->setModel(m_sends);
    tab_isend->verticalHeader()->setVisible(false);
    tab_isend->setColumnWidth(0,100);
    tab_isend->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_isend->hideRow(SENDS_UUID);
    tab_isend->hideRow(SENDS_GUID);
    tab_isend->hideRow(SENDS_SIGN);

    QPushButton *sends_append = new QPushButton(this);
    sends_append->setFlat(true);
    sends_append->setMinimumSize(97,44);
    sends_append->setText(tr("添加发货"));
    sends_append->setFocusPolicy(Qt::NoFocus);
    connect(sends_append,SIGNAL(clicked(bool)),this,SLOT(appendSends()));
    QPushButton *sends_delete = new QPushButton(this);
    sends_delete->setFlat(true);
    sends_delete->setMinimumSize(97,44);
    sends_delete->setText(tr("删除发货"));
    sends_delete->setFocusPolicy(Qt::NoFocus);
    connect(sends_delete,SIGNAL(clicked(bool)),this,SLOT(deleteSends()));
    QPushButton *sends_change = new QPushButton(this);
    sends_change->setFlat(true);
    sends_change->setMinimumSize(97,44);
    sends_change->setText(tr("修改发货"));
    sends_change->setFocusPolicy(Qt::NoFocus);
    connect(sends_change,SIGNAL(clicked(bool)),this,SLOT(changeSends()));

    QGridLayout *isendsLayout = new QGridLayout;
    isendsLayout->addWidget(tab_isend,0,0,1,4);
    isendsLayout->addWidget(sends_append,1,1);
    isendsLayout->addWidget(sends_delete,1,2);
    isendsLayout->addWidget(sends_change,1,3);
    isendsLayout->setColumnStretch(0,1);
    isendsLayout->setMargin(0);

    sendsWidget = new QWidget(this);
    sendsWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    sendsWidget->setLayout(isendsLayout);
    sendsWidget->hide();

    QHBoxLayout *orderlayout = new QHBoxLayout;
    orderlayout->addLayout(sorderLayout);
    orderlayout->addLayout(obtnsLayout);
    orderlayout->addWidget(orderWidget);
    orderlayout->addWidget(sendsWidget);
    orderlayout->setStretch(0,1);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(orderlayout);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(group);
    this->setLayout(mainLayout);
}

void OrderPage::initSql()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "erp_order");
    db.setDatabaseName("erp.db");
    db.open();
    sql_order = new SqlQueryModel(this);
    QString cmd = "select order_uuid,order_guid,order_sign,order_numb,order_date,";
    cmd += "sales_area,sales_name,custs_name,";
    cmd += "order_view,order_quan,order_dead,order_mark ";
    cmd += "from erp_order,erp_custs,erp_sales ";
    cmd += "where order_cust=custs_uuid and custs_sale=sales_uuid";
    sql_order->setQuery(cmd,db);
    for (int i=0; i < order_items.size(); i++)
        sql_order->setHeaderData(i, Qt::Horizontal, order_items.at(i));
    tab_order->setModel(sql_order);
    tab_order->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_order->hideColumn(ORDER_UUID);
    tab_order->hideColumn(ORDER_GUID);
    tab_order->hideColumn(ORDER_SIGN);

    sql_sends = new StandardSqlModel(this,db);
    sql_sends->setTable("erp_sends");
    sql_sends->select();
    for (int i=0; i < sends_items.size(); i++)
        sql_sends->setHeaderData(i, Qt::Horizontal, sends_items.at(i));
    tab_sends->setModel(sql_sends);
    tab_sends->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_sends->hideColumn(SENDS_UUID);
    tab_sends->hideColumn(SENDS_GUID);
    tab_sends->hideColumn(SENDS_SIGN);

    sql_custs = new StandardSqlModel(this,db);
    sql_custs->setTable("erp_custs");
    sql_custs->select();

    sql_sales = new StandardSqlModel(this,db);
    sql_sales->setTable("erp_sales");
    sql_sales->select();

}
void OrderPage::showTabOrder()
{
    if (orderWidget->isHidden()) {
        orderWidget->show();
        btn_order->setIcon(QIcon(":/icons/right.png"));
    } else {
        orderWidget->hide();
        btn_order->setIcon(QIcon(":/icons/left.png"));
    }

    QStringList sales_head;
    for (int i=0; i < sql_sales->rowCount(); i++)
        sales_head.append(sql_sales->data(sql_sales->index(i,SALES_NAME)).toString());
    sale_delegate->setItemHeaders(sales_head);

    QStringList areas_head;
    for (int i=0; i < sql_sales->rowCount(); i++)
        areas_head.append(sql_sales->data(sql_sales->index(i,SALES_AREA)).toString());
    area_delegate->setItemHeaders(areas_head);

    QStringList customs_head;
    for (int i=0; i < sql_custs->rowCount(); i++)
        customs_head.append(sql_custs->data(sql_custs->index(i,CUSTS_NAME)).toString());
    cust_delegate->setItemHeaders(customs_head);

    for (int i=0; i < m_order->rowCount(); i++) {
        m_order->item(i,1)->setText("");
    }

    autoNumber();
    m_order->item(ORDER_DATE,1)->setText(QDate::currentDate().toString("yyyy-MM-dd"));
}

void OrderPage::showTabSends()
{
    if (sendsWidget->isHidden()) {
        sendsWidget->show();
        btn_sends->setIcon(QIcon(":/icons/right.png"));
    } else {
        sendsWidget->hide();
        btn_sends->setIcon(QIcon(":/icons/left.png"));
    }
    m_sends->item(SENDS_DATE,1)->setText(QDate::currentDate().toString("yyyy-MM-dd"));
}

void OrderPage::autoNumber()
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
    m_order->item(ORDER_NUMB,1)->setText(odds);
}

void OrderPage::appendOrder()
{
    this->setFocus();

    sql_custs->select();
    qint64 uuid = 0;
    QString name = m_order->item(ORDER_CUST,1)->text();
    for (int i=0; i < sql_custs->rowCount(); i++) {
        if (name == sql_custs->data(sql_custs->index(i,CUSTS_NAME)).toString()) {
            uuid = sql_custs->data(sql_custs->index(i,CUSTS_UUID)).toDouble();
            break;
        }
    }

    QJsonObject obj;
    obj.insert("command","erp_order");
    obj.insert("order_sign",1);
    obj.insert("order_numb",m_order->item(0x03,1)->text());
    obj.insert("order_date",m_order->item(0x04,1)->text());
    obj.insert("order_cust",uuid);
    obj.insert("order_view",m_order->item(0x08,1)->text());
    obj.insert("order_dead",m_order->item(0x09,1)->text());
    obj.insert("order_quan",m_order->item(0x0A,1)->text());
    obj.insert("order_mark",m_order->item(0x0B,1)->text());
    emit sendJson(obj);

    updateOrder();
}

void OrderPage::deleteOrder()
{
    this->setFocus();

    sql_custs->select();
    qint64 uuid = 0;
    QString name = m_order->item(ORDER_CUST,1)->text();
    for (int i=0; i < sql_custs->rowCount(); i++) {
        if (name == sql_custs->data(sql_custs->index(i,CUSTS_NAME)).toString()) {
            uuid = sql_custs->data(sql_custs->index(i,CUSTS_UUID)).toDouble();
            break;
        }
    }

    QJsonObject obj;
    obj.insert("command","erp_order");
    obj.insert("order_sign",2);
    obj.insert("order_uuid",m_order->item(0x00,1)->text().toDouble());
    obj.insert("order_numb",m_order->item(0x03,1)->text());
    obj.insert("order_date",m_order->item(0x04,1)->text());
    obj.insert("order_cust",uuid);
    obj.insert("order_view",m_order->item(0x08,1)->text());
    obj.insert("order_dead",m_order->item(0x09,1)->text());
    obj.insert("order_quan",m_order->item(0x0A,1)->text());
    obj.insert("order_mark",m_order->item(0x0B,1)->text());
    emit sendJson(obj);

    updateOrder();
}

void OrderPage::changeOrder()
{
    this->setFocus();

    sql_custs->select();
    qint64 uuid = 0;
    QString name = m_order->item(ORDER_CUST,1)->text();
    for (int i=0; i < sql_custs->rowCount(); i++) {
        if (name == sql_custs->data(sql_custs->index(i,CUSTS_NAME)).toString()) {
            uuid = sql_custs->data(sql_custs->index(i,CUSTS_UUID)).toDouble();
            break;
        }
    }

    QJsonObject obj;
    obj.insert("command","erp_order");
    obj.insert("order_sign",3);
    obj.insert("order_uuid",m_order->item(0x00,1)->text().toDouble());
    obj.insert("order_numb",m_order->item(0x03,1)->text());
    obj.insert("order_date",m_order->item(0x04,1)->text());
    obj.insert("order_cust",uuid);
    obj.insert("order_view",m_order->item(0x08,1)->text());
    obj.insert("order_dead",m_order->item(0x09,1)->text());
    obj.insert("order_quan",m_order->item(0x0A,1)->text());
    obj.insert("order_mark",m_order->item(0x0B,1)->text());
    emit sendJson(obj);

    updateOrder();
}

void OrderPage::appendSends()
{
    QJsonObject obj;
    obj.insert("command","erp_sends");
    obj.insert("sends_sign",1);
    obj.insert("sends_numb",m_sends->item(0x03,1)->text());
    obj.insert("sends_date",m_sends->item(0x04,1)->text());
    obj.insert("sends_cust",m_sends->item(0x05,1)->text());
    obj.insert("sends_mode",m_sends->item(0x06,1)->text());
    obj.insert("sends_code",m_sends->item(0x07,1)->text());
    obj.insert("sends_prce",m_sends->item(0x08,1)->text());
    obj.insert("sends_quan",m_sends->item(0x09,1)->text());
    obj.insert("sends_mark",m_sends->item(0x0A,1)->text());
    emit sendJson(obj);

    updateSends();
}

void OrderPage::deleteSends()
{
    QJsonObject obj;
    obj.insert("command","erp_sends");
    obj.insert("sends_sign",2);
    obj.insert("sends_uuid",m_sends->item(0x00,1)->text().toDouble());
    obj.insert("sends_numb",m_sends->item(0x03,1)->text());
    obj.insert("sends_date",m_sends->item(0x04,1)->text());
    obj.insert("sends_cust",m_sends->item(0x05,1)->text());
    obj.insert("sends_mode",m_sends->item(0x06,1)->text());
    obj.insert("sends_code",m_sends->item(0x07,1)->text());
    obj.insert("sends_prce",m_sends->item(0x08,1)->text());
    obj.insert("sends_quan",m_sends->item(0x09,1)->text());
    obj.insert("sends_mark",m_sends->item(0x0A,1)->text());
    emit sendJson(obj);

    updateSends();
}

void OrderPage::changeSends()
{
    QJsonObject obj;
    obj.insert("command","erp_sends");
    obj.insert("sends_sign",3);
    obj.insert("sends_uuid",m_sends->item(0x00,1)->text().toDouble());
    obj.insert("sends_numb",m_sends->item(0x03,1)->text());
    obj.insert("sends_date",m_sends->item(0x04,1)->text());
    obj.insert("sends_cust",m_sends->item(0x05,1)->text());
    obj.insert("sends_mode",m_sends->item(0x06,1)->text());
    obj.insert("sends_code",m_sends->item(0x07,1)->text());
    obj.insert("sends_prce",m_sends->item(0x08,1)->text());
    obj.insert("sends_quan",m_sends->item(0x09,1)->text());
    obj.insert("sends_mark",m_sends->item(0x0A,1)->text());
    emit sendJson(obj);

    updateSends();
}

void OrderPage::initOrder()
{
    updateSends();
    updateCusts();
    updateSales();
    updateOrder();
}

void OrderPage::updateOrder()
{
    QSqlQuery query(db);
    qint64 logs_guid = 0;
    QJsonObject obj;

    query.prepare("select max(order_guid) from erp_order");
    query.exec();
    if (query.next())
        logs_guid = query.value(0).toDouble();

    obj.insert("command","erp_order");
    obj.insert("order_guid",logs_guid);
    obj.insert("order_sign",0);
    emit sendJson(obj);
}

void OrderPage::updateSends()
{
    QSqlQuery query(db);
    qint64 logs_guid = 0;
    QJsonObject obj;

    query.prepare("select max(sends_guid) from erp_sends");
    query.exec();
    if (query.next())
        logs_guid = query.value(0).toDouble();

    obj.insert("command","erp_sends");
    obj.insert("sends_guid",logs_guid);
    obj.insert("sends_sign",0);
    emit sendJson(obj);

    sql_sends->select();
}

void OrderPage::updateCusts()
{
    QSqlQuery query(db);
    qint64 logs_guid = 0;
    QJsonObject obj;

    query.prepare("select max(custs_guid) from erp_custs");
    query.exec();
    if (query.next())
        logs_guid = query.value(0).toDouble();

    obj.insert("command","erp_custs");
    obj.insert("logs_guid",logs_guid);
    obj.insert("logs_sign",0);
    emit sendJson(obj);

    sql_custs->select();
}

void OrderPage::updateSales()
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

void OrderPage::tabOrderSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_order->rowCount(); i++) {
        m_order->item(i,1)->setText(sql_order->index(row,i).data().toString());
    }
    m_sends->item(SENDS_NUMB,1)->setText(sql_order->index(row,ORDER_NUMB).data().toString());
    m_sends->item(SENDS_CUST,1)->setText(sql_order->index(row,ORDER_CUST).data().toString());
}

void OrderPage::tabSendsSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_sends->rowCount(); i++) {
        m_sends->item(i,1)->setText(sql_sends->index(row,i).data().toString());
    }
}

void OrderPage::recvNetJson(QJsonObject obj)
{
    QString cmd = obj.value("command").toString();
    if (cmd == "erp_order") {
        QString cmd = "select order_uuid,order_guid,order_sign,order_numb,order_date,";
        cmd += "sales_area,sales_name,custs_name,";
        cmd += "order_view,order_quan,order_dead,order_mark ";
        cmd += "from erp_order,erp_custs,erp_sales ";
        cmd += "where order_cust=custs_uuid and custs_sale=sales_uuid";
        sql_order->setQuery(cmd,db);
    }
    if (cmd == "erp_sends")
        sql_sends->select();
    if (cmd == "erp_custs")
        sql_custs->select();
    if (cmd == "erp_sales")
        sql_sales->select();
}

void OrderPage::recvAppShow(QString win)
{
    if (win != this->objectName())
        return;
    updateSends();
    updateCusts();
    updateSales();
    updateOrder();
}
