#include "FunTimeSharingDirectUp.h"

#include "StockDataMgr.h"

FunTimeSharingDirectUp::FunTimeSharingDirectUp()
{

}


FunTimeSharingDirectUp::~FunTimeSharingDirectUp()
{

}

QVector<QString>  FunTimeSharingDirectUp::doFunTimeSharingDirectUp(QVector<QString> & vecStockList, QString strDateTime)
{
    QVector<QString> vecResut;
    for(int i=0;i<vecStockList.size();i++)
    {
        if(IsTimeSharingDirectUp(vecStockList[i],strDateTime))
            vecResut.push_back(vecStockList[i]);
    }
    return vecResut;
}


bool FunTimeSharingDirectUp::IsTimeSharingDirectUp(QString strStockCode,QString strDateTime)
{
    StockData* pStockData=stockDataMgr()->FindStockData(strStockCode);
    if(!pStockData)
        return false;

    QStringList list = strDateTime.split(" ");

    QString strDate=list[0];
    QString strTime=list[1];

    QVector<QSharedPointer<StockDataInfo>> vecData=pStockData->GetStockDataInfoPreOfIndex(strDate,strTime,STOCK_DATA_TYPE_5MIN,48);

    if(vecData.size()!=48)
        return false;

    QSharedPointer<StockDataInfo> pStockDataInfoPre=nullptr;
    QSharedPointer<StockDataInfo> pStockDataInfoNext=nullptr;

    int index=-1;
    float fIndexMaxValue=0.0;
    for(int i=2;i<48;i++)
    {
        pStockDataInfoPre=vecData[i-1];
        pStockDataInfoNext=vecData[i];
        float fMinValue=pStockDataInfoPre->GetMinPrice()>pStockDataInfoNext->GetMinPrice()?pStockDataInfoNext->GetMinPrice():pStockDataInfoPre->GetMinPrice();
        float fMaxValue=pStockDataInfoPre->GetMaxPrice()>pStockDataInfoPre->GetMaxPrice()?pStockDataInfoPre->GetMaxPrice():pStockDataInfoPre->GetMaxPrice();

        float fPer=(fMaxValue-fMinValue)*100.0/fMinValue;

        if(fPer>3.0)
        {
            index=i;
            fIndexMaxValue=fMaxValue;
            break;
        }

    }

    if(index<0)
        return false;

    if(index>=46)
        return false;
    QSharedPointer<StockDataInfo> pStockDataInfo=vecData[47];
    float fEndValue=pStockDataInfo->GetEndPrice();

    float fPer=(fEndValue-fIndexMaxValue)*100.0/fEndValue;

    if(fPer<-3.0)
        return true;

    return false;

}
