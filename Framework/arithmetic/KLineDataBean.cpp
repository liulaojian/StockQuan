#include "KLineDataBean.h"

KLineDataBean::KLineDataBean()
{

}

KLineDataBean::~KLineDataBean()
{

}

QSharedPointer<KLineBaseData> KLineDataBean::ConvStockDataInfoToKLineBaseData(StockData *pStockData,int mKLineType,QString strDateTime,int mNums)
{
    int mDataType=STOCK_DATA_TYPE_DAY;
    if(mKLineType==K_LINE_DAY)
    {
        if(!pStockData->IsDataTypeValid(STOCK_DATA_TYPE_DAY))
            return nullptr;
        mDataType=STOCK_DATA_TYPE_DAY;
    }
    else
    {
        if(!pStockData->IsDataTypeValid(STOCK_DATA_TYPE_5MIN))
            return nullptr;
        mDataType=STOCK_DATA_TYPE_5MIN;
    }


    int mRealNums=mNums;
    if(mKLineType==K_LINE_15MIN)
        mRealNums=mRealNums*3;
    else if(mKLineType==K_LINE_30MIN)
        mRealNums=mRealNums*6;
    else if(mKLineType==K_LINE_60MIN)
        mRealNums=mRealNums*12;




    return nullptr;
}
