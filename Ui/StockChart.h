#ifndef STOCKCHART_H
#define STOCKCHART_H

#include    <QWidget>

class StockChart:public QWidget
{
Q_OBJECT
public:
    explicit StockChart(QWidget *parent = 0);
    virtual ~StockChart();



    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // STOCKCHART_H
