#include "StockTdxFileData.h"
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>
#include <thread>
#include <QDebug>
#include <QMutexLocker>
#include <QDateTime>
#include <QtGlobal>
#include "StockDataMgr.h"

StockTdxFileData::StockTdxFileData(QString stockcode):StockData(stockcode)
{

}


StockTdxFileData::~StockTdxFileData()
{

}


bool StockTdxFileData::Init(void)
{


    return true;
}


QSharedPointer<StockDataInfo> StockTdxFileData::FindStockDataInfo(QString strDate,QString strTime,int mDataType)
{
    if(mDataType==STOCK_DATA_TYPE_DAY)
        return FindStockDayDataInfo(strDate);
    else if(mDataType==STOCK_DATA_TYPE_5MIN)
        return FindStocMin5DataInfo(strDate,strTime);
    else
        return nullptr;
}

QSharedPointer<StockDataInfo> StockTdxFileData::FindStockDayDataInfo(QString strDate)
{
    QMutexLocker locker(&mDayMutex);
    auto iterator = std::find_if(vecStockDayData.begin(), vecStockDayData.end(), [&](auto pStockDayData) {
                if (pStockDayData->strDate == strDate && pStockDayData->strCode==strStockCode)
                    return true;
                else
                    return  false;
                });

   if (iterator != vecStockDayData.end())
   {
      return *iterator;
   }
    return nullptr;
}

QSharedPointer<StockDataInfo> StockTdxFileData::FindStocMin5DataInfo(QString strDate,QString strTime)
{
    QMutexLocker locker(&mMin5Mutex);
    auto iterator = std::find_if(vecStockMin5Data.begin(), vecStockMin5Data.end(), [&](auto pStockMin5Data) {
                if (pStockMin5Data->strDate == strDate && pStockMin5Data->strTime==strTime && pStockMin5Data->strCode==strStockCode)
                    return true;
                else
                    return  false;
                });

   if (iterator != vecStockMin5Data.end())
   {
      return *iterator;
   }

    return nullptr;
}

bool StockTdxFileData::IsStockDataInfoExist(QString strDate,QString strTime,int mDataType)
{
    if(mDataType==STOCK_DATA_TYPE_DAY)
    {
      if(FindStockDayDataInfo(strDate))
          return true;
      else
          return false;
    }
    else if(mDataType==STOCK_DATA_TYPE_5MIN)
    {
        if(FindStocMin5DataInfo(strDate,strTime))
            return true;
        else
            return false;
    }
    else
        return false;
}

bool StockTdxFileData::UpdateStockDataInfo(StockDataInfo* pUpdateDataInfo)
{
    if(pUpdateDataInfo->GetCode()!=strStockCode)
        return false;

    QString strDate=pUpdateDataInfo->GetDate();
    QString strTime=pUpdateDataInfo->GetTime();
    int mType=pUpdateDataInfo->GetType();
    QSharedPointer<StockDataInfo> pStockDataInfo=nullptr;
    if(mType==STOCK_DATA_TYPE_DAY)
    {
        pStockDataInfo=FindStockDayDataInfo(strDate);
    }
    else if(mType==STOCK_DATA_TYPE_5MIN)
    {
        pStockDataInfo=FindStocMin5DataInfo(strDate,strTime);
    }
    else
        return false;
    if(!pStockDataInfo)
        return false;

    pStockDataInfo->fBeginPrice=pUpdateDataInfo->fBeginPrice;
    pStockDataInfo->fMaxPrice=pUpdateDataInfo->fMaxPrice;
    pStockDataInfo->fMinPrice=pUpdateDataInfo->fEndPrice;
    pStockDataInfo->mTotalVolume=pUpdateDataInfo->mTotalVolume;
    pStockDataInfo->fTotalPrice=pUpdateDataInfo->fTotalPrice;
    return true;
}

