#ifndef SPLASH_H
#define SPLASH_H

#include <QWidget>

namespace Ui {
class Splash;
}

class MainWindow;
class Splash : public QWidget
{
    Q_OBJECT

public:
    explicit Splash(QWidget *parent = nullptr);
    ~Splash();

public:
    void waitFor(MainWindow *w,int int_ms);

public slots:
    void showMainWindow();
    void solt_stock_data_process(int index,int max_index);
private:
    Ui::Splash *ui;
    QTimer *timer;
    MainWindow *mainWin=nullptr;
    int mLoadPercent=0;
};

#endif // SPLASH_H
