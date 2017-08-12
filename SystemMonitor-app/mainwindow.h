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
        Ui::MainWindow*        ui_;
        QTimer*                timer_;

        AbstractSystemMonitor* cpuMonitor_;
        AbstractSystemMonitor* ramMonitor_;

        void createTimer();
        void startTimer();
        void connectTimerSlot();

    public slots:
        void realtimeDataSlot();
};

#endif // MAINWINDOW_H
