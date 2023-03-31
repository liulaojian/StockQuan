QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#CONFIG += console
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Framework/arithmetic/KLineDataBean.cpp \
    Framework/data/StockData.cpp \
    Framework/data/StockDataLoadRunable.cpp \
    Framework/data/StockDataMgr.cpp \
    Framework/data/StockTdxFileData.cpp \
    Ui/StockChart.cpp \
    dlgstocklist.cpp \
    main.cpp \
    mainwindow.cpp \
    splash.cpp

HEADERS += \
    Framework/arithmetic/KLineDataBean.h \
    Framework/data/StockData.h \
    Framework/data/StockDataLoadRunable.h \
    Framework/data/StockDataMgr.h \
    Framework/data/StockDataStruct.h \
    Framework/data/StockTdxFileData.h \
    Ui/StockChart.h \
    dlgstocklist.h \
    mainwindow.h \
    splash.h

FORMS += \
    dlgstocklist.ui \
    mainwindow.ui \
    splash.ui

INCLUDEPATH+=Ui Framework Framework/utils  Framework/function  Framework/data   Framework/arithmetic

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