bool StockTdxFileData::AddStockDataInfo(StockDataInfo* pAddDataInfo)
{
    if(pAddDataInfo->GetCode()!=strStockCode)
        return false;

    QString strDate=pAddDataInfo->GetDate();
    QString strTime=pAddDataInfo->GetTime();
    int mType=pAddDataInfo->GetType();
    QSharedPointer<StockDataInfo> pStockDataInfo=nullptr;
    if(mType==STOCK_DATA_TYPE_DAY)
    {

        pStockDataInfo=FindStockDayDataInfo(strDate);
        if(!pStockDataInfo)
            vecStockDayData.push_back(QSharedPointer<StockDataInfo>(pAddDataInfo));
    }
    else if(mType==STOCK_DATA_TYPE_5MIN)
    {
        pStockDataInfo=FindStocMin5DataInfo(strDate,strTime);
        if(!pStockDataInfo)
            vecStockMin5Data.push_back(QSharedPointer<StockDataInfo>(pAddDataInfo));
    }
    else
        return false;

    return true;
}


QVector<QSharedPointer<StockDataInfo>> StockTdxFileData::GetStockDataInfoList(QString strBeginDate,QString strEndDate,QString strBeginTime,
                                                     QString strEndTime,int mDataType)
{
    if(mDataType==STOCK_DATA_TYPE_DAY)
        return GetStockDayDataInfoList(strBeginDate,strEndDate);
    else if(mDataType==STOCK_DATA_TYPE_5MIN)
        return GetStockMin5DataInfoList(strBeginDate,strEndDate,strBeginTime,strEndTime);
    else
    {
        QVector<QSharedPointer<StockDataInfo>> vecStockDataInfo;
        return vecStockDataInfo;
    }
}

QVector<QSharedPointer<StockDataInfo>> StockTdxFileData::GetStockDayDataInfoList(QString strBeginDate,QString strEndDate)
{
    QVector<QSharedPointer<StockDataInfo>>  vecData;

    if(vecStockDayData.size()==0)
            return vecData;

    int mBeginPos=GetNearestStockDayIndex(strBeginDate);
    int mEndPos=GetNearestStockDayIndex(strEndDate);

    if(mBeginPos==-1 || mEndPos==-1 )
            return vecData;

    if(mBeginPos>mEndPos)
         return vecData;

     QSharedPointer<StockDataInfo> pStockDayData=nullptr;

     for(int i=mBeginPos;i<=mEndPos;i++)
     {
        pStockDayData=vecStockDayData[i];
        if(!pStockDayData)
             continue;
         vecData.push_back(pStockDayData);
     }
    return vecData;
}

QVector<QSharedPointer<StockDataInfo>> StockTdxFileData::GetStockMin5DataInfoList(QString strBeginDate,QString strEndDate,QString strBeginTime,QString strEndTime)
{
    QVector<QSharedPointer<StockDataInfo>>  vecData;

    if(vecStockMin5Data.size()==0)
        return vecData;

    QString strBeginDateTime=strBeginDate;
    strBeginDateTime+=" ";
    strBeginDateTime+=strBeginTime;

    QString strEndDateTime=strEndDate;
    strEndDateTime+=" ";
    strEndDateTime+=strEndTime;

    int mBeginPos=GetNearestStockMin5Index(strBeginDateTime);
    int mEndPos=GetNearestStockMin5Index(strEndDateTime);


    if(mBeginPos==-1 || mEndPos==-1 )
            return vecData;

    if(mBeginPos>mEndPos)
         return vecData;

     QSharedPointer<StockDataInfo> pStockMin5Data=nullptr;

     for(int i=mBeginPos;i<=mEndPos;i++)
     {
        pStockMin5Data=vecStockMin5Data[i];
        if(!pStockMin5Data)
             continue;
         vecData.push_back(pStockMin5Data);
     }
    return vecData;

}

QVector<QSharedPointer<StockDataInfo>> &  StockTdxFileData::GetAllStockDataInfoList(int mDataType)
{
    if(mDataType==STOCK_DATA_TYPE_DAY)
        return vecStockDayData;
    else if(mDataType==STOCK_DATA_TYPE_5MIN)
        return vecStockMin5Data;
    else
    {
        return vecStockDayData;
    }
}

