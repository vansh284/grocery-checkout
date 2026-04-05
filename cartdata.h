#ifndef CARTDATA_H
#define CARTDATA_H

#include <QWidget>
#include <QVector>


struct CartItem {
    QString name;
    int qty = 1;
    double price = 0.0;
    double weightKg = 0.0;       // for weighted items
    bool isWeighted = false;
};

class CartData
{
public:
    QVector<CartItem> items;
    double total = 0.00;
    QString language = "en";

    void clear() {
        items.clear();
        total = 0.00;
    }
};

#endif // CARTDATA_H
