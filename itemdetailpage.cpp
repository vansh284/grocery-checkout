#include "itemdetailpage.h"
#include "style.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QFont>

ItemDetailPage::ItemDetailPage(CartData *cart, QWidget *parent)
    : Page(cart,
           Style::BTN_YELLOW,
           Style::BG_YELLOW,
           true,
           true,
           true,
           parent)
{
    m_contentLayout->setContentsMargins(0, 34, 0, 26);
    m_contentLayout->setSpacing(0);

    m_categoryTitleLabel = new QLabel("Category");
    m_categoryTitleLabel->setAlignment(Qt::AlignCenter);
    m_categoryTitleLabel->setFont(QFont("Arial", 44, QFont::Bold));
    m_categoryTitleLabel->setStyleSheet("color:#111111;");
    m_contentLayout->addWidget(m_categoryTitleLabel);

    m_contentLayout->addSpacing(24);

    QLabel *selectedText = new QLabel("You have selected :");
    selectedText->setAlignment(Qt::AlignCenter);
    selectedText->setFont(QFont("Arial", 30));
    selectedText->setStyleSheet("color:#111111;");
    m_contentLayout->addWidget(selectedText);

    m_contentLayout->addSpacing(34);

    QHBoxLayout *middleRow = new QHBoxLayout();
    middleRow->setContentsMargins(110, 0, 110, 0);
    middleRow->setSpacing(40);
    middleRow->setAlignment(Qt::AlignCenter);

    QFrame *itemCard = new QFrame();
    itemCard->setFixedSize(300, 250);
    itemCard->setStyleSheet(R"(
        background-color: #F0DE86;
        border: none;
        border-radius: 22px;
    )");

    QVBoxLayout *itemCardLayout = new QVBoxLayout(itemCard);
    itemCardLayout->setContentsMargins(16, 18, 16, 18);
    itemCardLayout->setAlignment(Qt::AlignCenter);
    itemCardLayout->setSpacing(8);

    m_selectedEmojiLabel = new QLabel("🍌");
    m_selectedEmojiLabel->setAlignment(Qt::AlignCenter);
    m_selectedEmojiLabel->setFont(QFont("Arial", 76));

    m_selectedNameLabel = new QLabel("Banana");
    m_selectedNameLabel->setAlignment(Qt::AlignCenter);
    m_selectedNameLabel->setFont(QFont("Arial", 28, QFont::Bold));
    m_selectedNameLabel->setStyleSheet("color:#111111;");

    itemCardLayout->addStretch();
    itemCardLayout->addWidget(m_selectedEmojiLabel);
    itemCardLayout->addWidget(m_selectedNameLabel);
    itemCardLayout->addStretch();

    QVBoxLayout *infoCol = new QVBoxLayout();
    infoCol->setSpacing(24);
    infoCol->setAlignment(Qt::AlignVCenter);

    auto makeInfoRow = [&](const QString &labelText, QLabel *&valueLabel)
    {
        QHBoxLayout *row = new QHBoxLayout();
        row->setSpacing(26);
        row->setAlignment(Qt::AlignVCenter);

        QLabel *label = new QLabel(labelText);
        label->setFixedWidth(300);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setFont(QFont("Arial", 30));
        label->setStyleSheet("color:#111111;");

        valueLabel = new QLabel();
        valueLabel->setFixedSize(240, 96);
        valueLabel->setAlignment(Qt::AlignCenter);
        valueLabel->setFont(QFont("Arial", 28));
        valueLabel->setStyleSheet(R"(
            background-color: #EDEDED;
            border: 2px solid #111111;
            border-radius: 22px;
            color:#111111;
        )");

        row->addWidget(label);
        row->addWidget(valueLabel);
        return row;
    };

    infoCol->addLayout(makeInfoRow("Current Weight:", m_weightValueLabel));
    infoCol->addLayout(makeInfoRow("Total price:", m_priceValueLabel));

    middleRow->addWidget(itemCard, 0, Qt::AlignVCenter);
    middleRow->addLayout(infoCol);

    m_contentLayout->addLayout(middleRow);
    m_contentLayout->addSpacing(52);

    QPushButton *addBtn = new QPushButton("Add item");
    addBtn->setFixedSize(530, 118);
    addBtn->setFont(QFont("Arial", 34, QFont::Bold));
    addBtn->setCursor(Qt::PointingHandCursor);
    addBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #F2BE3E;
            color: white;
            border: none;
            border-radius: 22px;
        }
        QPushButton:hover  { background-color: #E9B12A; }
        QPushButton:pressed{ background-color: #DCA414; }
    )");

    connect(addBtn, &QPushButton::clicked, this, [this]() {
        emit addItemRequested(m_currentItem, m_currentWeightKg);
    });

    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->setAlignment(Qt::AlignCenter);
    btnRow->addWidget(addBtn);

    m_contentLayout->addLayout(btnRow);
    m_contentLayout->addStretch();
}

void ItemDetailPage::setItemDetail(const QString &categoryName,
                                   const CategoryPage::WeightedItem &item,
                                   double currentWeightKg)
{
    m_currentItem = item;
    m_currentWeightKg = currentWeightKg;

    m_categoryTitleLabel->setText(categoryName);
    m_selectedNameLabel->setText(item.name);
    m_selectedEmojiLabel->setText(item.emoji);
    m_weightValueLabel->setText(QString::number(currentWeightKg, 'f', 2) + "kg");
    m_priceValueLabel->setText("€" + QString::number(currentWeightKg * item.unitPricePerKg, 'f', 2));
}
