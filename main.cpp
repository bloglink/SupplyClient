#include "mainscreen.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScreen w;
    LoginPage login;
    int ret = login.exec();
    if (ret == QDialog::Rejected) {
        return 0;
    }
    if (ret == QDialog::Accepted) {
        w.showMaximized();
    }

    return a.exec();
}
