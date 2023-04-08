#pragma once
#include <QObject>
#include <QDateTime>

class Utils:public QObject
{
Q_OBJECT
public:
    Utils();
    virtual ~Utils();

   static QDateTime ConverToDateTime(QString strDateTime);
   static int GetSysKernalNum(void);
};
