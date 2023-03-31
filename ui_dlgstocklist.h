/********************************************************************************
** Form generated from reading UI file 'dlgstocklist.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGSTOCKLIST_H
#define UI_DLGSTOCKLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgStockList
{
public:
    QListWidget *listWidget;
    QTableWidget *tableWidget;

    void setupUi(QDialog *DlgStockList)
    {
        if (DlgStockList->objectName().isEmpty())
            DlgStockList->setObjectName(QString::fromUtf8("DlgStockList"));
        DlgStockList->resize(950, 553);
        listWidget = new QListWidget(DlgStockList);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(0, 1, 231, 551));
        tableWidget = new QTableWidget(DlgStockList);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(240, 0, 700, 550));

        retranslateUi(DlgStockList);

        QMetaObject::connectSlotsByName(DlgStockList);
    } // setupUi

    void retranslateUi(QDialog *DlgStockList)
    {
        DlgStockList->setWindowTitle(QApplication::translate("DlgStockList", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DlgStockList: public Ui_DlgStockList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGSTOCKLIST_H
