#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStackedWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QSizePolicy>
#include <QSignalMapper>

// ── Shared style constants ────────────────────────────────────────────────────
namespace Style {

const QString BG_YELLOW   = "#FAFAC8";   // background
const QString BTN_YELLOW  = "#F5C800";   // accent yellow (Pay, Add item…)
const QString BTN_RED     = "#E53935";   // Help button
const QString TEXT_DARK   = "#111111";

// Page background
const QString pageSheet =
    "QWidget#pageRoot { background-color: " + BG_YELLOW + "; }";

// Flag language button – circular, large emoji
inline QString flagButtonSheet(int radius) {
    return QString(R"(
        QPushButton {
            background-color: %1;
            border: 3px solid #CCBA00;
            border-radius: %2px;
            font-size: 64px;
        }
        QPushButton:hover  { background-color: #F0EFA0; border-color: #A89800; }
        QPushButton:pressed{ background-color: #E4E374; }
    )").arg(BG_YELLOW).arg(radius);
}

// Red Help button
const QString helpSheet = R"(
    QPushButton {
        background-color: #E53935;
        color: white;
        border-radius: 10px;
        font-size: 16px;
        font-weight: bold;
        padding: 0 20px;
    }
    QPushButton:hover  { background-color: #C62828; }
    QPushButton:pressed{ background-color: #B71C1C; }
)";

}

//  Construction / Destruction
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_selectedLanguage("en")
{
    ui->setupUi(this);
    setWindowTitle("Self-Checkout");
    resize(1280, 768);

    menuBar()->hide();
    statusBar()->hide();

    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    m_welcomePage = buildWelcomePage();
    m_mainPage    = buildMainPage();
    m_paymentPage = buildPaymentPage();
    m_receiptPage = buildReceiptPage();

    m_stack->insertWidget(PAGE_WELCOME, m_welcomePage);
    m_stack->insertWidget(PAGE_MAIN, m_mainPage);
    m_stack->insertWidget(PAGE_PAYMENT, m_paymentPage);
    m_stack->insertWidget(PAGE_RECEIPT, m_receiptPage);

    m_stack->setCurrentIndex(PAGE_WELCOME);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QWidget *MainWindow::makeTopBar(bool hasBack,
                                bool hasHelp,
                                bool hasLang,
                                const QString &flagObjectName)
{
    QWidget *layout = new QWidget();
    layout->setFixedHeight(64);
    layout->setStyleSheet("background-color: #F5C800;");   // yellow accent band

    QHBoxLayout *bar = new QHBoxLayout(layout);

    if (hasBack) {
        QPushButton *backBtn = new QPushButton(tr("← Back"));
        backBtn->setFixedHeight(48);
        backBtn->setFont(QFont("Arial", 16, QFont::Bold));
        backBtn->setCursor(Qt::PointingHandCursor);
        backBtn->setStyleSheet(R"(
            QPushButton {
                background-color: white;
                border: 2px solid #CCCC00;
                border-radius: 12px;
                padding: 0 20px;
                color: #111111;
            }
            QPushButton:hover  { background-color: #F5F5DC; }
            QPushButton:pressed{ background-color: #EBEBC8; }
        )");
        connect(backBtn, &QPushButton::clicked,
                this, &MainWindow::onBackClicked);
        bar->addWidget(backBtn);
    }

    bar->addStretch();

    if (hasLang) {
        const QMap<QString,QString> flagMap = {
                                                {"en",    "🇬🇧"}, {"fr", "🇫🇷"}, {"other", "🌍"},
                                                };
        QLabel *flagLabel = new QLabel(flagMap.value(m_selectedLanguage, "🇬🇧"));
        flagLabel->setFont(QFont("Arial", 28));
        flagLabel->setStyleSheet("background: transparent;");
        if (!flagObjectName.isEmpty())
            flagLabel->setObjectName(flagObjectName);
        bar->addWidget(flagLabel);
        bar->addSpacing(12);
    }

    if (hasHelp) {
        QPushButton *helpBtn = new QPushButton(tr("  Help  "));
        helpBtn->setFixedHeight(44);
        helpBtn->setStyleSheet(Style::helpSheet);
        connect(helpBtn, &QPushButton::clicked,
                this, &MainWindow::onHelpClicked);
        bar->addWidget(helpBtn);
    }

    return layout;
}


QWidget *MainWindow::buildWelcomePage()
{
    // Root widget
    QWidget *page = new QWidget();
    page->setObjectName("pageRoot");
    page->setStyleSheet(Style::pageSheet);

    // Outer layout
    QVBoxLayout *root = new QVBoxLayout(page);
    root->setAlignment(Qt::AlignCenter);
    root->setSpacing(48);
    root->setContentsMargins(0, 0, 0, 0);

    // "WELCOME" title
    QLabel *title = new QLabel("WELCOME");
    QFont titleFont("Arial", 72, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: " + Style::TEXT_DARK + ";");
    root->addWidget(title);

    // Flag buttons row
    QHBoxLayout *flagsRow = new QHBoxLayout();
    flagsRow->setAlignment(Qt::AlignCenter);
    flagsRow->setSpacing(64);

    // { emoji, langCode, tooltip }
    const QList<std::tuple<QString,QString,QString>> flags = {
                                                                { "🇬🇧", "en",    "English"  },
                                                                { "🇫🇷", "fr",    "Français" },
                                                                { "🌍",  "other", "Other"    },
                                                                };

    for (const auto &[emoji, code, tip] : flags) {
        QPushButton *btn = makeFlagButton(emoji, code, tip);
        flagsRow->addWidget(btn);
    }

    root->addLayout(flagsRow);

    // Subtitle
    QLabel *subtitle = new QLabel("Select a language or main an item to start");
    QFont subFont("Arial", 18, QFont::Bold);
    subtitle->setFont(subFont);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("color: " + Style::TEXT_DARK + ";");
    root->addWidget(subtitle);

    return page;
}

// Creates a single circular flag / globe button and connects it to the slot.
QPushButton *MainWindow::makeFlagButton(const QString &emoji,
                                        const QString &langCode,
                                        const QString &tooltip)
{
    const int SIZE   = 160;
    const int RADIUS = SIZE / 2;

    QPushButton *btn = new QPushButton(emoji);
    btn->setToolTip(tooltip);
    btn->setFixedSize(SIZE, SIZE);
    btn->setStyleSheet(Style::flagButtonSheet(RADIUS));
    btn->setCursor(Qt::PointingHandCursor);

    // Lambda captures langCode by value and emits it to the slot.
    connect(btn, &QPushButton::clicked, this,
            [this, langCode]() { onLanguageSelected(langCode); });

    return btn;
}


//  PAGE 2
QWidget *MainWindow::buildMainPage()
{
    QWidget *page = new QWidget();
    page->setObjectName("pageRoot");
    page->setStyleSheet(Style::pageSheet);

    QVBoxLayout *root = new QVBoxLayout(page);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    root->addWidget(makeTopBar(true, true, true, "mainFlagLabel"));

    // Separator line
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("color: #CCBB00;");
    root->addWidget(line);

    // Placeholder
    QLabel *placeholder = new QLabel("[ Main page – content coming soon ]");
    root->addWidget(placeholder);


    return page;
}

QWidget *MainWindow::buildPaymentPage() {
    QWidget *page = new QWidget();
    page->setObjectName("pageRoot");
    page->setStyleSheet(Style::pageSheet);

    QVBoxLayout *root = new QVBoxLayout(page);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    root->addWidget(makeTopBar(true, true, true, "paymentFlagLabel"));
    root->addSpacing(40);

    // Title
    QLabel *title = new QLabel("Select Payment Option");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("Arial", 36, QFont::Bold));
    title->setStyleSheet("color: #111111;");
    root->addWidget(title);

    root->addSpacing(48);

    // Payment option cards row
    //  Each card is a QPushButton with an emoji icon above a label.
    //  A helper lambda builds each card so we avoid repeating the stylesheet.

    auto makePaymentCard = [&](const QString &emoji,
                   const QString &label) -> QPushButton *
    {
        QPushButton *card = new QPushButton(emoji + "\n" + label);
        card->setFixedSize(280, 280);
        card->setFont(QFont("Arial", 22));
        card->setCursor(Qt::PointingHandCursor);
        card->setStyleSheet(R"(
        QPushButton {
        background-color: #FFF0A0;
        border: 2px solid #DDCC00;
        border-radius: 20px;
        color: #111111;
        font-size: 22px;
        font-weight: normal;
        /* emoji sits above label thanks to the newline + line-height */
        qproperty-iconSize: 0px;
        }
        QPushButton:hover  { background-color: #F5E87A; border-color: #B8A800; }
        QPushButton:pressed{ background-color: #EBD84A; }
    )");
        card->setText("");

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setAlignment(Qt::AlignCenter);
        cardLayout->setSpacing(12);

        QLabel *emojiLabel = new QLabel(emoji);
        emojiLabel->setAlignment(Qt::AlignCenter);
        emojiLabel->setFont(QFont("Arial", 72));          // large icon
        emojiLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        emojiLabel->setStyleSheet("background: transparent; border: none;");

        QLabel *textLabel = new QLabel(label);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setFont(QFont("Arial", 26));
        textLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        textLabel->setStyleSheet("background: transparent; border: none;");

        cardLayout->addWidget(emojiLabel);
        cardLayout->addWidget(textLabel);

        return card;
    };

    QPushButton *cashCard = makePaymentCard("💵", "Cash");
    QPushButton *cardCard = makePaymentCard("💳", "Card");

    connect(cashCard, &QPushButton::clicked,
        this, &MainWindow::onCashSelected);
    connect(cardCard, &QPushButton::clicked,
        this, &MainWindow::onCardSelected);

    QHBoxLayout *cardsRow = new QHBoxLayout();
    cardsRow->setAlignment(Qt::AlignCenter);
    cardsRow->setSpacing(64);
    cardsRow->addWidget(cashCard);
    cardsRow->addWidget(cardCard);

    root->addLayout(cardsRow);
    root->addStretch();

    return page;
}

QWidget *MainWindow::buildReceiptPage() {
    QWidget *page = new QWidget();
    page->setObjectName("pageRoot");
    page->setStyleSheet(Style::pageSheet);

    QVBoxLayout *root = new QVBoxLayout(page);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    root->addWidget(makeTopBar(false, true, true, "receiptFlagLabel"));

    root->addStretch();

    QLabel *title = new QLabel("Payment Completed!");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("Arial", 48, QFont::Bold));
    title->setStyleSheet("color: #111111;");
    root->addWidget(title);

    root->addSpacing(48);

    QFrame *card = new QFrame();
    card->setObjectName("receiptCard");
    card->setStyleSheet(R"(
        QFrame#receiptCard {
            background-color: white;
            border: 3px solid #111111;
            border-radius: 24px;
        }
    )");
    card->setFixedSize(520, 240);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(40, 32, 40, 32);
    cardLayout->setSpacing(32);

    // Prompt text
    QLabel *prompt = new QLabel("Print Receipt?");
    prompt->setAlignment(Qt::AlignCenter);
    prompt->setFont(QFont("Arial", 28));
    prompt->setStyleSheet("color: #111111; background: transparent; border: none;");
    cardLayout->addWidget(prompt);

    // No / Yes buttons
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->setAlignment(Qt::AlignCenter);
    btnRow->setSpacing(40);

    auto makeDialogBtn = [&](const QString &text,
                             const QString &textColor) -> QPushButton *
    {
        QPushButton *btn = new QPushButton(text);
        btn->setFixedSize(150, 60);
        btn->setFont(QFont("Arial", 22));
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(QString(R"(
            QPushButton {
                background-color: white;
                color: %1;
                border: 2px solid #CCCCCC;
                border-radius: 14px;
            }
            QPushButton:hover  { background-color: #F5F5F5; }
            QPushButton:pressed{ background-color: #EBEBEB; }
        )").arg(textColor));
        return btn;
    };

    QPushButton *noBtn  = makeDialogBtn("No",  "#E53935");  // red  – prototype
    QPushButton *yesBtn = makeDialogBtn("Yes", "#43A047");  // green – prototype

    connect(noBtn, &QPushButton::clicked, this, &MainWindow::onReceiptNoSelected);

    connect(yesBtn, &QPushButton::clicked, this, &MainWindow::onReceiptYesSelected);

    btnRow->addWidget(noBtn);
    btnRow->addWidget(yesBtn);
    cardLayout->addLayout(btnRow);

    // Centre the card horizontally
    QHBoxLayout *cardRow = new QHBoxLayout();
    cardRow->setAlignment(Qt::AlignCenter);
    cardRow->addWidget(card);
    root->addLayout(cardRow);

    root->addStretch();

    return page;

}

//  Navigation
void MainWindow::navigateTo(int pageIndex)
{
    m_pageHistory.push(m_stack->currentIndex());
    m_stack->setCurrentIndex(pageIndex);
}

void MainWindow::goToMain()
{
    // Update the flag emoji shown in the main page top bar.
    const QMap<QString,QString> flagMap = {
                                            {"en",    "🇬🇧"},
                                            {"fr",    "🇫🇷"},
                                            {"other", "🌍"},
                                            };
    if (QLabel *lbl = m_mainPage->findChild<QLabel*>("scanFlagLabel"))
        lbl->setText(flagMap.value(m_selectedLanguage, "🇬🇧"));

    navigateTo(PAGE_MAIN);
}

void MainWindow::goToWelcome()
{
    m_stack->setCurrentIndex(PAGE_RECEIPT);
    navigateTo(PAGE_WELCOME);
}

void MainWindow::goToPayment()
{
    navigateTo(PAGE_PAYMENT);
}

void MainWindow::goToReceipt()
{
    navigateTo(PAGE_RECEIPT);
}

//  Slots
void MainWindow::onLanguageSelected(const QString &langCode)
{
    m_selectedLanguage = langCode;
    goToPayment();
}

void MainWindow::onBackClicked()
{
    if (!m_pageHistory.isEmpty()) {
        int previousPage = m_pageHistory.pop(); // Gets the last page AND removes it from the stack
        m_stack->setCurrentIndex(previousPage);
    }
}

void MainWindow::onHelpClicked()
{
}

void MainWindow::onCashSelected()
{
    goToReceipt();
}

void MainWindow::onCardSelected()
{
    goToReceipt();
}

void MainWindow::onReceiptNoSelected()
{
    goToWelcome();
}

void MainWindow::onReceiptYesSelected() {
    goToWelcome();
}
