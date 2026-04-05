#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QVBoxLayout>
#include <QLabel>
#include "page.h"
#include "categorypage.h"

class MainPage : public Page
{
    Q_OBJECT
public:
    explicit MainPage(CartData *cart, QWidget *parent = nullptr);
    void refreshUI();

signals:
    void checkoutConfirmed();
    void categoryRequested(const QString &categoryName);

public slots:
    void addWeightedItem(const CategoryPage::WeightedItem &item, double weightKg);

private:
    QVBoxLayout *m_itemsList = nullptr;
    QLabel *m_subtotalLabel = nullptr;

    void addItem(const QString &name, int qty, double price);

private slots:
    void onPayClicked();
    void showBagDialog();
};

#endif // MAINPAGE_H
