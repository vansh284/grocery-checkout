#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include "cartdata.h"

#include "style.h"

class QLabel; 

class Page : public QWidget
{
    Q_OBJECT

public:
    explicit Page(CartData *cart,
                  const QString &topBarColor,
                  const QString &pageBGColor,
                  bool hasBack = true,
                  bool hasHelp = true,
                  bool hasLang = true,
                  QWidget *parent = nullptr);
public:
    void updateLanguageFlag(const QString &langCode);

signals:
    void backClicked();
    void helpClicked();
    void languageSelected(const QString &langCode);

protected:
    CartData *m_cart;
    QVBoxLayout *m_contentLayout;
    // Reserved space for content layout and flag label.
    QLabel *m_flagLabel;

private:
    QWidget* buildTopBar(const QString &color,
                         bool hasBack=true,
                         bool hasHelp=true,
                         bool hasLang=true);
};

#endif // PAGE_H
