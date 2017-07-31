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
    connect(order_update,SIGNAL(clicked(bool)),this,SLOT(updateOrder()));

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
    tab_iorder->setColumnWidth(0,100);
    tab_iorder->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_iorder->setItemDelegateForColumn(0,new ReadOnlyDelegate);
    tab_iorder->setItemDelegateForRow(ORDER_DATE,new DateEditDelegate);
    tab_iorder->setItemDelegateForRow(ORDER_CUST,cust_delegate);
    tab_iorder->setItemDelegateForRow(ORDER_AREA,area_delegate);
    tab_iorder->setItemDelegateForRow(ORDER_SALE,sale_delegate);
    tab_iorder->setItemDelegateForRow(ORDER_DEAD,new DateEditDelegate);
    tab_iorder->hideRow(ORDER_ID);
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
    sends_items << tr("编号") << tr("记录") << tr("操作") << tr("订单单号") << tr("评审单号")
                << tr("运单单号") << tr("发货方式") << tr("运费") << tr("备注内容");
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
    tab_isend->setColumnWidth(0,100);
    tab_isend->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_isend->hideRow(SEND_ID);
    tab_isend->hideRow(SEND_GUID);
    tab_isend->hideRow(SEND_SIGN);

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
    sql_order = new StandardSqlModel(this,db);
    sql_order->setTable("erp_order");
    sql_order->select();

    for (int i=0; i < order_items.size(); i++)
        sql_order->setHeaderData(i, Qt::Horizontal, order_items.at(i));
    tab_order->setModel(sql_order);
    tab_order->setColumnWidth(ORDER_ID,50);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_NUMB,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_CUST,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_SALE,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_AREA,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_QUAN,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_DEAD,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_PROD,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_STCK,QHeaderView::Stretch);
    tab_order->horizontalHeader()->setSectionResizeMode(ORDER_DNUM,QHeaderView::Stretch);
    tab_order->hideColumn(ORDER_ID);
    tab_order->hideColumn(ORDER_GUID);
    tab_order->hideColumn(ORDER_SIGN);

    sql_sends = new StandardSqlModel(this,db);
    sql_sends->setTable("erp_sends");
    sql_sends->select();
    for (int i=0; i < sends_items.size(); i++)
        sql_sends->setHeaderData(i, Qt::Horizontal, sends_items.at(i));
    tab_sends->setModel(sql_sends);
    tab_sends->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tab_sends->hideColumn(SEND_ID);
    tab_sends->hideColumn(SEND_GUID);
    tab_sends->hideColumn(SEND_SIGN);

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
        sales_head.append(sql_sales->data(sql_sales->index(i,1)).toString());
    sale_delegate->setItemHeaders(sales_head);

    QStringList areas_head;
    for (int i=0; i < sql_sales->rowCount(); i++)
        areas_head.append(sql_sales->data(sql_sales->index(i,2)).toString());
    area_delegate->setItemHeaders(areas_head);

    QStringList customs_head;
    for (int i=0; i < sql_custs->rowCount(); i++)
        customs_head.append(sql_custs->data(sql_custs->index(i,1)).toString());
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
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_order");
    obj.insert("logs_sign",1);
    obj.insert("order_numb",m_order->item(ORDER_NUMB,1)->text());
    obj.insert("order_date",m_order->item(ORDER_DATE,1)->text());
    obj.insert("order_view",m_order->item(ORDER_VIEW,1)->text());
    obj.insert("order_cust",m_order->item(ORDER_CUST,1)->text());
    obj.insert("order_sale",m_order->item(ORDER_SALE,1)->text());
    obj.insert("order_area",m_order->item(ORDER_AREA,1)->text());
    obj.insert("order_dead",m_order->item(ORDER_DEAD,1)->text());
    obj.insert("order_quan",m_order->item(ORDER_QUAN,1)->text());
    obj.insert("order_mark",m_order->item(ORDER_MARK,1)->text());
    obj.insert("order_prod",m_order->item(ORDER_PROD,1)->text());
    obj.insert("order_stck",m_order->item(ORDER_STCK,1)->text());
    obj.insert("order_lnum",m_order->item(ORDER_LNUM,1)->text());
    obj.insert("order_dnum",m_order->item(ORDER_DNUM,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_order->rowCount(); i++)
        m_order->item(i,1)->setText("");
}

