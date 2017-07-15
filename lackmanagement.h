#ifndef LACKMANAGEMENT_H
#define LACKMANAGEMENT_H

#include <QDate>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QSplitter>
#include <QGroupBox>
#include <QDateEdit>
#include <QTableView>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QToolButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDomDocument>
#include <QElapsedTimer>

#include "delegate.h"

#define PLAN_O_VIEW 0x00
#define PLAN_O_QUAN 0x01
#define PLAN_O_ODDS 0x02
#define PLAN_O_DATE 0x03
#define PLAN_O_SALE 0x04
#define PLAN_O_CSTM 0x05
#define PLAN_O_EDAT 0x06
#define PLAN_O_STTS 0x07

#define LACK_P_CSTM 0x00
#define LACK_P_CODE 0x01
#define LACK_P_PCOD 0x02
#define LACK_P_NAME 0x03
#define LACK_P_TYPE 0x04
#define LACK_P_UNIT 0x05
#define LACK_P_QUAN 0x06
#define LACK_P_OQUA 0x07
#define LACK_P_DATE 0x08
#define LACK_P_IOUT 0x09
#define LACK_P_PDAT 0x0A
#define LACK_P_EDAT 0x0B
#define LACK_P_TQUA 0x0C
#define LACK_P_PQUA 0x0D
#define LACK_P_BQUA 0x0E
#define LACK_P_MARK 0x0F

class LackManagement : public QWidget
{
    Q_OBJECT
public:
    explicit LackManagement(QWidget *parent = 0);

signals:
    void sendSocket(QUrl url);
private slots:
    void initUI();
    void initPlan();
    void addPlan();
    void delOrder();
    void uptOrder();
    void sndOrder();
    void tabPlanSync(QModelIndex index);
    void tabPrepSync(QModelIndex index);
    void recvSocket(QUrl url);
    void showAdd();
    void showSnd();
    void refresh();
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_plan;
    QTableView *tab_plan;
    QJsonArray json_plan;

    StandardItemModel *m_prep;
    QTableView *tab_prep;
    QJsonArray json_prep;

    StandardItemModel *m_add;
    QTableView *tab_add;
    QToolButton *btn_add;
    QWidget *w_add;

    StandardItemModel *m_snd;
    QTableView *tab_snd;
    QToolButton *btn_snd;
    QWidget *w_snd;

    int max_plan_id;
};

#endif // LACKMANAGEMENT_H
