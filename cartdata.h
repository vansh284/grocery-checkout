#ifndef CARTDATA_H
#define CARTDATA_H

#include <QWidget>
#include <QVector>


struct CartItem {
    QString name;
    int qty;
    double price; // single price
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
