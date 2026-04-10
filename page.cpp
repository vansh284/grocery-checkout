#include "page.h"
#include <QFont>
#include <QIcon>

Page::Page(CartData *cart,
           const QString &topBarColor,
           const QString &pageBGColor,
           bool hasBack,
           bool hasHelp,
           bool hasLang,
           QWidget *parent)
    : QWidget(parent), m_cart(cart), m_flagLabel(nullptr)
{
    for (const auto& item : Config::LanguageFlags) {
        // std::get<1> 是 langCode (如 "en")
        // std::get<0> 是 iconPath (如 ":/img/uk.png")
        m_flagMap.insert(std::get<1>(item), std::get<0>(item));
    }

    setObjectName("pageRoot");
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("QWidget#pageRoot { background-color: " + pageBGColor + "; }");

    // Basics
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // Top Bar
    rootLayout->addWidget(buildTopBar(topBarColor, hasBack, hasHelp, hasLang));

    m_contentLayout = new QVBoxLayout();
    m_contentLayout->setContentsMargins(20, 20, 20, 20);
    rootLayout->addLayout(m_contentLayout);
}


QWidget* Page::buildTopBar(const QString &color,
                           bool hasBack,
                           bool hasHelp,
                           bool hasLang)
{
    QWidget *topBar = new QWidget();
    topBar->setFixedHeight(70);
    topBar->setStyleSheet("background-color: " + color + ";");

    QHBoxLayout *barLayout = new QHBoxLayout(topBar);

    if (hasBack) {
        QPushButton *backBtn = new QPushButton("← Back");
        backBtn->setFixedHeight(48);
        backBtn->setFont(QFont("Arial", 16, QFont::Bold));
        backBtn->setCursor(Qt::PointingHandCursor);
        backBtn->setStyleSheet(Style::backSheet);
        connect(backBtn, &QPushButton::clicked, this, &Page::backClicked);
        barLayout->addWidget(backBtn);
    }

    barLayout->addStretch();

    if (hasLang) {
        QString iconPath = m_flagMap.value(m_cart->language, ":/img/uk.png");

        const int SIZE   = 56;
        const int RADIUS = SIZE / 2;

        m_flagLabel = new QPushButton();
        m_flagLabel->setIcon(QIcon(iconPath));
        m_flagLabel->setIconSize(QSize(48, 48));
        m_flagLabel->setFixedSize(SIZE, SIZE);
        m_flagLabel->setCursor(Qt::PointingHandCursor);
        m_flagLabel->setStyleSheet(Style::flagButtonSheet(RADIUS));

        barLayout->addWidget(m_flagLabel);
        barLayout->addSpacing(12);
}

    if (hasHelp) {
        QPushButton *helpBtn = new QPushButton("  Help  ");
        helpBtn->setFixedHeight(44);
        helpBtn->setStyleSheet(Style::helpSheet);
        connect(helpBtn, &QPushButton::clicked, this, &Page::helpClicked);
        barLayout->addWidget(helpBtn);
    }
    return topBar;
}

void Page::updateLanguageFlag(const QString &langCode) {
    if (m_flagLabel) {
        QString iconPath = m_flagMap.value(langCode, ":/img/uk.png");
        m_flagLabel->setIcon(QIcon(iconPath));
    }
}
