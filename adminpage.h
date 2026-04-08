#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include "page.h"
#include "numpaddialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>

class AdminPage : public Page
{
    Q_OBJECT

public:
    explicit AdminPage(CartData *cart, QWidget *parent = nullptr);

    // Called by MainWindow right before navigating to this page
    // to ensure the UI reflects the current state of m_cart
    void loadCart();

signals:
    void logoutClicked();

private:
    QVBoxLayout *m_itemsList;
    QLabel *m_subtotalLabel;

    // Helper function to create a single row in the admin list
    void addAdminItemUI(const QString &name, int qty, double price, int index);
};

#endif // ADMINPAGE_H
