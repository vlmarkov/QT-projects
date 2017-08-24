#include "qcustomdrawer.h"


QCustomDrawer::QCustomDrawer(QCustomPlot* customPlot)
{
    this->customPlot_ = customPlot;
    this->title_      = new QCPTextElement(this->customPlot_);
    this->subLayout_  = new QCPLayoutGrid;
}

QCustomDrawer::~QCustomDrawer()
{
    delete this->title_;
    delete this->subLayout_;
}

void QCustomDrawer::connectSignalSlot()
{
    QObject::connect(this->customPlot_->xAxis, SIGNAL(rangeChanged(QCPRange)),
                     this->customPlot_->xAxis2, SLOT(setRange(QCPRange)));

    QObject::connect(this->customPlot_->yAxis, SIGNAL(rangeChanged(QCPRange)),
                     this->customPlot_->yAxis2, SLOT(setRange(QCPRange)));
}

void QCustomDrawer::createCustomPlot(QString                   titleText,
                                     QString                   yLabelText,
                                     QString                   xLabelText,
                                     std::pair<double, double> yRange,
                                     int                       dataSize)
{
    this->customPlot_->axisRect()->setupFullAxesBox();
    this->customPlot_->yAxis->setRange(yRange.first, yRange.second);
    this->customPlot_->yAxis->setLabel(yLabelText);
    this->customPlot_->xAxis->setLabel(xLabelText);

    this->title_->setText(titleText);

    this->title_->setFont(QFont("sans", 12, QFont::Bold));

    this->customPlot_->plotLayout()->insertRow(0);
    this->customPlot_->legend->setVisible(true);

    this->customPlot_->plotLayout()->addElement(0, 0, this->title_);
    this->customPlot_->plotLayout()->addElement(2, 0, this->subLayout_);

    this->subLayout_->setMargins(QMargins(5, 0, 1, 5));
    this->subLayout_->addElement(0, 0, this->customPlot_->legend);

    this->customPlot_->legend->setWrap(dataSize / 6);
    this->customPlot_->legend->setRowSpacing(1);
    this->customPlot_->legend->setColumnSpacing(3);
    this->customPlot_->legend->setFillOrder(QCPLayoutGrid::FillOrder::foColumnsFirst, true);

    auto rowStretchFactor = 0.2;
    if ((double)(dataSize / 6) > 1.0)
        rowStretchFactor *= (double)(dataSize / 6);

    this->customPlot_->plotLayout()->setRowStretchFactor(2, rowStretchFactor);

    for (auto i = 0; i < dataSize; ++i) {
        this->customPlot_->addGraph();
    }
}

void QCustomDrawer::setPlotName(QString name, int idx)
{
    this->customPlot_->graph(idx)->setName(name);
}

void QCustomDrawer::setPlotColor(int idx)
{
    auto color = QPen(QColor(0, 0, 0));

    // TODO color generator
    switch (idx)
    {
        case 0:  color = QPen(QColor(0, 0, 255));     break;
        case 1:  color = QPen(QColor(0, 255, 0));     break;
        case 2:  color = QPen(QColor(255, 0, 0));     break;
        case 3:  color = QPen(QColor(150, 0, 255));   break;
        case 4:  color = QPen(QColor(0, 150, 255));   break;
        case 5:  color = QPen(QColor(255, 0, 150));   break;
        case 6:  color = QPen(QColor(255, 200, 200)); break;
        case 7:  color = QPen(QColor(10, 250, 200));  break;
        default: break;
    }

    this->customPlot_->graph(idx)->setPen(color);
}

void QCustomDrawer::setPlotData(double time, double data, int idx)
{
    this->customPlot_->graph(idx)->addData(time, data);
}

void QCustomDrawer::replotCustomPlot(double time)
{
    this->customPlot_->xAxis->setRange(time, 100, Qt::AlignRight);
    this->customPlot_->replot();
}
