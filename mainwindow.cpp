#include "mainwindow.h"

#include "welcomepage.h"
#include "mainpage.h"
#include "paymentpage.h"
#include "receiptpage.h"
#include "adminpage.h"

#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    statusBar()->hide();
    QMenu *systemMenu = menuBar()->addMenu("Simulation");


    QAction *scanAction = systemMenu->addAction("Scan");    // TODO
    QAction *weightAction = systemMenu->addAction("Weigh"); // TODO
    QAction *adminAction = systemMenu->addAction("Admin Mode");

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
    m_adminPage = new AdminPage(m_cartData, this);

    m_stack->insertWidget(PAGE_WELCOME, m_welcomePage);
    m_stack->insertWidget(PAGE_MAIN, m_mainPage);
    m_stack->insertWidget(PAGE_PAYMENT, m_paymentPage);
    m_stack->insertWidget(PAGE_RECEIPT, m_receiptPage);
    m_stack->insertWidget(PAGE_ADMIN, m_adminPage);


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

    connect(adminAction, &QAction::triggered, this, [this]() {
        int current = m_stack->currentIndex();
        // Only allow entering admin from Welcome or Main page
        if (current == PAGE_WELCOME || current == PAGE_MAIN) {
            // LOAD existing cart data into the UI right before jumping
            m_adminPage->loadCart();
            navigateTo(PAGE_ADMIN);
        }
    });

    connect(m_adminPage, &AdminPage::logoutClicked, this, [this]() {
        // Since we want to return exactly to where we came from (Welcome or Main),
        // we can simply use the Back button logic (pop history).
        // If you strictly want it to always force jump to PAGE_MAIN, you can replace
        // this with navigateTo(PAGE_MAIN);
        onBackClicked();
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

