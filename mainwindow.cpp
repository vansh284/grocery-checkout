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
#include <QMessageBox>
#include <QScrollArea>

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
    root->setContentsMargins(0,0,0,0);
    root->setSpacing(0);

    // ── TOP BAR ─────────────────────────────────────
    root->addWidget(makeTopBar(true, true, true, "scanFlagLabel"));
    root->addSpacing(16);

    // ── MAIN CONTENT ────────────────────────────────
    QHBoxLayout *content = new QHBoxLayout();
    content->setSpacing(20);
    content->setContentsMargins(20, 0, 20, 20);

    // LEFT SIDE: Scanned Ites
    QVBoxLayout *left = new QVBoxLayout();
    left->setSpacing(12);

    QLabel *listTitle = new QLabel("Scanned Items");
    listTitle->setFont(QFont("Arial", 24, QFont::Bold));
    left->addWidget(listTitle);

    // Scroll area for items
    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget();
    m_itemsList = new QVBoxLayout(scrollWidget);
    m_itemsList->setSpacing(2);
    m_itemsList->setAlignment(Qt::AlignTop); // keep items at top in order
    scrollWidget->setLayout(m_itemsList);
    scroll->setWidget(scrollWidget);

    left->addWidget(scroll, 1);

    // Subtotal
    QFrame *subtotalFrame = new QFrame();
    //subtotalFrame->setStyleSheet("background-color: #F5C800; border-radius: 12px;");
    subtotalFrame->setFixedHeight(80);
    QHBoxLayout *subtotalLayout = new QHBoxLayout(subtotalFrame);
    subtotalLayout->setContentsMargins(16, 0, 16, 0);

    QLabel *subtotalText = new QLabel("Subtotal:");
    subtotalText->setFont(QFont("Arial", 24, QFont::Bold));

    QLabel *subtotalPrice = new QLabel("€0.00");
    subtotalPrice->setFont(QFont("Arial", 24, QFont::Bold));
    subtotalPrice->setAlignment(Qt::AlignCenter);
    subtotalPrice->setStyleSheet("background-color: white; border-radius: 8px; padding: 8px; min-width:120px;");

    subtotalLayout->addWidget(subtotalText);
    subtotalLayout->addStretch();
    subtotalLayout->addWidget(subtotalPrice);

    left->addWidget(subtotalFrame);
    m_subtotalLabel = subtotalPrice;
    m_subtotalLabel->setText("");

    // PAY button
    QPushButton *payBtn = new QPushButton("PAY");
    payBtn->setFixedHeight(80);
    payBtn->setFont(QFont("Arial", 28, QFont::Bold));
    payBtn->setStyleSheet("background-color: #43A047; color: white; border-radius: 16px;");
    connect(payBtn, &QPushButton::clicked, this, &MainWindow::onPayClicked);

    left->addWidget(payBtn);

    QWidget *leftWidget = new QWidget();
    leftWidget->setLayout(left);
    leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // RIGHT SIDE: Add Items
    QVBoxLayout *right = new QVBoxLayout();
    right->setSpacing(24);

    QLabel *addTitle = new QLabel("Add Items");
    addTitle->setFont(QFont("Arial", 24, QFont::Bold));
    right->addWidget(addTitle);

    auto makeItemBtn = [&](QString text, QString emoji){
        QPushButton *btn = new QPushButton();
        btn->setFixedHeight(100);
        btn->setFont(QFont("Arial", 22, QFont::Bold));
        btn->setStyleSheet(R"(
            QPushButton {
                background-color: #F5C800;
                border: none;
                border-radius: 20px;
                padding-left: 24px;
            }
            QPushButton:hover { background-color: #F7DE50; }
            QPushButton:pressed { background-color: #EFC900; }
        )");

        // Horizontal layout inside button
        QHBoxLayout *layout = new QHBoxLayout(btn);
        layout->setContentsMargins(16,0,16,0);
        layout->setSpacing(16);
        layout->setAlignment(Qt::AlignCenter); // vertically center

        QLabel *emojiLabel = new QLabel(emoji);
        emojiLabel->setFont(QFont("Arial", 48));
        emojiLabel->setAlignment(Qt::AlignCenter);

        QLabel *textLabel = new QLabel(text);
        textLabel->setFont(QFont("Arial", 22, QFont::Bold));
        textLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        layout->addWidget(emojiLabel);
        layout->addWidget(textLabel);
        layout->addStretch();

        return btn;
    };

    QPushButton *breadBtn = makeItemBtn("Bread", "🍞");
    QPushButton *vegBtn   = makeItemBtn("Vegetables", "🥦");
    QPushButton *fruitBtn = makeItemBtn("Fruits", "🍎");

    connect(breadBtn, &QPushButton::clicked, this, [=](){ addItem("Bread", 1, 1.50); });
    connect(vegBtn, &QPushButton::clicked, this, [=](){ addItem("Vegetables", 1, 2.00); });
    connect(fruitBtn, &QPushButton::clicked, this, [=](){ addItem("Fruits", 1, 1.75); });

    right->addWidget(breadBtn);
    right->addWidget(vegBtn);
    right->addWidget(fruitBtn);
    right->addStretch();

    QWidget *rightWidget = new QWidget();
    rightWidget->setLayout(right);
    rightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    content->addWidget(leftWidget, 1);
    content->addWidget(rightWidget, 1);

    root->addLayout(content);

    return page;
}

void MainWindow::addItem(const QString &name, int qty, double price)
{
    double total = qty * price;
    m_total += total;

    QWidget *itemWidget = new QWidget();
    itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);
    itemLayout->setContentsMargins(12, 6, 12, 6);

    QLabel *qtyLabel = new QLabel(QString::number(qty));
    qtyLabel->setFont(QFont("Arial", 18));
    qtyLabel->setFixedWidth(40);

    QLabel *nameLabel = new QLabel(name);
    nameLabel->setFont(QFont("Arial", 18));

    QLabel *priceLabel = new QLabel("€" + QString::number(total, 'f', 2));
    priceLabel->setFont(QFont("Arial", 18));
    priceLabel->setFixedWidth(100);
    priceLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    itemLayout->addWidget(qtyLabel);
    itemLayout->addWidget(nameLabel, 1);
    itemLayout->addWidget(priceLabel);

    // Alternate background color
    if (m_itemsList->count() % 2 == 0)
        itemWidget->setStyleSheet("background-color: #FFFFFF; border-radius:8px;");
    else
        itemWidget->setStyleSheet("background-color: #F8F8F8; border-radius:8px;");

    m_itemsList->addWidget(itemWidget);

    // Update subtotal price
    if (m_total > 0.0)
        m_subtotalLabel->setText("€" + QString::number(m_total, 'f', 2));
    else
        m_subtotalLabel->setText("");
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
    // Clear previous items & subtotal
    QLayoutItem *child;
    while ((child = m_itemsList->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    m_total = 0.0;
    m_subtotalLabel->setText("Subtotal: €0.00");

    goToMain();
}

void MainWindow::onBackClicked()
{
    if (!m_pageHistory.isEmpty()) {
        int previousPage = m_pageHistory.pop(); // Gets the last page AND removes it from the stack
        m_stack->setCurrentIndex(previousPage);
    }
}


void MainWindow::showBagDialog()
{
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    dialog.setModal(true);
    dialog.setAttribute(Qt::WA_TranslucentBackground);
    dialog.setFixedSize(900, 400); // wider window

    QVBoxLayout *root = new QVBoxLayout(&dialog);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Card frame
    QFrame *card = new QFrame();
    card->setObjectName("bagCard");
    card->setStyleSheet(R"(
        QFrame#bagCard {
            background-color: white;
            border: 3px solid #111111;
            border-radius: 24px;
        }
    )");
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(40, 32, 40, 32);
    cardLayout->setSpacing(24);

    // Title
    QLabel *title = new QLabel("Need bags?");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("Arial", 28, QFont::Bold));
    title->setStyleSheet("color: #111111;");
    cardLayout->addWidget(title);

    // Bag selection
    struct Bag {
        QString name;
        double price;
        int qty;
    };

    QVector<Bag> bags = {
        {"S 🛍️", 0.10, 0},
        {"M 🛍️", 0.20, 0},
        {"L 🛍️", 0.30, 0}
    };

    QHBoxLayout *bagsRow = new QHBoxLayout();
    bagsRow->setSpacing(40);
    bagsRow->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < bags.size(); ++i) {
        QVBoxLayout *bagCol = new QVBoxLayout();
        bagCol->setAlignment(Qt::AlignCenter);
        bagCol->setSpacing(12);

        QLabel *nameLabel = new QLabel(bags[i].name + " €" + QString::number(bags[i].price, 'f', 2));
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setFont(QFont("Arial", 18, QFont::Bold));

        QLabel *qtyLabel = new QLabel("0");
        qtyLabel->setAlignment(Qt::AlignCenter);
        qtyLabel->setFont(QFont("Arial", 18));

        QHBoxLayout *btnRow = new QHBoxLayout();
        btnRow->setSpacing(8);

        QPushButton *minus = new QPushButton("-");
        minus->setFixedSize(50, 50);
        minus->setFont(QFont("Arial", 20));
        QPushButton *plus  = new QPushButton("+");
        plus->setFixedSize(50, 50);
        plus->setFont(QFont("Arial", 20));

        btnRow->addWidget(minus);
        btnRow->addWidget(qtyLabel);
        btnRow->addWidget(plus);

        bagCol->addWidget(nameLabel);
        bagCol->addLayout(btnRow);

        bagsRow->addLayout(bagCol);

        // Connections
        connect(plus, &QPushButton::clicked, this, [=, &bags]() mutable {
            bags[i].qty++;
            qtyLabel->setText(QString::number(bags[i].qty));
        });
        connect(minus, &QPushButton::clicked, this, [=, &bags]() mutable {
            if (bags[i].qty > 0) {
                bags[i].qty--;
                qtyLabel->setText(QString::number(bags[i].qty));
            }
        });
    }

    cardLayout->addLayout(bagsRow);

    // Buttons row
    QHBoxLayout *actionRow = new QHBoxLayout();
    actionRow->setSpacing(40);
    actionRow->setAlignment(Qt::AlignCenter);

    QPushButton *payBtn = new QPushButton("PAY");
    payBtn->setFixedSize(180, 60);
    payBtn->setFont(QFont("Arial", 22));
    payBtn->setStyleSheet("background:white; color:#43A047; border:2px solid #43A047; border-radius:14px;");

    QPushButton *returnBtn = new QPushButton("Return");
    returnBtn->setFixedSize(180, 60);
    returnBtn->setFont(QFont("Arial", 22));
    returnBtn->setStyleSheet("background:white; color:#E53935; border:2px solid #E53935; border-radius:14px;");

    connect(payBtn, &QPushButton::clicked, this, [&]() {
        for (const auto &b : bags)
            if (b.qty > 0) addItem(b.name, b.qty, b.price);
        dialog.accept();
        goToPayment();
    });

    connect(returnBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    actionRow->addWidget(returnBtn);
    actionRow->addWidget(payBtn);


    cardLayout->addLayout(actionRow);

    QHBoxLayout *cardRow = new QHBoxLayout();
    cardRow->setAlignment(Qt::AlignCenter);
    cardRow->addWidget(card);
    root->addLayout(cardRow);

    dialog.exec();
}

