#include "numpaddialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QFont>
#include <QPainter>
#include <QPainterPath>

class BackspaceButton : public QPushButton {
public:
    BackspaceButton(QWidget* parent = nullptr) : QPushButton("✖", parent) {
        setFixedSize(60, 50);
        setFont(QFont("Arial", 20, QFont::Bold));
        setCursor(Qt::PointingHandCursor);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        // 不调用基类的 paintEvent，我们完全自己画
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing); // 开启抗锯齿，边缘更平滑

        int w = width();
        int h = height();
        int arrowWidth = 15; // 左侧尖角的宽度（你可以自己调）

        // 定义多边形的 5 个顶点
        QPolygon polygon;
        polygon << QPoint(w, 0)                  // 右上角
                << QPoint(w, h)                  // 右下角
                << QPoint(arrowWidth, h)         // 左下折点
                << QPoint(0, h / 2)              // 最左边尖角 (垂直居中)
                << QPoint(arrowWidth, 0);        // 左上折点

        // 根据鼠标按下/松开的状态，设置不同的红色
        QColor bgColor = isDown() ? QColor("#B5403C") : QColor("#D45450");
        painter.setPen(Qt::NoPen);   // 不需要边框线
        painter.setBrush(bgColor);   // 填充颜色

        // 绘制多边形背景
        painter.drawPolygon(polygon);

        // 绘制中间的 "✖" 文字
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, text());
    }
};

