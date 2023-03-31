#pragma once

#include "StockData.h"

typedef struct
{
    QString strStockCode;
    QString strStockName;
    QString strStockDayPath;
    QString strStockMin5Path;
}StockFilePathXmlInfo;


class StockDataMgr:public QObject
{
 Q_OBJECT
public:
    StockDataMgr();
    virtual ~StockDataMgr();

public:
   static StockDataMgr* GetInstance();

   bool Init(void);

   StockData* FindStockData(QString strStockCode);


   QVector<StockData*> & GetStockDataList(void) {return vecStockData;}

   bool  IsExpStock(QString strStockCode);

signals:
    void stock_data_process(int index,int max_index);

private slots:
    void solt_data_process(int index);
private:
    bool doParseStockFilePathXmlFile(QString strXmlPath);
private:
    static StockDataMgr* s_pIntance;

    bool bInit=false;
    QVector<StockData*> vecStockData;
    QVector<StockFilePathXmlInfo*> vecStockFilePathXmlInfo;
    int mMaxStockDataNums=0;
};


#define stockDataMgr()	StockDataMgr::GetInstance()
