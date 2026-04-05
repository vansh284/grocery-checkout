#ifndef ITEMDETAILPAGE_H
#define ITEMDETAILPAGE_H

#include "page.h"
#include "categorypage.h"
#include <QLabel>

class ItemDetailPage : public Page
{
    Q_OBJECT
public:
    explicit ItemDetailPage(CartData *cart, QWidget *parent = nullptr);

    void setItemDetail(const QString &categoryName,
                       const CategoryPage::WeightedItem &item,
                       double currentWeightKg);

signals:
    void addItemRequested(const CategoryPage::WeightedItem &item, double weightKg);

private:
    CategoryPage::WeightedItem m_currentItem;
    double m_currentWeightKg = 0.67;

    QLabel *m_categoryTitleLabel = nullptr;
    QLabel *m_selectedNameLabel = nullptr;
    QLabel *m_selectedEmojiLabel = nullptr;
    QLabel *m_weightValueLabel = nullptr;
    QLabel *m_priceValueLabel = nullptr;
};

#endif // ITEMDETAILPAGE_H
