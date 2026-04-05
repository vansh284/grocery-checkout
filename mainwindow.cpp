#include "mainwindow.h"

#include "welcomepage.h"
#include "mainpage.h"
#include "paymentpage.h"
#include "receiptpage.h"
#include "adminpage.h"
#include "categorypage.h"
#include "itemdetailpage.h"
#include "helppage.h"

#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    statusBar()->hide();
    QMenu *systemMenu = menuBar()->addMenu("Simulation");

    QAction *scanAction = systemMenu->addAction("Scan");
    //QAction *weightAction = systemMenu->addAction("Weigh");
    QAction *adminAction = systemMenu->addAction("Admin Mode");

    Q_UNUSED(scanAction);
    //Q_UNUSED(weightAction);

    setWindowTitle("Self-Checkout System");
    resize(1280, 768);

    m_cartData = new CartData();

    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    m_welcomePage    = new WelcomePage(m_cartData, this);
    m_mainPage       = new MainPage(m_cartData, this);
    m_paymentPage    = new PaymentPage(m_cartData, this);
    m_receiptPage    = new ReceiptPage(m_cartData, this);
    m_adminPage      = new AdminPage(m_cartData, this);
    m_categoryPage   = new CategoryPage(m_cartData, this);
    m_itemDetailPage = new ItemDetailPage(m_cartData, this);
    m_helpPage       = new HelpPage(m_cartData, this);

    m_stack->insertWidget(PAGE_WELCOME,     m_welcomePage);
    m_stack->insertWidget(PAGE_MAIN,        m_mainPage);
    m_stack->insertWidget(PAGE_PAYMENT,     m_paymentPage);
    m_stack->insertWidget(PAGE_RECEIPT,     m_receiptPage);
    m_stack->insertWidget(PAGE_ADMIN,       m_adminPage);
    m_stack->insertWidget(PAGE_CATEGORY,    m_categoryPage);
    m_stack->insertWidget(PAGE_ITEM_DETAIL, m_itemDetailPage);
    m_stack->insertWidget(PAGE_HELP,        m_helpPage);

    connect(m_welcomePage, &WelcomePage::languageSelected,
            this, &MainWindow::onLanguageSelected);

    connect(m_mainPage,       &Page::backClicked, this, &MainWindow::onBackClicked);
    connect(m_paymentPage,    &Page::backClicked, this, &MainWindow::onBackClicked);
    connect(m_receiptPage,    &Page::backClicked, this, &MainWindow::onBackClicked);
    connect(m_categoryPage,   &Page::backClicked, this, &MainWindow::onBackClicked);
    connect(m_itemDetailPage, &Page::backClicked, this, &MainWindow::onBackClicked);
    connect(m_helpPage,       &Page::backClicked, this, &MainWindow::onBackClicked);

    connect(m_mainPage,       &Page::helpClicked, this, [this]() { navigateTo(PAGE_HELP); });
    connect(m_paymentPage,    &Page::helpClicked, this, [this]() { navigateTo(PAGE_HELP); });
    connect(m_receiptPage,    &Page::helpClicked, this, [this]() { navigateTo(PAGE_HELP); });
    connect(m_categoryPage,   &Page::helpClicked, this, [this]() { navigateTo(PAGE_HELP); });
    connect(m_itemDetailPage, &Page::helpClicked, this, [this]() { navigateTo(PAGE_HELP); });
    connect(m_adminPage,      &Page::helpClicked, this, [this]() { navigateTo(PAGE_HELP); });

    connect(m_mainPage, &MainPage::categoryRequested, this, [this](const QString &categoryName) {
        m_currentCategory = categoryName;
        m_currentCategoryItems = CategoryPage::itemsForCategory(categoryName);
        m_categoryPage->setCategory(m_currentCategory, m_currentCategoryItems, m_currentWeightKg);
        navigateTo(PAGE_CATEGORY);
    });

    connect(m_categoryPage, &CategoryPage::itemSelected, this, [this](const CategoryPage::WeightedItem &item) {
        if (!m_categoryPage->hasValidWeight()) {
            return;
        }

        m_currentWeightKg = m_categoryPage->currentWeightKg();
        m_itemDetailPage->setItemDetail(m_currentCategory, item, m_currentWeightKg);
        navigateTo(PAGE_ITEM_DETAIL);
    });

    connect(m_itemDetailPage, &ItemDetailPage::addItemRequested, this,
            [this](const CategoryPage::WeightedItem &item, double weightKg) {
                m_mainPage->addWeightedItem(item, weightKg);
                navigateTo(PAGE_MAIN);
            });

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
        if (m_stack->currentIndex() != PAGE_ADMIN) {
            m_adminPage->loadCart();
            navigateTo(PAGE_ADMIN);
        }
    });

    connect(m_adminPage, &AdminPage::logoutClicked, this, [this]() {
        onBackClicked();
    });

    m_stack->setCurrentIndex(PAGE_WELCOME);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onLanguageSelected(const QString &langCode)
{
    m_cartData->clear();
    m_cartData->language = langCode;

    m_mainPage->updateLanguageFlag(langCode);
    m_paymentPage->updateLanguageFlag(langCode);
    m_receiptPage->updateLanguageFlag(langCode);
    m_adminPage->updateLanguageFlag(langCode);
    m_categoryPage->updateLanguageFlag(langCode);
    m_itemDetailPage->updateLanguageFlag(langCode);
    m_helpPage->updateLanguageFlag(langCode);

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
        int previousPage = m_pageHistory.pop();
        m_stack->setCurrentIndex(previousPage);
    }
}

void MainWindow::resetToWelcome()
{
    m_pageHistory.clear();
    m_stack->setCurrentIndex(PAGE_WELCOME);
}

