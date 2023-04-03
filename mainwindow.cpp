#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGroupBox>
#include <QMessageBox>
#include <QDebug>

#include "dlgstocklist.h"

#include "StockDataMgr.h"

#include "FunTimeSharingDirectUp.h"
#include "dlgtimesharingdirectup.h"

#include "dlgresult.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->toolBox->setFrameStyle(QFrame::Panel);
    ui->toolBox->setItemIcon(0,QIcon(":/image/basic.png"));
    ui->toolBox->setItemText(0,"基础功能");

    ui->toolBox->setItemIcon(1,QIcon(":/image/extend.png"));
    ui->toolBox->setItemText(1,"扩展功能");

    QWidget *widget=ui->toolBox->widget(0);


    QToolButton *btnStockList=CreateToolButton("股票列表",":/image/list.png");
    QToolButton *btnJettonFilter=CreateToolButton("分时筛选",":/image/filter.png");

    connect(btnStockList,SIGNAL(clicked()),this,SLOT(btn_stocklist_click()));
    connect(btnJettonFilter,SIGNAL(clicked()),this,SLOT(btn_jetteonfilter_click()));

    QVBoxLayout *pLayOut=new QVBoxLayout();
    pLayOut->addWidget(btnStockList);
    pLayOut->addWidget(btnJettonFilter);
    pLayOut->addStretch();
    pLayOut->setAlignment(Qt::AlignHCenter);
    widget->setLayout(pLayOut);

   /* QToolButton *btnNewStockList=CreateToolButton("股票列表",":/image/list.png");
    QWidget *newwidget=new QWidget(ui->toolBox);
    QVBoxLayout *pNewLayOut=new QVBoxLayout();
    pNewLayOut->addWidget(btnNewStockList);
    pNewLayOut->addStretch();
    pNewLayOut->setAlignment(Qt::AlignHCenter);
    newwidget->setLayout(pNewLayOut);
    ui->toolBox->addItem(newwidget,QIcon(":/image/basic.png"),"设置");
*/

    ui->toolBox->setCurrentIndex(0);


    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->verticalHeader()->setVisible(false);
    QStringList header;
    header<<"序号"<<"代码"<<"名称"<<"日数据"<<"5分钟数据";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;color: black;}");
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget->setColumnWidth(1,150);
    ui->tableWidget->setColumnWidth(2,150);
    ui->tableWidget->setColumnWidth(3,150);
    ui->tableWidget->setColumnWidth(4,150);

    m_pStockListMenu=new QMenu(this);
    m_pActionOpen=new QAction(this);
    m_pActionOpen->setText("打开...");
    m_pStockListMenu->addAction(m_pActionOpen);

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(solt_stocklist_customContextMenuRequested(const QPoint&)));
    connect(m_pStockListMenu,SIGNAL(triggered(QAction *)),this,SLOT(solt_stockActionOpen_triggered(QAction *)));
    //InitStockList();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::btn_stocklist_click()
{
   // QMessageBox::information(this,"注意","btn_stocklist_click");
   //DlgStockList dlg;
   // dlg.exec();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::btn_jetteonfilter_click()
{
    //QMessageBox::information(this,"注意","btn_jetteonfilter_click");
    //ui->stackedWidget->setCurrentIndex(1);

    DlgTimeSharingDirectUp dlg;

    int ret=dlg.exec();

    if (ret!=QDialog::Accepted)
        return;

    float fUpValue=dlg.GetUpValue();
    float fDownValue=dlg.GetDownValue();

    StockData* pStockData=stockDataMgr()->FindStockData("SH000001");
    QSharedPointer<StockDataInfo> pStockDataInfo=pStockData->GetLastStockDataInfo(STOCK_DATA_TYPE_5MIN);
    QString strDateTime=pStockDataInfo->GetDateTime();

    QVector<QString> vecStockList=stockDataMgr()->GetStockCodeList();

    QSharedPointer<FunTimeSharingDirectUp> pFunTimeSharingDirectUp=QSharedPointer<FunTimeSharingDirectUp>(new FunTimeSharingDirectUp());

    QVector<QString> vecResult=pFunTimeSharingDirectUp->doFunTimeSharingDirectUp(vecStockList,strDateTime);

    for(int i=0;i<vecResult.size();i++)
    {
        qDebug()<<vecResult[i];
    }

    DlgResult dlgResult;
    dlgResult.SetResultList(vecResult);
    dlgResult.InitList();
    dlgResult.exec();
}


void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()!=QEvent::WindowStateChange) {
        QWidget::changeEvent(event);
        return;
    }

    if(this->windowState()==Qt::WindowMaximized)
    {
        QLayout *layout= ui->centralwidget->layout();
       // qDebug()<<"WindowMaximized  "<< layout->metaObject()->className()<<"\n";
        QHBoxLayout *hboxlayout=qobject_cast<QHBoxLayout*>(layout);
        if(hboxlayout)
        {
             //qDebug()<<"hboxlayout"<<"\n";
            hboxlayout->setStretch(0,1);
            hboxlayout->setStretch(1,10);
        }
    }

    QWidget::changeEvent(event);
}

QToolButton* MainWindow::CreateToolButton(QString text, QString icon)
{
   QToolButton *toolBtn=new QToolButton(this);
    toolBtn->setText(text);
    toolBtn->setIcon(QIcon(icon));
    toolBtn->setIconSize(QSize(48,48));
    toolBtn->setAutoRaise(true);
    toolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    return toolBtn;
}


bool MainWindow::InitStockList(void)
{
    QVector<StockData*>  vecStockData=stockDataMgr()->GetStockDataList();
    ui->tableWidget->setRowCount(vecStockData.size());

    StockData* pStockData=nullptr;

    QSharedPointer<StockDataInfo> pStockDataInfo=nullptr;

    for(int i=0;i<vecStockData.size();i++)
    {
        pStockData=vecStockData[i];

        QString strId=QString::asprintf("%d",i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(strId));
        ui->tableWidget->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->tableWidget->setItem(i,1,new QTableWidgetItem(pStockData->GetStockCode()));
        ui->tableWidget->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        QVariant data=QVariant::fromValue((void*)pStockData);
        ui->tableWidget->item(i,1)->setData(Qt::UserRole,data);

        ui->tableWidget->setItem(i,2,new QTableWidgetItem(pStockData->GetStockName()));
        ui->tableWidget->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        pStockDataInfo=pStockData->GetLastStockDataInfo(STOCK_DATA_TYPE_DAY);
        if(pStockDataInfo)
        {
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(pStockDataInfo->GetDate()));
            ui->tableWidget->item(i,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }

        pStockDataInfo=pStockData->GetLastStockDataInfo(STOCK_DATA_TYPE_5MIN);
        if(pStockDataInfo)
        {
            ui->tableWidget->setItem(i,4,new QTableWidgetItem(pStockDataInfo->GetDateTime()));
            ui->tableWidget->item(i,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }


    }

    return true;
}

void MainWindow::solt_stocklist_customContextMenuRequested(const QPoint &pos)
{
     m_pStockListMenu->exec(QCursor::pos());
}

void  MainWindow::solt_stockActionOpen_triggered(QAction *action)
{
    if(action==m_pActionOpen)
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
}
