#pragma once
#include <QString>
#include <QVector>
#include <QSharedPointer>
enum
{
    STOCK_DATA_TYPE_1MIN = 0,
    STOCK_DATA_TYPE_5MIN,
    STOCK_DATA_TYPE_15MIN,
    STOCK_DATA_TYPE_30MIN,
    STOCK_DATA_TYPE_60MIN,
    STOCK_DATA_TYPE_DAY,
};

typedef struct StockDataInfoTag
{
    QString strCode;
    QString strDate;		//日期
    QString strTime;		//时间
    int    mType;           //数据类型
    float  fBeginPrice;		//开盘价格
    float  fMaxPrice;		//最高价格
    float  fMinPrice;		//最低价格
    float  fEndPrice;		//收盘价格
    long   mTotalVolume;		//成交量
    float  fTotalPrice;		//成交金额

    QString GetCode(void) {return strCode;}
    void SetCode(QString strVal) {strCode=strVal;}

    QString GetDate(void) {return strDate;}
    void SetDate(QString strVal) {strDate=strVal;}

    QString GetTime(void) {return strTime;}
    void SetTime(QString strVal) {strTime=strVal;}

    void SetBeginPrice(float fVal) {fBeginPrice=fVal;}
    float GetBeginPrice(void) {return fBeginPrice;}

    void SetMaxPrice(float fVal) {fMaxPrice=fVal;}
    float GetMaxPrice(void) {return fMaxPrice;}

    void SetMinPrice(float fVal) {fMinPrice=fVal;}
    float GetMinPrice(void) {return fMinPrice;}

    void SetEndPrice(float fVal) {fEndPrice=fVal;}
    float GetEndPrice(void) {return fEndPrice;}

    void SetTotalVolume(long mVal) {mTotalVolume=mVal;}
    long GetTotalVolume(void) {return mTotalVolume;}

    void SetTotalPrice(float fVal) {fTotalPrice=fVal;}
    float GetTotalPrice(void) {return fTotalPrice;}

    void SetType(int type) {mType=type;}
    int  GetType(void) {return mType;}

    QString GetDateTime(void) {
        QString strDateTime;
        strDateTime=strDate;
        strDateTime+=" ";
        strDateTime+=strTime;
        return strDateTime;
    }

} StockDataInfo;

typedef QVector<QSharedPointer<StockDataInfo> > VecStockDataInfo;
