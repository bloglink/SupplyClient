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
    connect(tab_order,SIGNAL(clicked(QModelIndex)),this,SLOT(tabSync(QModelIndex)));

    QPushButton *order_update = new QPushButton(this);
    order_update->setFlat(true);
    order_update->setMinimumSize(97,44);
    order_update->setText(tr("刷新显示"));
    order_update->setFocusPolicy(Qt::NoFocus);
    connect(order_update,SIGNAL(clicked(bool)),this,SLOT(updateOrder()));

    QGridLayout *sorderLayout = new QGridLayout;
    sorderLayout->addWidget(tab_order,0,0,1,2);
    sorderLayout->addWidget(order_update,1,1);
    sorderLayout->setColumnStretch(0,1);

    btn_order = new QToolButton(this);
    btn_order->setIcon(QIcon(":/icons/left.png"));
    btn_order->setIconSize(QSize(30,30));
    btn_order->setFocusPolicy(Qt::NoFocus);
    btn_order->setText(tr("订\n单\n管\n理"));
    btn_order->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_order,SIGNAL(clicked(bool)),this,SLOT(showTabOrder()));

    QVBoxLayout *obtnsLayout = new QVBoxLayout;
    obtnsLayout->addWidget(btn_order);
    obtnsLayout->addStretch();

    order_items << tr("编号") << tr("订单单号") << tr("录入日期") << tr("评审单号")
                << tr("客户名称") << tr("销售经理") << tr("所属区域") << tr("发货日期")
                << tr("订货数量") << tr("在产数量") << tr("入库数量") << tr("发货数量");
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
    tab_iorder->hideRow(ORDER_PROD);
    tab_iorder->hideRow(ORDER_STCK);
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

    QHBoxLayout *orderlayout = new QHBoxLayout;
    orderlayout->addLayout(sorderLayout);
    orderlayout->addLayout(obtnsLayout);
    orderlayout->addWidget(orderWidget);
    orderlayout->setStretch(0,1);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(orderlayout);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(group);
    this->setLayout(mainLayout);
}

void OrderPage::initSql()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "erp_orders");
    db.setDatabaseName("erp.db");
    db.open();
    sql_order = new StandardSqlModel(this,db);
    sql_order->setTable("erp_orders");
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

void OrderPage::initData()
{
    sql_custs->select();
    sql_order->select();
    sql_sales->select();
}

void OrderPage::appendOrder()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_orders");
    obj.insert("logs_sign",1);
    obj.insert("order_numb",m_order->item(ORDER_NUMB,1)->text());
    obj.insert("order_date",m_order->item(ORDER_DATE,1)->text());
    obj.insert("order_view",m_order->item(ORDER_VIEW,1)->text());
    obj.insert("order_cust",m_order->item(ORDER_CUST,1)->text());
    obj.insert("order_sale",m_order->item(ORDER_SALE,1)->text());
    obj.insert("order_area",m_order->item(ORDER_AREA,1)->text());
    obj.insert("order_dead",m_order->item(ORDER_DEAD,1)->text());
    obj.insert("order_quan",m_order->item(ORDER_QUAN,1)->text());
    obj.insert("order_prod",m_order->item(ORDER_PROD,1)->text());
    obj.insert("order_stck",m_order->item(ORDER_STCK,1)->text());
    obj.insert("order_dnum",m_order->item(ORDER_DNUM,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_order->rowCount(); i++)
        m_order->item(i,1)->setText("");
}

void OrderPage::deleteOrder()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_orders");
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
    obj.insert("order_prod",m_order->item(ORDER_PROD,1)->text());
    obj.insert("order_stck",m_order->item(ORDER_STCK,1)->text());
    obj.insert("order_dnum",m_order->item(ORDER_DNUM,1)->text());
    emit sendJson(obj);
    for (int i=0; i < m_order->rowCount(); i++)
        m_order->item(i,1)->setText("");
}

void OrderPage::changeOrder()
{
    QJsonObject obj;
    obj.insert("logs_cmmd","erp_orders");
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
    obj.insert("order_prod",m_order->item(ORDER_PROD,1)->text());
    obj.insert("order_stck",m_order->item(ORDER_STCK,1)->text());
    obj.insert("order_dnum",m_order->item(ORDER_DNUM,1)->text());
    emit sendJson(obj);
}

void OrderPage::updateOrder()
{
    sql_order->select();
}

void OrderPage::tabSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_order->rowCount(); i++) {
        m_order->item(i,1)->setText(sql_order->index(row,i).data().toString());
    }
}

