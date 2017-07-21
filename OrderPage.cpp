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

    sql_custs = new StandardSqlModel(this,db);
    sql_custs->setTable("erp_customs");
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
    autoNumber();
    int row = sql_order->rowCount();
    sql_order->insertRow(row);
    for (int i=1; i < m_order->rowCount(); i++)
        sql_order->setData(sql_order->index(row,i),m_order->item(i,1)->text());
    sql_order->submitAll();

    for (int i=ORDER_QUAN+1; i < m_order->rowCount(); i++) {
        m_order->item(i,1)->setText("");
    }

    autoNumber();
    m_order->item(ORDER_DATE,1)->setText(QDate::currentDate().toString("yyyy-MM-dd"));
}

void OrderPage::deleteOrder()
{
    int row = tab_order->currentIndex().row();
    sql_order->removeRow(row);
    sql_order->submitAll();
    sql_order->select();
}

void OrderPage::changeOrder()
{
    int row = tab_order->currentIndex().row();
    for (int i=1; i < m_order->rowCount(); i++)
        sql_order->setData(sql_order->index(row,i),m_order->item(i,1)->text());
    sql_order->submitAll();
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

void OrderPage::recvSocket(QUrl url)
{
    //    QString cmd = url.query();
    //    QString usr = url.userName();
    //    if (usr != "OrderPage")
    //        return;
    //    QByteArray byte = QByteArray::fromBase64(url.fragment().toUtf8());
    //    if (cmd == "orderinfo") {
    //        json_show = QJsonDocument::fromJson(byte).array();
    //        initData();
    //    } else if (cmd == "saleinfo") {
    //        json_sale = QJsonDocument::fromJson(byte).array();
    //        QStringList items;
    //        for (int i=0; i < json_sale.size(); i++) {
    //            QJsonObject obj = json_sale.at(i).toObject();
    //            items.append(obj.value("erp_solename").toString());
    //        }
    //        sale_delegate->setItemHeaders(items);
    //    } else if (cmd == "customerinfo") {
    //        json_customer = QJsonDocument::fromJson(byte).array();
    //        QStringList items;
    //        for (int i=0; i < json_customer.size(); i++) {
    //            QJsonObject obj = json_customer.at(i).toObject();
    //            items.append(obj.value("erp_solename").toString());
    //        }
    //        cust_delegate->setItemHeaders(items);
    //    } else if (cmd == "pmstayinfo") {
    //        qDebug() << QJsonDocument::fromJson(byte).array();
    //    } else {
    //        qDebug() << "recv others" << url.toString();
    //    }
}

void OrderPage::showEvent(QShowEvent *e)
{
    initData();
    e->accept();
}
