#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize some static strings
    QStringList headers;
    headers << tr("Torrent")
            << tr("Peers/Seeds")
            << tr("Progress")
            << tr("Down rate")
            << tr("Up rate")
            << tr("Status");
}

MainWindow::~MainWindow()
{
    delete ui;
}
