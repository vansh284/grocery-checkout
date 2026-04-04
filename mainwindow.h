#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QStack>
#include "cartdata.h"

// 前置声明，加快编译速度
class WelcomePage;
class MainPage;
class PaymentPage;
class ReceiptPage;

enum PageIndex {
    PAGE_WELCOME = 0,
    PAGE_MAIN = 1,
    PAGE_PAYMENT = 2,
    PAGE_RECEIPT = 3
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLanguageSelected(const QString &langCode);
    void onBackClicked();

private:
    QStackedWidget *m_stack;
    CartData *m_cartData;
    QStack<int> m_pageHistory;
    void navigateTo(int pageIndex);

    WelcomePage *m_welcomePage;
    MainPage *m_mainPage;
    PaymentPage *m_paymentPage;
    ReceiptPage *m_receiptPage;

    // index
    enum PageIndex {
        PAGE_WELCOME = 0,
        PAGE_MAIN = 1
    };

    void resetToWelcome()
    {
        m_pageHistory.clear();
        m_stack->setCurrentIndex(PAGE_WELCOME); // DON'T use navigateTo() here.
    }


};

#endif // MAINWINDOW_H
