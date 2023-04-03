#pragma once
#include <QObject>
#include <QThread>
class FunGlobal:public QThread
{
 Q_OBJECT
 public:
    FunGlobal() {}
    virtual ~FunGlobal() {}

public:
    void StopFun(void) {bStop=true;}

signals:
   void fun_process(int percent);

public:
    virtual void WaitForFun(void)=0;

protected:
   bool bStop=false;

};
