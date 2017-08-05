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
        Ui::MainWindow*        ui;
        QTimer*                timer_;

        void createTimer();
        void startTimer();
        void connectTimerSlot();

    public slots:
        void realtimeDataSlot();
};

#endif // MAINWINDOW_H
