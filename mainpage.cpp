#include "mainpage.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QFont>
#include <QScrollArea>
#include <QDialog>

MainPage::MainPage(CartData *cart, QWidget *parent)
    : Page(cart,
           Style::BTN_YELLOW,
           Style::BG_YELLOW,
           false,
           true,
           true,
           parent)
{
    QHBoxLayout *content = new QHBoxLayout();
    content->setSpacing(20);

    // --- LEFT SIDE: Scanned Items ---
    QVBoxLayout *left = new QVBoxLayout();
    left->setSpacing(12);

    QLabel *listTitle = new QLabel("Scanned Items");
    listTitle->setFont(QFont("Arial", 24, QFont::Bold));
    listTitle->setStyleSheet("color: #111111;");
    left->addWidget(listTitle);

    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea { border: none; background: transparent; }");

    QWidget *scrollWidget = new QWidget();
    scrollWidget->setStyleSheet("background: transparent;");
    m_itemsList = new QVBoxLayout(scrollWidget);
    m_itemsList->setSpacing(2);
    m_itemsList->setAlignment(Qt::AlignTop);
    scroll->setWidget(scrollWidget);
    left->addWidget(scroll, 1);

    QFrame *subtotalFrame = new QFrame();
    subtotalFrame->setFixedHeight(80);
    QHBoxLayout *subtotalLayout = new QHBoxLayout(subtotalFrame);
    subtotalLayout->setContentsMargins(16, 0, 16, 0);

    QLabel *subtotalText = new QLabel("Subtotal:");
    subtotalText->setFont(QFont("Arial", 24, QFont::Bold));
    subtotalText->setStyleSheet("color: #111111;");

    m_subtotalLabel = new QLabel("€0.00");
    m_subtotalLabel->setFont(QFont("Arial", 24, QFont::Bold));
    m_subtotalLabel->setAlignment(Qt::AlignCenter);
    m_subtotalLabel->setStyleSheet(
        "background-color: white;"
        "color: #111111;"
        "border-radius: 8px;"
        "padding: 8px;"
        "min-width:120px;"
        );

    subtotalLayout->addWidget(subtotalText);
    subtotalLayout->addStretch();
    subtotalLayout->addWidget(m_subtotalLabel);
    left->addWidget(subtotalFrame);

    QPushButton *payBtn = new QPushButton("PAY");
    payBtn->setFixedHeight(80);
    payBtn->setFont(QFont("Arial", 28, QFont::Bold));
    payBtn->setStyleSheet(
        "background-color: #43A047;"
        "color: #111111;"
        "border-radius: 16px;"
        );
    payBtn->setCursor(Qt::PointingHandCursor);

    connect(payBtn, &QPushButton::clicked, this, &MainPage::onPayClicked);
    left->addWidget(payBtn);

    QWidget *leftWidget = new QWidget();
    leftWidget->setLayout(left);
    leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // --- RIGHT SIDE: Add Items ---
    QVBoxLayout *right = new QVBoxLayout();
    right->setSpacing(24);

    QLabel *addTitle = new QLabel("Add Weighted Items");
    addTitle->setFont(QFont("Arial", 24, QFont::Bold));
    addTitle->setStyleSheet("color: #111111;");
    right->addWidget(addTitle);

    auto makeItemBtn = [&](QString text, QString emoji) {
        QPushButton *btn = new QPushButton();
        btn->setFixedHeight(100);
        btn->setStyleSheet(Style::itemSheet_1);
        btn->setCursor(Qt::PointingHandCursor);

        QHBoxLayout *layout = new QHBoxLayout(btn);
        layout->setContentsMargins(16, 0, 16, 0);
        layout->setSpacing(16);

        QLabel *emojiLabel = new QLabel(emoji);
        emojiLabel->setFont(QFont("Arial", 48));

        QLabel *textLabel = new QLabel(text);
        textLabel->setFont(QFont("Arial", 22, QFont::Bold));
        textLabel->setStyleSheet("color: #111111;");

        layout->addWidget(emojiLabel);
        layout->addWidget(textLabel);
        layout->addStretch();
        return btn;
    };

    QPushButton *breadBtn = makeItemBtn("Bread", "🍞");
    QPushButton *vegBtn   = makeItemBtn("Vegetables", "🥦");
    QPushButton *fruitBtn = makeItemBtn("Fruits", "🍎");

    connect(breadBtn, &QPushButton::clicked, this, [this]() {
        emit categoryRequested("Bread");
    });
    connect(vegBtn, &QPushButton::clicked, this, [this]() {
        emit categoryRequested("Vegetables");
    });
    connect(fruitBtn, &QPushButton::clicked, this, [this]() {
        emit categoryRequested("Fruits");
    });

    right->addWidget(breadBtn);
    right->addWidget(vegBtn);
    right->addWidget(fruitBtn);
    right->addStretch();

    QWidget *rightWidget = new QWidget();
    rightWidget->setLayout(right);

    content->addWidget(leftWidget, 1);
    content->addWidget(rightWidget, 1);

    m_contentLayout->addLayout(content);
}

