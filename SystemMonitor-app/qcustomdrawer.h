#ifndef _QCUSTOM_DRAWER_H_
#define _QCUSTOM_DRAWER_H_

#include "abstractdrawer.h"
#include "ui_mainwindow.h"

class QCustomDrawer : public AbstractDrawer
{
    public:
        QCustomDrawer(QCustomPlot* customPlot);
        ~QCustomDrawer();

        void connectSignalSlot();

        void createCustomPlot(QString                   titleText,
                              QString                   yLabelText,
                              QString                   xLabelText,
                              std::pair<double, double> yRange,
                              int                       dataSize);

        void setPlotName(QString name, int idx);
        void setPlotColor(int idx);
        void setPlotData(double time, double data, int idx);
        void replotCustomPlot(double time);

    private:
        QCustomPlot*    customPlot_;
        QCPTextElement* title_;
        QCPLayoutGrid*  subLayout_;
};

#endif /* _QCUSTOM_DRAWER_H_ */

