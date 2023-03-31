#include "splash.h"
#include "ui_splash.h"

#include "mainwindow.h"
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include "StockDataMgr.h"
Splash::Splash(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Splash)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags()); //设置成无边框
    timer = new QTimer(this);
    connect(stockDataMgr(), SIGNAL(stock_data_process(int,int)), this, SLOT(solt_stock_data_process(int,int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(showMainWindow()));
    timer->start(100);
    stockDataMgr()->Init();
}

Splash::~Splash()
{
    delete ui;
}


void Splash::waitFor(MainWindow *w,int int_ms)
{
    mainWin=w;
    //QTimer::singleShot(int_ms,this,SLOT(showMainWindow()));
}


void Splash::showMainWindow()
{

    if(mainWin!=nullptr && mLoadPercent>=100)
    {
        timer->stop();
        mainWin->InitStockList();
        mainWin->show();  //showMaximized
        this->close();
    }
}


void Splash::solt_stock_data_process(int index,int max_index)
{
    qDebug()<<"index="<<index<<"  maxindex="<<max_index<<"\n";
    float f_percent=(index*100.0)/max_index;
    mLoadPercent=f_percent;
    ui->progressBar->setValue(mLoadPercent);
}
