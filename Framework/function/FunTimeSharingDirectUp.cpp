#include "FunTimeSharingDirectUp.h"

#include "StockDataMgr.h"

#include "dlgfunwait.h"
#include <QDateTime>
#include <QDebug>
FunTimeSharingDirectUp::FunTimeSharingDirectUp()
{

}


FunTimeSharingDirectUp::~FunTimeSharingDirectUp()
{

}

QVector<QString> FunTimeSharingDirectUp::doFunTimeSharingDirectUp(QVector<QString> & vecStockList, QString strDateTime)
{
    if(isRunning())
        return vecResult;
    vecInputStockList=vecStockList;
    strInputDateTime=strDateTime;
    setPriority(QThread::HighestPriority);
    start();
    WaitForFun();
    return vecResult;
}


void FunTimeSharingDirectUp::WaitForFun(void)
{
    DlgFunWait dlg;
    dlg.SetFunGlobal(this);
    connect(this, SIGNAL(fun_process(int)), &dlg, SLOT(solt_fun_process(int)));
    dlg.exec();
}

void FunTimeSharingDirectUp::run()
{
    vecResult=runFunTimeSharingDirectUp(vecInputStockList,strInputDateTime);

}

QVector<QString>  FunTimeSharingDirectUp::runFunTimeSharingDirectUp(QVector<QString> & vecStockList, QString strDateTime)
{
    QVector<QString> vecResut;
    for(int i=0;i<vecStockList.size();i++)
    {
        if(IsTimeSharingDirectUp(vecStockList[i],strDateTime))
        {
            QString strStockName=stockDataMgr()->GetStockName(vecStockList[i]);
            if(strStockName.indexOf("ST")<0 && strStockName!="")
                vecResut.push_back(vecStockList[i]);
        }
        if(bStop)
            break;
    }
    emit fun_process(100);
    return vecResut;
}


bool FunTimeSharingDirectUp::IsTimeSharingDirectUp(QString strStockCode,QString strDateTime)
{
    mHandleIndex++;

    int mMaxNums=stockDataMgr()->GetStockCodeListNums();

    int percent=mHandleIndex*100/mMaxNums;

    StockData* pStockData=stockDataMgr()->FindStockData(strStockCode);
    if(!pStockData)
    {
        emit fun_process(percent);
        return false;
    }

    QStringList list = strDateTime.split(" ");

    QString strDate=list[0];
    QString strTime=list[1];

    QVector<QSharedPointer<StockDataInfo>> vecData_2day=pStockData->GetStockDataInfoPreOfIndex(strDate,strTime,STOCK_DATA_TYPE_5MIN,96);

    if(vecData_2day.size()!=96)
    {
        emit fun_process(percent);
        return false;
    }

    QVector<QSharedPointer<StockDataInfo>> vecData;
    QVector<QSharedPointer<StockDataInfo>> vecData_Pre;

    for(int i=0;i<48;i++)
        vecData_Pre.push_back(vecData_2day[i]);

    for(int i=48;i<96;i++)
        vecData.push_back(vecData_2day[i]);

    float f_pre_end_value=vecData_Pre[47]->GetEndPrice();
    float f_now_end_value=vecData[47]->GetEndPrice();
    float f_pre_now_per=(f_now_end_value-f_pre_end_value)*100.0/f_pre_end_value;
    if(f_pre_now_per>1.0)
    {
        emit fun_process(percent);
        return false;
    }


    QSharedPointer<StockDataInfo> pStockDataInfoPre=nullptr;
    QSharedPointer<StockDataInfo> pStockDataInfoNext=nullptr;

    int index=-1;
    float fIndexMaxValue=0.0;
    for(int i=2;i<48;i++)
    {
        pStockDataInfoPre=vecData[i-1];
        pStockDataInfoNext=vecData[i];
        float fMinValue;
        float fMaxValue;
        if(pStockDataInfoPre->GetMaxPrice()>pStockDataInfoPre->GetMaxPrice())
        {
            fMinValue=pStockDataInfoPre->GetMinPrice();
            fMaxValue=pStockDataInfoPre->GetMaxPrice();

        }
        else
        {
            fMinValue=pStockDataInfoPre->GetMinPrice();
            fMaxValue=pStockDataInfoNext->GetMaxPrice();

        }

        float fPer=(fMaxValue-fMinValue)*100.0/f_pre_end_value;
        if(fPer>fUpValue)  //3.0
        {
            index=i;
            fIndexMaxValue=fMaxValue;
            break;
        }


    }

    if(index<0 || index>=46)
    {
        emit fun_process(percent);
        return false;
    }


    QSharedPointer<StockDataInfo> pStockDataInfo=vecData[47];
    float fEndValue=pStockDataInfo->GetEndPrice();

    float fPer=(fEndValue-fIndexMaxValue)*100.0/f_pre_end_value;

    if(fDownValue>0.0)
        fDownValue=-fDownValue;
    if(fPer<fDownValue) //-3.0
    {
        emit fun_process(percent);
        return true;
    }

    emit fun_process(percent);
    return false;

}