//---
int StockTdxFileData::GetStockDayIndex(QString strDate)
{
    QMutexLocker locker(&mDayMutex);
    if(vecStockDayData.size()==0)
            return -1;
    QSharedPointer<StockDataInfo> pStockDayData=nullptr;
    for(int i=0;i<vecStockDayData.size();i++)
    {
        pStockDayData=vecStockDayData[i];
        if(!pStockDayData)
             continue;
        if(pStockDayData->GetDate()==strDate)
            return i;
    }
    return -1;
}

int StockTdxFileData::GetNearestStockDayIndex(QString strDate)
{
    QMutexLocker locker(&mDayMutex);
    if(vecStockDayData.size()==0)
            return -1;
    QString strTempDate=strDate.left(5);
    QSharedPointer<StockDataInfo> pStockDayData=nullptr;
    int mBeginPos=0;

    for(int i=0;i<vecStockDayData.size();i++)
    {
       pStockDayData=vecStockDayData[i];
       if(!pStockDayData)
           continue;
       QString strTemp=pStockDayData->GetDate().left(5);
       if(strTemp==strTempDate)
       {
           mBeginPos=i;
           break;
       }
    }

    strDate+=" 09:30";
    strDate = strDate.trimmed();
    QDateTime mDateTime=QDateTime::fromString(strDate ,"yyyy/MM/dd hh:mm");
    int mNearestDays=99999;
    int mNearestPos=-1;
    for(int i=mBeginPos;i<vecStockDayData.size();i++)
    {
        pStockDayData=vecStockDayData[i];
        if(!pStockDayData)
           continue;
        QString strCheckTime=vecStockDayData[i]->GetDate();
        strCheckTime+=" 09:30";
        strCheckTime = strCheckTime.trimmed();
        QDateTime mCheckDateTime=QDateTime::fromString(strCheckTime ,"yyyy/MM/dd hh:mm");

        int iDay=mCheckDateTime.daysTo(mDateTime);
        iDay=qAbs(iDay);
        if(iDay==0)
            return i;
        if((iDay<mNearestDays)&&(iDay>0))
        {
            mNearestDays=iDay;
            mNearestPos=i;
        }


    }

    if(mNearestPos>=0)
       return mNearestPos;

    return -1;
}


int  StockTdxFileData::GetStockMin5Index(QString strDateTime)
{
    if(vecStockMin5Data.size()==0)
            return -1;
     QMutexLocker locker(&mMin5Mutex);

     QSharedPointer<StockDataInfo> pStockMin5Data=nullptr;

     for(int i=0;i<vecStockMin5Data.size();i++)
     {
         pStockMin5Data=vecStockMin5Data[i];
         if(!pStockMin5Data)
              continue;
          QString strTempDateTime=pStockMin5Data->GetDate();
          strTempDateTime+=" ";
          strTempDateTime+=pStockMin5Data->GetTime();

          if(strTempDateTime==strDateTime)
             return i;


     }
     return -1;
}

