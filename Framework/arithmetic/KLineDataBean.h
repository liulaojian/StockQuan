#pragma once

#include <QString>
#include <QVector>

#include "StockData.h"

enum
{
    K_LINE_DAY=0,				//ID
    K_LINE_5MIN=1,
    K_LINE_15MIN=2,
    K_LINE_30MIN=3,
    K_LINE_60MIN=4,
};

typedef struct KLineBaseDataTag
{
    QVector<long> vec_time;			//时间
    QVector<float> vec_open_price;	//开盘价
    QVector<float> vec_high_price;	//最高价
    QVector<float> vec_low_price;	//最低价
    QVector<float> vec_close_price;	//结束价
    QVector<float> vec_avg_price;	//平均价格
    QVector<long> vec_volume;	//交易量
    QVector<float> vec_volume_price;	////交易总价格
    int m_length;	//长度
    int m_type;		//类型
} KLineBaseData;
typedef QVector<KLineBaseData *> Vec_KLineBaseData;


class KLineDataBean:public QObject
{
Q_OBJECT
public:
    KLineDataBean();
    virtual ~KLineDataBean();

    //将股票数据转换为K线基础数据strDateTime是日期，mNums日期 向前mNums个(包括strDate)
    //mKLineType 转换的K线类型
   static QSharedPointer<KLineBaseData>  ConvStockDataInfoToKLineBaseData(StockData *pStockData,int mKLineType,QString strDateTime,int mNums);


};


