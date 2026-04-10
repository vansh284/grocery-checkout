#include "categorypage.h"
#include "style.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QWidget>
#include <QHBoxLayout>
#include <QDoubleValidator>
#include <algorithm>

CategoryPage::CategoryPage(CartData *cart, QWidget *parent)
    : Page(cart,
           Style::BTN_YELLOW,
           Style::BG_YELLOW,
           true,
           true,
           true,
           parent)
{
    m_contentLayout->setContentsMargins(0, 20, 0, 20);
    m_contentLayout->setSpacing(0);

    m_contentLayout->addSpacing(18);

    m_titleLabel = new QLabel("Category");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setFont(QFont("Arial", 40, QFont::Bold));
    m_titleLabel->setStyleSheet("color:#111111;");
    m_contentLayout->addWidget(m_titleLabel);

    m_contentLayout->addSpacing(34);

    QHBoxLayout *itemsRow = new QHBoxLayout();
    itemsRow->setContentsMargins(0, 0, 0, 0);
    itemsRow->setSpacing(18);
    itemsRow->setAlignment(Qt::AlignCenter);

    m_prevButton = makeNavButton(QString::fromUtf8("⬅"));
    m_nextButton = makeNavButton(QString::fromUtf8("➡"));

    connect(m_prevButton, &QPushButton::clicked, this, [this]() {
        if (m_currentPage > 0) {
            --m_currentPage;
            updatePage();
        }
    });

    connect(m_nextButton, &QPushButton::clicked, this, [this]() {
        const int totalPages = (static_cast<int>(m_allItems.size()) + m_itemsPerPage - 1) / m_itemsPerPage;
        if (m_currentPage + 1 < totalPages) {
            ++m_currentPage;
            updatePage();
        }
    });

    QWidget *gridWrap = new QWidget();
    gridWrap->setFixedHeight(360);

    m_grid = new QGridLayout(gridWrap);
    m_grid->setContentsMargins(30, 20, 30, 20);
    m_grid->setHorizontalSpacing(40);
    m_grid->setVerticalSpacing(34);

    itemsRow->addWidget(m_prevButton, 0, Qt::AlignVCenter);
    itemsRow->addWidget(gridWrap, 0, Qt::AlignVCenter);
    itemsRow->addWidget(m_nextButton, 0, Qt::AlignVCenter);

    m_contentLayout->addLayout(itemsRow);

    m_contentLayout->addSpacing(44);

    QHBoxLayout *weightRow = new QHBoxLayout();
    weightRow->setContentsMargins(0, 0, 0, 36);
    weightRow->setSpacing(36);

    m_weightTextLabel = new QLabel("Current Weight:");
    m_weightTextLabel->setFont(QFont("Arial", 32));
    m_weightTextLabel->setStyleSheet("color:#111111;");

    m_weightEdit = new QLineEdit();
    m_weightEdit->setFixedSize(250, 104);
    m_weightEdit->setAlignment(Qt::AlignCenter);
    m_weightEdit->setFont(QFont("Arial", 30));
    m_weightEdit->setPlaceholderText("kg");
    m_weightEdit->setValidator(new QDoubleValidator(0.0, 999.999, 3, m_weightEdit));
    m_weightEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: #EDEDED;
            border: 2px solid #111111;
            border-radius: 30px;
            color:#111111;
            padding: 0 16px;
        }
    )");

    weightRow->addStretch();
    weightRow->addWidget(m_weightTextLabel);
    weightRow->addWidget(m_weightEdit);
    weightRow->addStretch();

    m_contentLayout->addLayout(weightRow);
    m_contentLayout->addSpacing(24);
}

void CategoryPage::setCategory(const QString &categoryName,
                               const QVector<WeightedItem> &items,
                               double currentWeightKg)
{
    Q_UNUSED(currentWeightKg);

    m_titleLabel->setText(categoryName);
    m_weightEdit->clear();

    m_allItems = items;
    m_currentPage = 0;

    updatePage();
}

double CategoryPage::currentWeightKg() const
{
    bool ok = false;
    const double value = m_weightEdit->text().toDouble(&ok);
    return ok ? value : 0.0;
}

bool CategoryPage::hasValidWeight() const
{
    return currentWeightKg() > 0.0;
}

