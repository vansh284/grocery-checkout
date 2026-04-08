#ifndef NUMPADDIALOG_H
#define NUMPADDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QEvent>

class NumpadDialog : public QDialog
{
    Q_OBJECT

public:
    // 定义对话框的工作模式
    enum Mode {
        BarcodeMode,
        PriceMode
    };

    explicit NumpadDialog(Mode mode, QWidget *parent = nullptr);

    // 获取条形码（仅在 BarcodeMode 有效）
    QString getBarcode() const;
    // 获取价格（仅在 PriceMode 有效）
    double getPrice() const;

protected:
    // 拦截鼠标点击事件，用于切换输入框的选中状态
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onNumberClicked();
    void onBackspaceClicked();
    void onConfirmClicked();
    void onCancelClicked();

private:
    // 追踪当前激活的输入框
    enum ActiveField {
        FieldBarcode,
        FieldEuros,
        FieldCents
    };

    void setActiveField(ActiveField field);

    Mode m_mode;
    ActiveField m_activeField;

    // 根据模式使用的输入框指针
    QLineEdit *m_barcodeDisplay; // 条形码输入框
    QLineEdit *m_euroDisplay;    // 价格模式：元的输入框
    QLineEdit *m_centDisplay;    // 价格模式：分的输入框
};

#endif // NUMPADDIALOG_H
