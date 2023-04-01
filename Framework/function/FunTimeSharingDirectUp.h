#pragma once

#include  "StockData.h"
//分时图直拉过滤功能

class FunTimeSharingDirectUp:public QObject
{
 Q_OBJECT
public:
    FunTimeSharingDirectUp();
    virtual ~FunTimeSharingDirectUp();

    QVector<QString>  doFunTimeSharingDirectUp(QVector<QString> & vecStockList, QString strDateTime);

private:
    bool IsTimeSharingDirectUp(QString strStockCode,QString strDateTime);

};
