#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QVBoxLayout>
#include <QLabel>
#include "page.h"

class MainPage : public Page
{
    Q_OBJECT
public:
    explicit MainPage(CartData *cart, QWidget *parent = nullptr);

signals:
    void checkoutConfirmed();

private:
    QVBoxLayout *m_itemsList;
    QLabel *m_subtotalLabel;
    void addItem(const QString &name, int qty, double price);

private slots:
    void onPayClicked();
    void showBagDialog();

public:
    void refreshUI();
};

#endif // MAINPAGE_H
