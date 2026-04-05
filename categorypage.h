#ifndef CATEGORYPAGE_H
#define CATEGORYPAGE_H

#include "page.h"
#include <QVector>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class CategoryPage : public Page
{
    Q_OBJECT
public:
    struct WeightedItem {
        QString name;
        QString emoji;
        double unitPricePerKg;
    };

    explicit CategoryPage(CartData *cart, QWidget *parent = nullptr);

    void setCategory(const QString &categoryName,
                     const QVector<WeightedItem> &items,
                     double currentWeightKg = 0.0);

    double currentWeightKg() const;
    bool hasValidWeight() const;

    static QVector<WeightedItem> itemsForCategory(const QString &categoryName);

signals:
    void itemSelected(const CategoryPage::WeightedItem &item);

private:
    QLabel *m_titleLabel = nullptr;
    QLabel *m_weightTextLabel = nullptr;
    QLineEdit *m_weightEdit = nullptr;
    QGridLayout *m_grid = nullptr;
    QPushButton *m_prevButton = nullptr;
    QPushButton *m_nextButton = nullptr;

    QVector<WeightedItem> m_allItems;
    int m_currentPage = 0;
    int m_itemsPerPage = 8;

    QPushButton *makeItemButton(const WeightedItem &item);
    QPushButton *makeNavButton(const QString &text);
    void rebuildGrid(const QVector<WeightedItem> &items);
    void updatePage();
    void updateNavButtons();
};

#endif // CATEGORYPAGE_H