NumpadDialog::NumpadDialog(Mode mode, QWidget *parent)
    : QDialog(parent),
    m_mode(mode),
    m_barcodeDisplay(nullptr),
    m_euroDisplay(nullptr),
    m_centDisplay(nullptr)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(420, 580); // 稍微加宽一点点以容纳价格模式的多个控件

    QFrame *mainFrame = new QFrame(this);
    mainFrame->setFixedSize(420, 580);
    mainFrame->setStyleSheet(
        "QFrame {"
        "  background-color: #FAF2D4;"
        "  border: 3px solid black;"
        "  border-radius: 16px;"
        "}"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(mainFrame);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // 1. 标题
    QLabel *titleLabel = new QLabel();
    titleLabel->setFont(QFont("Arial", 28));
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("border: none;");
    if (m_mode == BarcodeMode) {
        titleLabel->setText("Enter barcode");
    } else {
        titleLabel->setText("Price");
    }
    mainLayout->addWidget(titleLabel);

    // 2. 顶部输入区域
    QHBoxLayout *inputLayout = new QHBoxLayout();

    // 退格键 (两个模式通用，靠右侧)
    BackspaceButton *backspaceBtn = new BackspaceButton();
    connect(backspaceBtn, &QPushButton::clicked, this, &NumpadDialog::onBackspaceClicked);

    if (m_mode == BarcodeMode) {
        // --- 条码模式的输入框 ---
        m_barcodeDisplay = new QLineEdit();
        m_barcodeDisplay->setFixedHeight(60);
        m_barcodeDisplay->setFont(QFont("Arial", 24));
        m_barcodeDisplay->setReadOnly(true);
        m_activeField = FieldBarcode; // 永远激活

        inputLayout->addWidget(m_barcodeDisplay);
        inputLayout->addWidget(backspaceBtn);

        setActiveField(FieldBarcode); // 设置紫边样式

    } else {
        // --- 价格模式的输入框 ---
        inputLayout->setAlignment(Qt::AlignCenter);

        // 创建一个“画板”容器，用来固定里面的绝对坐标
        QWidget *priceContainer = new QWidget();
        priceContainer->setFixedSize(290, 60);

        m_euroDisplay = new QLineEdit("0", priceContainer);
        m_euroDisplay->setFixedSize(80, 60);
        m_euroDisplay->setFont(QFont("Arial", 24, QFont::Bold));
        m_euroDisplay->setAlignment(Qt::AlignCenter);
        m_euroDisplay->setReadOnly(true);
        // 安装事件过滤器以捕获点击
        m_euroDisplay->installEventFilter(this);
        m_euroDisplay->setGeometry(85, 0, 80, 60);

        QLabel *dotLabel = new QLabel(".", priceContainer);
        dotLabel->setFont(QFont("Arial", 32, QFont::Bold));
        dotLabel->setStyleSheet("border: none"); // 让小数点靠下一点
        dotLabel->setGeometry(165, 10, 20, 50);

        m_centDisplay = new QLineEdit("00", priceContainer);
        m_centDisplay->setFixedSize(70, 60);
        m_centDisplay->setFont(QFont("Arial", 24, QFont::Bold));
        m_centDisplay->setAlignment(Qt::AlignCenter);
        m_centDisplay->setReadOnly(true);
        // 安装事件过滤器
        m_centDisplay->installEventFilter(this);
        m_centDisplay->setGeometry(180, 0, 70, 60);

        QLabel *euroSignLabel = new QLabel("€", priceContainer);
        euroSignLabel->setFont(QFont("Arial", 24, QFont::Bold));
        euroSignLabel->setStyleSheet("border: none");
        euroSignLabel->setGeometry(260, 0, 40, 60);

        inputLayout->addWidget(priceContainer);
        inputLayout->addSpacing(15);
        inputLayout->addWidget(backspaceBtn);

        // 默认选中元
        setActiveField(FieldEuros);
    }

    mainLayout->addLayout(inputLayout);

    // 3. 数字键盘网格 (完全复用)
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(15);

    QString numBtnStyle =
        "QPushButton {"
        "  background-color: #F1D413;"
        "  color: black;"
        "  border: 3px solid black;"
        "  border-radius: 12px;"
        "}"
        "QPushButton:pressed { background-color: #D6BC0F; }";

    for (int i = 0; i < 9; ++i) {
        int row = i / 3;
        int col = i % 3;
        QPushButton *btn = new QPushButton(QString::number(i + 1));
        btn->setFixedSize(90, 90);
        btn->setFont(QFont("Arial", 28, QFont::Bold));
        btn->setStyleSheet(numBtnStyle);
        connect(btn, &QPushButton::clicked, this, &NumpadDialog::onNumberClicked);
        gridLayout->addWidget(btn, row, col);
    }

    QPushButton *cancelBtn = new QPushButton("✖");
    cancelBtn->setFixedSize(90, 90);
    cancelBtn->setFont(QFont("Arial", 40, QFont::Bold));
    cancelBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #C92A2A;"
        "  color: white;"
        "  border: 3px solid black;"
        "  border-radius: 12px;"
        "}"
        "QPushButton:pressed { background-color: #A32222; }"
        );
    connect(cancelBtn, &QPushButton::clicked, this, &NumpadDialog::onCancelClicked);

    QPushButton *zeroBtn = new QPushButton("0");
    zeroBtn->setFixedSize(90, 90);
    zeroBtn->setFont(QFont("Arial", 28, QFont::Bold));
    zeroBtn->setStyleSheet(numBtnStyle);
    connect(zeroBtn, &QPushButton::clicked, this, &NumpadDialog::onNumberClicked);

    QPushButton *confirmBtn = new QPushButton("✔");
    confirmBtn->setFixedSize(90, 90);
    confirmBtn->setFont(QFont("Arial", 40, QFont::Bold));
    confirmBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #A4D156;"
        "  color: white;"
        "  border: 3px solid black;"
        "  border-radius: 12px;"
        "}"
        "QPushButton:pressed { background-color: #8BB247; }"
        );
    connect(confirmBtn, &QPushButton::clicked, this, &NumpadDialog::onConfirmClicked);

    gridLayout->addWidget(cancelBtn, 3, 0);
    gridLayout->addWidget(zeroBtn, 3, 1);
    gridLayout->addWidget(confirmBtn, 3, 2);

    mainLayout->addLayout(gridLayout);
}

