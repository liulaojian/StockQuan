#include "Utils.h"
#include <QStringList>
Utils::Utils()
{

}

Utils::~Utils()
{

}

//xxxx/xx/xx  xx:xx
QDateTime Utils::ConverToDateTime(QString strDateTime)
{
    QStringList list1=strDateTime.split(" ");
    QString strDate=list1[0];
    QString strTime=list1[1];

    QStringList list2=strDate.split("/");
    QString strYear=list2[0];
    QString strMonth=list2[1];
    QString strDay=list2[2];

    QStringList list3=strTime.split(":");
    QString strHour=list3[0];
    QString strMin=list3[1];

    QDate date(strYear.toInt(),strMonth.toInt(),strDay.toInt());
    QTime time(strHour.toInt(),strMin.toInt());
    QDateTime datetime(date,time);
    return datetime;
}
