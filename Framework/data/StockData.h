#pragma once
#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QMutex>

#include "StockDataStruct.h"



class StockData:public QObject
{
 Q_OBJECT
public:
    StockData(QString stockcode);
    virtual ~StockData();

public:
    virtual bool Init(void)=0;

    virtual QSharedPointer<StockDataInfo> FindStockDataInfo(QString strDate,QString strTime="",int mDataType=STOCK_DATA_TYPE_DAY)=0;
    virtual bool IsStockDataInfoExist(QString strDate,QString strTime="",int mDataType=STOCK_DATA_TYPE_DAY)=0;
    virtual bool UpdateStockDataInfo(StockDataInfo* pUpdateDataInfo)=0;
    virtual bool AddStockDataInfo(StockDataInfo* pAddDataInfo)=0;


    virtual QVector<QSharedPointer<StockDataInfo> > GetStockDataInfoList(QString strBeginDate,QString strEndDate,QString strBeginTime="",
                                                         QString strEndTime="",int mDataType=STOCK_DATA_TYPE_DAY)=0;

    virtual QVector<QSharedPointer<StockDataInfo> > &  GetAllStockDataInfoList(int mDataType)=0;

    virtual QSharedPointer<StockDataInfo> GetLastStockDataInfo(int mDataType)=0;

    //获取指定日期前prenums个数据
    virtual QVector<QSharedPointer<StockDataInfo>> GetStockDataInfoPreOfIndex(QString strDate,QString strTime,int mDataType,int prenums)=0;

    virtual bool IsDataTypeValid(int mDataType)=0;

    virtual int  GetStockDataInfoSize(int mDataType)=0;

    virtual bool ReadAllStockDataFromStore(void)=0;
    virtual bool WriteAllStockDataToStore(void)=0;



public:
    void SetStockName(QString name) {strStockName=name;}
    QString GetStockName(void) {return strStockName;}

    QString GetStockCode(void) {return strStockCode;}

signals:
    void data_process(int index);

protected:

   QString strStockCode;
   QString strStockName;

};




