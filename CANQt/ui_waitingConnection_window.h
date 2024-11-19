/********************************************************************************
** Form generated from reading UI file 'waitingConnection_window.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAITINGCONNECTION_WINDOW_H
#define UI_WAITINGCONNECTION_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>

QT_BEGIN_NAMESPACE

class Ui_WaitingConnectionWindow
{
public:
    QProgressBar *progressBar;
    QLabel *top_label;
    QLabel *note_label;
    QLabel *icon_label;

    void setupUi(QDialog *WaitingConnectionWindow)
    {
        if (WaitingConnectionWindow->objectName().isEmpty())
            WaitingConnectionWindow->setObjectName(QString::fromUtf8("WaitingConnectionWindow"));
        WaitingConnectionWindow->resize(400, 150);
        WaitingConnectionWindow->setMaximumSize(QSize(400, 150));
        WaitingConnectionWindow->setStyleSheet(QString::fromUtf8("QWidget {\n"
                                                                             "    background-color: #E6F7FF;  \n" // Couleur de fond
                                                                             "    color: #444444;               \n" // Couleur du texte
                                                                             "}\n"
                                                                             "\n"
                                                                             "QProgressBar {\n"
                                                                             "    border: 2px solid #0099CC;   \n" // Bordure de la barre de progression
                                                                             "    border-radius: 5px;         \n"
                                                                             "    text-align: center;           \n"
                                                                             "    color: #444444;            \n" // Couleur du texte dans la barre de progression
                                                                             "}\n"
                                                                             "\n"
                                                                             "QProgressBar::chunk {\n"
                                                                             "    background-color: fromRgb(0, 128, 0, 180);    \n" // Couleur de la partie remplie de la barre
                                                                             "    width: 20px;\n"
                                                                             "}\n"
                                                                             "\n"
                                                                             "QPushButton {\n"
                                                                             "    background-color: #66CCFF;   \n" // Couleur pour "On"
                                                                             "    color: #444444;              \n" // Couleur du texte du bouton
                                                                             "    border: 2px solid #66CCFF;   \n"
                                                                             "    border-radius: 5px;          \n"
                                                                             "    padding: 8px;\n"
                                                                             "    font-size: 14px;\n"
                                                                             "}\n"
                                                                             "\n"
                                                                             "QPushButton:hover {\n"
                                                                             "    background-color: #0099CC;    \n" // Couleur de fond au survol
                                                                             "    border: 2px solid #0099CC;\n"
                                                                             "}\n"
                                                                             "\n"
                                                                             "QPushButton:pressed {\n"
                                                                             "    background-color: fromRgb(0, 128, 0, 180);    \n" // Couleur lors du clic
                                                                             "}\n"
                                                                             "\n"
                                                                             "QPushButton#offButton {\n" // Spécifique à l'état "Off"
                                                                             "    background-color: #444444;   \n" // Couleur pour "Off"
                                                                             "    color: #FFFFFF;              \n"
                                                                             "    border: 2px solid #444444;   \n"
                                                                             "}\n"
                                                                             "\n"
                                                                             "QPushButton#offButton:hover {\n" // État hover pour "Off"
                                                                             "    background-color: #555555;    \n"
                                                                             "    border: 2px solid #555555;\n"
                                                                             "}\n"
                                                                             "\n"
                                                                             "QLabel {\n"
                                                                             "    color: #444444;               \n" // Couleur du texte des étiquettes
                                                                             "    font-size: 16px;              \n"
                                                                             "    font-weight: bold;\n"
                                                                             "    background-color: #E6F7FF;  \n" // Couleur de fond des étiquettes
                                                                             "    padding: 10px;\n"
                                                                             "    border-radius: 10px;\n"
                                                                             "}\n"
                                                                             "\n"
                                                                             "QLabel#note_label {\n"
                                                                             "    color: #444444;               \n"
                                                                             "    font-size: 10px;         "
                                                                             "    background-color: #E6F7FF;   \n" // Couleur de fond pour note_label
                                                                             "    padding: 5px;               \n"
                                                                             "    border-radius: 5px;           \n"
                                                                             "}\n"
                                                                             "\n"
                                                                             ""));

        WaitingConnectionWindow->setSizeGripEnabled(false);
        progressBar = new QProgressBar(WaitingConnectionWindow);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(50, 110, 300, 29));
        progressBar->setMaximumSize(QSize(300, 75));
        progressBar->setValue(0);
        top_label = new QLabel(WaitingConnectionWindow);
        top_label->setObjectName(QString::fromUtf8("top_label"));
        top_label->setGeometry(QRect(90, 10, 171, 51));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        top_label->setFont(font);
        top_label->setLineWidth(1);
        top_label->setTextFormat(Qt::PlainText);
        top_label->setScaledContents(true);
        top_label->setAlignment(Qt::AlignCenter);
        note_label = new QLabel(WaitingConnectionWindow);
        note_label->setObjectName(QString::fromUtf8("note_label"));
        note_label->setGeometry(QRect(20, 70, 351, 31));
        note_label->setAlignment(Qt::AlignCenter);
        icon_label = new QLabel(WaitingConnectionWindow);
        icon_label->setObjectName(QString::fromUtf8("icon_label"));
        icon_label->setGeometry(QRect(240, 10, 67, 51));

        retranslateUi(WaitingConnectionWindow);

        QMetaObject::connectSlotsByName(WaitingConnectionWindow);
    } // setupUi

    void retranslateUi(QDialog *WaitingConnectionWindow)
    {
        WaitingConnectionWindow->setWindowTitle(QCoreApplication::translate("WaitingConnectionWindow", "Dialog", nullptr));
        top_label->setText(QCoreApplication::translate("WaitingConnectionWindow", "Welcome ! ", nullptr));
        note_label->setText(QCoreApplication::translate("WaitingConnectionWindow", "Make sure that you have connected the CAN bus cable to your USB port.\n"
"Please ensure the connection is secure.\n"
"", nullptr));
        icon_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WaitingConnectionWindow: public Ui_WaitingConnectionWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAITINGCONNECTION_WINDOW_H
