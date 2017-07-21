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
    connect(prod_update,SIGNAL(clicked(bool)),this,SLOT(updateProd()));


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
    prod_items << tr("编号") << tr("订单单号") << tr("录入日期") << tr("评审单号")
               << tr("客户名称") << tr("销售经理") << tr("所属区域") << tr("发货日期")
               << tr("订货数量") << tr("生产数量") << tr("生产单号") << tr("产品大类")
               << tr("产品编号") << tr("产品名称") << tr("产品规格") << tr("仪表编号") << tr("入库标志");
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
    sql_plan->setTable("erp_orders");
    QStringList order_items;
    order_items << tr("编号") << tr("订单单号") << tr("录入日期") << tr("评审单号")
                << tr("客户名称") << tr("销售经理") << tr("所属区域") << tr("发货日期")
                << tr("订货数量") << tr("在产数量") << tr("入库数量") << tr("发货数量");;
    for (int i=0; i < order_items.size(); i++)
        sql_plan->setHeaderData(i, Qt::Horizontal, order_items.at(i));
    tab_plan->setModel(sql_plan);
    tab_plan->setColumnWidth(ORDER_ID,50);
    tab_plan->horizontalHeader()->setSectionResizeMode(ORDER_NUMB,QHeaderView::Stretch);
    tab_plan->horizontalHeader()->setSectionResizeMode(ORDER_DATE,QHeaderView::Stretch);
    tab_plan->horizontalHeader()->setSectionResizeMode(ORDER_VIEW,QHeaderView::Stretch);
    tab_plan->horizontalHeader()->setSectionResizeMode(ORDER_CUST,QHeaderView::Stretch);
    tab_plan->horizontalHeader()->setSectionResizeMode(ORDER_SALE,QHeaderView::Stretch);
    tab_plan->horizontalHeader()->setSectionResizeMode(ORDER_AREA,QHeaderView::Stretch);
    tab_plan->horizontalHeader()->setSectionResizeMode(ORDER_QUAN,QHeaderView::Stretch);
    tab_plan->horizontalHeader()->setSectionResizeMode(ORDER_DEAD,QHeaderView::Stretch);
    tab_plan->horizontalHeader()->setSectionResizeMode(ORDER_PROD,QHeaderView::Stretch);
    tab_plan->hideColumn(ORDER_STCK);
    tab_plan->hideColumn(ORDER_DNUM);

    sql_prod = new StandardSqlModel(this,db);
    sql_prod->setTable("erp_prods");
    for (int i=0; i < prod_items.size(); i++)
        sql_prod->setHeaderData(i, Qt::Horizontal, prod_items.at(i));
    tab_prod->setModel(sql_prod);
    tab_prod->setColumnWidth(PROD_ID,50);
    tab_prod->horizontalHeader()->setSectionResizeMode(PROD_NUMB,QHeaderView::Stretch);
    tab_prod->horizontalHeader()->setSectionResizeMode(PROD_DATE,QHeaderView::Stretch);
    tab_prod->horizontalHeader()->setSectionResizeMode(PROD_VIEW,QHeaderView::Stretch);
    tab_prod->horizontalHeader()->setSectionResizeMode(PROD_CUST,QHeaderView::Stretch);
    tab_prod->horizontalHeader()->setSectionResizeMode(PROD_SALE,QHeaderView::Stretch);
    tab_prod->horizontalHeader()->setSectionResizeMode(PROD_AREA,QHeaderView::Stretch);
    tab_prod->horizontalHeader()->setSectionResizeMode(PROD_DEAD,QHeaderView::Stretch);
    tab_prod->horizontalHeader()->setSectionResizeMode(PROD_PNUM,QHeaderView::Stretch);
    tab_prod->horizontalHeader()->setSectionResizeMode(PROD_TYPE,QHeaderView::Stretch);
    tab_prod->horizontalHeader()->setSectionResizeMode(PROD_CODE,QHeaderView::Stretch);
    tab_prod->horizontalHeader()->setSectionResizeMode(PROD_NAME,QHeaderView::Stretch);
    tab_prod->horizontalHeader()->setSectionResizeMode(PROD_MODE,QHeaderView::Stretch);
    tab_prod->hideColumn(PROD_NEED);
    tab_prod->hideColumn(PROD_QUAN);
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

    //    QStringList sales_head;
    //    for (int i=0; i < sql_sales->rowCount(); i++)
    //        sales_head.append(sql_sales->data(sql_sales->index(i,1)).toString());
    //    sale_delegate->setItemHeaders(sales_head);

    //    QStringList areas_head;
    //    for (int i=0; i < sql_sales->rowCount(); i++)
    //        areas_head.append(sql_sales->data(sql_sales->index(i,2)).toString());
    //    area_delegate->setItemHeaders(areas_head);

    //    QStringList customs_head;
    //    for (int i=0; i < sql_customs->rowCount(); i++)
    //        customs_head.append(sql_customs->data(sql_customs->index(i,1)).toString());
    //    cust_delegate->setItemHeaders(customs_head);

    //    autoNumber();
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
    int rowCount = sql_prod->rowCount();
    int numb = m_prod->item(PROD_QUAN,1)->text().toInt();
    for (int k=0; k < numb; k++) {
        sql_prod->insertRow(rowCount+k);
        for (int i=PROD_QUAN; i < m_prod->rowCount(); i++)
            sql_prod->setData(sql_prod->index(rowCount+k,i),m_prod->item(i,1)->text());
        sql_prod->setData(sql_prod->index(rowCount+k,PROD_VIEW),m_prod->item(PROD_VIEW,1)->text());
        sql_prod->submitAll();
    }
    matchProds();
}

void ProdsPage::deleteProd()
{
    QItemSelectionModel *selections = tab_prod->selectionModel(); //返回当前的选择模式
    QModelIndexList selecteds = selections->selectedIndexes(); //返回所有选定的模型项目索引列表
    foreach (QModelIndex index, selecteds) {
        int row = index.row(); //删除所有被选中的行
        sql_prod->removeRow(row);
    }
    sql_prod->submitAll();
    sql_prod->select();
}

void ProdsPage::changeProd()
{
    int row = tab_prod->currentIndex().row();
    for (int i=1; i < m_prod->rowCount(); i++)
        sql_prod->setData(sql_prod->index(row,i),m_prod->item(i,1)->text());
    sql_prod->submitAll();
}

void ProdsPage::updateProd()
{
    sql_plan->select();
    sql_prod->select();
}

void ProdsPage::tabPlanSync(QModelIndex index)
{
    int row = index.row();
    for (int i=1; i <= ORDER_QUAN; i++) {
        m_prod->item(i,1)->setText(sql_plan->index(row,i).data().toString());
    }
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
            for (int k=1; k < PROD_QUAN; k++)
                sql_prod->setData(sql_prod->index(i,k),sql_plan->index(row,k).data().toString());
            sql_prod->submitAll();
            prod++;
            if (quan == (prod+stck+dnum))
                break;
        }
    }
    sql_plan->setData(sql_plan->index(row,ORDER_PROD),QString::number(prod));
    sql_plan->submitAll();
    initData();
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

void ProdsPage::recvSocket(QUrl url)
{
    qDebug() << url;
    //    QString cmd = url.query();
    //    QString usr = url.userName();
    //    if (usr != "ProdPage")
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

void ProdsPage::showEvent(QShowEvent *e)
{
    initData();
    e->accept();
}
