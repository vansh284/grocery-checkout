#include "receiptpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QFont>

ReceiptPage::ReceiptPage(CartData *cart, QWidget *parent)
    : Page(cart,
           Style::BTN_YELLOW,
           Style::BG_YELLOW,
           false, false, true,
           parent)
{
    m_contentLayout->addStretch();

    // title
    QLabel *title = new QLabel("Payment Completed!");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("Arial", 48, QFont::Bold));
    title->setStyleSheet("color: #111111;");
    m_contentLayout->addWidget(title);

    m_contentLayout->addSpacing(48);

    // Print receipt?
    QFrame *card = new QFrame();
    card->setObjectName("receiptCard");
    card->setFixedSize(520, 240);
    card->setStyleSheet(R"(
        QFrame#receiptCard {
            background-color: white;
            border: 3px solid #111111;
            border-radius: 24px;
        }
    )");

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(40, 32, 40, 32);
    cardLayout->setSpacing(32);

    QLabel *prompt = new QLabel("Print Receipt?");
    prompt->setAlignment(Qt::AlignCenter);
    prompt->setFont(QFont("Arial", 28));
    prompt->setStyleSheet("color: #111111; background: transparent; border: none;");
    cardLayout->addWidget(prompt);

    // Buttons
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->setAlignment(Qt::AlignCenter);
    btnRow->setSpacing(40);

    auto makeDialogBtn = [&](const QString &text, const QString &textColor) -> QPushButton * {
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

    QPushButton *noBtn  = makeDialogBtn("No",  "#E53935");
    QPushButton *yesBtn = makeDialogBtn("Yes", "#43A047");

    connect(noBtn, &QPushButton::clicked, this, &ReceiptPage::finished);
    connect(yesBtn, &QPushButton::clicked, this, &ReceiptPage::finished);

    btnRow->addWidget(noBtn);
    btnRow->addWidget(yesBtn);
    cardLayout->addLayout(btnRow);

    // Center the card
    QHBoxLayout *cardRow = new QHBoxLayout();
    cardRow->setAlignment(Qt::AlignCenter);
    cardRow->addWidget(card);
    m_contentLayout->addLayout(cardRow);

    m_contentLayout->addStretch();
}
