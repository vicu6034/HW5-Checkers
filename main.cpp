#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // standard Qt main function
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
