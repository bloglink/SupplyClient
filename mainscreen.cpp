#include "mainscreen.h"

MainScreen::MainScreen(QWidget *parent)
    : QMainWindow(parent)
{
    initUI();
    initUdp();
    preindex = 0;
}

MainScreen::~MainScreen()
{

}

int MainScreen::login()
{
    LoginScreen w;
    connect(&w,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),&w,SLOT(recvSocket(QUrl)));
    return w.exec();
}

void MainScreen::initUI()
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    //设置界面风格
    QFile file;
    QString qss;
    file.setFileName(":/skins/gn_bu.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    QApplication::setOrganizationName("AIP");
    QApplication::setApplicationName("QFramer");
    QApplication::setWindowIcon(QIcon(":/icons/link.ico"));
    QSystemTrayIcon *trayicon = new QSystemTrayIcon(QIcon(QString(":/icons/link.ico")), this);
    trayicon->setToolTip(QString(qApp->applicationName()));
    trayicon->show();

    QLabel *icon  = new QLabel(this);
    icon->setPixmap(QPixmap(":/icons/link.ico"));
    icon->setScaledContents(true);
    icon->setMaximumSize(25,25);
    QLabel *text = new QLabel(tr("青岛艾普供应链管理系统"));
    QToolButton *ext = new QToolButton(this);
    ext->setIcon(QIcon(":/icons/close.png"));
    ext->setFocusPolicy(Qt::NoFocus);
    ext->setIconSize(QSize(25,25));
    ext->resize(25,25);
    connect(ext,SIGNAL(clicked(bool)),this,SLOT(animationHide()));

    QToolButton *app = new QToolButton(this);
    app->setIcon(QIcon(":/icons/app.png"));
    app->setFocusPolicy(Qt::NoFocus);
    app->setIconSize(QSize(25,25));
    app->resize(25,25);
    connect(app,SIGNAL(clicked(bool)),this,SLOT(swithMaxNormal()));

    QToolButton *min = new QToolButton(this);
    min->setIcon(QIcon(":/icons/minus.png"));
    min->setFocusPolicy(Qt::NoFocus);
    min->setIconSize(QSize(25,25));
    min->resize(25,25);
    connect(min,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));

    QHBoxLayout *title = new QHBoxLayout;
    title->addWidget(icon);
    title->addWidget(text);
    title->addStretch();
    title->addWidget(min);
    title->addWidget(app);
    title->addWidget(ext);
    title->setMargin(0);
    title->setSpacing(5);

    QToolButton *title_order = new QToolButton(this);
    QToolButton *title_product = new QToolButton(this);
    QToolButton *title_purchase = new QToolButton(this);
    QToolButton *title_users = new QToolButton(this);
    QToolButton *title_roles = new QToolButton(this);
    QToolButton *title_action = new QToolButton(this);
    QToolButton *title_about = new QToolButton(this);
    title_about->setObjectName("aboutusscreen");
    title_users->setObjectName("usermanagerment");
    title_roles->setObjectName("rolemanagerment");
    title_order->setObjectName("ordermanagement");
    title_product->setObjectName("productionmanagement");
    title_purchase->setObjectName("purchasemanagement");

    initToolButton(title_order);
    initToolButton(title_product);
    initToolButton(title_purchase);
    initToolButton(title_users);
    initToolButton(title_roles);
    initToolButton(title_action);
    initToolButton(title_about);

    title_order->setIcon(QIcon(":/icons/note.png"));
    title_order->setText(tr("订单管理"));
    title_product->setIcon(QIcon(":/icons/industry.png"));
    title_product->setText(tr("生产管理"));
    title_purchase->setIcon(QIcon(":/icons/dollar.png"));
    title_purchase->setText(tr("采购管理"));
    title_users->setIcon(QIcon(":/icons/user.png"));
    title_users->setText(tr("用户管理"));
    title_roles->setIcon(QIcon(":/icons/man.png"));
    title_roles->setText(tr("角色管理"));
    title_action->setIcon(QIcon(":/icons/stop.png"));
    title_action->setText(tr("权限管理"));
    title_about->setIcon(QIcon(":/icons/link.ico"));
    title_about->setText(tr("关于软件"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(title_order);
    layout->addWidget(title_product);
    layout->addWidget(title_purchase);
    layout->addWidget(title_users);
    layout->addWidget(title_roles);
    layout->addWidget(title_action);
    layout->addWidget(title_about);
    layout->addStretch();
    layout->setMargin(0);

    stack = new QStackedWidget(this);
    stack->addWidget(new AboutUsScreen(this));

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addLayout(title);
    main_layout->addLayout(layout);
    main_layout->addWidget(stack);
    main_layout->setMargin(0);

    QWidget *desktop = new QWidget(this);
    desktop->setLayout(main_layout);
    setCentralWidget(desktop);
    this->resize(1280,900);

    user = new UserManagerment(this);
    connect(user,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),user,SLOT(recvSocket(QUrl)));
    stack->addWidget(user);

    role = new RoleManagerment(this);
    connect(role,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),role,SLOT(recvSocket(QUrl)));
    stack->addWidget(role);

    order = new OrderManagement(this);
    connect(order,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),order,SLOT(recvSocket(QUrl)));
    stack->addWidget(order);

    prod = new ProductionManagement(this);
    connect(prod,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),prod,SLOT(recvSocket(QUrl)));
    stack->addWidget(prod);

    lack = new LackManagement(this);
    connect(lack,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),lack,SLOT(recvSocket(QUrl)));
    stack->addWidget(lack);

    purchase = new PurchaseManagement(this);
    connect(purchase,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),purchase,SLOT(recvSocket(QUrl)));
    stack->addWidget(purchase);
}