int StockTdxFileData::GetNearestStockMin5Index(QString strDateTime)
{
    if(vecStockMin5Data.size()==0)
            return -1;

    QMutexLocker locker(&mMin5Mutex);

    QString strTempDate=strDateTime.left(5);
    QSharedPointer<StockDataInfo> pStockMin5Data=nullptr;
    int mBeginPos=0;

    for(int i=0;i<vecStockMin5Data.size();i++)
    {
       pStockMin5Data=vecStockMin5Data[i];
       if(!pStockMin5Data)
           continue;
       QString strTemp=pStockMin5Data->GetDateTime().left(5);
       if(strTemp==strTempDate)
       {
           mBeginPos=i;
           break;
       }
    }

    strDateTime = strDateTime.trimmed();
    QDateTime mDateTime=QDateTime::fromString(strDateTime ,"yyyy/MM/dd hh:mm");
    int mDateTime5Mins=mDateTime.toTime_t()/300;

    int mNearest5Mins=999999;
    int mNearestPos=-1;
    for(int i=mBeginPos;i<vecStockMin5Data.size();i++)
    {
       pStockMin5Data=vecStockMin5Data[i];
       if(!pStockMin5Data)
           continue;
       QString strCheckTime=vecStockMin5Data[i]->GetDateTime();
       strCheckTime = strCheckTime.trimmed();
       QDateTime mCheckDateTime=QDateTime::fromString(strCheckTime ,"yyyy/MM/dd hh:mm");
       int mCheckDateTime5Mins=mCheckDateTime.toTime_t()/300;
       int mDif5Mins=mDateTime5Mins-mCheckDateTime5Mins;
       mDif5Mins=qAbs(mDif5Mins);

       if(mDif5Mins==0)
           return i;

       if((mDif5Mins<mNearest5Mins)&&(mDif5Mins>0))
       {
           mNearest5Mins=mDif5Mins;
           mNearestPos=i;
       }
   }

   if(mNearestPos>=0)
       return mNearestPos;

    return -1;

}

QSharedPointer<StockDataInfo> StockTdxFileData::GetLastStockDataInfo(int mDataType)
{

    if(mDataType==STOCK_DATA_TYPE_DAY)
    {
        if(vecStockDayData.size()==0)
            return nullptr;
        return vecStockDayData[vecStockDayData.size()-1];
    }
    else if(mDataType==STOCK_DATA_TYPE_5MIN)
    {
       if(vecStockMin5Data.size()==0)
           return nullptr;
       return vecStockMin5Data[vecStockMin5Data.size()-1];
    }
    else
        return nullptr;
}


QVector<QSharedPointer<StockDataInfo>> StockTdxFileData::GetStockDataInfoPreOfIndex(QString strDate,QString strTime,int mDataType,int prenums)
{
    QVector<QSharedPointer<StockDataInfo>> vecData;
    if(prenums<=0)
        return vecData;
    if(mDataType==STOCK_DATA_TYPE_DAY)
    {
        int indexpos=GetNearestStockDayIndex(strDate);
        if(indexpos<0)
            return vecData;

        int beginpos=indexpos-prenums+1;

        for(int i=beginpos;i<=indexpos;i++)
        {
           vecData.push_back(vecStockDayData[i]);
        }
        return vecData;
    }
    else if(mDataType==STOCK_DATA_TYPE_5MIN)
    {
        QString strDateTime=strDate;
        strDateTime+=" ";
        strDateTime+=strTime;

        int indexpos=GetNearestStockMin5Index(strDateTime);
        if(indexpos<0)
            return vecData;
        int beginpos=indexpos-prenums+1;

        for(int i=beginpos;i<=indexpos;i++)
        {
           vecData.push_back(vecStockMin5Data[i]);
        }
        return vecData;
    }
    else
        return vecData;
}

bool StockTdxFileData::IsDataTypeValid(int mDataType)
{
    if(mDataType==STOCK_DATA_TYPE_DAY)
        return true;
    else if(mDataType==STOCK_DATA_TYPE_5MIN)
        return true;
    else
        return false;
}
//--------------------------------------------------------------------------
static int m_data_index=0;
bool StockTdxFileData::ReadAllStockDataFromStore(void)
{
#if 0
    QEventLoop loop;
    auto thread = QSharedPointer<QThread>(new QThread);
    connect( thread.data(), &QThread::started, [&]() {
        LoadStockDayFile();
        LoadStockMin5File();
        loop.quit();
    } );
    thread->start();
    loop.exec();
    thread->quit();
    thread->wait();
    thread=nullptr;
#else

   /* std::thread([&](){
              // std::thread::id tid = std::this_thread::get_id();
        LoadStockDayFile();
        LoadStockMin5File();
           }).detach();*/
    if(stockDataMgr()->IsExpStock(strStockCode))
    {
        LoadStockDayFile();
        LoadStockMin5File();
    }
    else
    {
        StockData* pExpStockData=stockDataMgr()->FindStockData("SH000001");
        if(pExpStockData)
        {
            StockTdxFileData *pTdxFileExpStockData=qobject_cast<StockTdxFileData*>(pExpStockData);
            LoadFromDayFileRepairDate(pTdxFileExpStockData);
            LoadFromMin5FileRepairDate(pTdxFileExpStockData);
        }

    }

    m_data_index++;
    emit data_process(m_data_index);
#endif

    return true;
}

