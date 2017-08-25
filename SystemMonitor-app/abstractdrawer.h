#ifndef _ABSTRACT_DRAWER_H_
#define _ABSTRACT_DRAWER_H_

#include <qcustomplot/qcustomplot.h>

#include <utility>

class AbstractDrawer
{
    public:
        AbstractDrawer() {}
        virtual ~AbstractDrawer() {}

        virtual void connectSignalSlot() = 0;

        virtual void createCustomPlot(QString           &titleText,
                              QString                   &yLabelText,
                              QString                   &xLabelText,
                              std::pair<double, double> &yRange,
                              int                        dataSize,
                              double                     stretchFactor) = 0;

        virtual void setPlotName(QString &name, int idx) = 0;
        virtual void setPlotColor(int idx) = 0;
        virtual void setPlotData(double time, double data, int idx) = 0;
        virtual void replotCustomPlot(double time, double size) = 0;
};

#endif /* _ABSTRACT_DRAWER_H_ */