void MainPage::addItem(const QString &name, int qty, double price)
{
    CartItem newItem;
    newItem.name = name;
    newItem.qty = qty;
    newItem.price = price;
    newItem.weightKg = 0.0;
    newItem.isWeighted = false;

    m_cart->items.append(newItem);

    double itemTotal = qty * price;
    m_cart->total += itemTotal;

    QWidget *itemWidget = new QWidget();
    QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);
    itemLayout->setContentsMargins(12, 6, 12, 6);

    QLabel *qtyLabel = new QLabel(QString::number(qty));
    qtyLabel->setFont(QFont("Arial", 18));
    qtyLabel->setStyleSheet("color: #111111;");
    qtyLabel->setFixedWidth(40);

    QLabel *nameLabel = new QLabel(name);
    nameLabel->setFont(QFont("Arial", 18));
    nameLabel->setStyleSheet("color: #111111;");

    QLabel *priceLabel = new QLabel("€" + QString::number(itemTotal, 'f', 2));
    priceLabel->setFont(QFont("Arial", 18));
    priceLabel->setStyleSheet("color: #111111;");
    priceLabel->setFixedWidth(100);
    priceLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    itemLayout->addWidget(qtyLabel);
    itemLayout->addWidget(nameLabel, 1);
    itemLayout->addWidget(priceLabel);

    if (m_itemsList->count() % 2 == 0)
        itemWidget->setStyleSheet("background-color: #FFFFFF; border-radius:8px;");
    else
        itemWidget->setStyleSheet("background-color: #E6E6E6; border-radius:8px;");

    m_itemsList->addWidget(itemWidget);

    m_subtotalLabel->setText("€" + QString::number(m_cart->total, 'f', 2));
}

void MainPage::addWeightedItem(const CategoryPage::WeightedItem &item, double weightKg)
{
    const double totalPrice = weightKg * item.unitPricePerKg;

    CartItem newItem;
    newItem.name = item.name;
    newItem.qty = 1;
    newItem.price = item.unitPricePerKg;
    newItem.weightKg = weightKg;
    newItem.isWeighted = true;

    m_cart->items.append(newItem);
    m_cart->total += totalPrice;

    QWidget *itemWidget = new QWidget();
    QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);
    itemLayout->setContentsMargins(12, 6, 12, 6);

    QLabel *qtyLabel = new QLabel(QString::number(weightKg, 'f', 3) + " kg");
    qtyLabel->setFont(QFont("Arial", 18));
    qtyLabel->setStyleSheet("color: #111111;");
    qtyLabel->setFixedWidth(100);

    QLabel *nameLabel = new QLabel(item.name);
    nameLabel->setFont(QFont("Arial", 18));
    nameLabel->setStyleSheet("color: #111111;");

    QLabel *priceLabel = new QLabel("€" + QString::number(totalPrice, 'f', 2));
    priceLabel->setFont(QFont("Arial", 18));
    priceLabel->setStyleSheet("color: #111111;");
    priceLabel->setFixedWidth(100);
    priceLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    itemLayout->addWidget(qtyLabel);
    itemLayout->addWidget(nameLabel, 1);
    itemLayout->addWidget(priceLabel);

    if (m_itemsList->count() % 2 == 0)
        itemWidget->setStyleSheet("background-color: #FFFFFF; border-radius:8px;");
    else
        itemWidget->setStyleSheet("background-color: #E6E6E6; border-radius:8px;");

    m_itemsList->addWidget(itemWidget);
    m_subtotalLabel->setText("€" + QString::number(m_cart->total, 'f', 2));
}