bool StockTdxFileData::WriteAllStockDataToStore(void)
{

    return true;
}

//-----------------------------------



bool StockTdxFileData::LoadStockDayFile(void)
{
   // qDebug()<<strStockDayFilePath<<"   m_nums=" <<m_data_index << "\n";
    QFile file(strStockDayFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QMutexLocker locker(&mDayMutex);
    QTextStream in(&file);
    int mLine=0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        mLine++;
     //   if(line.contains(QString::fromLocal8Bit("数据来源")))
     //       continue;
        if(mLine>2)
        {
            QStringList  arryVal=line.split('\t',QString::SkipEmptyParts);
            if(arryVal.count()==7)
            {
                QString strDate=arryVal.at(0);
                float  fBeginPrice=arryVal.at(1).toFloat();
                float  fMaxPrice=arryVal.at(2).toFloat();
                float  fMinPrice=arryVal.at(3).toFloat();
                float  fEndPrice=arryVal.at(4).toFloat();
                long   mTotalVolume=arryVal.at(5).toLong();
                float  fTotalPrice=arryVal.at(6).toFloat();
                QSharedPointer<StockDataInfo> pStockDayData=QSharedPointer<StockDataInfo>(new StockDataInfo());
                pStockDayData->SetCode(strStockCode);
                pStockDayData->SetDate(strDate);
                pStockDayData->SetTime("");
                pStockDayData->SetBeginPrice(fBeginPrice);
                pStockDayData->SetMaxPrice(fMaxPrice);
                pStockDayData->SetMinPrice(fMinPrice);
                pStockDayData->SetEndPrice(fEndPrice);
                pStockDayData->SetTotalVolume(mTotalVolume);
                pStockDayData->SetTotalPrice(fTotalPrice);
                pStockDayData->SetType(STOCK_DATA_TYPE_DAY);
                vecStockDayData.push_back(pStockDayData);
            }


        }
    }
    file.close();
    return true;
}

