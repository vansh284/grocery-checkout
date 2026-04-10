#include "welcomepage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QFile>
#include <QDebug>
#include "style.h"

WelcomePage::WelcomePage(CartData *cart, QWidget *parent)
    : QWidget(parent), m_cart(cart)
{
    setObjectName("pageRoot");
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(Style::pageSheet);

    QVBoxLayout *root = new QVBoxLayout(this);
    root->setAlignment(Qt::AlignCenter);
    root->setSpacing(48);
    root->setContentsMargins(0, 0, 0, 0);

    QLabel *title = new QLabel("WELCOME");
    title->setFont(QFont("Arial", 72, QFont::Bold));
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #111111;");
    root->addWidget(title);

    QHBoxLayout *flagsRow = new QHBoxLayout();
    flagsRow->setAlignment(Qt::AlignCenter);
    flagsRow->setSpacing(64);

    for (const auto &[path, code, tip] : Config::LanguageFlags) {
        QPushButton *btn = makeFlagButton(path, code, tip);
        flagsRow->addWidget(btn);
    }
    root->addLayout(flagsRow);

    QLabel *subtitle = new QLabel("Select a language or scan an item to start");
    subtitle->setFont(QFont("Arial", 24, QFont::Bold));
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("color: #111111;");
    root->addWidget(subtitle);
}

QPushButton *WelcomePage::makeFlagButton(const QString &iconPath, const QString &langCode, const QString &tooltip)
{
    const int SIZE   = 160;
    const int RADIUS = SIZE / 2;

    QPushButton *btn = new QPushButton();

    btn->setIcon(QIcon(iconPath));
    btn->setIconSize(QSize(150, 150));

    btn->setToolTip(tooltip);
    btn->setFixedSize(SIZE, SIZE);
    btn->setStyleSheet(Style::flagButtonSheet(RADIUS));
    btn->setCursor(Qt::PointingHandCursor);

    connect(btn, &QPushButton::clicked, this, [this, langCode]() {
        m_cart->language = langCode;
        emit languageSelected(langCode);
    });

    return btn;
}
