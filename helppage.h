#ifndef HELPPAGE_H
#define HELPPAGE_H

#include "page.h"

class HelpPage : public Page
{
    Q_OBJECT
public:
    explicit HelpPage(CartData *cart, QWidget *parent = nullptr);
};

#endif // HELPPAGE_H
