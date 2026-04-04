#include "mainwindow.h"
#include "welcomepage.h"
#include "mainpage.h"
#include "paymentpage.h"
#include "receiptpage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Self-Checkout System");
    resize(1280, 768);

    // Initialize cart data -- the shared data model.
    m_cartData = new CartData();

    // Initialize StackedWidget
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    // Build Pages
    m_welcomePage = new WelcomePage(m_cartData, this);
    m_mainPage    = new MainPage(m_cartData, this);
    m_paymentPage = new PaymentPage(m_cartData, this);
    m_receiptPage = new ReceiptPage(m_cartData, this);

    m_stack->insertWidget(PAGE_WELCOME, m_welcomePage);
    m_stack->insertWidget(PAGE_MAIN, m_mainPage);
    m_stack->insertWidget(PAGE_PAYMENT, m_paymentPage);
    m_stack->insertWidget(PAGE_RECEIPT, m_receiptPage);


    connect(m_welcomePage, &WelcomePage::languageSelected,
            this, &MainWindow::onLanguageSelected);

    // Back button
    connect(m_mainPage,     &Page::backClicked, this, &MainWindow::onBackClicked);
    connect(m_paymentPage,  &Page::backClicked, this, &MainWindow::onBackClicked);
    connect(m_receiptPage,  &Page::backClicked, this, &MainWindow::onBackClicked);

    connect(m_mainPage, &MainPage::checkoutConfirmed, this, [this]() {
        navigateTo(PAGE_PAYMENT);
    });

    connect(m_paymentPage, &PaymentPage::cashSelected, this, [this]() {
        navigateTo(PAGE_RECEIPT);
    });

    connect(m_paymentPage, &PaymentPage::cardSelected, this, [this]() {
        navigateTo(PAGE_RECEIPT);
    });

    connect(m_receiptPage, &ReceiptPage::finished, this, [this]() {
        resetToWelcome();
    });

    // Start at Welcome Page
    m_stack->setCurrentIndex(PAGE_WELCOME);
}

MainWindow::~MainWindow()
{
}


void MainWindow::onLanguageSelected(const QString &langCode)
{
    m_cartData->clear();

    m_mainPage->updateLanguageFlag(langCode);
    m_paymentPage->updateLanguageFlag(langCode);
    m_receiptPage->updateLanguageFlag(langCode);
    m_mainPage->refreshUI();
    navigateTo(PAGE_MAIN);
}

void MainWindow::navigateTo(int pageIndex)
    {
        m_pageHistory.push(m_stack->currentIndex());
        m_stack->setCurrentIndex(pageIndex);
    }

void MainWindow::onBackClicked()
{
    if (!m_pageHistory.isEmpty()) {
        int previousPage = m_pageHistory.pop(); // pop out (and delete) the last history page.
        m_stack->setCurrentIndex(previousPage);
    }
}

