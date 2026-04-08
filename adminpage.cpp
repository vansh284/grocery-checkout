#include "adminpage.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QFont>
#include <QScrollArea>
#include <QDialog>

AdminPage::AdminPage(CartData *cart, QWidget *parent)
    : Page(cart,
           Style::BTN_BLUE_1,
           Style::BG_BLUE,
           false, false, false,
           parent)
{
    QHBoxLayout *content = new QHBoxLayout();
    content->setSpacing(20);
    // No need to set margins here. It has been set in the parent Page class for m_contentLayout.

    // --- LEFT SIDE: Scanned Items ---
    QVBoxLayout *left = new QVBoxLayout();
    left->setSpacing(12);

    QLabel *listTitle = new QLabel("Scanned Items");
    listTitle->setFont(QFont("Arial", 24, QFont::Bold));
    left->addWidget(listTitle);

    // Scroll area for items
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

    // Subtotal
    QFrame *subtotalFrame = new QFrame();
    subtotalFrame->setFixedHeight(80);
    QHBoxLayout *subtotalLayout = new QHBoxLayout(subtotalFrame);
    subtotalLayout->setContentsMargins(16, 0, 16, 0);

    QLabel *subtotalText = new QLabel("Subtotal:");
    subtotalText->setFont(QFont("Arial", 24, QFont::Bold));
    m_subtotalLabel = new QLabel("€0.00");
    m_subtotalLabel->setFont(QFont("Arial", 24, QFont::Bold));
    m_subtotalLabel->setAlignment(Qt::AlignCenter);
    m_subtotalLabel->setStyleSheet("background-color: white; border-radius: 8px; padding: 8px; min-width:120px;");

    subtotalLayout->addWidget(subtotalText);
    subtotalLayout->addStretch();
    subtotalLayout->addWidget(m_subtotalLabel);
    left->addWidget(subtotalFrame);

    // Pay Button removed.


    QWidget *leftWidget = new QWidget();
    leftWidget->setLayout(left);
    leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    // --- RIGHT SIDE: Add Items ---
    QVBoxLayout *right = new QVBoxLayout();
    right->setSpacing(24);

    QLabel *addTitle = new QLabel("Add Items");
    addTitle->setFont(QFont("Arial", 24, QFont::Bold));
    right->addWidget(addTitle);

    // A Lambda that helps create buttons for adding items.
    auto makeMenuBtn = [&](QString text){
        QPushButton *btn = new QPushButton();
        btn->setFixedHeight(100);
        btn->setStyleSheet(Style::itemSheet_2);
        btn->setCursor(Qt::PointingHandCursor);

        QHBoxLayout *layout = new QHBoxLayout(btn);
        layout->setContentsMargins(16,0,16,0);
        layout->setSpacing(16);

        QLabel *textLabel = new QLabel(text);
        textLabel->setFont(QFont("Arial", 22, QFont::Bold));

        layout->addWidget(textLabel);
        layout->addStretch();
        return btn;
    };

    QPushButton *addPrice = makeMenuBtn("Manual Price");
    QPushButton *addCode   = makeMenuBtn("Manual Barcode");

    connect(addCode, &QPushButton::clicked, this, [this]() {
        NumpadDialog dialog(NumpadDialog::BarcodeMode, this);
        // dialog.exec() will stop the window, till the tick is pressed. QDialog::Accepted
        if (dialog.exec() == QDialog::Accepted) {
            QString inputCode = dialog.getBarcode();
            // EXAMPLE: name=barcode, quantity=1, price=1
            CartItem newItem;
            newItem.name = inputCode;
            newItem.qty = 1;
            newItem.price = 1.00;
            // refresh cart
            m_cart->items.append(newItem);
            m_cart->total += newItem.price;

            loadCart();
        }
    });

    connect(addPrice, &QPushButton::clicked, this, [this]() {
        NumpadDialog dialog(NumpadDialog::PriceMode, this);
        if (dialog.exec() == QDialog::Accepted) {
            CartItem newItem;
            newItem.name = "Manual Item";
            newItem.qty = 1;
            newItem.price = dialog.getPrice();
            // refresh cart
            m_cart->items.append(newItem);
            m_cart->total += newItem.price;

            loadCart();
        }
    });


    // LOG OUT Button
    QPushButton *logoutBtn = new QPushButton("    LOG OUT");
    logoutBtn->setFixedHeight(100);
    logoutBtn->setFont(QFont("Arial", 28, QFont::Bold));
    logoutBtn->setCursor(Qt::PointingHandCursor);
    logoutBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #7B9EE5;
            color: black;
            border-radius: 16px;
        }
        QPushButton:hover { background-color: #6A8DD4; }
    )");

    connect(logoutBtn, &QPushButton::clicked, this, &AdminPage::logoutClicked);

    right->addWidget(addPrice);
    right->addWidget(addCode);
    right->addWidget(logoutBtn);

    right->addStretch();

    QWidget *rightWidget = new QWidget();
    rightWidget->setLayout(right);


    // Combine left and right
    content->addWidget(leftWidget, 1);
    content->addWidget(rightWidget, 1);

    // IMPORTANT: in the end, put all content inside m_contentLayout.
    m_contentLayout->addLayout(content);
}