void MainWindow::onPayClicked()
{
    // Frameless dialog with custom shape
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    dialog.setModal(true);
    dialog.setAttribute(Qt::WA_TranslucentBackground); // allows rounded corners
    dialog.setFixedSize(520, 240);

    QVBoxLayout *root = new QVBoxLayout(&dialog);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Card frame is the entire dialog
    QFrame *card = new QFrame();
    card->setObjectName("scanConfirmCard");
    card->setStyleSheet(R"(
        QFrame#scanConfirmCard {
            background-color: white;
            border: 3px solid #111111;
            border-radius: 24px;
        }
    )");
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(40, 32, 40, 32);
    cardLayout->setSpacing(32);

    // Title
    QLabel *title = new QLabel("Did you scan all your items?");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("Arial", 28, QFont::Bold));
    title->setStyleSheet("color: #111111; background: transparent; border: none;");
    cardLayout->addWidget(title);

    // Buttons row
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->setAlignment(Qt::AlignCenter);
    btnRow->setSpacing(40);

    auto makeDialogBtn = [&](const QString &text, const QString &color) -> QPushButton* {
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
        )").arg(color));
        return btn;
    };

    QPushButton *yesBtn = makeDialogBtn("Yes", "#43A047");
    QPushButton *noBtn  = makeDialogBtn("No", "#E53935");

    connect(yesBtn, &QPushButton::clicked, &dialog, [this, &dialog]() {
        dialog.accept();
        showBagDialog();
    });

    connect(noBtn, &QPushButton::clicked, &dialog, [&dialog]() {
        dialog.accept(); // just close dialog
    });

    btnRow->addWidget(noBtn);
    btnRow->addWidget(yesBtn);
    cardLayout->addLayout(btnRow);

    root->addWidget(card);
    dialog.exec();
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
