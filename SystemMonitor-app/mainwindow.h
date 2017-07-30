#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "abstractsystemmonitor.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    AbstractSystemMonitor* cpuMonitor;
    Ui::MainWindow *ui;
    QTimer* dataTimer;

public slots:
    void realtimeDataSlot();
};

#endif // MAINWINDOW_H