bool StockTdxFileData::LoadFromDayFileRepairDate(StockTdxFileData *pExpStockData)
{
    QFile file(strStockDayFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;



    QVector<QSharedPointer<StockDataInfo>> vecTempStockDayData;

    QTextStream in(&file);
    int mLine=0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        mLine++;
     //   if(line.contains(QString::fromLocal8Bit("数据来源")))
     //       continue;
        if(mLine>2)
        {
            QStringList  arryVal=line.split('\t',QString::SkipEmptyParts);
            if(arryVal.count()==7)
            {
                QString strDate=arryVal.at(0);
                float  fBeginPrice=arryVal.at(1).toFloat();
                float  fMaxPrice=arryVal.at(2).toFloat();
                float  fMinPrice=arryVal.at(3).toFloat();
                float  fEndPrice=arryVal.at(4).toFloat();
                long   mTotalVolume=arryVal.at(5).toLong();
                float  fTotalPrice=arryVal.at(6).toFloat();
                QSharedPointer<StockDataInfo> pStockDayData=QSharedPointer<StockDataInfo>(new StockDataInfo());
                pStockDayData->SetCode(strStockCode);
                pStockDayData->SetDate(strDate);
                pStockDayData->SetTime("");
                pStockDayData->SetBeginPrice(fBeginPrice);
                pStockDayData->SetMaxPrice(fMaxPrice);
                pStockDayData->SetMinPrice(fMinPrice);
                pStockDayData->SetEndPrice(fEndPrice);
                pStockDayData->SetTotalVolume(mTotalVolume);
                pStockDayData->SetTotalPrice(fTotalPrice);
                pStockDayData->SetType(STOCK_DATA_TYPE_DAY);
                vecTempStockDayData.push_back(pStockDayData);
            }
        }
    }
    file.close();

    if(vecTempStockDayData.size()==0)
            return false;



    QSharedPointer<StockDataInfo> pStockDayData=nullptr;

    pStockDayData=vecTempStockDayData[0];
    QString strBeginDate=pStockDayData->GetDate();
    int mExpBeginPos=pExpStockData->GetStockDayIndex(strBeginDate);

    if(mExpBeginPos==-1)
       return false;

    QVector<QSharedPointer<StockDataInfo>> & vecExpStockDayData=pExpStockData->GetAllStockDataInfoList(STOCK_DATA_TYPE_DAY);

    QMutexLocker locker(&mDayMutex);
    QSharedPointer<StockDataInfo> pExpStockDayData=nullptr;
    int pos=0;
    int i=0;
    for(i=mExpBeginPos;i<vecExpStockDayData.size();i++)
    {
        pExpStockDayData=vecExpStockDayData[i];
        if(!pExpStockDayData)
            continue;
        pStockDayData=vecTempStockDayData[pos];
        if(!pStockDayData)
            continue;
        if(pStockDayData->GetDate()==pExpStockDayData->GetDate())
        {
           vecStockDayData.push_back(pStockDayData);
           pos++;
           if(pos>=vecTempStockDayData.size())
           {
               break;
           }
         }
         else
         {
              QSharedPointer<StockDataInfo> pData=QSharedPointer<StockDataInfo> (new StockDataInfo());
              if(pos>0)
              {
                 QSharedPointer<StockDataInfo> pPreStockDayData=vecTempStockDayData[pos-1];
                 pStockDayData->SetCode(strStockCode);
                 pStockDayData->SetTime("");
                 pData->SetDate(pExpStockDayData->GetDate());
                 pData->SetBeginPrice(pPreStockDayData->GetEndPrice());
                 pData->SetMaxPrice(pPreStockDayData->GetEndPrice());
                 pData->SetMinPrice(pPreStockDayData->GetEndPrice());
                 pData->SetEndPrice(pPreStockDayData->GetEndPrice());
                 pData->SetTotalVolume(0);
                 pData->SetTotalPrice(0.0);
                 pStockDayData->SetType(STOCK_DATA_TYPE_DAY);
                 vecStockDayData.push_back(pData);
              }

         }
    }

    if(i<vecExpStockDayData.size()-1)
    {
        for(int j=i+1;j<vecExpStockDayData.size();j++)
        {
           pExpStockDayData=vecExpStockDayData[j];
           if(!pExpStockDayData)
                continue;
            QSharedPointer<StockDataInfo> pData=QSharedPointer<StockDataInfo>(new StockDataInfo());
            QSharedPointer<StockDataInfo> pPreStockDayData=vecTempStockDayData[vecTempStockDayData.size()-1];
            pStockDayData->SetCode(strStockCode);
            pStockDayData->SetTime("");
            pData->SetDate(pExpStockDayData->GetDate());
            pData->SetBeginPrice(pPreStockDayData->GetEndPrice());
            pData->SetMaxPrice(pPreStockDayData->GetEndPrice());
            pData->SetMinPrice(pPreStockDayData->GetEndPrice());
            pData->SetEndPrice(pPreStockDayData->GetEndPrice());
            pData->SetTotalVolume(0);
            pData->SetTotalPrice(0.0);
            pStockDayData->SetType(STOCK_DATA_TYPE_DAY);
            vecStockDayData.push_back(pData);
         }
    }

    return true;
}

