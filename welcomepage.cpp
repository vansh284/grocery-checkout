#include "welcomepage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include "style.h"

WelcomePage::WelcomePage(CartData *cart, QWidget *parent) 
    : QWidget(parent), m_cart(cart)
{
    setObjectName("pageRoot");
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(Style::pageSheet);

    // 将 this 作为父组件，取代原代码中的 new QWidget()
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setAlignment(Qt::AlignCenter);
    root->setSpacing(48);
    root->setContentsMargins(0, 0, 0, 0);

    QLabel *title = new QLabel("WELCOME");
    title->setFont(QFont("Arial", 72, QFont::Bold));
    title->setAlignment(Qt::AlignCenter);
    root->addWidget(title);

    QHBoxLayout *flagsRow = new QHBoxLayout();
    flagsRow->setAlignment(Qt::AlignCenter);
    flagsRow->setSpacing(64);

    const QList<std::tuple<QString,QString,QString>> flags = {
                                                                { "🇬🇧", "en",    "English"  },
                                                                { "🇫🇷", "fr",    "Français" },
                                                                { "🌍", "other", "Other"    },
                                                                };

    for (const auto &[emoji, code, tip] : flags) {
        QPushButton *btn = makeFlagButton(emoji, code, tip);
        flagsRow->addWidget(btn);
    }
    root->addLayout(flagsRow);

    QLabel *subtitle = new QLabel("Select a language or scan an item to start");
    subtitle->setFont(QFont("Arial", 18, QFont::Bold));
    subtitle->setAlignment(Qt::AlignCenter);
    root->addWidget(subtitle);
}

QPushButton *WelcomePage::makeFlagButton(const QString &emoji, const QString &langCode, const QString &tooltip)
{
    const int SIZE   = 160;
    const int RADIUS = SIZE / 2;

    QPushButton *btn = new QPushButton(emoji);
    btn->setToolTip(tooltip);
    btn->setFixedSize(SIZE, SIZE);
    btn->setStyleSheet(Style::flagButtonSheet(RADIUS));
    btn->setCursor(Qt::PointingHandCursor);

    // CHANGE: emit signal, and update the language immediately.
    connect(btn, &QPushButton::clicked, this, [this, langCode]() {
        m_cart->language = langCode;
        emit languageSelected(langCode);
    });

    return btn;
}