void OrderPage::deleteOrder()
{
    if (m_order->item(ORDER_ID,1)->text().isEmpty())
        return;
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_order");
    obj.insert("logs_sign",2);
    obj.insert("tabs_guid",m_order->item(ORDER_ID,1)->text().toDouble());
    obj.insert("order_numb",m_order->item(ORDER_NUMB,1)->text());
    obj.insert("order_date",m_order->item(ORDER_DATE,1)->text());
    obj.insert("order_view",m_order->item(ORDER_VIEW,1)->text());
    obj.insert("order_cust",m_order->item(ORDER_CUST,1)->text());
    obj.insert("order_sale",m_order->item(ORDER_SALE,1)->text());
    obj.insert("order_area",m_order->item(ORDER_AREA,1)->text());
    obj.insert("order_dead",m_order->item(ORDER_DEAD,1)->text());
    obj.insert("order_quan",m_order->item(ORDER_QUAN,1)->text());
    obj.insert("order_mark",m_order->item(ORDER_MARK,1)->text());
    obj.insert("order_prod",m_order->item(ORDER_PROD,1)->text());
    obj.insert("order_stck",m_order->item(ORDER_STCK,1)->text());
    obj.insert("order_lnum",m_order->item(ORDER_LNUM,1)->text());
    obj.insert("order_dnum",m_order->item(ORDER_DNUM,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_order->rowCount(); i++)
        m_order->item(i,1)->setText("");
}

void OrderPage::changeOrder()
{
    if (m_order->item(ORDER_ID,1)->text().isEmpty())
        return;
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_order");
    obj.insert("logs_sign",3);
    obj.insert("tabs_guid",m_order->item(ORDER_ID,1)->text().toDouble());
    obj.insert("order_numb",m_order->item(ORDER_NUMB,1)->text());
    obj.insert("order_date",m_order->item(ORDER_DATE,1)->text());
    obj.insert("order_view",m_order->item(ORDER_VIEW,1)->text());
    obj.insert("order_cust",m_order->item(ORDER_CUST,1)->text());
    obj.insert("order_sale",m_order->item(ORDER_SALE,1)->text());
    obj.insert("order_area",m_order->item(ORDER_AREA,1)->text());
    obj.insert("order_dead",m_order->item(ORDER_DEAD,1)->text());
    obj.insert("order_quan",m_order->item(ORDER_QUAN,1)->text());
    obj.insert("order_mark",m_order->item(ORDER_MARK,1)->text());
    obj.insert("order_prod",m_order->item(ORDER_PROD,1)->text());
    obj.insert("order_stck",m_order->item(ORDER_STCK,1)->text());
    obj.insert("order_lnum",m_order->item(ORDER_LNUM,1)->text());
    obj.insert("order_dnum",m_order->item(ORDER_DNUM,1)->text());
    emit sendJson(obj);
}

void OrderPage::appendSends()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_sends");
    obj.insert("logs_sign",1);
    obj.insert("send_numb",m_sends->item(SEND_NUMB,1)->text());
    obj.insert("send_view",m_sends->item(SEND_VIEW,1)->text());
    obj.insert("send_mode",m_sends->item(SEND_MODE,1)->text());
    obj.insert("send_code",m_sends->item(SEND_CODE,1)->text());
    obj.insert("send_prce",m_sends->item(SEND_PRCE,1)->text());
    obj.insert("send_mark",m_sends->item(SEND_MARK,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_sends->rowCount(); i++)
        m_sends->item(i,1)->setText("");
}

void OrderPage::deleteSends()
{
    if (m_sends->item(SEND_ID,1)->text().isEmpty())
        return;
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_sends");
    obj.insert("logs_sign",2);
    obj.insert("tabs_guid",m_sends->item(SEND_ID,1)->text().toDouble());
    obj.insert("send_numb",m_sends->item(SEND_NUMB,1)->text());
    obj.insert("send_view",m_sends->item(SEND_VIEW,1)->text());
    obj.insert("send_mode",m_sends->item(SEND_MODE,1)->text());
    obj.insert("send_code",m_sends->item(SEND_CODE,1)->text());
    obj.insert("send_prce",m_sends->item(SEND_PRCE,1)->text());
    obj.insert("send_mark",m_sends->item(SEND_MARK,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_sends->rowCount(); i++)
        m_sends->item(i,1)->setText("");
}

void OrderPage::changeSends()
{
    if (m_sends->item(SEND_ID,1)->text().isEmpty())
        return;
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_sends");
    obj.insert("logs_sign",3);
    obj.insert("tabs_guid",m_sends->item(SEND_ID,1)->text().toDouble());
    obj.insert("send_numb",m_sends->item(SEND_NUMB,1)->text());
    obj.insert("send_view",m_sends->item(SEND_VIEW,1)->text());
    obj.insert("send_mode",m_sends->item(SEND_MODE,1)->text());
    obj.insert("send_code",m_sends->item(SEND_CODE,1)->text());
    obj.insert("send_prce",m_sends->item(SEND_PRCE,1)->text());
    obj.insert("send_mark",m_sends->item(SEND_MARK,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_sends->rowCount(); i++)
        m_sends->item(i,1)->setText("");
}

void OrderPage::updateOrder()
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

    sql_order->select();
}

void OrderPage::updateSends()
{
    QSqlQuery query(db);
    qint64 logs_guid = 0;
    QJsonObject obj;

    query.prepare("select max(logs_guid) from erp_sends");
    query.exec();
    query.next();
    logs_guid = query.value(0).toDouble();

    obj.insert("logs_cmmd","erp_sends");
    obj.insert("logs_guid",logs_guid);
    obj.insert("logs_sign",0);
    emit sendJson(obj);

    sql_sends->select();
}

void OrderPage::updateCusts()
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

    sql_custs->select();
}

void OrderPage::updateSales()
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

void OrderPage::tabOrderSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_order->rowCount(); i++) {
        m_order->item(i,1)->setText(sql_order->index(row,i).data().toString());
    }
}