bool StockTdxFileData::LoadStockMin5File(void)
{
    QFile file(strStockMin5FilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QMutexLocker locker(&mMin5Mutex);
    QTextStream in(&file);
    int mLine=0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        mLine++;
        //if(line.contains(QString::fromLocal8Bit("数据来源")))
        //    continue;
        if(mLine>2)
        {
            QStringList  arryVal=line.split('\t',QString::SkipEmptyParts);
            if(arryVal.count()==8)
            {
                QString strDate=arryVal.at(0);
                QString strTime=arryVal.at(1);
                if(strTime.length()==4)
                {
                    QString strTemp=strTime.left(2);
                    QString strTemp2=strTime.right(2);
                    strTime=strTemp;
                    strTime+=":";
                    strTime+=strTemp2;
                }
                float  fBeginPrice=arryVal.at(2).toFloat();
                float  fMaxPrice=arryVal.at(3).toFloat();
                float  fMinPrice=arryVal.at(4).toFloat();
                float  fEndPrice=arryVal.at(5).toFloat();
                long   mTotalVolume=arryVal.at(6).toLong();
                float  fTotalPrice=arryVal.at(7).toFloat();

               QSharedPointer<StockDataInfo> pStockMin5Data=QSharedPointer<StockDataInfo>(new StockDataInfo());
               pStockMin5Data->SetCode(strStockCode);
               pStockMin5Data->SetDate(strDate);
               pStockMin5Data->SetTime(strTime);
               pStockMin5Data->SetBeginPrice(fBeginPrice);
               pStockMin5Data->SetMaxPrice(fMaxPrice);
               pStockMin5Data->SetMinPrice(fMinPrice);
               pStockMin5Data->SetEndPrice(fEndPrice);
               pStockMin5Data->SetTotalVolume(mTotalVolume);
               pStockMin5Data->SetTotalPrice(fTotalPrice);
               pStockMin5Data->SetType(STOCK_DATA_TYPE_5MIN);
               vecStockMin5Data.push_back(pStockMin5Data);


            }
        }

    }
    return true;
}

