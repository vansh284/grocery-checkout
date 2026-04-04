#ifndef RECEIPTPAGE_H
#define RECEIPTPAGE_H

#include "page.h"

class ReceiptPage : public Page
{
    Q_OBJECT

public:
    explicit ReceiptPage(CartData *cart, QWidget *parent = nullptr);

signals:
    void finished();
};

#endif // RECEIPTPAGE_H
