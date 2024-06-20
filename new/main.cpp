#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::cout.precision(12);
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    w.show();
    return a.exec();
}