void AdminPage::loadCart()
{
    // 1. Clear existing UI items safely
    QLayoutItem *child;
    while ((child = m_itemsList->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->hide();
            child->widget()->deleteLater();
        }
        delete child;
    }

    // 2. Re-populate the list directly from the shared m_cart data
    for (int i = 0; i < m_cart->items.size(); ++i) {
        const CartItem &item = m_cart->items[i];
        addAdminItemUI(item.name, item.qty, item.price, i);
    }

    // 3. Update the subtotal
    m_subtotalLabel->setText("€" + QString::number(m_cart->total, 'f', 2));
}

void AdminPage::addAdminItemUI(const QString &name, int qty, double price, int index)
{
    double itemTotal = qty * price;

    QWidget *itemWidget = new QWidget();
    QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);
    itemLayout->setContentsMargins(12, 6, 12, 6);

    // The Red 'X' Delete Button (Functionality can be added later)
    QPushButton *deleteBtn = new QPushButton("✖");
    deleteBtn->setFixedSize(32, 32);
    deleteBtn->setFont(QFont("Arial", 16, QFont::Bold));
    deleteBtn->setStyleSheet("background-color: #E53935; color: white; border-radius: 16px;");
    deleteBtn->setCursor(Qt::PointingHandCursor);

    connect(deleteBtn, &QPushButton::clicked, this, [this, index]() {
        // 1. 安全检查，防止索引越界
        if (index < 0 || index >= m_cart->items.size()) return;

        // 2. 数量减 1
        m_cart->items[index].qty--;

        // 3. 如果数量减到 0，直接从列表中移除该项
        if (m_cart->items[index].qty <= 0) {
            m_cart->items.removeAt(index);
        }

        // 4. 重新计算购物车总价 (避免浮点数精度丢失导致奇怪的小数)
        m_cart->total = 0.0;
        for (const auto &item : std::as_const(m_cart->items)) {
            m_cart->total += (item.price * item.qty);
        }

        // 5. 重新加载 UI，这会清空当前列表并根据更新后的 m_cart 重新生成
        loadCart();
    });

    QLabel *qtyLabel = new QLabel(QString::number(qty));
    qtyLabel->setFont(QFont("Arial", 18));
    qtyLabel->setFixedWidth(60);
    qtyLabel->setAlignment(Qt::AlignCenter);

    QLabel *nameLabel = new QLabel(name);
    nameLabel->setFont(QFont("Arial", 18));

    QLabel *priceLabel = new QLabel("€" + QString::number(itemTotal, 'f', 2));
    priceLabel->setFont(QFont("Arial", 18));
    priceLabel->setFixedWidth(100);
    priceLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    itemLayout->addWidget(deleteBtn);
    itemLayout->addSpacing(16);
    itemLayout->addWidget(qtyLabel);
    itemLayout->addWidget(nameLabel, 1);
    itemLayout->addWidget(priceLabel);

    if (index % 2 == 0)
        itemWidget->setStyleSheet("background-color: #FFFFFF; border-radius:8px;");
    else
        itemWidget->setStyleSheet("background-color: #E6E6E6; border-radius:8px;");

    m_itemsList->addWidget(itemWidget);
}
