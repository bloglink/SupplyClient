#ifndef CWINABOUT_H
#define CWINABOUT_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QMargins>

class AboutPage : public QWidget
{
    Q_OBJECT
public:
    explicit AboutPage(QWidget *parent = 0);

signals:

public slots:
    void initUI();
};

#endif // CWINABOUT_H
