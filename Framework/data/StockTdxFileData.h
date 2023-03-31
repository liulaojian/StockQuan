#pragma once
#include "StockData.h"

class StockTdxFileData:public StockData
{
 Q_OBJECT
public:
    StockTdxFileData(QString stockcode);
    virtual ~StockTdxFileData();

public:
    virtual bool Init(void);


    virtual QSharedPointer<StockDataInfo> FindStockDataInfo(QString strDate,QString strTime="",int mDataType=STOCK_DATA_TYPE_DAY);
    virtual bool IsStockDataInfoExist(QString strDate,QString strTime="",int mDataType=STOCK_DATA_TYPE_DAY);
    virtual bool UpdateStockDataInfo(StockDataInfo* pUpdateDataInfo);
    virtual bool AddStockDataInfo(StockDataInfo* pAddDataInfo);

    virtual QVector<QSharedPointer<StockDataInfo> > GetStockDataInfoList(QString strBeginDate,QString strEndDate,QString strBeginTime="",
                                                         QString strEndTime="",int mDataType=STOCK_DATA_TYPE_DAY);

    virtual QVector<QSharedPointer<StockDataInfo> > &  GetAllStockDataInfoList(int mDataType);

    virtual QSharedPointer<StockDataInfo> GetLastStockDataInfo(int mDataType);

    virtual bool IsDataTypeValid(int mDataType);

    virtual bool ReadAllStockDataFromStore(void);
    virtual bool WriteAllStockDataToStore(void);

public:
    void SetStockDayFilePath(QString path) {strStockDayFilePath=path;}
    void SetStockMin5FilePath(QString path) {strStockMin5FilePath=path;}
private:
    QSharedPointer<StockDataInfo> FindStockDayDataInfo(QString strDate);

    QSharedPointer<StockDataInfo> FindStocMin5DataInfo(QString strDate,QString strTime);

    QVector<QSharedPointer<StockDataInfo> > GetStockDayDataInfoList(QString strBeginDate,QString strEndDate);

    QVector<QSharedPointer<StockDataInfo> > GetStockMin5DataInfoList(QString strBeginDate,QString strEndDate,QString strBeginTime,QString strEndTime);

private:
    int GetStockDayIndex(QString strDate);
    int GetNearestStockDayIndex(QString strDate);

    int GetStockMin5Index(QString strDateTime);
    int GetNearestStockMin5Index(QString strDate);
private:
    bool LoadStockDayFile(void);
    //加载股票数据，利用指数数据进行修补（主要是停牌时修补)
    bool LoadFromDayFileRepairDate(StockTdxFileData *pExpStockData);

    bool LoadStockMin5File(void);

    //加载股票数据，利用指数数据进行修补（主要是停牌时修补)
    bool  LoadFromMin5FileRepairDate(StockTdxFileData *pExpStockData);
private:
    QVector<QSharedPointer<StockDataInfo> > vecStockDayData;
    QMutex mDayMutex;

    QVector<QSharedPointer<StockDataInfo> > vecStockMin5Data;
    QMutex mMin5Mutex;

    QString strStockDayFilePath="";
    QString strStockMin5FilePath="";
private:
    bool bInit=false;



};
