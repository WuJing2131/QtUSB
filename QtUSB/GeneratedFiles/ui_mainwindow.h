/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionUSBSetting;
    QAction *actionLogerShow;
    QAction *action_Save;
    QAction *actionUSBSend;
    QAction *actionSoftware_Info;
    QAction *actionSerialPortStart;
    QAction *actionUSBStart;
    QAction *actionSerialSetting;
    QAction *actionExit;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QCustomPlot *customPlot;
    QMenuBar *menuBar;
    QMenu *menu_USB;
    QMenu *menu_File;
    QMenu *menuAbout;
    QMenu *menuDebug;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *USBSettingdockWidget;
    QWidget *dockWidgetContents;
    QGroupBox *USBgroupBox;
    QLabel *USBVIDlabel;
    QLabel *USBPIDlabel;
    QLineEdit *USBVID;
    QLineEdit *USBPID;
    QLineEdit *USBEPIN;
    QLabel *USBEPINlable;
    QLabel *USBVIDlabel_3;
    QLineEdit *USBEPOUT;
    QGroupBox *ReceiveSettingsgroupBox;
    QRadioButton *DataReceiveHexFormatradioButton;
    QRadioButton *DataReceiveASCIIFormatradioButton;
    QPushButton *USBOPEN;
    QPushButton *SerialPortOPEN;
    QGroupBox *SerialPortgroupBox;
    QLabel *USBVIDlabel_2;
    QLabel *USBPIDlabel_2;
    QLabel *USBEPINlable_2;
    QLabel *USBVIDlabel_4;
    QLabel *USBEPINlable_3;
    QLabel *USBVIDlabel_5;
    QComboBox *serialPortInfoListBox;
    QComboBox *baudRateBox;
    QComboBox *dataBitsBox;
    QComboBox *stopBitsBox;
    QComboBox *parityBox;
    QComboBox *flowControlBox;
    QDockWidget *LogdockWidget;
    QWidget *LogdockWidgetContents;
    QHBoxLayout *horizontalLayout1;
    QTextEdit *USBLOG;
    QVBoxLayout *usbSendverticalLayout;
    QTextEdit *USBSENDINPUT;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *UsbSendgroupBox;
    QRadioButton *UsbSendHexradioButton;
    QRadioButton *UsbSendASCiiradioButton;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *USBSEND;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(802, 642);
        QIcon icon;
        icon.addFile(QStringLiteral(":/image/Resources/images/zju_ico32x32.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setAutoFillBackground(true);
        MainWindow->setStyleSheet(QLatin1String("color: rgb(0, 170, 127);\n"
"alternate-background-color: rgb(0, 170, 127);"));
        MainWindow->setIconSize(QSize(24, 24));
        actionUSBSetting = new QAction(MainWindow);
        actionUSBSetting->setObjectName(QStringLiteral("actionUSBSetting"));
        actionUSBSetting->setCheckable(true);
        actionLogerShow = new QAction(MainWindow);
        actionLogerShow->setObjectName(QStringLiteral("actionLogerShow"));
        actionLogerShow->setCheckable(true);
        action_Save = new QAction(MainWindow);
        action_Save->setObjectName(QStringLiteral("action_Save"));
        actionUSBSend = new QAction(MainWindow);
        actionUSBSend->setObjectName(QStringLiteral("actionUSBSend"));
        actionUSBSend->setCheckable(true);
        actionSoftware_Info = new QAction(MainWindow);
        actionSoftware_Info->setObjectName(QStringLiteral("actionSoftware_Info"));
        actionSerialPortStart = new QAction(MainWindow);
        actionSerialPortStart->setObjectName(QStringLiteral("actionSerialPortStart"));
        actionSerialPortStart->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/image/Resources/images/disconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon1.addFile(QStringLiteral(":/image/Resources/images/connect.png"), QSize(), QIcon::Normal, QIcon::On);
        actionSerialPortStart->setIcon(icon1);
        actionUSBStart = new QAction(MainWindow);
        actionUSBStart->setObjectName(QStringLiteral("actionUSBStart"));
        actionUSBStart->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/image/Resources/images/usb_128dis.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon2.addFile(QStringLiteral(":/image/Resources/images/usb_128_con.png"), QSize(), QIcon::Normal, QIcon::On);
        actionUSBStart->setIcon(icon2);
        actionSerialSetting = new QAction(MainWindow);
        actionSerialSetting->setObjectName(QStringLiteral("actionSerialSetting"));
        actionSerialSetting->setCheckable(true);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        customPlot = new QCustomPlot(centralWidget);
        customPlot->setObjectName(QStringLiteral("customPlot"));

        horizontalLayout->addWidget(customPlot);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 802, 29));
        QFont font;
        font.setFamily(QStringLiteral("Segoe Print"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        menuBar->setFont(font);
        menuBar->setStyleSheet(QLatin1String("\n"
"color: rgb(0, 170, 127);"));
        menu_USB = new QMenu(menuBar);
        menu_USB->setObjectName(QStringLiteral("menu_USB"));
        QFont font1;
        font1.setFamily(QStringLiteral("Segoe Print"));
        font1.setBold(true);
        font1.setWeight(75);
        menu_USB->setFont(font1);
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        menu_File->setFont(font1);
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        menuAbout->setFont(font1);
        menuDebug = new QMenu(menuBar);
        menuDebug->setObjectName(QStringLiteral("menuDebug"));
        menuDebug->setFont(font1);
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QPalette palette;
        QBrush brush(QColor(0, 170, 127, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        mainToolBar->setPalette(palette);
        mainToolBar->setStyleSheet(QStringLiteral("color: rgb(0, 170, 127);"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setFont(font);
        MainWindow->setStatusBar(statusBar);
        USBSettingdockWidget = new QDockWidget(MainWindow);
        USBSettingdockWidget->setObjectName(QStringLiteral("USBSettingdockWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(USBSettingdockWidget->sizePolicy().hasHeightForWidth());
        USBSettingdockWidget->setSizePolicy(sizePolicy);
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        USBSettingdockWidget->setPalette(palette1);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        USBgroupBox = new QGroupBox(dockWidgetContents);
        USBgroupBox->setObjectName(QStringLiteral("USBgroupBox"));
        USBgroupBox->setGeometry(QRect(20, 0, 161, 161));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        USBgroupBox->setPalette(palette2);
        QFont font2;
        font2.setFamily(QStringLiteral("Forte"));
        font2.setPointSize(22);
        font2.setBold(true);
        font2.setWeight(75);
        USBgroupBox->setFont(font2);
        USBVIDlabel = new QLabel(USBgroupBox);
        USBVIDlabel->setObjectName(QStringLiteral("USBVIDlabel"));
        USBVIDlabel->setGeometry(QRect(10, 30, 61, 21));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        USBVIDlabel->setPalette(palette3);
        QFont font3;
        font3.setFamily(QStringLiteral("Segoe Script"));
        font3.setPointSize(18);
        font3.setBold(true);
        font3.setItalic(true);
        font3.setWeight(75);
        USBVIDlabel->setFont(font3);
        USBPIDlabel = new QLabel(USBgroupBox);
        USBPIDlabel->setObjectName(QStringLiteral("USBPIDlabel"));
        USBPIDlabel->setGeometry(QRect(10, 60, 61, 21));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        USBPIDlabel->setPalette(palette4);
        USBPIDlabel->setFont(font3);
        USBVID = new QLineEdit(USBgroupBox);
        USBVID->setObjectName(QStringLiteral("USBVID"));
        USBVID->setGeometry(QRect(80, 29, 71, 21));
        USBPID = new QLineEdit(USBgroupBox);
        USBPID->setObjectName(QStringLiteral("USBPID"));
        USBPID->setGeometry(QRect(80, 59, 71, 21));
        USBEPIN = new QLineEdit(USBgroupBox);
        USBEPIN->setObjectName(QStringLiteral("USBEPIN"));
        USBEPIN->setGeometry(QRect(110, 90, 41, 21));
        USBEPINlable = new QLabel(USBgroupBox);
        USBEPINlable->setObjectName(QStringLiteral("USBEPINlable"));
        USBEPINlable->setGeometry(QRect(10, 91, 71, 21));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        USBEPINlable->setPalette(palette5);
        USBEPINlable->setFont(font3);
        USBVIDlabel_3 = new QLabel(USBgroupBox);
        USBVIDlabel_3->setObjectName(QStringLiteral("USBVIDlabel_3"));
        USBVIDlabel_3->setGeometry(QRect(10, 120, 101, 21));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Text, brush);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        USBVIDlabel_3->setPalette(palette6);
        USBVIDlabel_3->setFont(font3);
        USBEPOUT = new QLineEdit(USBgroupBox);
        USBEPOUT->setObjectName(QStringLiteral("USBEPOUT"));
        USBEPOUT->setGeometry(QRect(110, 120, 41, 21));
        ReceiveSettingsgroupBox = new QGroupBox(dockWidgetContents);
        ReceiveSettingsgroupBox->setObjectName(QStringLiteral("ReceiveSettingsgroupBox"));
        ReceiveSettingsgroupBox->setGeometry(QRect(20, 170, 161, 81));
        QFont font4;
        font4.setFamily(QStringLiteral("Forte"));
        font4.setPointSize(16);
        font4.setBold(true);
        font4.setWeight(75);
        ReceiveSettingsgroupBox->setFont(font4);
        DataReceiveHexFormatradioButton = new QRadioButton(ReceiveSettingsgroupBox);
        DataReceiveHexFormatradioButton->setObjectName(QStringLiteral("DataReceiveHexFormatradioButton"));
        DataReceiveHexFormatradioButton->setGeometry(QRect(20, 20, 101, 31));
        QFont font5;
        font5.setFamily(QStringLiteral("Segoe Print"));
        font5.setPointSize(16);
        DataReceiveHexFormatradioButton->setFont(font5);
        DataReceiveHexFormatradioButton->setChecked(true);
        DataReceiveASCIIFormatradioButton = new QRadioButton(ReceiveSettingsgroupBox);
        DataReceiveASCIIFormatradioButton->setObjectName(QStringLiteral("DataReceiveASCIIFormatradioButton"));
        DataReceiveASCIIFormatradioButton->setGeometry(QRect(20, 50, 101, 31));
        DataReceiveASCIIFormatradioButton->setFont(font5);
        USBOPEN = new QPushButton(dockWidgetContents);
        USBOPEN->setObjectName(QStringLiteral("USBOPEN"));
        USBOPEN->setGeometry(QRect(50, 320, 91, 31));
        QFont font6;
        font6.setFamily(QStringLiteral("Segoe Print"));
        font6.setPointSize(14);
        USBOPEN->setFont(font6);
        SerialPortOPEN = new QPushButton(dockWidgetContents);
        SerialPortOPEN->setObjectName(QStringLiteral("SerialPortOPEN"));
        SerialPortOPEN->setGeometry(QRect(50, 320, 91, 31));
        SerialPortOPEN->setFont(font6);
        SerialPortgroupBox = new QGroupBox(dockWidgetContents);
        SerialPortgroupBox->setObjectName(QStringLiteral("SerialPortgroupBox"));
        SerialPortgroupBox->setGeometry(QRect(160, 90, 171, 211));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Text, brush);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        SerialPortgroupBox->setPalette(palette7);
        SerialPortgroupBox->setFont(font2);
        USBVIDlabel_2 = new QLabel(SerialPortgroupBox);
        USBVIDlabel_2->setObjectName(QStringLiteral("USBVIDlabel_2"));
        USBVIDlabel_2->setGeometry(QRect(10, 30, 61, 21));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Active, QPalette::Text, brush);
        palette8.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette8.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        USBVIDlabel_2->setPalette(palette8);
        USBVIDlabel_2->setFont(font3);
        USBPIDlabel_2 = new QLabel(SerialPortgroupBox);
        USBPIDlabel_2->setObjectName(QStringLiteral("USBPIDlabel_2"));
        USBPIDlabel_2->setGeometry(QRect(10, 60, 71, 21));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette9.setBrush(QPalette::Active, QPalette::Text, brush);
        palette9.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette9.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        USBPIDlabel_2->setPalette(palette9);
        USBPIDlabel_2->setFont(font3);
        USBEPINlable_2 = new QLabel(SerialPortgroupBox);
        USBEPINlable_2->setObjectName(QStringLiteral("USBEPINlable_2"));
        USBEPINlable_2->setGeometry(QRect(10, 91, 71, 21));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette10.setBrush(QPalette::Active, QPalette::Text, brush);
        palette10.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette10.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        USBEPINlable_2->setPalette(palette10);
        USBEPINlable_2->setFont(font3);
        USBVIDlabel_4 = new QLabel(SerialPortgroupBox);
        USBVIDlabel_4->setObjectName(QStringLiteral("USBVIDlabel_4"));
        USBVIDlabel_4->setGeometry(QRect(10, 120, 101, 21));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette11.setBrush(QPalette::Active, QPalette::Text, brush);
        palette11.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette11.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        USBVIDlabel_4->setPalette(palette11);
        USBVIDlabel_4->setFont(font3);
        USBEPINlable_3 = new QLabel(SerialPortgroupBox);
        USBEPINlable_3->setObjectName(QStringLiteral("USBEPINlable_3"));
        USBEPINlable_3->setGeometry(QRect(10, 150, 71, 21));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette12.setBrush(QPalette::Active, QPalette::Text, brush);
        palette12.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette12.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        USBEPINlable_3->setPalette(palette12);
        USBEPINlable_3->setFont(font3);
        USBVIDlabel_5 = new QLabel(SerialPortgroupBox);
        USBVIDlabel_5->setObjectName(QStringLiteral("USBVIDlabel_5"));
        USBVIDlabel_5->setGeometry(QRect(10, 179, 101, 21));
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette13.setBrush(QPalette::Active, QPalette::Text, brush);
        palette13.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette13.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette13.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette13.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette13.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        USBVIDlabel_5->setPalette(palette13);
        USBVIDlabel_5->setFont(font3);
        serialPortInfoListBox = new QComboBox(SerialPortgroupBox);
        serialPortInfoListBox->setObjectName(QStringLiteral("serialPortInfoListBox"));
        serialPortInfoListBox->setGeometry(QRect(80, 30, 81, 22));
        serialPortInfoListBox->setFont(font6);
        baudRateBox = new QComboBox(SerialPortgroupBox);
        baudRateBox->setObjectName(QStringLiteral("baudRateBox"));
        baudRateBox->setGeometry(QRect(80, 60, 81, 22));
        baudRateBox->setFont(font6);
        dataBitsBox = new QComboBox(SerialPortgroupBox);
        dataBitsBox->setObjectName(QStringLiteral("dataBitsBox"));
        dataBitsBox->setGeometry(QRect(80, 90, 81, 22));
        dataBitsBox->setFont(font6);
        stopBitsBox = new QComboBox(SerialPortgroupBox);
        stopBitsBox->setObjectName(QStringLiteral("stopBitsBox"));
        stopBitsBox->setGeometry(QRect(80, 120, 81, 22));
        stopBitsBox->setFont(font6);
        parityBox = new QComboBox(SerialPortgroupBox);
        parityBox->setObjectName(QStringLiteral("parityBox"));
        parityBox->setGeometry(QRect(80, 150, 81, 22));
        parityBox->setFont(font6);
        flowControlBox = new QComboBox(SerialPortgroupBox);
        flowControlBox->setObjectName(QStringLiteral("flowControlBox"));
        flowControlBox->setGeometry(QRect(80, 180, 81, 22));
        flowControlBox->setFont(font6);
        USBSettingdockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), USBSettingdockWidget);
        LogdockWidget = new QDockWidget(MainWindow);
        LogdockWidget->setObjectName(QStringLiteral("LogdockWidget"));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette14.setBrush(QPalette::Active, QPalette::Text, brush);
        palette14.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette14.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette14.setBrush(QPalette::Active, QPalette::Window, brush);
        palette14.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette14.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        LogdockWidget->setPalette(palette14);
        LogdockWidget->setStyleSheet(QStringLiteral(""));
        LogdockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
        LogdockWidgetContents = new QWidget();
        LogdockWidgetContents->setObjectName(QStringLiteral("LogdockWidgetContents"));
        horizontalLayout1 = new QHBoxLayout(LogdockWidgetContents);
        horizontalLayout1->setSpacing(6);
        horizontalLayout1->setContentsMargins(11, 11, 11, 11);
        horizontalLayout1->setObjectName(QStringLiteral("horizontalLayout1"));
        USBLOG = new QTextEdit(LogdockWidgetContents);
        USBLOG->setObjectName(QStringLiteral("USBLOG"));
        USBLOG->setEnabled(true);
        QFont font7;
        font7.setFamily(QStringLiteral("SimSun-ExtB"));
        font7.setPointSize(12);
        font7.setBold(true);
        font7.setWeight(75);
        USBLOG->setFont(font7);
        USBLOG->setTabStopWidth(80);

        horizontalLayout1->addWidget(USBLOG);

        usbSendverticalLayout = new QVBoxLayout();
        usbSendverticalLayout->setSpacing(6);
        usbSendverticalLayout->setObjectName(QStringLiteral("usbSendverticalLayout"));
        usbSendverticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        USBSENDINPUT = new QTextEdit(LogdockWidgetContents);
        USBSENDINPUT->setObjectName(QStringLiteral("USBSENDINPUT"));
        USBSENDINPUT->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(USBSENDINPUT->sizePolicy().hasHeightForWidth());
        USBSENDINPUT->setSizePolicy(sizePolicy1);
        USBSENDINPUT->setFont(font7);
        USBSENDINPUT->setTabStopWidth(80);

        usbSendverticalLayout->addWidget(USBSENDINPUT);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        UsbSendgroupBox = new QGroupBox(LogdockWidgetContents);
        UsbSendgroupBox->setObjectName(QStringLiteral("UsbSendgroupBox"));
        sizePolicy.setHeightForWidth(UsbSendgroupBox->sizePolicy().hasHeightForWidth());
        UsbSendgroupBox->setSizePolicy(sizePolicy);
        QFont font8;
        font8.setFamily(QStringLiteral("Forte"));
        font8.setPointSize(20);
        font8.setBold(true);
        font8.setWeight(75);
        UsbSendgroupBox->setFont(font8);
        UsbSendHexradioButton = new QRadioButton(UsbSendgroupBox);
        UsbSendHexradioButton->setObjectName(QStringLiteral("UsbSendHexradioButton"));
        UsbSendHexradioButton->setGeometry(QRect(60, 35, 161, 21));
        UsbSendHexradioButton->setFont(font6);
        UsbSendHexradioButton->setChecked(true);
        UsbSendASCiiradioButton = new QRadioButton(UsbSendgroupBox);
        UsbSendASCiiradioButton->setObjectName(QStringLiteral("UsbSendASCiiradioButton"));
        UsbSendASCiiradioButton->setGeometry(QRect(60, 60, 161, 31));
        UsbSendASCiiradioButton->setFont(font6);

        horizontalLayout_2->addWidget(UsbSendgroupBox);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        USBSEND = new QPushButton(LogdockWidgetContents);
        USBSEND->setObjectName(QStringLiteral("USBSEND"));
        QFont font9;
        font9.setFamily(QStringLiteral("Segoe Print"));
        font9.setPointSize(18);
        font9.setBold(true);
        font9.setWeight(75);
        USBSEND->setFont(font9);

        verticalLayout->addWidget(USBSEND);


        horizontalLayout_2->addLayout(verticalLayout);


        usbSendverticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout1->addLayout(usbSendverticalLayout);

        LogdockWidget->setWidget(LogdockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), LogdockWidget);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menu_USB->menuAction());
        menuBar->addAction(menuDebug->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menu_USB->addAction(actionUSBSetting);
        menu_USB->addAction(actionSerialSetting);
        menu_USB->addAction(actionLogerShow);
        menu_USB->addAction(actionUSBSend);
        menu_File->addAction(action_Save);
        menu_File->addAction(actionExit);
        menuAbout->addAction(actionSoftware_Info);
        menuDebug->addAction(actionSerialPortStart);
        menuDebug->addAction(actionUSBStart);
        mainToolBar->addAction(actionSerialPortStart);
        mainToolBar->addAction(actionUSBStart);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ZJU9506QtUSB", 0));
        actionUSBSetting->setText(QApplication::translate("MainWindow", "USBSetting", 0));
        actionLogerShow->setText(QApplication::translate("MainWindow", "LogerShow", 0));
        action_Save->setText(QApplication::translate("MainWindow", "&Save", 0));
        actionUSBSend->setText(QApplication::translate("MainWindow", "DataSend", 0));
#ifndef QT_NO_TOOLTIP
        actionUSBSend->setToolTip(QApplication::translate("MainWindow", "DataSend", 0));
#endif // QT_NO_TOOLTIP
        actionSoftware_Info->setText(QApplication::translate("MainWindow", "Software Info", 0));
        actionSerialPortStart->setText(QApplication::translate("MainWindow", "SerialPortOpen", 0));
        actionUSBStart->setText(QApplication::translate("MainWindow", "USBOpen", 0));
        actionSerialSetting->setText(QApplication::translate("MainWindow", "SerialSetting", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        menu_USB->setTitle(QApplication::translate("MainWindow", "&Setting", 0));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0));
        menuAbout->setTitle(QApplication::translate("MainWindow", "About", 0));
        menuDebug->setTitle(QApplication::translate("MainWindow", "Debug", 0));
        USBgroupBox->setTitle(QApplication::translate("MainWindow", "USB", 0));
        USBVIDlabel->setText(QApplication::translate("MainWindow", "VID", 0));
        USBPIDlabel->setText(QApplication::translate("MainWindow", "PID", 0));
        USBVID->setText(QApplication::translate("MainWindow", "1234", 0));
        USBPID->setText(QApplication::translate("MainWindow", "4321", 0));
        USBEPIN->setText(QApplication::translate("MainWindow", "81", 0));
        USBEPINlable->setText(QApplication::translate("MainWindow", "EPIN", 0));
        USBVIDlabel_3->setText(QApplication::translate("MainWindow", "EPOUT", 0));
        USBEPOUT->setText(QApplication::translate("MainWindow", "02", 0));
        ReceiveSettingsgroupBox->setTitle(QApplication::translate("MainWindow", "Receive Setting", 0));
        DataReceiveHexFormatradioButton->setText(QApplication::translate("MainWindow", "Hex", 0));
        DataReceiveASCIIFormatradioButton->setText(QApplication::translate("MainWindow", "ASCII", 0));
        USBOPEN->setText(QApplication::translate("MainWindow", "Open", 0));
        SerialPortOPEN->setText(QApplication::translate("MainWindow", "Open", 0));
        SerialPortgroupBox->setTitle(QApplication::translate("MainWindow", "SerialPort", 0));
        USBVIDlabel_2->setText(QApplication::translate("MainWindow", "Port", 0));
        USBPIDlabel_2->setText(QApplication::translate("MainWindow", "BdRt", 0));
        USBEPINlable_2->setText(QApplication::translate("MainWindow", "DtBt", 0));
        USBVIDlabel_4->setText(QApplication::translate("MainWindow", "StBt", 0));
        USBEPINlable_3->setText(QApplication::translate("MainWindow", "Prty", 0));
        USBVIDlabel_5->setText(QApplication::translate("MainWindow", "FlCt", 0));
        UsbSendgroupBox->setTitle(QApplication::translate("MainWindow", "Data Send Setting", 0));
        UsbSendHexradioButton->setText(QApplication::translate("MainWindow", "HexFormat", 0));
        UsbSendASCiiradioButton->setText(QApplication::translate("MainWindow", "ASCIIFormat", 0));
        USBSEND->setText(QApplication::translate("MainWindow", "Send", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