void MainScreen::initUdp()
{
    udp.initSocket();
    connect(this,SIGNAL(sendSocket(QUrl)),&udp,SLOT(sendSocket(QUrl)));
    connect(&udp,SIGNAL(recvSocket(QUrl)),this,SLOT(recvSocket(QUrl)));
}
void MainScreen::swithMaxNormal()
{
    if(isMaximized())
        this->showNormal();
    else
        this->showMaximized();
}

void MainScreen::initToolButton(QToolButton *btn)
{
    btn->setFixedSize(90,80);
    btn->setIconSize(QSize(50,60));
    btn->setFocusPolicy(Qt::NoFocus);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(readToolButton()));
}
void MainScreen::readToolButton()
{
    stackChange(QObject::sender()->objectName().toUtf8());
}

void MainScreen::stackChange(QByteArray win)
{
    for (int i=0; i < stack->count(); i++) {
        if (stack->widget(i)->objectName() == win) {
            if (preindex == i)
                break;
            stack->setCurrentIndex(i);
            cloudAntimation();
            preindex = i;
            break;
        }
    }
}

void MainScreen::animationHide()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    connect(animation, SIGNAL(finished()), this, SLOT(animationClose()));
    animation->setDuration(500);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
}

void MainScreen::animationClose()
{
    this->hide();
    QTimer *timer = new QTimer(this);
    timer->singleShot(50,this,SLOT(close()));
}

void MainScreen::recvSocket(QUrl url)
{
    QString cmd = url.query();
    emit sendMsg(url);
    if (cmd == "action" || cmd == "error") {
        url.setUserName("loginscreen");
        emit sendMsg(url);
    } else if (cmd == "userinfo") {
        url.setUserName("usermanagerment");
        emit sendMsg(url);
    } else if (cmd == "roleinfo") {
        url.setUserName("usermanagerment");
        emit sendMsg(url);
        url.setUserName("rolemanagerment");
        emit sendMsg(url);
    } else if (cmd == "orderinfo") {
        url.setUserName("ordermanagement");
        emit sendMsg(url);
    } else if (cmd == "saleinfo" || cmd == "customerinfo") {
        url.setUserName("ordermanagement");
        emit sendMsg(url);
    } else if (cmd == "pdprepinfo" || cmd == "pdplaninfo") {
        url.setUserName("productionmanagement");
        emit sendMsg(url);
    } else if (cmd == "lackinfo") {
        url.setUserName("lackmanagement");
        emit sendMsg(url);
    } else if (cmd == "buyinfo") {
        url.setUserName("purchasemanagement");
        emit sendMsg(url);
    } else {
        qDebug() << "recv others" << url.toString();
    }
}

void MainScreen::cloudAntimation()
{
    QLabel* circle = new QLabel(stack->currentWidget());
    QLabel* line = new QLabel(this);
    line->setObjectName(QString("AntimationLine"));
    line->resize(0, 2);
    line->show();

    circle->setPixmap(stack->widget(preindex)->grab());

    circle->show();
    circle->resize(stack->currentWidget()->size());
    QPropertyAnimation *animation = new QPropertyAnimation(circle, "geometry");

    animation->setDuration(1000);
    animation->setStartValue(circle->geometry());

    QPropertyAnimation* animation_line = new QPropertyAnimation(line, "size");
    animation_line->setDuration(1000);
    animation_line->setEasingCurve(QEasingCurve::OutCubic);

    animation->setEndValue(QRect(-3, 0, 0, circle->height()));
    line->move(stack->x(), stack->y() - 2);
    animation_line->setStartValue(QSize(0, 2));
    animation_line->setEndValue(QSize(stack->width(), 2));

    animation->setEasingCurve(QEasingCurve::OutCubic);

    QPropertyAnimation* animation_opacity = new QPropertyAnimation(circle, "windowOpacity");
    animation_opacity->setDuration(1000);
    animation_opacity->setStartValue(1);
    animation_opacity->setEndValue(0);
    animation_opacity->setEasingCurve(QEasingCurve::OutCubic);

    QParallelAnimationGroup *group = new QParallelAnimationGroup;

    connect(group,SIGNAL(finished()), circle, SLOT(hide()));
    connect(group,SIGNAL(finished()), circle, SLOT(deleteLater()));
    connect(group,SIGNAL(finished()), line, SLOT(deleteLater()));
    connect(group,SIGNAL(finished()), group, SLOT(deleteLater()));
    connect(group,SIGNAL(finished()), animation, SLOT(deleteLater()));
    connect(group,SIGNAL(finished()), animation_opacity, SLOT(deleteLater()));
    connect(group,SIGNAL(finished()), animation_line, SLOT(deleteLater()));
    group->addAnimation(animation);
    group->addAnimation(animation_opacity);
    group->addAnimation(animation_line);
    group->start();
}

void MainScreen::mousePressEvent(QMouseEvent *e)
{
    if(e->button() & Qt::LeftButton) {
        if(e->y() < this->height() and e->x() > this->width() - 120) {
            leftbuttonpressed = false;
        } else {
            dragPosition = e->globalPos() - frameGeometry().topLeft();
            leftbuttonpressed = true;
        }
    }
    e->accept();
}

void MainScreen::mouseReleaseEvent(QMouseEvent *e)
{
    leftbuttonpressed = false;
    e->accept();
}

void MainScreen::mouseMoveEvent(QMouseEvent *e)
{
    if(isMaximized()) {
        e->ignore();
    } else {
        if(e->y() < this->height() and e->x() > this->width() - 120) {
            e->ignore();
        } else {
            if(leftbuttonpressed) {
                move(e->globalPos() - dragPosition);
                e->accept();
            }
        }
    }
}
