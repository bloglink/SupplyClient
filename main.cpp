#include "mainscreen.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScreen w;
    int ret = w.login();
    if (ret == QDialog::Rejected) {
        return 0;
    }
    if (ret == QDialog::Accepted) {
        w.showMaximized();
    }

    return a.exec();
}
