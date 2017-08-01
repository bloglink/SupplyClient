#include "PurchPage.h"

PurchPage::PurchPage(QWidget *parent) : QWidget(parent)
{
    initUI();
//    initSql();
}

PurchPage::~PurchPage()
{
    db.close();
}

void PurchPage::initUI()
{
    this->setObjectName("PurchPage");

    tab_purch = new QTableView(this);
    tab_purch->setItemDelegate(new ReadOnlyDelegate);
    tab_purch->horizontalHeader()->setHighlightSections(false);
    tab_purch->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(tab_purch,SIGNAL(clicked(QModelIndex)),this,SLOT(tabPurchSync(QModelIndex)));

    QPushButton *purch_update = new QPushButton(this);
    purch_update->setFlat(true);
    purch_update->setMinimumSize(97,44);
    purch_update->setText(tr("刷新显示"));
    purch_update->setFocusPolicy(Qt::NoFocus);
    connect(purch_update,SIGNAL(clicked(bool)),this,SLOT(updatePurch()));

    QGridLayout *spurchLayout = new QGridLayout;
    spurchLayout->addWidget(tab_purch,0,0,1,2);
    spurchLayout->addWidget(purch_update,1,1);
    spurchLayout->setColumnStretch(0,1);
    spurchLayout->setMargin(0);

    btn_purch = new QToolButton(this);//显示订单
    btn_purch->setIcon(QIcon(":/icons/left.png"));
    btn_purch->setIconSize(QSize(30,30));
    btn_purch->setFocusPolicy(Qt::NoFocus);
    btn_purch->setText(tr("采\n购\n管\n理"));
    btn_purch->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn_purch,SIGNAL(clicked(bool)),this,SLOT(showTabProd()));

    QVBoxLayout *pbtnsLayout = new QVBoxLayout;
    pbtnsLayout->addWidget(btn_purch);
    pbtnsLayout->addStretch();
    purch_items << tr("编号") << tr("客户名称") << tr("计划单号") << tr("物料编号") << tr("物料名称")
               << tr("产品规格") << tr("物料单位") << tr("缺料数量") << tr("订货数量")
               << tr("订货时间") << tr("外出采购") << tr("预计日期") << tr("实际日期")
               << tr("到货数量") << tr("在途数量") << tr("返修数量") << tr("备注");
    m_pruch = new StandardItemModel();
    QStringList purch_header;
    purch_header << tr("项目") << tr("参数");
    m_pruch->setHorizontalHeaderLabels(purch_header);
    for (int i=0; i < purch_items.size(); i++) {
        m_pruch->setItem(i,0,new QStandardItem(purch_items.at(i)));
        m_pruch->setItem(i,1,new QStandardItem(""));
    }
    //    sale_delegate = new ComboBoxDelegate;
    //    area_delegate = new ComboBoxDelegate;
    //    cust_delegate = new ComboBoxDelegate;
    tab_ipurch = new QTableView(this);
    tab_ipurch->setModel(m_pruch);
    tab_ipurch->setColumnWidth(0,100);
    tab_ipurch->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    //    tab_iprod->setItemDelegateForColumn(0,new ReadOnlyDelegate);
    //    tab_iorder->setItemDelegateForRow(ORDER_CUST,cust_delegate);
    //    tab_iorder->setItemDelegateForRow(ORDER_AREA,area_delegate);
    //    tab_iorder->setItemDelegateForRow(ORDER_SALE,sale_delegate);
    //    tab_iorder->setItemDelegateForRow(ORDER_DEAD,new DateEditDelegate);

    QPushButton *purch_append = new QPushButton(this);
    purch_append->setFlat(true);
    purch_append->setMinimumSize(97,44);
    purch_append->setText(tr("添加计划"));
    purch_append->setFocusPolicy(Qt::NoFocus);
    connect(purch_append,SIGNAL(clicked(bool)),this,SLOT(appendPurch()));
    QPushButton *purch_delete = new QPushButton(this);
    purch_delete->setFlat(true);
    purch_delete->setMinimumSize(97,44);
    purch_delete->setText(tr("删除计划"));
    purch_delete->setFocusPolicy(Qt::NoFocus);
    connect(purch_delete,SIGNAL(clicked(bool)),this,SLOT(deletePurch()));
    QPushButton *purch_change = new QPushButton(this);
    purch_change->setFlat(true);
    purch_change->setMinimumSize(97,44);
    purch_change->setText(tr("修改计划"));
    purch_change->setFocusPolicy(Qt::NoFocus);
    connect(purch_change,SIGNAL(clicked(bool)),this,SLOT(changePurch()));

    QGridLayout *ipruchLayout = new QGridLayout;
    ipruchLayout->addWidget(tab_ipurch,0,0,1,4);
    ipruchLayout->addWidget(purch_append,1,1);
    ipruchLayout->addWidget(purch_delete,1,2);
    ipruchLayout->addWidget(purch_change,1,3);
    ipruchLayout->setColumnStretch(0,1);
    ipruchLayout->setMargin(0);

    pruchWidget = new QWidget(this);
    pruchWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    pruchWidget->setLayout(ipruchLayout);
    pruchWidget->hide();

    QHBoxLayout *pruchLayout = new QHBoxLayout;
    pruchLayout->addLayout(spurchLayout);
    pruchLayout->addLayout(pbtnsLayout);
    pruchLayout->addWidget(pruchWidget);
    pruchLayout->setStretch(0,1);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(pruchLayout);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(group);
    this->setLayout(mainLayout);
}

void PurchPage::initSql()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "erp_purchs");
    db.setDatabaseName("erp.db");
    db.open();

    sql_purch = new StandardSqlModel(this,db);
    sql_purch->setTable("erp_purchs");
    sql_purch->select();

    for (int i=0; i < purch_items.size(); i++)
        sql_purch->setHeaderData(i, Qt::Horizontal, purch_items.at(i));
    tab_purch->setModel(sql_purch);
    tab_purch->setColumnWidth(PRODS_UUID,50);
    tab_purch->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void PurchPage::showTabProd()
{
    if (pruchWidget->isHidden()) {
        pruchWidget->show();
        btn_purch->setIcon(QIcon(":/icons/right.png"));
    } else {
        pruchWidget->hide();
        btn_purch->setIcon(QIcon(":/icons/left.png"));
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

void PurchPage::initData()
{

}

void PurchPage::appendPurch()
{
    int row = sql_purch->rowCount();
    sql_purch->insertRow(row);
    for (int i=1; i < m_pruch->rowCount(); i++)
        sql_purch->setData(sql_purch->index(row,i),m_pruch->item(i,1)->text());
    sql_purch->submitAll();
}

void PurchPage::deletePurch()
{
    int row = tab_purch->currentIndex().row();
    sql_purch->removeRow(row);
    sql_purch->submitAll();
    sql_purch->select();
}

void PurchPage::changePurch()
{
    int row = tab_purch->currentIndex().row();
    for (int i=1; i < m_pruch->rowCount(); i++)
        sql_purch->setData(sql_purch->index(row,i),m_pruch->item(i,1)->text());
    sql_purch->submitAll();
}

void PurchPage::updatePurch()
{
    sql_purch->select();
}

void PurchPage::tabPurchSync(QModelIndex index)
{
    int row = index.row();
    for (int i=0; i < m_pruch->rowCount(); i++) {
        m_pruch->item(i,1)->setText(sql_purch->index(row,i).data().toString());
    }
}

void PurchPage::recvSocket(QUrl url)
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

void PurchPage::showEvent(QShowEvent *e)
{
    e->accept();
}
