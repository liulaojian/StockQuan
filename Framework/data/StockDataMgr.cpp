﻿#include "StockDataMgr.h"
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include "StockTdxFileData.h"
//#include <thread>
#include <QThreadPool>
#include "StockDataLoadRunable.h"
StockDataMgr* StockDataMgr::s_pIntance = nullptr;

StockDataMgr::StockDataMgr()
{

}


StockDataMgr::~StockDataMgr()
{

}


StockDataMgr* StockDataMgr::GetInstance()
{
    if (s_pIntance == NULL)
    {
        s_pIntance = new StockDataMgr();
        Q_ASSERT(s_pIntance);
    }
    return s_pIntance;
}

bool StockDataMgr::Init(void)
{
    QString strIniFilePath=QCoreApplication::applicationDirPath();
    strIniFilePath+="/stockquan.ini";
    QSettings setting(strIniFilePath,QSettings::IniFormat);

    QString strXmlPath=setting.value("MyStock/StockXmlFile").toString();
    qDebug()<<strXmlPath;
    doParseStockFilePathXmlFile(strXmlPath);
    if(vecStockFilePathXmlInfo.size()==0)
        return false;

    for(int i=0;i<vecStockFilePathXmlInfo.size();i++)
    {
        QString strStockCode=vecStockFilePathXmlInfo[i]->strStockCode;
        QString strStockName=vecStockFilePathXmlInfo[i]->strStockName;

        StockTdxFileData *pStockTdxFileData=new StockTdxFileData(strStockCode);
        pStockTdxFileData->SetStockName(strStockName);
        pStockTdxFileData->SetStockDayFilePath(vecStockFilePathXmlInfo[i]->strStockDayPath);
        pStockTdxFileData->SetStockMin5FilePath(vecStockFilePathXmlInfo[i]->strStockMin5Path);
        connect(pStockTdxFileData, SIGNAL(data_process(int)), this, SLOT(solt_data_process(int)));
        vecStockData.push_back(pStockTdxFileData);
    }

    mMaxStockDataNums=vecStockData.size();
    /*std::thread([&](){
               // std::thread::id tid = std::this_thread::get_id();
        for(int i=0;i<vecStockData.size();i++)
        {
            vecStockData[i]->Init();
            vecStockData[i]->ReadAllStockDataFromStore();
        }

     }).detach();
    */

    StockData* pExpStockData=FindStockData("SH000001");
    if(!pExpStockData)
        return false;

    pExpStockData->Init();
    pExpStockData->ReadAllStockDataFromStore();

    //QVector<QSharedPointer<StockDataInfo>> vecData=pExpStockData->GetStockDataInfoList("2022/04/19","2022/05/06","09:30","15:00",STOCK_DATA_TYPE_5MIN);

    //QVector<QSharedPointer<StockDataInfo>> vecData=pExpStockData->GetStockDataInfoList("2022/04/01","2022/05/06");

    QThreadPool::globalInstance()->setMaxThreadCount(10);

    for(int i=0;i<vecStockData.size();i++)
    {
        if(vecStockData[i]->GetStockCode()!="SH000001")
        {
            StockDataLoadRunable* task = new StockDataLoadRunable(vecStockData[i]);
            task->setAutoDelete(true);
            QThreadPool::globalInstance()->start(task);
        }
    }

    return true;
}

bool  StockDataMgr::IsExpStock(QString strStockCode)
{
    if(strStockCode=="SH000001" || strStockCode=="SH000016" || strStockCode=="SH000300" || strStockCode=="SZ399001"||  strStockCode=="SZ399006" )
            return true;
        return false;
}

bool StockDataMgr::doParseStockFilePathXmlFile(QString strXmlPath)
{
    QDomDocument doc("qdocument");
    QFile file(strXmlPath);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    QDomNodeList subList= doc.elementsByTagName("StockCode");
    for (int index = 0; index != subList.size(); ++index)
    {
        QDomNode node = subList.at(index);
        QDomElement ele = node.toElement();

        QString strStockCode=ele.attribute("code","");
        QString strStockName=ele.attribute("name","");
        QString strStockDayFilePath=ele.attribute("dayfilepath","");
        QString strStockMin5FilePath=ele.attribute("minfilepath","");
        StockFilePathXmlInfo *pStockFilePathXmlInfo=new StockFilePathXmlInfo();
        pStockFilePathXmlInfo->strStockCode=strStockCode;
        pStockFilePathXmlInfo->strStockName=strStockName;
        pStockFilePathXmlInfo->strStockDayPath=strStockDayFilePath;
        pStockFilePathXmlInfo->strStockMin5Path=strStockMin5FilePath;
        vecStockFilePathXmlInfo.push_back(pStockFilePathXmlInfo);

    }

    return true;
}

void StockDataMgr::solt_data_process(int index)
{
    emit stock_data_process(index,mMaxStockDataNums);
}


StockData* StockDataMgr::FindStockData(QString strStockCode)
{
    auto iterator = std::find_if(vecStockData.begin(), vecStockData.end(), [&](auto pStockData) {
                if (pStockData->GetStockCode() == strStockCode)
                    return true;
                else
                    return  false;
                });

   if (iterator != vecStockData.end())
   {
      return *iterator;
   }

   return nullptr;
}

