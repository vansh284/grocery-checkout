#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>
#include <QPushButton>
#include "cartdata.h"

class WelcomePage : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomePage(CartData *cart, QWidget *parent = nullptr);

signals:
    void languageSelected(const QString &langCode);

private:
    CartData *m_cart;
    QPushButton *makeFlagButton(const QString &emoji, const QString &langCode, const QString &tooltip);
};

#endif //WELCOMEPAGE_H