void OrderPage::recvOrderJson(QJsonObject obj)
{
    QSqlQuery query(db);
    qint64 logs_sign = obj.value("logs_sign").toDouble();
    qint64 logs_guid = obj.value("logs_guid").toDouble();
    qint64 tabs_guid = obj.value("tabs_guid").toDouble();

    query.prepare("select count(*) from erp_orders_log where id=:id");
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
            query.prepare("select * from erp_orders_log");
        } else {
            query.prepare("select * from erp_orders_log where id>:id");
            query.bindValue(":id",logs_guid);
        }
        query.exec();
        while (query.next()) {
            QJsonObject send_obj;
            send_obj.insert("sendto",obj.value("sender").toString());
            send_obj.insert("logs_cmmd","erp_orders");
            send_obj.insert("logs_guid",query.value(0).toDouble());
            send_obj.insert("logs_sign",query.value(1).toDouble());
            send_obj.insert("tabs_guid",query.value(2).toDouble());
            send_obj.insert("order_numb",query.value(3).toString());
            send_obj.insert("order_date",query.value(4).toString());
            send_obj.insert("order_view",query.value(5).toString());
            send_obj.insert("order_cust",query.value(6).toString());
            send_obj.insert("order_sale",query.value(7).toString());
            send_obj.insert("order_area",query.value(8).toString());
            send_obj.insert("order_dead",query.value(9).toString());
            send_obj.insert("order_quan",query.value(10).toString());
            send_obj.insert("order_prod",query.value(11).toString());
            send_obj.insert("order_stck",query.value(12).toString());
            send_obj.insert("order_dnum",query.value(13).toString());
            emit sendJson(send_obj);
        }
        return;
        break;
    case 1://增加
        tabs_guid = logs_guid;
        query.prepare("insert into erp_orders values(?,?,?,?,?,?,?,?,?,?,?,?)");
        query.bindValue(0,tabs_guid);
        query.bindValue(1,obj.value("order_numb").toString());
        query.bindValue(2,obj.value("order_date").toString());
        query.bindValue(3,obj.value("order_view").toString());
        query.bindValue(4,obj.value("order_cust").toString());
        query.bindValue(5,obj.value("order_sale").toString());
        query.bindValue(6,obj.value("order_area").toString());
        query.bindValue(7,obj.value("order_dead").toString());
        query.bindValue(8,obj.value("order_quan").toString());
        query.bindValue(9,obj.value("order_prod").toString());
        query.bindValue(10,obj.value("order_stck").toString());
        query.bindValue(11,obj.value("order_dnum").toString());
        query.exec();
        break;
    case 2://删除
        query.prepare("delete from erp_orders where id=:id");
        query.bindValue(":id",tabs_guid);
        query.exec();
        break;
    case 3://修改
        cmd += "update erp_orders set ";
        cmd += "order_numb=:order_numb,";
        cmd += "order_date=:order_date,";
        cmd += "order_view=:order_view,";
        cmd += "order_cust=:order_cust,";
        cmd += "order_sale=:order_sale,";
        cmd += "order_area=:order_area,";
        cmd += "order_dead=:order_dead,";
        cmd += "order_quan=:order_quan,";
        cmd += "order_prod=:order_prod,";
        cmd += "order_stck=:order_stck,";
        cmd += "order_dnum=:order_dnum ";
        cmd += "where id=:tabs_guid";
        query.prepare(cmd);
        query.bindValue(":order_numb",obj.value("order_numb").toString());
        query.bindValue(":order_date",obj.value("order_date").toString());
        query.bindValue(":order_view",obj.value("order_view").toString());
        query.bindValue(":order_cust",obj.value("order_cust").toString());
        query.bindValue(":order_sale",obj.value("order_sale").toString());
        query.bindValue(":order_area",obj.value("order_area").toString());
        query.bindValue(":order_dead",obj.value("order_dead").toString());
        query.bindValue(":order_quan",obj.value("order_quan").toString());
        query.bindValue(":order_prod",obj.value("order_prod").toString());
        query.bindValue(":order_stck",obj.value("order_stck").toString());
        query.bindValue(":order_dnum",obj.value("order_dnum").toString());
        query.bindValue(":tabs_guid",tabs_guid);
        query.exec();
        break;
    default:
        break;
    }
    query.prepare("insert into erp_orders_log values(?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.bindValue(0,logs_guid);
    query.bindValue(1,logs_sign);
    query.bindValue(2,tabs_guid);
    query.bindValue(3,obj.value("order_numb").toString());
    query.bindValue(4,obj.value("order_date").toString());
    query.bindValue(5,obj.value("order_view").toString());
    query.bindValue(6,obj.value("order_cust").toString());
    query.bindValue(7,obj.value("order_sale").toString());
    query.bindValue(8,obj.value("order_area").toString());
    query.bindValue(9,obj.value("order_dead").toString());
    query.bindValue(10,obj.value("order_quan").toString());
    query.bindValue(11,obj.value("order_prod").toString());
    query.bindValue(12,obj.value("order_stck").toString());
    query.bindValue(13,obj.value("order_dnum").toString());
    query.exec();
    sql_order->select();
}

void OrderPage::showEvent(QShowEvent *e)
{
    initData();
    e->accept();
}