QVector<CategoryPage::WeightedItem> CategoryPage::itemsForCategory(const QString &categoryName)
{
    if (categoryName == "Fruits") {
        return {
            {"Apple",      "🍎", 3.49},
            {"Banana",     "🍌", 2.29},
            {"Cherry",     "🍒", 8.99},
            {"Date",       "🌰", 7.50},
            {"Grape",      "🍇", 4.99},
            {"Lemon",      "🍋", 2.79},
            {"Mango",      "🥭", 5.99},
            {"Orange",     "🍊", 3.19},
            {"Pear",       "🍐", 3.89},
            {"Peach",      "🍑", 4.99},
            {"Pineapple",  "🍍", 3.99},
            {"Strawberry", "🍓", 7.99},
            {"Watermelon", "🍉", 1.99},
            {"Kiwi",       "🥝", 5.49},
            {"Coconut",    "🥥", 4.99},
            {"Melon",      "🍈", 2.99}
        };
    } else if (categoryName == "Vegetables") {
        return {
            {"Tomato",    "🍅", 3.29},
            {"Potato",    "🥔", 1.69},
            {"Carrot",    "🥕", 1.99},
            {"Broccoli",  "🥦", 3.99},
            {"Corn",      "🌽", 2.99},
            {"Cucumber",  "🥒", 2.49},
            {"Lettuce",   "🥬", 2.19},
            {"Garlic",    "🧄", 5.99},
            {"Onion",     "🧅", 1.79},
            {"Pepper",    "🫑", 4.49},
            {"Mushroom",  "🍄", 6.49},
            {"Eggplant",  "🍆", 3.49}
        };
    } else {
        return {
            {"Toast",      "🍞", 4.99},
            {"Baguette",   "🥖", 3.99},
            {"Croissant",  "🥐", 11.99},
            {"Donut",      "🍩", 12.99},
            {"Bagel",      "🥯", 8.99},
            {"Pretzel",    "🥨", 7.99},
            {"Cupcake",    "🧁", 14.99},
            {"Pancake",    "🥞", 9.99},
            {"Waffle",     "🧇", 10.99},
            {"Cookie",     "🍪", 13.99},
            {"Pie",        "🥧", 12.49},
            {"Sandwich",   "🥪", 9.49}
        };
    }
}

QPushButton *CategoryPage::makeItemButton(const WeightedItem &item)
{
    QPushButton *btn = new QPushButton();
    btn->setFixedSize(180, 140);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setStyleSheet(R"(
        QPushButton {
            background-color: #F0DE86;
            border: none;
            border-radius: 20px;
        }
        QPushButton:hover  { background-color: #EAD56A; }
        QPushButton:pressed{ background-color: #E2CC56; }
    )");

    QVBoxLayout *layout = new QVBoxLayout(btn);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(6);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *emojiLabel = new QLabel(item.emoji);
    emojiLabel->setAlignment(Qt::AlignCenter);
    emojiLabel->setFont(QFont("Arial", 42));
    emojiLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    QLabel *textLabel = new QLabel(item.name);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setFont(QFont("Arial", 18, QFont::Bold));
    textLabel->setStyleSheet("color:#111111;");
    textLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    layout->addStretch();
    layout->addWidget(emojiLabel);
    layout->addWidget(textLabel);
    layout->addStretch();

    connect(btn, &QPushButton::clicked, this, [this, item]() {
        emit itemSelected(item);
    });

    return btn;
}

QPushButton *CategoryPage::makeNavButton(const QString &text)
{
    QPushButton *btn = new QPushButton(text);
    btn->setFixedSize(88, 88);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setFont(QFont("Segoe UI Symbol", 40, QFont::Bold));
    btn->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            border: none;
            color: #111111;
            padding: 0;
        }
        QPushButton:hover  { color: #111111; }
        QPushButton:pressed{ color: #111111; }
        QPushButton:disabled { color: #B8B8B8; }
    )");
    return btn;
}

void CategoryPage::rebuildGrid(const QVector<WeightedItem> &items)
{
    while (QLayoutItem *child = m_grid->takeAt(0)) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }

    for (int i = 0; i < m_itemsPerPage; ++i) {
        if (i < items.size()) {
            QPushButton *btn = makeItemButton(items[i]);
            m_grid->addWidget(btn, i / 4, i % 4, Qt::AlignCenter);
        } else {
            QWidget *placeholder = new QWidget();
            placeholder->setFixedSize(180, 140);
            placeholder->setStyleSheet("background: transparent;");
            m_grid->addWidget(placeholder, i / 4, i % 4, Qt::AlignCenter);
        }
    }
}

void CategoryPage::updatePage()
{
    QVector<WeightedItem> pageItems;

    const int start = m_currentPage * m_itemsPerPage;
    const int end = std::min(start + m_itemsPerPage, static_cast<int>(m_allItems.size()));

    for (int i = start; i < end; ++i) {
        pageItems.append(m_allItems[i]);
    }

    rebuildGrid(pageItems);
    updateNavButtons();
}

void CategoryPage::updateNavButtons()
{
    const int totalPages = (static_cast<int>(m_allItems.size()) + m_itemsPerPage - 1) / m_itemsPerPage;

    m_prevButton->setVisible(totalPages > 1);
    m_nextButton->setVisible(totalPages > 1);

    m_prevButton->setEnabled(m_currentPage > 0);
    m_nextButton->setEnabled(m_currentPage + 1 < totalPages);
}
