#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication arg(argc, argv);
    MainWindow   torrentMainWindow;

    torrentMainWindow.show();

    return arg.exec();
}
