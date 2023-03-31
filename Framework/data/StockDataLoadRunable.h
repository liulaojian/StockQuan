#pragma once

#include <QObject>
#include <QRunnable>
#include <QDebug>
#include <QThread>

class StockData;
class StockDataLoadRunable:public QObject,public QRunnable
{
    Q_OBJECT
    public:
        explicit StockDataLoadRunable(StockData *stockdata);
        ~StockDataLoadRunable();
    protected:
        void run();

    private:
        StockData *pStockData=nullptr;
};
