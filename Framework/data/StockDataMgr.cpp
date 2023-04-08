#include "StockDataMgr.h"
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include "StockTdxDatData.h"
//#include <thread>
#include <QThreadPool>
#include <QFileInfo>
#include "StockDataLoadRunable.h"
#include "Utils.h"

#define TDXDir  "D:\\new_jyplug"

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
       // QFileInfo file(vecStockFilePathXmlInfo[i]->strStockDayPath);
       // if(file.exists())
       // {
            QString strStockCode=vecStockFilePathXmlInfo[i]->strStockCode;
            QString strStockName=vecStockFilePathXmlInfo[i]->strStockName;

            StockTdxDatData *pStockTdxDatData=new StockTdxDatData(strStockCode);
            pStockTdxDatData->SetStockName(strStockName);

            QString strDayPath=TDXDir;
            QString strDayName=strStockCode.toLower();
            strDayName+=".day";
            strDayPath+="\\vipdoc";
            if(strStockCode.left(2)=="SH")
            {
                strDayPath+="\\sh\\lday\\";
                strDayPath+=strDayName;
            }
            else
            {
                strDayPath+="\\sz\\lday\\";
                strDayPath+=strDayName;
            }

             QString strMin5Path=TDXDir;
             QString strMin5Name=strStockCode.toLower();
             strMin5Name+=".lc5";
             strMin5Path+="\\vipdoc";
             if(strStockCode.left(2)=="SH")
             {
                 strMin5Path+="\\sh\\fzline\\";
                 strMin5Path+=strMin5Name;
             }
             else
             {
                 strMin5Path+="\\sz\\fzline\\";
                 strMin5Path+=strMin5Name;
             }

             QFileInfo dayFileInfo(strDayPath);
             QFileInfo min5FileInfo(strMin5Path);

            if(dayFileInfo.exists() && min5FileInfo.exists())
            {
                pStockTdxDatData->SetStockDayFilePath(strDayPath);
                pStockTdxDatData->SetStockMin5FilePath(strMin5Path);
                connect(pStockTdxDatData, SIGNAL(data_process(int)), this, SLOT(solt_data_process(int)));

                vecStockData.push_back(pStockTdxDatData);
                vecStockCodeList.push_back(strStockCode);
            }
       // }
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

     StockTdxDatData *pExpTdxStockDatData=qobject_cast<StockTdxDatData*>(pExpStockData);

    //QVector<QSharedPointer<StockDataInfo>> vecData=pExpStockData->GetStockDataInfoList("2022/04/19","2022/05/06","09:30","15:00",STOCK_DATA_TYPE_5MIN);

    //QVector<QSharedPointer<StockDataInfo>> vecData=pExpStockData->GetStockDataInfoList("2022/04/01","2022/05/06");

    int kernelnums=Utils::GetSysKernalNum();
    QThreadPool::globalInstance()->setMaxThreadCount(kernelnums); //12

    for(int i=0;i<vecStockData.size();i++)
    {
        if(vecStockData[i]->GetStockCode()!="SH000001")
        {
            StockTdxDatData *pTdxStockDatData=qobject_cast<StockTdxDatData*>(vecStockData[i]);
            pTdxStockDatData->SetStockExpTdxDatData(pExpTdxStockDatData);
            StockDataLoadRunable* task = new StockDataLoadRunable(vecStockData[i]);
            task->setAutoDelete(true);
            QThreadPool::globalInstance()->start(task);
        }
    }
   // QThreadPool::globalInstance()->waitForDone();
   // pExpStockData=FindStockData("SZ002487");
   // QVector<QSharedPointer<StockDataInfo>> vecData=pExpStockData->GetStockDataInfoList("2023/04/04","2023/05/06","09:30","15:00",STOCK_DATA_TYPE_5MIN);
    return true;
}

bool StockDataMgr::doReInitStockList(void)
{
    QVector<StockData*>  vecNewStockData;
    QVector<QString>    vecNewStockCodeList;
    for(int i=0;i<vecStockData.size();i++)
    {
        if(vecStockData[i]->GetStockDataInfoSize(STOCK_DATA_TYPE_DAY)>0)
        {
            vecNewStockCodeList.push_back(vecStockData[i]->GetStockCode());
            vecNewStockData.push_back(vecStockData[i]);
        }
    }
    vecStockCodeList=vecNewStockCodeList;
    vecStockData=vecNewStockData;
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

QString StockDataMgr::GetStockName(QString strStockCode)
{
    auto iterator = std::find_if(vecStockData.begin(), vecStockData.end(), [&](auto pStockData) {
                if (pStockData->GetStockCode() == strStockCode)
                    return true;
                else
                    return  false;
                });

   if (iterator != vecStockData.end())
   {
      return (*iterator)->GetStockName();
   }
   return "";
}
