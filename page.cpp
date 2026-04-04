#include "page.h"
#include "style.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFont>

Page::Page(CartData *cart,
           const QString &topBarColor,
           const QString &pageBGColor,
           bool hasBack,
           bool hasHelp,
           bool hasLang,
           QWidget *parent)
    : QWidget(parent), m_cart(cart), m_flagLabel(nullptr)
{
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
    topBar->setFixedHeight(64);
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
        const QMap<QString,QString> flagMap = {
                                                {"en", "🇬🇧"}, {"fr", "🇫🇷"}, {"other", "🌍"},
                                                };
        m_flagLabel = new QLabel(flagMap.value(m_cart->language, "🇬🇧"));
        m_flagLabel->setFont(QFont("Arial", 28));
        m_flagLabel->setStyleSheet("background: transparent;");
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
        const QMap<QString,QString> flagMap = {
                                                {"en", "🇬🇧"}, {"fr", "🇫🇷"}, {"other", "🌍"},
                                                };
        m_flagLabel->setText(flagMap.value(langCode, "🇬🇧"));
    }
}