// 样式更新控制中枢
void NumpadDialog::setActiveField(ActiveField field) {
    m_activeField = field;

    // 基础的 QLineEdit 样式
    QString baseStyle = "background-color: white; border-radius: 8px; color: #666666;";

    if (m_mode == BarcodeMode && m_barcodeDisplay) {
        m_barcodeDisplay->setStyleSheet("QLineEdit {" + baseStyle + " border: 3px solid #5C32E6; padding: 0 10px; }");
    }
    else if (m_mode == PriceMode) {
        QString activeBorder = "border: 4px solid black;";
        QString inactiveBorder = "border: 2px solid black;";

        if (field == FieldEuros) {
            m_euroDisplay->setStyleSheet("QLineEdit {" + baseStyle + activeBorder + "}");
            m_centDisplay->setStyleSheet("QLineEdit {" + baseStyle + inactiveBorder + "}");
        } else if (field == FieldCents) {
            m_euroDisplay->setStyleSheet("QLineEdit {" + baseStyle + inactiveBorder + "}");
            m_centDisplay->setStyleSheet("QLineEdit {" + baseStyle + activeBorder + "}");
        }
    }
}

// 事件过滤器拦截点击以切换选中框
bool NumpadDialog::eventFilter(QObject *watched, QEvent *event) {
    if (m_mode == PriceMode && event->type() == QEvent::MouseButtonPress) {
        if (watched == m_euroDisplay) {
            setActiveField(FieldEuros);
            return true;
        } else if (watched == m_centDisplay) {
            setActiveField(FieldCents);
            return true;
        }
    }
    return QDialog::eventFilter(watched, event);
}

QString NumpadDialog::getBarcode() const {
    return m_barcodeDisplay ? m_barcodeDisplay->text() : "";
}

double NumpadDialog::getPrice() const {
    if (m_mode == PriceMode) {
        QString euros = m_euroDisplay->text().isEmpty() ? "0" : m_euroDisplay->text();
        QString cents = m_centDisplay->text().isEmpty() ? "00" : m_centDisplay->text();
        QString priceStr = euros + "." + cents;
        return priceStr.toDouble();
    }
    return 0.0;
}

void NumpadDialog::onNumberClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QString val = btn->text();

    if (m_activeField == FieldBarcode) {
        m_barcodeDisplay->setText(m_barcodeDisplay->text() + val);
    }
    else if (m_activeField == FieldEuros) {
        QString t = m_euroDisplay->text();
        if (t == "0") t = ""; // 消除默认值
        if (t.length() < 3) {
            m_euroDisplay->setText(t + val);
        }
    }
    else if (m_activeField == FieldCents) {
        QString currentCents = m_centDisplay->text();
        // 提取当前的第二位（即右边的数字），它将变成第一位（左边的数字）
        QString newFirstChar = currentCents.right(1);
        // 组合新的两位数：原来的右边位 + 新输入的数字
        QString newCents = newFirstChar + val;

        m_centDisplay->setText(newCents);
    }
}

void NumpadDialog::onBackspaceClicked() {
    if (m_activeField == FieldBarcode) {
        QString t = m_barcodeDisplay->text();
        if (!t.isEmpty()) {
            t.chop(1);
            m_barcodeDisplay->setText(t);
        }
    }
    else if (m_activeField == FieldEuros) {
        QString t = m_euroDisplay->text();
        if (t != "0") {
            t.chop(1);
            if (t.isEmpty()) t = "0"; // 恢复默认值
            m_euroDisplay->setText(t);
        }
    }
    else if (m_activeField == FieldCents) {
        QString currentCents = m_centDisplay->text();

        // 如果已经是 00，就不做任何操作
        if (currentCents == "00") return;

        // 提取当前的第一位（即左边的数字），它将退回到第二位（右边的数字）
        QString oldFirstChar = currentCents.left(1);

        // 左边补 0，右边是原来左边的数字
        QString newCents = "0" + oldFirstChar;

        m_centDisplay->setText(newCents);
        // =========================
    }
}

void NumpadDialog::onCancelClicked() {
    reject();
}

void NumpadDialog::onConfirmClicked() {
    // 条码模式不能为空，价格模式则默认都有数字所以可以直接确认
    if (m_mode == BarcodeMode && m_barcodeDisplay->text().isEmpty()) {
        return;
    }
    accept();
}
