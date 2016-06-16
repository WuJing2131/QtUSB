/********************************************************************************
** Form generated from reading UI file 'aboutform.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTFORM_H
#define UI_ABOUTFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AboutForm
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;

    void setupUi(QWidget *AboutForm)
    {
        if (AboutForm->objectName().isEmpty())
            AboutForm->setObjectName(QStringLiteral("AboutForm"));
        AboutForm->resize(550, 387);
        AboutForm->setStyleSheet(QStringLiteral("color: rgb(0, 170, 127);"));
        label = new QLabel(AboutForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 221, 31));
        QFont font;
        font.setFamily(QStringLiteral("Segoe Print"));
        font.setPointSize(20);
        label->setFont(font);
        label_2 = new QLabel(AboutForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(70, 100, 221, 31));
        QFont font1;
        font1.setFamily(QStringLiteral("Segoe Print"));
        font1.setPointSize(14);
        label_2->setFont(font1);
        label_3 = new QLabel(AboutForm);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(70, 140, 221, 31));
        label_3->setFont(font1);
        label_4 = new QLabel(AboutForm);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(80, 220, 221, 31));
        label_4->setFont(font1);
        label_5 = new QLabel(AboutForm);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 60, 221, 31));
        QFont font2;
        font2.setFamily(QStringLiteral("Segoe Print"));
        font2.setPointSize(18);
        label_5->setFont(font2);
        label_6 = new QLabel(AboutForm);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(40, 180, 491, 31));
        label_6->setFont(font2);
        label_7 = new QLabel(AboutForm);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(260, 60, 221, 31));
        label_7->setFont(font2);
        label_8 = new QLabel(AboutForm);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(280, 100, 221, 31));
        label_8->setFont(font1);
        label_9 = new QLabel(AboutForm);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(220, 140, 311, 31));
        label_9->setFont(font1);
        label_10 = new QLabel(AboutForm);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(250, 260, 231, 31));
        label_10->setFont(font2);
        label_11 = new QLabel(AboutForm);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(250, 300, 221, 31));
        label_11->setFont(font2);
        label_12 = new QLabel(AboutForm);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(250, 340, 221, 31));
        label_12->setFont(font2);
        label_13 = new QLabel(AboutForm);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(70, 270, 121, 91));
        label_13->setAutoFillBackground(true);
        label_13->setTextFormat(Qt::PlainText);
        label_13->setPixmap(QPixmap(QString::fromUtf8(":/Images/Resources/images/zjupng100x100.png")));

        retranslateUi(AboutForm);

        QMetaObject::connectSlotsByName(AboutForm);
    } // setupUi

    void retranslateUi(QWidget *AboutForm)
    {
        AboutForm->setWindowTitle(QApplication::translate("AboutForm", "ZJU9506 USB SoftWare", 0));
        label->setText(QApplication::translate("AboutForm", "About QtUSB", 0));
        label_2->setText(QApplication::translate("AboutForm", "LibUSB-Win32", 0));
        label_3->setText(QApplication::translate("AboutForm", "QT5.6.0", 0));
        label_4->setText(QApplication::translate("AboutForm", "MSCV2015_X64", 0));
        label_5->setText(QApplication::translate("AboutForm", " library file:", 0));
        label_6->setText(QApplication::translate("AboutForm", "Integrated Development Environment:", 0));
        label_7->setText(QApplication::translate("AboutForm", "Graph Code:", 0));
        label_8->setText(QApplication::translate("AboutForm", "QCustomPlot", 0));
        label_9->setText(QApplication::translate("AboutForm", "modern plotting widget for Qt", 0));
        label_10->setText(QApplication::translate("AboutForm", "Author: ZJU9506", 0));
        label_11->setText(QApplication::translate("AboutForm", "Date: 2016.5.29 ", 0));
        label_12->setText(QApplication::translate("AboutForm", "Version: 0.0.1 ", 0));
        label_13->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AboutForm: public Ui_AboutForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTFORM_H