void OrderPage::tabSendsSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_sends->rowCount(); i++) {
        m_sends->item(i,1)->setText(sql_sends->index(row,i).data().toString());
    }
}

void OrderPage::recvOrderJson(QJsonObject obj)
{
    QSqlQuery query(db);
    qint64 logs_sign = obj.value("logs_sign").toDouble();
    qint64 logs_guid = obj.value("logs_guid").toDouble();
    qint64 tabs_guid = obj.value("tabs_guid").toDouble();

    switch (logs_sign) {
    case 0://查询
        updateOrder();
        return;
        break;
    case 1://增加
    case 3://修改
        query.prepare("replace into erp_order values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        query.bindValue(ORDER_ID,tabs_guid);
        query.bindValue(ORDER_GUID,logs_guid);
        query.bindValue(ORDER_SIGN,logs_sign);
        query.bindValue(ORDER_NUMB,obj.value("order_numb").toString());
        query.bindValue(ORDER_DATE,obj.value("order_date").toString());
        query.bindValue(ORDER_VIEW,obj.value("order_view").toString());
        query.bindValue(ORDER_CUST,obj.value("order_cust").toString());
        query.bindValue(ORDER_SALE,obj.value("order_sale").toString());
        query.bindValue(ORDER_AREA,obj.value("order_area").toString());
        query.bindValue(ORDER_DEAD,obj.value("order_dead").toString());
        query.bindValue(ORDER_QUAN,obj.value("order_quan").toString());
        query.bindValue(ORDER_MARK,obj.value("order_mark").toString());
        query.bindValue(ORDER_PROD,obj.value("order_prod").toString());
        query.bindValue(ORDER_STCK,obj.value("order_stck").toString());
        query.bindValue(ORDER_LNUM,obj.value("order_lnum").toString());
        query.bindValue(ORDER_DNUM,obj.value("order_dnum").toString());
        query.exec();
        break;
    case 2://删除
        query.prepare("delete from erp_order where id=:id");
        query.bindValue(":id",tabs_guid);
        query.exec();
        break;
    default:
        break;
    }
    sql_order->select();
}

void OrderPage::recvSendsJson(QJsonObject obj)
{
    QSqlQuery query(db);
    qint64 logs_sign = obj.value("logs_sign").toDouble();
    qint64 logs_guid = obj.value("logs_guid").toDouble();
    qint64 tabs_guid = obj.value("tabs_guid").toDouble();

    switch (logs_sign) {
    case 0://查询
        updateSends();
        return;
        break;
    case 1://增加
    case 3://修改
        query.prepare("replace into erp_sends values(?,?,?,?,?,?,?,?,?)");
        query.bindValue(ORDER_ID,tabs_guid);
        query.bindValue(ORDER_GUID,logs_guid);
        query.bindValue(ORDER_SIGN,logs_sign);
        query.bindValue(SEND_NUMB,obj.value("send_numb").toString());
        query.bindValue(SEND_VIEW,obj.value("send_view").toString());
        query.bindValue(SEND_MODE,obj.value("send_mode").toString());
        query.bindValue(SEND_CODE,obj.value("send_code").toString());
        query.bindValue(SEND_PRCE,obj.value("send_prce").toString());
        query.bindValue(SEND_MARK,obj.value("send_mark").toString());
        query.exec();
        break;
    case 2://删除
        query.prepare("delete from erp_sends where id=:id");
        query.bindValue(":id",tabs_guid);
        query.exec();
        break;
    default:
        break;
    }
    sql_sends->select();
}

void OrderPage::showEvent(QShowEvent *e)
{
    updateOrder();
    updateSends();
    updateCusts();
    updateSales();
    e->accept();
}
