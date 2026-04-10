#include "helppage.h"
#include "style.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QFont>

HelpPage::HelpPage(CartData *cart, QWidget *parent)
    : Page(cart,
           Style::BTN_YELLOW,
           Style::BG_YELLOW,
           true,
           false,
           true,
           parent)
{
    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(0);

    m_contentLayout->addStretch();

    QFrame *card = new QFrame();
    card->setFixedSize(650, 270);
    card->setStyleSheet(R"(
        QFrame {
            background-color: transparent;
            border: 6px solid #111111;
            border-radius: 55px;
        }
    )");

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(30, 26, 30, 26);
    cardLayout->setSpacing(16);

    QLabel *text = new QLabel("Don’t worry ! Our\nstaff is coming");
    text->setAlignment(Qt::AlignCenter);
    text->setFont(QFont("Arial", 48));
    text->setStyleSheet("color:#111111; border:none; background:transparent;");

    QLabel *dots = new QLabel("......");
    dots->setAlignment(Qt::AlignCenter);
    dots->setFont(QFont("Arial", 26, QFont::Bold));
    dots->setStyleSheet("color:#111111; border:none; background:transparent;");

    cardLayout->addStretch();
    cardLayout->addWidget(text);
    cardLayout->addWidget(dots);
    cardLayout->addStretch();

    QHBoxLayout *cardRow = new QHBoxLayout();
    cardRow->setAlignment(Qt::AlignCenter);
    cardRow->addWidget(card);

    m_contentLayout->addLayout(cardRow);
    m_contentLayout->addStretch();

    QHBoxLayout *bottomRow = new QHBoxLayout();
    bottomRow->setContentsMargins(0, 0, 36, 24);
    bottomRow->addStretch();

    QLabel *illustration = new QLabel("🧒 🛒 👩");
    illustration->setFont(QFont("Arial", 42));
    illustration->setStyleSheet("background: transparent; color:#111111;");
    bottomRow->addWidget(illustration);

    m_contentLayout->addLayout(bottomRow);
}

