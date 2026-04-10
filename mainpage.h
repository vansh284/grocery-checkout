#ifndef MAINPAGE_H
#define MAINPAGE_H

#include "page.h"
#include "categorypage.h"

class QLabel;
class QVBoxLayout;

class MainPage : public Page
{
    Q_OBJECT

public:
    explicit MainPage(CartData *cart, QWidget *parent = nullptr);

    void addItem(const QString &name, int qty, double price);
    void addWeightedItem(const CategoryPage::WeightedItem &item, double weightKg);
    void refreshUI();
    void loadCart();

signals:
    void checkoutConfirmed();
    void categoryRequested(const QString &categoryName);

private slots:
    void onPayClicked();

private:
    void showBagDialog();

    QVBoxLayout *m_itemsList;
    QLabel *m_subtotalLabel;
};

#endif // MAINPAGE_H
