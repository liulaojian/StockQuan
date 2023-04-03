#pragma once


#include "FunGlobal.h"
#include  "StockData.h"

//分时图直拉过滤功能
class DlgFunWait;
class FunTimeSharingDirectUp:public FunGlobal
{
 Q_OBJECT
public:
    FunTimeSharingDirectUp();
    virtual ~FunTimeSharingDirectUp();

public:
    virtual void WaitForFun(void) override;
    QVector<QString> doFunTimeSharingDirectUp(QVector<QString> & vecStockList, QString strDateTime);

    QVector<QString> GetResult(void) {return vecResult;}

    void SetUpValue(float value) {fUpValue=value;}
    void SetDownValue(float value) {fDownValue=value;}

    QString GetInputDateTime(void) {return strInputDateTime;}
protected:
    void run() Q_DECL_OVERRIDE;

private:
    QVector<QString>  runFunTimeSharingDirectUp(QVector<QString> & vecStockList, QString strDateTime);
    bool IsTimeSharingDirectUp(QString strStockCode,QString strDateTime);
private:

    int  mHandleIndex=0;

    float fUpValue=3.0;
    float fDownValue=3.0;

    QVector<QString> vecInputStockList;
    QString strInputDateTime;
    QVector<QString>  vecResult;
};
