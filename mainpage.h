#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
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
    void refreshUI() // 或对应你清空商品列表的函数
    {
        // 1. 安全清空 UI 列表
        QLayoutItem *child;
        while ((child = m_itemsList->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->hide();          // 先隐藏，防止视觉卡顿
                child->widget()->deleteLater();   // ✨ 核心：交给 Qt 的垃圾回收器安全销毁
            }
            delete child; // 仅删除 layout 容器项
        }

        // 2. 底层数据归零 (正是你怀疑的 m_cartData)
        m_cart->clear();
        m_subtotalLabel->setText("€0.00");
    }
};

#endif // MAINPAGE_H
