/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "StockChart.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QToolBox *toolBox;
    QWidget *page_basic;
    QWidget *page_extend;
    QStackedWidget *stackedWidget;
    QWidget *page_stocklist;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;
    QWidget *page_filterinfo;
    StockChart *page_stockchart;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1280, 761);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        toolBox = new QToolBox(centralwidget);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        page_basic = new QWidget();
        page_basic->setObjectName(QString::fromUtf8("page_basic"));
        page_basic->setGeometry(QRect(0, 0, 157, 641));
        toolBox->addItem(page_basic, QString::fromUtf8("\345\237\272\347\241\200\345\212\237\350\203\275"));
        page_extend = new QWidget();
        page_extend->setObjectName(QString::fromUtf8("page_extend"));
        page_extend->setGeometry(QRect(0, 0, 157, 641));
        toolBox->addItem(page_extend, QString::fromUtf8("\346\211\251\345\261\225\345\212\237\350\203\275"));

        horizontalLayout_2->addWidget(toolBox);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page_stocklist = new QWidget();
        page_stocklist->setObjectName(QString::fromUtf8("page_stocklist"));
        verticalLayout_2 = new QVBoxLayout(page_stocklist);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableWidget = new QTableWidget(page_stocklist);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        verticalLayout->addWidget(tableWidget);


        verticalLayout_2->addLayout(verticalLayout);

        stackedWidget->addWidget(page_stocklist);
        page_filterinfo = new QWidget();
        page_filterinfo->setObjectName(QString::fromUtf8("page_filterinfo"));
        stackedWidget->addWidget(page_filterinfo);
        page_stockchart = new StockChart();
        page_stockchart->setObjectName(QString::fromUtf8("page_stockchart"));
        stackedWidget->addWidget(page_stockchart);

        horizontalLayout_2->addWidget(stackedWidget);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 7);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1280, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        toolBox->setItemText(toolBox->indexOf(page_basic), QApplication::translate("MainWindow", "\345\237\272\347\241\200\345\212\237\350\203\275", nullptr));
        toolBox->setItemText(toolBox->indexOf(page_extend), QApplication::translate("MainWindow", "\346\211\251\345\261\225\345\212\237\350\203\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