bool  StockTdxFileData::LoadFromMin5FileRepairDate(StockTdxFileData *pExpStockData)
{
    QFile file(strStockMin5FilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QVector<QSharedPointer<StockDataInfo>> vecTempStockMin5Data;

    QTextStream in(&file);
    int mLine=0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        mLine++;
        //if(line.contains(QString::fromLocal8Bit("数据来源")))
        //    continue;
        if(mLine>2)
        {
            QStringList  arryVal=line.split('\t',QString::SkipEmptyParts);
            if(arryVal.count()==8)
            {
                QString strDate=arryVal.at(0);
                QString strTime=arryVal.at(1);
                if(strTime.length()==4)
                {
                    QString strTemp=strTime.left(2);
                    QString strTemp2=strTime.right(2);
                    strTime=strTemp;
                    strTime+=":";
                    strTime+=strTemp2;
                }
                float  fBeginPrice=arryVal.at(2).toFloat();
                float  fMaxPrice=arryVal.at(3).toFloat();
                float  fMinPrice=arryVal.at(4).toFloat();
                float  fEndPrice=arryVal.at(5).toFloat();
                long   mTotalVolume=arryVal.at(6).toLong();
                float  fTotalPrice=arryVal.at(7).toFloat();

               QSharedPointer<StockDataInfo> pStockMin5Data= QSharedPointer<StockDataInfo>(new StockDataInfo());
               pStockMin5Data->SetCode(strStockCode);
               pStockMin5Data->SetDate(strDate);
               pStockMin5Data->SetTime(strTime);
               pStockMin5Data->SetBeginPrice(fBeginPrice);
               pStockMin5Data->SetMaxPrice(fMaxPrice);
               pStockMin5Data->SetMinPrice(fMinPrice);
               pStockMin5Data->SetEndPrice(fEndPrice);
               pStockMin5Data->SetTotalVolume(mTotalVolume);
               pStockMin5Data->SetTotalPrice(fTotalPrice);
               pStockMin5Data->SetType(STOCK_DATA_TYPE_5MIN);
               vecTempStockMin5Data.push_back(pStockMin5Data);


            }
        }

    }

    if(vecTempStockMin5Data.size()==0)
            return false;
    QSharedPointer<StockDataInfo> pStockMin5Data=nullptr;
    pStockMin5Data=vecTempStockMin5Data[0];
    QString stDateTime=pStockMin5Data->GetDateTime();
    int mExpBeginPos=pExpStockData->GetStockMin5Index(stDateTime);
    if(mExpBeginPos==-1)
        return false;

    QVector<QSharedPointer<StockDataInfo>> & vecExpStockMin5Data=pExpStockData->GetAllStockDataInfoList(STOCK_DATA_TYPE_5MIN);

    QMutexLocker locker(&mMin5Mutex);
    QSharedPointer<StockDataInfo> pExpStockMin5Data=NULL;
    int pos=0;
    int i=0;
    for(i=mExpBeginPos;i<vecExpStockMin5Data.size();i++)
    {
            pExpStockMin5Data=vecExpStockMin5Data[i];
            if(!pExpStockMin5Data)
                continue;

            pStockMin5Data=vecTempStockMin5Data[pos];
            if(!pStockMin5Data)
                continue;
            if(pStockMin5Data->GetDateTime()==pExpStockMin5Data->GetDateTime())
            {
                vecStockMin5Data.push_back(pStockMin5Data);
                pos++;
                if(pos>=vecTempStockMin5Data.size())
                {
                    break;
                }
            }
            else
            {

                if(pos>0)
                {
                    QSharedPointer<StockDataInfo> pStockMin5Data=QSharedPointer<StockDataInfo>(new StockDataInfo());

                    QSharedPointer<StockDataInfo> pPreStockMin5Data=vecTempStockMin5Data[pos-1];
                    pStockMin5Data->SetCode(strStockCode);
                    pStockMin5Data->SetDate(pExpStockMin5Data->GetDate());
                    pStockMin5Data->SetTime(pExpStockMin5Data->GetTime());
                    pStockMin5Data->SetBeginPrice(pPreStockMin5Data->GetBeginPrice());
                    pStockMin5Data->SetMaxPrice(pPreStockMin5Data->GetMaxPrice());
                    pStockMin5Data->SetMinPrice(pPreStockMin5Data->GetMinPrice());
                    pStockMin5Data->SetEndPrice(pPreStockMin5Data->GetEndPrice());
                    pStockMin5Data->SetTotalPrice(0.0);
                    pStockMin5Data->SetTotalVolume(0);
                    pStockMin5Data->SetType(STOCK_DATA_TYPE_5MIN);
                    vecStockMin5Data.push_back(pStockMin5Data);

                }
            }

    }

    if(i<vecExpStockMin5Data.size()-1)
    {
            for(int j=i+1;j<vecExpStockMin5Data.size();j++)
            {
                pExpStockMin5Data=vecExpStockMin5Data[j];
                if(!pExpStockMin5Data)
                    continue;
                QSharedPointer<StockDataInfo> pData=QSharedPointer<StockDataInfo>(new StockDataInfo());
                QSharedPointer<StockDataInfo> pPreStockMin5Data=vecTempStockMin5Data[vecTempStockMin5Data.size()-1];
                pStockMin5Data->SetCode(strStockCode);
                pData->SetDate(pExpStockMin5Data->GetDate());
                pData->SetTime(pExpStockMin5Data->GetTime());
                pData->SetBeginPrice(pPreStockMin5Data->GetEndPrice());
                pData->SetMaxPrice(pPreStockMin5Data->GetEndPrice());
                pData->SetMinPrice(pPreStockMin5Data->GetEndPrice());
                pData->SetEndPrice(pPreStockMin5Data->GetEndPrice());
                pData->SetTotalVolume(0);
                pData->SetTotalPrice(0.0);
                pStockMin5Data->SetType(STOCK_DATA_TYPE_5MIN);
                vecStockMin5Data.push_back(pData);
            }

    }
    return true;
}