void MainPage::showBagDialog()
{
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    dialog.setModal(true);
    dialog.setAttribute(Qt::WA_TranslucentBackground);
    dialog.setFixedSize(900, 420);

    QVBoxLayout *root = new QVBoxLayout(&dialog);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

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
    cardLayout->setContentsMargins(40, 28, 40, 28);
    cardLayout->setSpacing(22);

    QLabel *title = new QLabel("Need bags?");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("Arial", 28, QFont::Bold));
    title->setStyleSheet("color: #111111;");
    cardLayout->addWidget(title);

    struct Bag {
        QString sizeName;
        QString emoji;
        double price;
        int qty;
    };

    QVector<Bag> bags = {
        {"S", "🛍️", 0.10, 0},
        {"M", "🛍️", 0.20, 0},
        {"L", "🛍️", 0.30, 0}
    };

    QHBoxLayout *bagsRow = new QHBoxLayout();
    bagsRow->setSpacing(70);
    bagsRow->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < bags.size(); ++i) {
        QVBoxLayout *bagCol = new QVBoxLayout();
        bagCol->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        bagCol->setSpacing(8);

        QLabel *emojiLabel = new QLabel(bags[i].emoji);
        emojiLabel->setAlignment(Qt::AlignCenter);
        emojiLabel->setFont(QFont("Arial", 52));
        emojiLabel->setStyleSheet("color: #111111;");

        QLabel *infoLabel = new QLabel(
            bags[i].sizeName + "  €" + QString::number(bags[i].price, 'f', 2)
            );
        infoLabel->setAlignment(Qt::AlignCenter);
        infoLabel->setFont(QFont("Arial", 20, QFont::Bold));
        infoLabel->setStyleSheet("color: #111111;");

        QLabel *qtyLabel = new QLabel("0");
        qtyLabel->setAlignment(Qt::AlignCenter);
        qtyLabel->setFont(QFont("Arial", 22));
        qtyLabel->setStyleSheet("color: #111111;");
        qtyLabel->setFixedWidth(36);

        QHBoxLayout *btnRow = new QHBoxLayout();
        btnRow->setSpacing(14);
        btnRow->setAlignment(Qt::AlignCenter);

        QPushButton *minus = new QPushButton("-");
        minus->setFixedSize(44, 44);
        minus->setFont(QFont("Arial", 20, QFont::Bold));
        minus->setCursor(Qt::PointingHandCursor);
        minus->setStyleSheet(R"(
            QPushButton {
                background: white;
                color: #111111;
                border: none;
            }
            QPushButton:hover {
                background: #F5F5F5;
                border-radius: 10px;
            }
        )");

        QPushButton *plus = new QPushButton("+");
        plus->setFixedSize(44, 44);
        plus->setFont(QFont("Arial", 20, QFont::Bold));
        plus->setCursor(Qt::PointingHandCursor);
        plus->setStyleSheet(R"(
            QPushButton {
                background: white;
                color: #111111;
                border: none;
            }
            QPushButton:hover {
                background: #F5F5F5;
                border-radius: 10px;
            }
        )");

        btnRow->addWidget(minus);
        btnRow->addWidget(qtyLabel);
        btnRow->addWidget(plus);

        bagCol->addWidget(emojiLabel);
        bagCol->addWidget(infoLabel);
        bagCol->addSpacing(6);
        bagCol->addLayout(btnRow);

        bagsRow->addLayout(bagCol);

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

    QHBoxLayout *actionRow = new QHBoxLayout();
    actionRow->setSpacing(60);
    actionRow->setAlignment(Qt::AlignCenter);

    QPushButton *returnBtn = new QPushButton("Return");
    returnBtn->setFixedSize(180, 60);
    returnBtn->setFont(QFont("Arial", 22));
    returnBtn->setCursor(Qt::PointingHandCursor);
    returnBtn->setStyleSheet(
        "background:white;"
        "color:#E53935;"
        "border:2px solid #E53935;"
        "border-radius:14px;"
        );

    QPushButton *payBtn = new QPushButton("PAY");
    payBtn->setFixedSize(180, 60);
    payBtn->setFont(QFont("Arial", 22));
    payBtn->setCursor(Qt::PointingHandCursor);
    payBtn->setStyleSheet(
        "background:white;"
        "color:#43A047;"
        "border:2px solid #43A047;"
        "border-radius:14px;"
        );

    connect(payBtn, &QPushButton::clicked, this, [&]() {
        for (const auto &b : bags)
            if (b.qty > 0) addItem(b.sizeName + " 🛍️", b.qty, b.price);
        dialog.accept();
        emit checkoutConfirmed();
    });

    connect(returnBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    actionRow->addWidget(returnBtn);
    actionRow->addWidget(payBtn);

    cardLayout->addSpacing(8);
    cardLayout->addLayout(actionRow);

    QHBoxLayout *cardRow = new QHBoxLayout();
    cardRow->setAlignment(Qt::AlignCenter);
    cardRow->addWidget(card);
    root->addLayout(cardRow);

    dialog.exec();
}

void MainPage::onPayClicked()
{
    if (m_cart->total == 0) return;
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    dialog.setModal(true);
    dialog.setAttribute(Qt::WA_TranslucentBackground);
    dialog.setFixedSize(520, 240);

    QVBoxLayout *root = new QVBoxLayout(&dialog);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

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
    cardLayout->setContentsMargins(28, 24, 28, 24);
    cardLayout->setSpacing(24);

    QLabel *title = new QLabel("Did you scan all your items?");
    title->setAlignment(Qt::AlignCenter);
    title->setWordWrap(true);
    title->setFont(QFont("Arial", 24, QFont::Bold));
    title->setStyleSheet("color: #111111; background: transparent; border: none;");
    cardLayout->addWidget(title);

    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->setAlignment(Qt::AlignCenter);
    btnRow->setSpacing(28);

    auto makeDialogBtn = [&](const QString &text, const QString &color) -> QPushButton* {
        QPushButton *btn = new QPushButton(text);
        btn->setFixedSize(150, 60);
        btn->setFont(QFont("Arial", 22));
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(QString(R"(
            QPushButton {
                background-color: white;
                color: %1;
                border: 2px solid #111111;
                border-radius: 14px;
            }
            QPushButton:hover  { background-color: #F5F5F5; }
            QPushButton:pressed{ background-color: #EBEBEB; }
        )").arg(color));
        return btn;
    };

    QPushButton *backBtn = makeDialogBtn("Back", "#E53935");
    QPushButton *yesBtn  = makeDialogBtn("Yes", "#5B8F5A");

    connect(yesBtn, &QPushButton::clicked, &dialog, [this, &dialog]() {
        dialog.accept();
        showBagDialog();
    });

    connect(backBtn, &QPushButton::clicked, &dialog, [&dialog]() {
        dialog.accept();
    });

    btnRow->addWidget(backBtn);
    btnRow->addWidget(yesBtn);
    cardLayout->addLayout(btnRow);

    root->addWidget(card);
    dialog.exec();
}


void MainPage::refreshUI()
{
    QLayoutItem *child;
    while ((child = m_itemsList->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->hide();
            child->widget()->deleteLater();
        }
        delete child;
    }

    m_cart->clear();
    m_subtotalLabel->setText("€0.00");
}

void MainPage::loadCart()
{
    QLayoutItem *child;
    while ((child = m_itemsList->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->hide();
            child->widget()->deleteLater();
        }
        delete child;
    }

    for (int i = 0; i < m_cart->items.size(); ++i) {
        const CartItem &item = m_cart->items[i];

        QWidget *itemWidget = new QWidget();
        QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);
        itemLayout->setContentsMargins(12, 6, 12, 6);

        QLabel *qtyLabel = new QLabel();
        double itemTotal = 0.0;

        if (item.isWeighted) {
            qtyLabel->setText(QString::number(item.weightKg, 'f', 3) + " kg");
            qtyLabel->setFixedWidth(100);
            itemTotal = item.weightKg * item.price;
        } else {
            qtyLabel->setText(QString::number(item.qty));
            qtyLabel->setFixedWidth(40);
            itemTotal = item.qty * item.price;
        }
        qtyLabel->setFont(QFont("Arial", 18));
        qtyLabel->setStyleSheet("color: #111111;");

        QLabel *nameLabel = new QLabel(item.name);
        nameLabel->setFont(QFont("Arial", 18));
        nameLabel->setStyleSheet("color: #111111;");

        QLabel *priceLabel = new QLabel("€" + QString::number(itemTotal, 'f', 2));
        priceLabel->setFont(QFont("Arial", 18));
        priceLabel->setStyleSheet("color: #111111;");
        priceLabel->setFixedWidth(100);
        priceLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        itemLayout->addWidget(qtyLabel);
        itemLayout->addWidget(nameLabel, 1);
        itemLayout->addWidget(priceLabel);

        if (i % 2 == 0)
            itemWidget->setStyleSheet("background-color: #FFFFFF; border-radius:8px;");
        else
            itemWidget->setStyleSheet("background-color: #E6E6E6; border-radius:8px;");

        m_itemsList->addWidget(itemWidget);
    }

    m_subtotalLabel->setText("€" + QString::number(m_cart->total, 'f', 2));
}
