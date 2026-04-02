#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QStack>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//  MainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Called when the user taps a language flag on the welcome page.
    void onLanguageSelected(const QString &langCode);
    void onBackClicked();
    void onHelpClicked();
    void onCashSelected();
    void onCardSelected();
    void onReceiptYesSelected();
    void onReceiptNoSelected();

private:
    Ui::MainWindow *ui;

    // Pages
    QStackedWidget *m_stack;
    QWidget        *m_welcomePage;
    QWidget        *m_mainPage;
    QWidget        *m_paymentPage;
    QWidget        *m_receiptPage;


    static constexpr int PAGE_WELCOME = 0;
    static constexpr int PAGE_MAIN    = 1;
    static constexpr int PAGE_PAYMENT = 2;
    static constexpr int PAGE_RECEIPT = 3;

    // State
    QString m_selectedLanguage;   // "en" | "fr" | "other"
    QStack<int> m_pageHistory;

    // Builder
    QWidget    *buildWelcomePage();
    QWidget    *buildMainPage();
    QWidget    *buildPaymentPage();
    QWidget    *buildReceiptPage();
    QPushButton *makeFlagButton(const QString &emoji,
                                const QString &langCode,
                                const QString &tooltip);
    QWidget    *makeTopBar(bool hasBack,
                        bool hasHelp,
                        bool hasLang,
                        const QString &flagObjectName = QString());

    // Navigation
    void navigateTo(int pageIndex);
    void goToMain();
    void goToWelcome();
    void goToPayment();
    void goToReceipt();

    //Items
    QVBoxLayout *m_itemsList;
    QLabel *m_subtotalLabel;
    double m_total = 0.0;

    void addItem(const QString &name, int qty, double price);
    void onPayClicked();
    void showBagDialog();
};


#endif // MAINWINDOW_H
