#ifndef PAYMENTPAGE_H
#define PAYMENTPAGE_H

#include "page.h"
#include <QPushButton>

class PaymentPage : public Page
{
    Q_OBJECT
    
public:
    explicit PaymentPage(CartData *cart, QWidget *parent = nullptr);

signals:
    void cashSelected();
    void cardSelected();
};

#endif // PAYMENTPAGE_H
