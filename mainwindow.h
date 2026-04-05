#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QStack>
#include "cartdata.h"
#include "categorypage.h"

class WelcomePage;
class MainPage;
class PaymentPage;
class ReceiptPage;
class AdminPage;
class CategoryPage;
class ItemDetailPage;
class HelpPage;

enum PageIndex {
    PAGE_WELCOME     = 0,
    PAGE_MAIN        = 1,
    PAGE_PAYMENT     = 2,
    PAGE_RECEIPT     = 3,
    PAGE_ADMIN       = 4,
    PAGE_CATEGORY    = 5,
    PAGE_ITEM_DETAIL = 6,
    PAGE_HELP        = 7
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
    QStackedWidget *m_stack = nullptr;
    CartData *m_cartData = nullptr;
    QStack<int> m_pageHistory;

    WelcomePage *m_welcomePage = nullptr;
    MainPage *m_mainPage = nullptr;
    PaymentPage *m_paymentPage = nullptr;
    ReceiptPage *m_receiptPage = nullptr;
    AdminPage *m_adminPage = nullptr;
    CategoryPage *m_categoryPage = nullptr;
    ItemDetailPage *m_itemDetailPage = nullptr;
    HelpPage *m_helpPage = nullptr;

    QString m_currentCategory;
    QVector<CategoryPage::WeightedItem> m_currentCategoryItems;
    double m_currentWeightKg = 0.0;

    void navigateTo(int pageIndex);
    void resetToWelcome();
};

#endif // MAINWINDOW_H

