#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "UsbReceivethread.h"
#include <QTimer>
#include <QString>
#include <qsettings.h>
#include "aboutform.hpp"
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QIntValidator>

enum CommunicationSettingSettingFormState {
	SettingFormClose,
	UsbSettingFormOpen,
	SerialPortSettingFormOpen,
};

enum CommunicationInterfaceState {
	NoInterfaceConnected,
	UsbInterfaceConnected,
	SerialPortInterfaceConnected
};


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	struct SerialPortSettings {
		QString name;
		qint32 baudRate;
		QString stringBaudRate;
		QSerialPort::DataBits dataBits;
		QString stringDataBits;
		QSerialPort::Parity parity;
		QString stringParity;
		QSerialPort::StopBits stopBits;
		QString stringStopBits;
		QSerialPort::FlowControl flowControl;
		QString stringFlowControl;
		bool localEchoEnabled;
	};

	void setupDemo(int demoIndex);
	void setupQuadraticDemo(QCustomPlot *customPlot);
	void setupSimpleDemo(QCustomPlot *customPlot);
	void setupSincScatterDemo(QCustomPlot *customPlot);
	void setupScatterStyleDemo(QCustomPlot *customPlot);
	void setupLineStyleDemo(QCustomPlot *customPlot);
	void setupScatterPixmapDemo(QCustomPlot *customPlot);
	void setupDateDemo(QCustomPlot *customPlot);
	void setupTextureBrushDemo(QCustomPlot *customPlot);
	void setupMultiAxisDemo(QCustomPlot *customPlot);
	void setupLogarithmicDemo(QCustomPlot *customPlot);
	void setupRealtimeDataDemo(QCustomPlot *customPlot);
	void setupParametricCurveDemo(QCustomPlot *customPlot);
	void setupBarChartDemo(QCustomPlot *customPlot);
	void setupStatisticalDemo(QCustomPlot *customPlot);
	void setupSimpleItemDemo(QCustomPlot *customPlot);
	void setupItemDemo(QCustomPlot *customPlot);
	void setupStyledDemo(QCustomPlot *customPlot);
	void setupAdvancedAxesDemo(QCustomPlot *customPlot);
	void setupColorMapDemo(QCustomPlot *customPlot);
	void setupFinancialDemo(QCustomPlot *customPlot);

	void setupPlayground(QCustomPlot *customPlot);
	int hexToStr(char *hex, char *ch);
	int hexCharToValue(const char ch);
	char valueToHexCh(const int value);
	QByteArray hexStringtoByteArray(QString hex);
	void formatString(QString &org, int n = 2, const QChar &ch = QChar(' '));
	QByteArray hexStringtoByteArrayWithSapce(QString hex);
	bool InitUsb(unsigned short UsbVid, unsigned short UsbPid);
	bool WriteData(const QString& pBuffer);
	bool ReadData(QString &pBuffer);
	//bool WaitResponse(int pTimeout, QStringList &pResponseList);
	void CloseUsb();
	bool SendCommand(const QString &pCommand, QStringList &pResponseList);

	void UsbSettingFormShow();
	void SerialPortSettingFormShow();
	void UsbSettingFormHide();
	void SerialPortSettingFormHide();

	void WriteSettings();
	void ReadSettings();
	void fillPortsParameters();
	void fillPortsInfo();
	void updateSerialPortSettings();
	
private slots:
	void realtimeDataSlot();
	void bracketDataSlot();
	void screenShot();
	void allScreenShots();
	void OnUsbOpen();
	void OnUsbSettingTarged();
	void OnUsbLogerTarged();
	void OnGraphTypeChanged();
	void OnGraphSet();
	void OnUsbLogTextChanged();
	void OnUsbReceiveResponse(const QString &s,int len);
	void OnUsbSendmenuTarged();
	void OnUSBSendClicked();
	void OnUSBEPINChanged();
	void OnUSBEPOUTChanged();
	void OnFileSaveTarged();
	void OnUSBSendTextChanged();
	void OnSoftwareInfoTarged();
	void OnSerialPortSettingTarged();
	void showPortInfo(int idx);
	void checkCustomBaudRatePolicy(int idx);
	void checkCustomDevicePathPolicy(int idx);
	void OnSerialportOpen();
	void OnSerialPortClose();
	void SerialportwriteData(const QByteArray &data);
	void SerialportreadData();
	void SerialporthandleError(QSerialPort::SerialPortError error);
	void DataReceiveHexFormatchanged();
	void DataReceiveACSIIFormatchanged();
	
private:
	Ui::MainWindow *ui;
	//Ui::MainWindowClass ui;
	QString demoName;
	QTimer dataTimer;
	QCPItemTracer *itemDemoPhaseTracer;
	int currentDemoIndex;
	QDateTime CurrentSystemtime;
	int GraphType = 15;

	UsbReceiveThread *m_UsbReceiveThread;

	QComboBox *GraphTypeComb;
	QPushButton *GraphRunbtn;
	QLabel *SendShow;
	QLabel *ReceiveShow;

	CommunicationSettingSettingFormState  m_CommunicationSettingSettingFormState = CommunicationSettingSettingFormState::UsbSettingFormOpen;
	CommunicationInterfaceState m_CommunicationInterfaceState = CommunicationInterfaceState::NoInterfaceConnected;
	
	SerialPortSettings currentSettings;
	QIntValidator *intValidator;
	QSerialPort *serial;
};

#endif // MAINWINDOW_H
