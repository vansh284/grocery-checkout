#include "paymentpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>

PaymentPage::PaymentPage(CartData *cart, QWidget *parent)
    : Page(cart,
           Style::BTN_YELLOW,
           Style::BG_YELLOW,
           parent)
{
    m_contentLayout->addSpacing(40);

    // Title
    QLabel *title = new QLabel("Select Payment Option");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("Arial", 36, QFont::Bold));
    title->setStyleSheet("color: #111111;");
    m_contentLayout->addWidget(title);
    
    m_contentLayout->addSpacing(48);

    // Payment option cards (Lambda)
    auto makePaymentCard = [&](const QString &emoji, const QString &label) -> QPushButton * {
        QPushButton *card = new QPushButton();
        card->setFixedSize(280, 280);
        card->setCursor(Qt::PointingHandCursor);
        card->setStyleSheet(R"(
            QPushButton {
                background-color: #FFF0A0;
                border: 2px solid #DDCC00;
                border-radius: 20px;
            }
            QPushButton:hover  { background-color: #F5E87A; border-color: #B8A800; }
            QPushButton:pressed{ background-color: #EBD84A; }
        )");

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setAlignment(Qt::AlignCenter);
        cardLayout->setSpacing(12);

        QLabel *emojiLabel = new QLabel(emoji);
        emojiLabel->setAlignment(Qt::AlignCenter);
        emojiLabel->setFont(QFont("Arial", 72));
        emojiLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

        QLabel *textLabel = new QLabel(label);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setFont(QFont("Arial", 26));
        textLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

        cardLayout->addWidget(emojiLabel);
        cardLayout->addWidget(textLabel);

        return card;
    };

    QPushButton *cashCard = makePaymentCard("💵", "Cash");
    QPushButton *cardCard = makePaymentCard("💳", "Card");

    connect(cashCard, &QPushButton::clicked, this, &PaymentPage::cashSelected);
    connect(cardCard, &QPushButton::clicked, this, &PaymentPage::cardSelected);

    QHBoxLayout *cardsRow = new QHBoxLayout();
    cardsRow->setAlignment(Qt::AlignCenter);
    cardsRow->setSpacing(64);
    cardsRow->addWidget(cashCard);
    cardsRow->addWidget(cardCard);

    m_contentLayout->addLayout(cardsRow);
    m_contentLayout->addStretch();
}

