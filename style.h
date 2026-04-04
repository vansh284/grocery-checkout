#ifndef STYLE_H
#define STYLE_H

#include <QString>
namespace Style {

const QString BG_YELLOW   = "#FAFAC8";  // background
const QString BTN_YELLOW  = "#F5C800";  // accent yellow (Pay, Add item…)
const QString BTN_RED     = "#E53935";  // Help button
const QString TEXT_DARK   = "#111111";
const QString BG_BLUE     = "#E1EDF7";  // background for admin page
const QString BTN_BLUE_1  = "#B0D5F4";  // light blue button
const QString BTN_BLUE_2  = "#6C93EB";  // dark blue button


// Page background
const QString pageSheet =
    "QWidget#pageRoot { background-color: " + BG_YELLOW + "; }";

const QString pageSheetAdmin =
    "QWidget#pageRoot { background-color: " + BG_BLUE + "; }";


// Flag language button – circular, large emoji
inline QString flagButtonSheet(int radius) {
    return QString(R"(
        QPushButton {
            background-color: %1;
            border: 3px solid #CCBA00;
            border-radius: %2px;
            font-size: 64px;
        }
        QPushButton:hover  { background-color: #F0EFA0; border-color: #A89800; }
        QPushButton:pressed{ background-color: #E4E374; }
    )").arg(BG_YELLOW).arg(radius);
}

// Red Help button
const QString helpSheet = R"(
    QPushButton {
        background-color: #E53935;
        color: white;
        border-radius: 10px;
        font-size: 16px;
        font-weight: bold;
        padding: 0 20px;
    }
    QPushButton:hover  { background-color: #C62828; }
    QPushButton:pressed{ background-color: #B71C1C; }
)";

// Back button
const QString backSheet = R"(
    QPushButton {
        background-color: white;
        border: 2px solid #CCCC00;
        border-radius: 12px;
        padding: 0 20px;
        color: #111111;
    }
    QPushButton:hover  { background-color: #F5F5DC; }
    QPushButton:pressed{ background-color: #EBEBC8; }
)";

// Add item button 1
const QString itemSheet_1 = R"(
    QPushButton {
        background-color: #F5C800;
        border: none;
        border-radius: 20px;
        padding-left: 24px;
    }
    QPushButton:hover { background-color: #F7DE50; }
    QPushButton:pressed { background-color: #EFC900; }
)";

// Add item button on Admin Page
const QString itemSheet_2 = R"(
            QPushButton {
                background-color: #B0C4DE;
                border: none;
                border-radius: 20px;
            }
            QPushButton:hover { background-color: #B0D5F4; }
            QPushButton:pressed { background-color: ##73bfff; }
        )";
}


#endif // STYLE_H
