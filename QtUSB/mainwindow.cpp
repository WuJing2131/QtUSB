#include "mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include "libusb\include\lusb0_usb.h"

//USB *******************************
#define MY_CONFIG 1
#define MY_INTF 0
// Device endpoint(s)
//#define EP_IN 0x81
//#define EP_OUT 0x02
uint g_USBVID;
uint g_USBPID;
uint  EP_IN = 0x81;
uint  EP_OUT = 0x02;

usb_dev_handle *gusb_handle;
HANDLE m_hDevice = INVALID_HANDLE_VALUE;
//CWinThread *m_pReadReportThread = NULL;
OVERLAPPED ReadOverlapped;
char ReadReportBuffer[1024] = { 0 };
BYTE KeyStatus = 0;
volatile BOOL m_bReadReportRunFlag = FALSE;

QString g_szSend;
//USB *******************************

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

void MainWindow::UsbSettingFormShow()
{
	
	ui->USBgroupBox->setGeometry(QRect(20, 0, 161, 161));
	ui->USBgroupBox->show();
	ui->ReceiveSettingsgroupBox->setGeometry(QRect(20, 170, 161, 81));
	ui->USBOPEN->show();
}

void MainWindow::UsbSettingFormHide()
{
	ui->USBgroupBox->hide();
	ui->USBOPEN->hide();
}
void MainWindow::SerialPortSettingFormShow()
{
	ui->ReceiveSettingsgroupBox->setGeometry(QRect(20, 230, 161, 81));
	ui->SerialPortgroupBox->setGeometry(QRect(15, 0, 171, 211));
	ui->SerialPortgroupBox->show();
	ui->SerialPortOPEN->show();
	
}


void MainWindow::SerialPortSettingFormHide()
{
	ui->SerialPortgroupBox->hide();
	ui->SerialPortOPEN->hide();
}


void MainWindow::updateSerialPortSettings()
{
	currentSettings.name = ui->serialPortInfoListBox->currentText();

	if (ui->baudRateBox->currentIndex() == 4) {
		currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
	}
	else {
		currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
			ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
	}
	currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

	currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
		ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
	currentSettings.stringDataBits = ui->dataBitsBox->currentText();

	currentSettings.parity = static_cast<QSerialPort::Parity>(
		ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
	currentSettings.stringParity = ui->parityBox->currentText();

	currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
		ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
	currentSettings.stringStopBits = ui->stopBitsBox->currentText();

	currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
		ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
	currentSettings.stringFlowControl = ui->flowControlBox->currentText();
}

void MainWindow::OnSerialportOpen()
{
	if (ui->SerialPortOPEN->text() == "Open")
	{
		
		ui->USBSEND->setEnabled(true);
		updateSerialPortSettings();
		serial->setPortName(currentSettings.name);
		serial->setBaudRate(currentSettings.baudRate);
		serial->setDataBits(currentSettings.dataBits);
		serial->setParity(currentSettings.parity);
		serial->setStopBits(currentSettings.stopBits);
		serial->setFlowControl(currentSettings.flowControl);
		if (serial->open(QIODevice::ReadWrite)) {
			ui->USBLOG->append(tr("State: SerialPort Connected to %1 : %2, %3, %4, %5, %6")
				.arg(currentSettings.name).arg(currentSettings.stringBaudRate).arg(currentSettings.stringDataBits)
				.arg(currentSettings.stringParity).arg(currentSettings.stringStopBits).arg(currentSettings.stringFlowControl));
			ui->actionSerialPortStart->setChecked(true);
			m_CommunicationInterfaceState = CommunicationInterfaceState::SerialPortInterfaceConnected;
			ui->SerialPortOPEN->setText("Close");
		}
		else {
			QMessageBox::critical(this, tr("State: Error"), serial->errorString());
			ui->actionSerialPortStart->setChecked(false);
			ui->USBLOG->append(tr("State: Open error"));
			m_CommunicationInterfaceState = CommunicationInterfaceState::NoInterfaceConnected;
		}
	}
	else
	{
		ui->actionSerialPortStart->setChecked(false);
		ui->USBSEND->setEnabled(false);
		OnSerialPortClose();
		ui->SerialPortOPEN->setText("Open");
		m_CommunicationInterfaceState = CommunicationInterfaceState::NoInterfaceConnected;
	}

	
}

void MainWindow::OnSerialPortClose()
{
	if (serial->isOpen())
		serial->close();
	ui->USBLOG->append(tr("State: SerialPort Close!"));
}

void MainWindow::SerialportwriteData(const QByteArray &data)
{
	serial->write(data);
}

void MainWindow::SerialportreadData()
{
	QByteArray data = serial->readAll();
	int ret = data.length();
	if (ui->DataReceiveHexFormatradioButton->isChecked())
	{
		QString szStr;
		for (int i = 0; i < ret; i++)
		{
			szStr += QString::number(data[i] & 0xFF, 16) + " ";
		}
		OnUsbReceiveResponse(szStr, ret);
	}
	else
	{
		QString szStr = data;
		OnUsbReceiveResponse(szStr, ret);
	}
	
}

void MainWindow::SerialporthandleError(QSerialPort::SerialPortError error)
{
	if (error == QSerialPort::ResourceError) {
		QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
		OnSerialPortClose();
	}
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setGeometry(200, 100, 1000, 600);

	serial = new QSerialPort(this);

	ui->USBSettingdockWidget->setFixedWidth(190);
	ui->LogdockWidget->setFixedHeight(200);
	ui->USBSENDINPUT->setFixedHeight(50);
	ui->USBSENDINPUT->setFixedWidth(400);
	QFont qF("Forte",16,QFont::Normal);
	QLabel *graphtext = new QLabel();
	graphtext->setFont(qF);
	graphtext->setText(" Graph Type: ");
	GraphTypeComb = new QComboBox();
	GraphTypeComb->setFixedWidth(175);
	GraphTypeComb->setFont(qF);
	GraphRunbtn = new QPushButton();
	GraphRunbtn->setText("Start");
	GraphRunbtn->setFont(qF);
	ui->mainToolBar->addWidget(graphtext);
	ui->mainToolBar->addWidget(GraphTypeComb);
	ui->mainToolBar->addWidget(GraphRunbtn);
	m_UsbReceiveThread = NULL;
	ui->USBgroupBox->setGeometry(QRect(20, 0, 161, 161));
	ui->ReceiveSettingsgroupBox->setGeometry(QRect(20, 170, 161, 81));
	ui->SerialPortgroupBox->hide(); 
	ui->SerialPortOPEN->hide();
	/*SendShow=new QLabel();
	ReceiveShow = new QLabel();
	QPixmap pixmapsend(":/image/Resources/images/send.png");
	SendShow->setPixmap(pixmapsend);
	QPixmap pixmapReceive(":/image/Resources/images/receive.png");
	ReceiveShow->setPixmap(pixmapReceive);
	ui->mainToolBar->addWidget(ReceiveShow);
	ui->mainToolBar->addWidget(SendShow);
	ReceiveShow->show();
	SendShow->show();*/
	ui->actionUSBSetting->setChecked(true);
	ui->actionLogerShow->setChecked(true);
	ui->actionUSBSend->setChecked(true);
	//connect(m_UsbReceiveThread, &UsbReceiveThread::Sendresponse, this, &MainWindow::OnUsbReceiveResponse);
	// connect(runButton, &QPushButton::clicked, this, &Dialog::transaction);
	connect(ui->actionUSBSetting, &QAction::triggered, this, &MainWindow::OnUsbSettingTarged);
	connect(ui->actionLogerShow, &QAction::triggered, this, &MainWindow::OnUsbLogerTarged); 
	connect(ui->actionUSBSend, &QAction::triggered, this, &MainWindow::OnUsbSendmenuTarged);
	connect(ui->USBOPEN, &QPushButton::clicked, this, &MainWindow::OnUsbOpen);
	connect(ui->action_Save, &QAction::triggered, this, &MainWindow::OnFileSaveTarged);
	connect(ui->actionSoftware_Info, &QAction::triggered, this, &MainWindow::OnSoftwareInfoTarged);
	connect(ui->actionUSBStart, &QAction::triggered, this, &MainWindow::OnUsbOpen);
	connect(ui->actionSerialSetting, &QAction::triggered, this, &MainWindow::OnSerialPortSettingTarged);
	connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
	GraphTypeComb->addItem(tr("Quadratic"));
	GraphTypeComb->addItem(tr("Simple"));
	GraphTypeComb->addItem(tr("SincScatter"));
	GraphTypeComb->addItem(tr("ScatterStyle"));
	GraphTypeComb->addItem(tr("ScatterPixmap"));
	GraphTypeComb->addItem(tr("LineStyle"));
	GraphTypeComb->addItem(tr("Date"));
	GraphTypeComb->addItem(tr("TextureBrush"));
	GraphTypeComb->addItem(tr("MultiAxis"));
	GraphTypeComb->addItem(tr("Logarithmic"));
	GraphTypeComb->addItem(tr("RealtimeData"));
	GraphTypeComb->addItem(tr("ParametricCurve"));
	GraphTypeComb->addItem(tr("BarChar"));
	GraphTypeComb->addItem(tr("Statistical"));
	GraphTypeComb->addItem(tr("SimpleItem"));
	GraphTypeComb->addItem(tr("Item"));
	GraphTypeComb->addItem(tr("Styled"));
	GraphTypeComb->addItem(tr("AdvancedAxes"));
	GraphTypeComb->addItem(tr("ColorMap"));
	GraphTypeComb->addItem(tr("Financial"));
	GraphTypeComb->setCurrentIndex(GraphType);
	connect(GraphTypeComb, &QComboBox::currentTextChanged, this, &MainWindow::OnGraphTypeChanged);
	connect(GraphRunbtn, &QPushButton::clicked, this, &MainWindow::OnGraphSet);
	connect(ui->USBLOG, &QTextEdit::textChanged, this, &MainWindow::OnUsbLogTextChanged);
	connect(ui->USBSEND, &QPushButton::clicked, this, &MainWindow::OnUSBSendClicked);
	connect(ui->USBEPIN, &QLineEdit::textChanged, this, &MainWindow::OnUSBEPINChanged);
	connect(ui->USBEPOUT, &QLineEdit::textChanged, this, &MainWindow::OnUSBEPOUTChanged);
	connect(ui->USBSENDINPUT, &QTextEdit::textChanged, this, &MainWindow::OnUSBSendTextChanged);
	ReadSettings();
	ui->USBVID->setText(QString::number(g_USBVID,16));
	ui->USBPID->setText(QString::number(g_USBPID, 16));
	ui->USBEPIN->setText(QString::number(EP_IN, 16));
	ui->USBEPOUT->setText(QString::number(EP_OUT, 16));
	/*ui->USBVID->setText(tr("C255"));
	ui->USBPID->setText(tr("4355"));
	ui->USBEPIN->setText(QString::number(EP_IN, 16));
	ui->USBEPOUT->setText(QString::number(EP_OUT, 16));*/
	ui->USBSENDINPUT->append(g_szSend);
	//ui->USBSENDINPUT->append(tr("EE 01 55 FF FC FF FF")); 
	ui->USBSEND->setEnabled(false);

	fillPortsParameters();
	fillPortsInfo();
	connect(ui->serialPortInfoListBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this, &MainWindow::showPortInfo);
	connect(ui->baudRateBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this, &MainWindow::checkCustomBaudRatePolicy);
	connect(ui->serialPortInfoListBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this, &MainWindow::checkCustomDevicePathPolicy);


	connect(ui->SerialPortOPEN, &QPushButton::clicked, this, &MainWindow::OnSerialportOpen);
	connect(ui->actionSerialPortStart, &QAction::triggered, this, &MainWindow::OnSerialportOpen);
	connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
		this, &MainWindow::SerialporthandleError);
	connect(serial, &QSerialPort::readyRead, this, &MainWindow::SerialportreadData);

	connect(ui->DataReceiveHexFormatradioButton, &QRadioButton::clicked, this, &MainWindow::DataReceiveHexFormatchanged);
	connect(ui->DataReceiveASCIIFormatradioButton, &QRadioButton::clicked, this, &MainWindow::DataReceiveACSIIFormatchanged);
	//    QImage *ppm = new QImage("../Image/gtk_yes.png");
	//    ui->USBOpen->setPixmap(QPixmap::fromImage(*ppm));
	// setupDemo(10);
	//setupPlayground(ui->customPlot);
	// 0:  setupQuadraticDemo(ui->customPlot);
	// 1:  setupSimpleDemo(ui->customPlot);
	// 2:  setupSincScatterDemo(ui->customPlot);
	// 3:  setupScatterStyleDemo(ui->customPlot);
	// 4:  setupScatterPixmapDemo(ui->customPlot);
	// 5:  setupLineStyleDemo(ui->customPlot);
	// 6:  setupDateDemo(ui->customPlot);
	// 7:  setupTextureBrushDemo(ui->customPlot);
	// 8:  setupMultiAxisDemo(ui->customPlot);
	// 9:  setupLogarithmicDemo(ui->customPlot);
	// 10: setupRealtimeDataDemo(ui->customPlot);
	// 11: setupParametricCurveDemo(ui->customPlot);
	// 12: setupBarChartDemo(ui->customPlot);
	// 13: setupStatisticalDemo(ui->customPlot);
	// 14: setupSimpleItemDemo(ui->customPlot);
	// 15: setupItemDemo(ui->customPlot);
	// 16: setupStyledDemo(ui->customPlot);
	// 17: setupAdvancedAxesDemo(ui->customPlot);
	// 18: setupColorMapDemo(ui->customPlot);
	// 19: setupFinancialDemo(ui->customPlot);

	// for making screenshots of the current demo or all demos (for website screenshots):
	//QTimer::singleShot(1500, this, SLOT(allScreenShots()));
	//QTimer::singleShot(4000, this, SLOT(screenShot()));
}

void MainWindow::DataReceiveHexFormatchanged()
{
	if (ui->DataReceiveHexFormatradioButton->isChecked()
		&& m_CommunicationInterfaceState == CommunicationInterfaceState::UsbInterfaceConnected)
	{
		m_UsbReceiveThread->HexAsciiFormatSetting(true);
	}
}

void MainWindow::DataReceiveACSIIFormatchanged()
{
	if (ui->DataReceiveASCIIFormatradioButton->isChecked()
		&& m_CommunicationInterfaceState == CommunicationInterfaceState::UsbInterfaceConnected)
	{
		m_UsbReceiveThread->HexAsciiFormatSetting(false);
	}
}

void MainWindow::showPortInfo(int idx)
{
	if (idx == -1)
		return;

	QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
	ui->USBLOG->append(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
	ui->USBLOG->append(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
	ui->USBLOG->append(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
	ui->USBLOG->append(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
	ui->USBLOG->append(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
	ui->USBLOG->append(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}


void MainWindow::checkCustomBaudRatePolicy(int idx)
{
	bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
	ui->baudRateBox->setEditable(isCustomBaudRate);
	if (isCustomBaudRate) {
		ui->baudRateBox->clearEditText();
		QLineEdit *edit = ui->baudRateBox->lineEdit();
		edit->setValidator(intValidator);
	}
}

void MainWindow::checkCustomDevicePathPolicy(int idx)
{
	bool isCustomPath = !ui->serialPortInfoListBox->itemData(idx).isValid();
	ui->serialPortInfoListBox->setEditable(isCustomPath);
	if (isCustomPath)
		ui->serialPortInfoListBox->clearEditText();
}

void MainWindow::fillPortsInfo()
{
	ui->serialPortInfoListBox->clear();
	QString description;
	QString manufacturer;
	QString serialNumber;
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
		QStringList list;
		description = info.description();
		manufacturer = info.manufacturer();
		serialNumber = info.serialNumber();
		list << info.portName()
			<< (!description.isEmpty() ? description : blankString)
			<< (!manufacturer.isEmpty() ? manufacturer : blankString)
			<< (!serialNumber.isEmpty() ? serialNumber : blankString)
			<< info.systemLocation()
			<< (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
			<< (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

		ui->serialPortInfoListBox->addItem(list.first(), list);
	}

	ui->serialPortInfoListBox->addItem(tr("Custom"));
}

void MainWindow::fillPortsParameters()
{
	intValidator = new QIntValidator(0, 4000000, this);

	ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

	ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
	ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
	ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
	ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
	ui->baudRateBox->addItem(tr("Custom"));

	ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
	ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
	ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
	ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
	ui->dataBitsBox->setCurrentIndex(3);

	ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
	ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
	ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
	ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
	ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

	ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
	ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
	ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

	ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
	ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
	ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void MainWindow::OnSoftwareInfoTarged()
{
	AboutForm *about = new AboutForm();
	about->show();
	//QMessageBox::about(this, "ZJU9506 USB Software","Libusb-win32 Qt MSCV2015_X64      ");
}

MainWindow::~MainWindow()
{
	WriteSettings();
	//delete m_UsbReceiveThread;

	delete GraphTypeComb;
	delete GraphRunbtn;
	delete intValidator;
	delete serial;

	delete ui;
}

void MainWindow::OnUSBSendTextChanged()
{
	g_szSend = ui->USBSENDINPUT->document()->toPlainText();
}

void MainWindow::OnUSBEPINChanged()
{
	EP_IN = hexStringtoByteArray(ui->USBEPIN->text())[0] & 0xFF;
	//WriteSettings();
}

void MainWindow::OnUSBEPOUTChanged()
{
	EP_OUT = hexStringtoByteArray(ui->USBEPOUT->text())[0] & 0xFF;
	//WriteSettings();
}

void MainWindow::OnUsbOpen()
{
	if (ui->USBOPEN->text() == "Open")
	{
		ui->actionUSBStart->setChecked(true);
		ui->USBSEND->setEnabled(true);
		QByteArray retVid, retPid;
		retVid=hexStringtoByteArray(ui->USBVID->text());
		retPid=hexStringtoByteArray(ui->USBPID->text());
		g_USBVID = retVid[0] * 256 + retVid[1] & 0xFFFF;
		g_USBPID = retPid[0] * 256 + retPid[1] & 0xFFFF;
		//WriteSettings();
		InitUsb(retVid[0]*256+retVid[1],retPid[0]*256+retPid[1]);
		m_UsbReceiveThread = new UsbReceiveThread();
		if (m_UsbReceiveThread ==NULL)
		{
			ui->USBLOG->append(tr("Thread Create Failed!"));
			CloseHandle(m_hDevice);
			m_hDevice = INVALID_HANDLE_VALUE;
			exit(0);
		}
		connect(m_UsbReceiveThread, &UsbReceiveThread::Sendresponse, this, &MainWindow::OnUsbReceiveResponse);
		bool isUsbThreadRunning = m_UsbReceiveThread->startProcess();
		ui->USBOPEN->setText("Close");
		m_CommunicationInterfaceState = CommunicationInterfaceState::UsbInterfaceConnected;
	}
	else
	{
		ui->actionUSBStart->setChecked(false);
		ui->USBSEND->setEnabled(false);
		m_UsbReceiveThread->stopProcess();
		delete m_UsbReceiveThread;
		CloseHandle(m_hDevice);
		CloseUsb();
		ui->USBOPEN->setText("Open");
		m_CommunicationInterfaceState = CommunicationInterfaceState::NoInterfaceConnected;
	}

	//CurrentSystemtime = QDateTime::currentDateTime();//获取系统现在的时间
	//QString str = CurrentSystemtime.toString("yyyy-MM-dd hh:mm:ss:~"); //设置显示格式
	//str += ":hello";
	//ui->USBLOG->append(str);


	//if(ui->USBOPEN->text()=="Open")
	//{
	//    QByteArray retVid,retPid;
	//    retVid=hexStringtoByteArray(ui->USBVID->text());
	//    retPid=hexStringtoByteArray(ui->USBPID->text());
	//    InitUsb(retVid[0]*256+retVid[1],retPid[0]*256+retPid[1]);
	//    // m_pReadReportThread = AfxBeginThread(ReadReportThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	//    if (m_pReadReportThread == NULL)
	//    {
	//		ui->USBLOG->append(tr("Thread Create Failed!"));
	//        CloseHandle(m_hDevice);
	//        m_hDevice = INVALID_HANDLE_VALUE;
	//        exit(0);
	//    }
	//    m_pReadReportThread->m_bAutoDelete = FALSE;
	//    m_pReadReportThread->ResumeThread();
	//    ui->USBOPEN->setText("Close");
	//}
	//else
	//{
	//    ui->USBOPEN->setText("Open");
	//}


	/*QString str;
	QByteArray ret;
	ret=hexStringtoByteArray(ui->USBVID->text());
	byte xx=ret[0];
	str.prepend(ret);
	QMessageBox::about(NULL,"Test",QString::number(xx,10));*/
}

int MainWindow::hexToStr(char *hex, char *ch)
{
	int high, low;
	int tmp = 0;
	if (hex == NULL || ch == NULL) {
		return -1;
	}

	if (strlen(hex) % 2 == 1) {
		return -2;
	}

	while (*hex) {
		high = hexCharToValue(*hex);
		if (high < 0) {
			*ch = '\0';
			return -3;
		}
		hex++; 
		low = hexCharToValue(*hex);
		if (low < 0) {
			*ch = '\0';
			return -3;
		}
		tmp = (high << 4) + low;
		*ch++ = (char)tmp;
		hex++;
	}
	*ch = '\0';
	return 0;
}

int MainWindow::hexCharToValue(const char ch) {
	int result = 0;
	if (ch >= '0' && ch <= '9') {
		result = (int)(ch - '0');
	}
	else if (ch >= 'a' && ch <= 'z') {
		result = (int)(ch - 'a') + 10;
	}
	else if (ch >= 'A' && ch <= 'Z') {
		result = (int)(ch - 'A') + 10;
	}
	else {
		result = -1;
	}
	return result;
}

char MainWindow::valueToHexCh(const int value)
{
	char result = '\0';
	if (value >= 0 && value <= 9) {
		result = (char)(value + 48); //48为ascii编码的‘0’字符编码值
	}
	else if (value >= 10 && value <= 15) {
		result = (char)(value - 10 + 65); //减去10则找出其在16进制的偏移量，65为ascii的'A'的字符编码值
	}
	else {
		;
	}

	return result;
}

/*************************************************
Function: hexStringtoByteArray()
Description: 十六进制字串转化为十六进制编码
Calls: formatString()
Called By:
Input: hex->待转化的十六进制字串
Output: NULL
Return: QByteArray
Others: NULL
*************************************************/
QByteArray MainWindow::hexStringtoByteArray(QString hex)
{
	QByteArray ret;
	hex = hex.trimmed();
	formatString(hex, 2, ' ');
	QStringList sl = hex.split(" ");
	foreach(QString s, sl)
	{
		if (!s.isEmpty())
			ret.append((char)s.toInt(0, 16) & 0xFF);
	}
	return ret;
}

QByteArray MainWindow::hexStringtoByteArrayWithSapce(QString hex)
{
	QByteArray ret;
	QStringList sl = hex.split(" ");
	foreach(QString s, sl)
	{
		if (!s.isEmpty())
			ret.append((char)s.toInt(0, 16) & 0xFF);
	}
	return ret;
}

/*************************************************
Function: formatString()
Description: 将十六进制字串每字节中间加空格分隔
Calls:
Called By:hexStringtoByteArray()
Input: org->待处理的字串
n->间隔数默认为2
ch->分隔标志，在此取空格
Output: NULL
Return: void
Others: NULL
*************************************************/
void MainWindow::formatString(QString &org, int n, const QChar &ch)
{
	int size = org.size();
	int space = qRound(size*1.0 / n + 0.5) - 1;
	if (space <= 0)
		return;
	for (int i = 0, pos = n; i<space; ++i, pos += (n + 1))
	{
		org.insert(pos, ch);
	}
}
void MainWindow::OnSerialPortSettingTarged()
{
	/*void UsbSettingFormShow();
	void SerialPortSettingFormShow();
	void UsbSettingFormHide();
	void SerialPortSettingFormHide();*/
	if (m_CommunicationSettingSettingFormState == CommunicationSettingSettingFormState::SettingFormClose)
	{
		ui->USBSettingdockWidget->show();
		UsbSettingFormHide();
		SerialPortSettingFormShow();
		m_CommunicationSettingSettingFormState = CommunicationSettingSettingFormState::SerialPortSettingFormOpen;
		ui->actionUSBSetting->setChecked(false);
	}
	else if (m_CommunicationSettingSettingFormState == CommunicationSettingSettingFormState::UsbSettingFormOpen)
	{
		UsbSettingFormHide();
		SerialPortSettingFormShow();
		m_CommunicationSettingSettingFormState = CommunicationSettingSettingFormState::SerialPortSettingFormOpen;
		ui->actionUSBSetting->setChecked(false);
	}
	else
	{
		ui->USBSettingdockWidget->hide();
		m_CommunicationSettingSettingFormState = CommunicationSettingSettingFormState::SettingFormClose;
		ui->actionUSBSetting->setChecked(false);
	}
}

void MainWindow::OnUsbSettingTarged()
{
	/*void UsbSettingFormShow();
	void SerialPortSettingFormShow();
	void UsbSettingFormHide();
	void SerialPortSettingFormHide();*/
	if (m_CommunicationSettingSettingFormState == CommunicationSettingSettingFormState::SettingFormClose)
	{
		ui->USBSettingdockWidget->show();
		SerialPortSettingFormHide();
		UsbSettingFormShow();
		m_CommunicationSettingSettingFormState = CommunicationSettingSettingFormState::UsbSettingFormOpen;
		ui->actionSerialSetting->setChecked(false);
	}
	else if (m_CommunicationSettingSettingFormState == CommunicationSettingSettingFormState::SerialPortSettingFormOpen)
	{
		SerialPortSettingFormHide();
		UsbSettingFormShow();
		m_CommunicationSettingSettingFormState = CommunicationSettingSettingFormState::UsbSettingFormOpen;
		ui->actionSerialSetting->setChecked(false);
	}
	else
	{
		ui->USBSettingdockWidget->hide();
		m_CommunicationSettingSettingFormState = CommunicationSettingSettingFormState::SettingFormClose;
		ui->actionSerialSetting->setChecked(false);
	}
	
}

void MainWindow::OnUsbLogerTarged()
{
	if (ui->LogdockWidget->isHidden())
	{
		ui->LogdockWidget->show();
		ui->actionLogerShow->setChecked(true);
	}
	else
	{
		ui->LogdockWidget->hide();
		ui->actionLogerShow->setChecked(false);

		ui->USBSENDINPUT->hide();
		ui->UsbSendgroupBox->hide();
		ui->UsbSendASCiiradioButton->hide();
		ui->USBSEND->hide();
		ui->UsbSendHexradioButton->hide();
		ui->actionUSBSend->setChecked(false);
	}
}

void MainWindow::OnUsbSendmenuTarged()
{
	if (ui->USBSENDINPUT->isHidden())
	{
		ui->LogdockWidget->show();
		ui->actionLogerShow->setChecked(true);

		ui->USBSENDINPUT->show();
		ui->UsbSendgroupBox->show();
		ui->UsbSendASCiiradioButton->show();
		ui->USBSEND->show();
		ui->UsbSendHexradioButton->show(); 
		ui->actionUSBSend->setChecked(true);

	}
		
	else
	{
		ui->actionUSBSend->setChecked(false);
		ui->USBSENDINPUT->hide();
		ui->UsbSendgroupBox->hide();
		ui->UsbSendASCiiradioButton->hide();
		ui->USBSEND->hide();
		ui->UsbSendHexradioButton->hide();
	}
}
void MainWindow::OnGraphTypeChanged()
{

	GraphType = GraphTypeComb->currentIndex();

}

void MainWindow::OnGraphSet()
{
	if (GraphRunbtn->text() == "Start")
	{

		dataTimer.start();
		ui->customPlot->setEnabled(true);
		ui->customPlot->clearGraphs();
		ui->customPlot->clearItems();
		ui->customPlot->clearMask();
		ui->customPlot->clearPlottables();
		// ui->customPlot->removeLayer(ui->customPlot->currentLayer());
		GraphRunbtn->setText("Stop");
		setupDemo(GraphType);
	}
	else
	{
		dataTimer.stop();

		ui->customPlot->setDisabled(true);

		GraphRunbtn->setText("Start");

	}
}

void MainWindow::OnUsbLogTextChanged()
{
	if (ui->USBLOG->document()->lineCount() >= 5000)
		ui->USBLOG->clear();
}

void MainWindow::OnUsbReceiveResponse(const QString &s,int len)
{
	CurrentSystemtime = QDateTime::currentDateTime();//获取系统现在的时间
	QString str = CurrentSystemtime.toString("yyyy-MM-dd hh:mm:ss >>>>>>"); //设置显示格式
	str += tr("Data>>>>>> %1 *****Length:%2*****").arg(s).arg(len);
	ui->USBLOG->append(str);
	//ui->USBLOG ->append(tr("Status: %1.").arg(s));
}

void MainWindow::OnUSBSendClicked()
{
	char buffer[1024];
	QByteArray baSend;
	int sendlen;
	if (ui->UsbSendHexradioButton->isChecked())
	{
		
		baSend = hexStringtoByteArrayWithSapce(ui->USBSENDINPUT->toPlainText());
		sendlen = baSend.length();
		memcpy(buffer, baSend.data(), baSend.length());

		//WriteData(ui->USBSENDINPUT->toPlainText());
	}
	if (ui->UsbSendASCiiradioButton->isChecked())
	{
		baSend=ui->USBSENDINPUT->toPlainText().toLocal8Bit();
		sendlen = baSend.length();
		memcpy(buffer, baSend.data(), baSend.length());
	}
	

	if (m_CommunicationInterfaceState == CommunicationInterfaceState::UsbInterfaceConnected)
	{
		/*int ret =  */usb_bulk_write(gusb_handle, EP_OUT, buffer, sendlen, 500);
	}

	if (m_CommunicationInterfaceState == CommunicationInterfaceState::SerialPortInterfaceConnected)
	{
		SerialportwriteData(baSend);
	}

}


void MainWindow::OnFileSaveTarged()
{
	CurrentSystemtime = QDateTime::currentDateTime();//获取系统现在的时间
	QString str = CurrentSystemtime.toString("yyyy-MM-dd_hh-mm-ss"); //设置显示格式
	ui->customPlot->saveBmp(str+".bmp");
	ui->USBLOG->append(tr("state:File Save******** %1 save at %2")
		.arg("Image").arg("qApp->applicationDirPath"));
	
}

bool MainWindow::InitUsb(unsigned short UsbVid, unsigned short UsbPid)
{
	usb_init();
	usb_find_busses();
	usb_find_devices();
	struct usb_bus *bus;
	struct usb_device *dev;
	for (bus = usb_get_busses(); bus; bus = bus->next)
	{
		for (dev = bus->devices; dev; dev = dev->next)
		{
			if ((dev->descriptor.idVendor == UsbVid) && (dev->descriptor.idProduct == UsbPid))
			{
				gusb_handle = usb_open(dev);
				if (!gusb_handle)
				{
					ui->USBLOG->append(tr("Error Open USB Device Handle :%1")
						.arg(usb_strerror()));
				}
				else
				{
					if ((dev->descriptor.idVendor == UsbVid) && (dev->descriptor.idProduct == UsbPid))
						ui->USBLOG->append(tr("success: USB device idVendor:0x%1 idProduct:0x%2 opened")
						.arg(QString::number(UsbVid,16)).arg(QString::number(UsbPid,16)));
				}
			}
		}
	}
	try 
	{
		if (usb_set_configuration(gusb_handle, MY_CONFIG) < 0)
		{
			ui->USBLOG->append(tr("error setting config #%1: %2")
				.arg(MY_CONFIG).arg(usb_strerror()));
			throw QString("error setting config!");
			usb_close(gusb_handle);
			return false;
		}
		else
		{
			ui->USBLOG->append(tr("success: set configuration #%1")
				.arg(MY_CONFIG));
		}
		if (usb_claim_interface(gusb_handle, 0) < 0)
		{
			ui->USBLOG->append(tr("error claiming interface #%1:\n%2")
				.arg(MY_INTF).arg(usb_strerror()));
			throw QString("error claiming interface!");
			usb_close(gusb_handle);
			return false;
		}
		else
		{
			ui->USBLOG->append(tr("success: claim_interface #%1")
				.arg(MY_INTF));
		}
		
	}
	catch (QString ex)
	{
		ui->USBOPEN->setText("Open");
		ui->actionUSBStart->setChecked(false);
		ui->USBLOG->append(ex);
	}
	
	
}

void MainWindow::CloseUsb()
{
	if (gusb_handle)
	{
		usb_release_interface(gusb_handle, 0);
		usb_close(gusb_handle);
	}
}

bool MainWindow::ReadData(QString &pBuffer)
{
	return true;
}

bool MainWindow::WriteData(const QString& pBuffer)
{
	char buffer[1024];
	QString command = pBuffer;
	while (command.length() > 0)
	{
		QString sendcommand = command.left(sizeof(buffer));
		command.remove(0, sizeof(buffer));
		int sendlen = sendcommand.length();
		QByteArray ba = sendcommand.toLatin1();
		memcpy(buffer, ba.data(), sendcommand.length());
		int ret = usb_bulk_write(gusb_handle, EP_OUT, buffer, sendlen, 500);
		if (ret < 0)
		{
			ui->USBLOG->append(tr("usb_bulk_write failed"));
			return false;
		}
		else
		{
			ui->USBLOG->append(tr("write %1 success\n")
				.arg(buffer));
			//qDebug("write %s success", buffer);
		}
		//        QThread::sleep(1);
	}
	return true;
}

bool MainWindow::SendCommand(const QString &pCommand, QStringList &pResponseList)
{
	//    if(! WriteData(pCommand)) {
	//        ui->USBLOG->append(tr("Write Data failed"));
	//        return false;
	//    }
	//    if(! WaitResponse(100, pResponseList)) {
	//        ui->USBLOG->append(tr("WaitResponse failed"));
	//        return false;
	//    }
	//    if (pResponseList[0] != pCommand) {
	//        return false;
	//    }
	return true;
}

void MainWindow::WriteSettings()
{
	QSettings settings("QtUSBOption.ini", QSettings::IniFormat); // 当前目录的INI文件
	settings.beginGroup("DevOption");
	settings.setValue("USBVID", g_USBVID);
	settings.setValue("USBPID", g_USBPID);
	settings.setValue("USBEPIN", EP_IN);
	settings.setValue("USBEPOUT", EP_OUT);
	settings.setValue("USBSEND", g_szSend);
	settings.endGroup();
}

void MainWindow::ReadSettings()
{
	QSettings settings("QtUSBOption.ini", QSettings::IniFormat);
	g_USBVID = settings.value("DevOption/USBVID").toInt();
	g_USBPID = settings.value("DevOption/USBPID").toInt();
	EP_IN = settings.value("DevOption/USBEPIN").toInt();
	EP_OUT = settings.value("DevOption/USBEPOUT").toInt();
	g_szSend= settings.value("DevOption/USBSEND").toString();
}

void MainWindow::setupDemo(int demoIndex)
{
	switch (demoIndex)
	{
	case 0:  setupQuadraticDemo(ui->customPlot); break;
	case 1:  setupSimpleDemo(ui->customPlot); break;
	case 2:  setupSincScatterDemo(ui->customPlot); break;
	case 3:  setupScatterStyleDemo(ui->customPlot); break;
	case 4:  setupScatterPixmapDemo(ui->customPlot); break;
	case 5:  setupLineStyleDemo(ui->customPlot); break;
	case 6:  setupDateDemo(ui->customPlot); break;
	case 7:  setupTextureBrushDemo(ui->customPlot); break;
	case 8:  setupMultiAxisDemo(ui->customPlot); break;
	case 9:  setupLogarithmicDemo(ui->customPlot); break;
	case 10: setupRealtimeDataDemo(ui->customPlot); break;
	case 11: setupParametricCurveDemo(ui->customPlot); break;
	case 12: setupBarChartDemo(ui->customPlot); break;
	case 13: setupStatisticalDemo(ui->customPlot); break;
	case 14: setupSimpleItemDemo(ui->customPlot); break;
	case 15: setupItemDemo(ui->customPlot); break;
	case 16: setupStyledDemo(ui->customPlot); break;
	case 17: setupAdvancedAxesDemo(ui->customPlot); break;
	case 18: setupColorMapDemo(ui->customPlot); break;
	case 19: setupFinancialDemo(ui->customPlot); break;
	}
	setWindowTitle("QCustomPlot: " + demoName);
	statusBar()->clearMessage();
	currentDemoIndex = demoIndex;
	ui->customPlot->replot();
}

void MainWindow::setupQuadraticDemo(QCustomPlot *customPlot)
{
	demoName = "Quadratic Demo";
	// generate some data:
	QVector<double> x(101), y(101); // initialize with entries 0..100
	for (int i = 0; i<101; ++i)
	{
		x[i] = i / 50.0 - 1; // x goes from -1 to 1
		y[i] = x[i] * x[i];  // let's plot a quadratic function
	}
	// create graph and assign data to it:
	customPlot->addGraph();
	customPlot->graph(0)->setData(x, y);
	// give the axes some labels:
	customPlot->xAxis->setLabel("x");
	customPlot->yAxis->setLabel("y");
	// set axes ranges, so we see all data:
	customPlot->xAxis->setRange(-1, 1);
	customPlot->yAxis->setRange(0, 1);
}

void MainWindow::setupSimpleDemo(QCustomPlot *customPlot)
{
	demoName = "Simple Demo";

	// add two new graphs and set their look:
	customPlot->addGraph();
	customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
	customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
	customPlot->addGraph();
	customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
												 // generate some points of data (y0 for first, y1 for second graph):
	QVector<double> x(250), y0(250), y1(250);
	for (int i = 0; i<250; ++i)
	{
		x[i] = i;
		y0[i] = qExp(-i / 150.0)*qCos(i / 10.0); // exponentially decaying cosine
		y1[i] = qExp(-i / 150.0);              // exponential envelope
	}
	// configure right and top axis to show ticks but no labels:
	// (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
	customPlot->xAxis2->setVisible(true);
	customPlot->xAxis2->setTickLabels(false);
	customPlot->yAxis2->setVisible(true);
	customPlot->yAxis2->setTickLabels(false);
	// make left and bottom axes always transfer their ranges to right and top axes:
	connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
	// pass data points to graphs:
	customPlot->graph(0)->setData(x, y0);
	customPlot->graph(1)->setData(x, y1);
	// let the ranges scale themselves so graph 0 fits perfectly in the visible area:
	customPlot->graph(0)->rescaleAxes();
	// same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
	customPlot->graph(1)->rescaleAxes(true);
	// Note: we could have also just called customPlot->rescaleAxes(); instead
	// Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
	customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void MainWindow::setupSincScatterDemo(QCustomPlot *customPlot)
{
	demoName = "Sinc Scatter Demo";
	customPlot->legend->setVisible(true);
	customPlot->legend->setFont(QFont("Helvetica", 9));
	// set locale to english, so we get english decimal separator:
	customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
	// add confidence band graphs:
	customPlot->addGraph();
	QPen pen;
	pen.setStyle(Qt::DotLine);
	pen.setWidth(1);
	pen.setColor(QColor(180, 180, 180));
	customPlot->graph(0)->setName("Confidence Band 68%");
	customPlot->graph(0)->setPen(pen);
	customPlot->graph(0)->setBrush(QBrush(QColor(255, 50, 30, 20)));
	customPlot->addGraph();
	customPlot->legend->removeItem(customPlot->legend->itemCount() - 1); // don't show two confidence band graphs in legend
	customPlot->graph(1)->setPen(pen);
	customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));
	// add theory curve graph:
	customPlot->addGraph();
	pen.setStyle(Qt::DashLine);
	pen.setWidth(2);
	pen.setColor(Qt::red);
	customPlot->graph(2)->setPen(pen);
	customPlot->graph(2)->setName("Theory Curve");
	// add data point graph:
	customPlot->addGraph();
	customPlot->graph(3)->setPen(QPen(Qt::blue));
	customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
	customPlot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 4));
	customPlot->graph(3)->setErrorType(QCPGraph::etValue);
	customPlot->graph(3)->setErrorPen(QPen(QColor(180, 180, 180)));
	customPlot->graph(3)->setName("Measurement");

	// generate ideal sinc curve data and some randomly perturbed data for scatter plot:
	QVector<double> x0(250), y0(250);
	QVector<double> yConfUpper(250), yConfLower(250);
	for (int i = 0; i<250; ++i)
	{
		x0[i] = (i / 249.0 - 0.5) * 30 + 0.01; // by adding a small offset we make sure not do divide by zero in next code line
		y0[i] = qSin(x0[i]) / x0[i]; // sinc function
		yConfUpper[i] = y0[i] + 0.15;
		yConfLower[i] = y0[i] - 0.15;
		x0[i] *= 1000;
	}
	QVector<double> x1(50), y1(50), y1err(50);
	for (int i = 0; i<50; ++i)
	{
		// generate a gaussian distributed random number:
		double tmp1 = rand() / (double)RAND_MAX;
		double tmp2 = rand() / (double)RAND_MAX;
		double r = qSqrt(-2 * qLn(tmp1))*qCos(2 * M_PI*tmp2); // box-muller transform for gaussian distribution
															  // set y1 to value of y0 plus a random gaussian pertubation:
		x1[i] = (i / 50.0 - 0.5) * 30 + 0.25;
		y1[i] = qSin(x1[i]) / x1[i] + r*0.15;
		x1[i] *= 1000;
		y1err[i] = 0.15;
	}
	// pass data to graphs and let QCustomPlot determine the axes ranges so the whole thing is visible:
	customPlot->graph(0)->setData(x0, yConfUpper);
	customPlot->graph(1)->setData(x0, yConfLower);
	customPlot->graph(2)->setData(x0, y0);
	customPlot->graph(3)->setDataValueError(x1, y1, y1err);
	customPlot->graph(2)->rescaleAxes();
	customPlot->graph(3)->rescaleAxes(true);
	// setup look of bottom tick labels:
	customPlot->xAxis->setTickLabelRotation(30);
	customPlot->xAxis->setAutoTickCount(9);
	customPlot->xAxis->setNumberFormat("ebc");
	customPlot->xAxis->setNumberPrecision(1);
	customPlot->xAxis->moveRange(-10);
	// make top right axes clones of bottom left axes. Looks prettier:
	customPlot->axisRect()->setupFullAxesBox();
}

void MainWindow::setupScatterStyleDemo(QCustomPlot *customPlot)
{
	demoName = "Scatter Style Demo";
	customPlot->legend->setVisible(true);
	customPlot->legend->setFont(QFont("Helvetica", 9));
	customPlot->legend->setRowSpacing(-3);
	QVector<QCPScatterStyle::ScatterShape> shapes;
	shapes << QCPScatterStyle::ssCross;
	shapes << QCPScatterStyle::ssPlus;
	shapes << QCPScatterStyle::ssCircle;
	shapes << QCPScatterStyle::ssDisc;
	shapes << QCPScatterStyle::ssSquare;
	shapes << QCPScatterStyle::ssDiamond;
	shapes << QCPScatterStyle::ssStar;
	shapes << QCPScatterStyle::ssTriangle;
	shapes << QCPScatterStyle::ssTriangleInverted;
	shapes << QCPScatterStyle::ssCrossSquare;
	shapes << QCPScatterStyle::ssPlusSquare;
	shapes << QCPScatterStyle::ssCrossCircle;
	shapes << QCPScatterStyle::ssPlusCircle;
	shapes << QCPScatterStyle::ssPeace;
	shapes << QCPScatterStyle::ssCustom;

	QPen pen;
	// add graphs with different scatter styles:
	for (int i = 0; i<shapes.size(); ++i)
	{
		customPlot->addGraph();
		pen.setColor(QColor(qSin(i*0.3) * 100 + 100, qSin(i*0.6 + 0.7) * 100 + 100, qSin(i*0.4 + 0.6) * 100 + 100));
		// generate data:
		QVector<double> x(10), y(10);
		for (int k = 0; k<10; ++k)
		{
			x[k] = k / 10.0 * 4 * 3.14 + 0.01;
			y[k] = 7 * qSin(x[k]) / x[k] + (shapes.size() - i) * 5;
		}
		customPlot->graph()->setData(x, y);
		customPlot->graph()->rescaleAxes(true);
		customPlot->graph()->setPen(pen);
		customPlot->graph()->setName(QCPScatterStyle::staticMetaObject.enumerator(QCPScatterStyle::staticMetaObject.indexOfEnumerator("ScatterShape")).valueToKey(shapes.at(i)));
		customPlot->graph()->setLineStyle(QCPGraph::lsLine);
		// set scatter style:
		if (shapes.at(i) != QCPScatterStyle::ssCustom)
		{
			customPlot->graph()->setScatterStyle(QCPScatterStyle(shapes.at(i), 10));
		}
		else
		{
			QPainterPath customScatterPath;
			for (int i = 0; i<3; ++i)
				customScatterPath.cubicTo(qCos(2 * M_PI*i / 3.0) * 9, qSin(2 * M_PI*i / 3.0) * 9, qCos(2 * M_PI*(i + 0.9) / 3.0) * 9, qSin(2 * M_PI*(i + 0.9) / 3.0) * 9, 0, 0);
			customPlot->graph()->setScatterStyle(QCPScatterStyle(customScatterPath, QPen(Qt::black, 0), QColor(40, 70, 255, 50), 10));
		}
	}
	// set blank axis lines:
	customPlot->rescaleAxes();
	customPlot->xAxis->setTicks(false);
	customPlot->yAxis->setTicks(false);
	customPlot->xAxis->setTickLabels(false);
	customPlot->yAxis->setTickLabels(false);
	// make top right axes clones of bottom left axes:
	customPlot->axisRect()->setupFullAxesBox();
}

void MainWindow::setupLineStyleDemo(QCustomPlot *customPlot)
{
	demoName = "Line Style Demo";
	customPlot->legend->setVisible(true);
	customPlot->legend->setFont(QFont("Helvetica", 9));
	QPen pen;
	QStringList lineNames;
	lineNames << "lsNone" << "lsLine" << "lsStepLeft" << "lsStepRight" << "lsStepCenter" << "lsImpulse";
	// add graphs with different line styles:
	for (int i = QCPGraph::lsNone; i <= QCPGraph::lsImpulse; ++i)
	{
		customPlot->addGraph();
		pen.setColor(QColor(qSin(i * 1 + 1.2) * 80 + 80, qSin(i*0.3 + 0) * 80 + 80, qSin(i*0.3 + 1.5) * 80 + 80));
		customPlot->graph()->setPen(pen);
		customPlot->graph()->setName(lineNames.at(i - QCPGraph::lsNone));
		customPlot->graph()->setLineStyle((QCPGraph::LineStyle)i);
		customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
		// generate data:
		QVector<double> x(15), y(15);
		for (int j = 0; j<15; ++j)
		{
			x[j] = j / 15.0 * 5 * 3.14 + 0.01;
			y[j] = 7 * qSin(x[j]) / x[j] - (i - QCPGraph::lsNone) * 5 + (QCPGraph::lsImpulse) * 5 + 2;
		}
		customPlot->graph()->setData(x, y);
		customPlot->graph()->rescaleAxes(true);
	}
	// zoom out a bit:
	customPlot->yAxis->scaleRange(1.1, customPlot->yAxis->range().center());
	customPlot->xAxis->scaleRange(1.1, customPlot->xAxis->range().center());
	// set blank axis lines:
	customPlot->xAxis->setTicks(false);
	customPlot->yAxis->setTicks(true);
	customPlot->xAxis->setTickLabels(false);
	customPlot->yAxis->setTickLabels(true);
	// make top right axes clones of bottom left axes:
	customPlot->axisRect()->setupFullAxesBox();
}

void MainWindow::setupScatterPixmapDemo(QCustomPlot *customPlot)
{
	demoName = "Scatter Pixmap Demo";
	customPlot->axisRect()->setBackground(QPixmap("./solarpanels.jpg"));
	customPlot->addGraph();
	customPlot->graph()->setLineStyle(QCPGraph::lsLine);
	QPen pen;
	pen.setColor(QColor(255, 200, 20, 200));
	pen.setStyle(Qt::DashLine);
	pen.setWidthF(2.5);
	customPlot->graph()->setPen(pen);
	customPlot->graph()->setBrush(QBrush(QColor(255, 200, 20, 70)));
	customPlot->graph()->setScatterStyle(QCPScatterStyle(QPixmap("./sun.png")));
	// set graph name, will show up in legend next to icon:
	customPlot->graph()->setName("Data from Photovoltaic\nenergy barometer 2011");
	// set data:
	QVector<double> year, value;
	year << 2005 << 2006 << 2007 << 2008 << 2009 << 2010;
	value << 2.17 << 3.42 << 4.94 << 10.38 << 15.86 << 29.33;
	customPlot->graph()->setData(year, value);

	// set title of plot:
	customPlot->plotLayout()->insertRow(0);
	customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, "Regenerative Energies"));
	// set a fixed tick-step to one tick per year value:
	customPlot->xAxis->setAutoTickStep(false);
	customPlot->xAxis->setTickStep(1);
	customPlot->xAxis->setSubTickCount(3);
	// other axis configurations:
	customPlot->xAxis->setLabel("Year");
	customPlot->yAxis->setLabel("Installed Gigawatts of\nphotovoltaic in the European Union");
	customPlot->xAxis2->setVisible(true);
	customPlot->yAxis2->setVisible(true);
	customPlot->xAxis2->setTickLabels(false);
	customPlot->yAxis2->setTickLabels(false);
	customPlot->xAxis2->setTicks(false);
	customPlot->yAxis2->setTicks(false);
	customPlot->xAxis2->setSubTickCount(0);
	customPlot->yAxis2->setSubTickCount(0);
	customPlot->xAxis->setRange(2004.5, 2010.5);
	customPlot->yAxis->setRange(0, 30);
	// setup legend:
	customPlot->legend->setFont(QFont(font().family(), 7));
	customPlot->legend->setIconSize(50, 20);
	customPlot->legend->setVisible(true);
}

void MainWindow::setupDateDemo(QCustomPlot *customPlot)
{
	demoName = "Date Demo";
	// set locale to english, so we get english month names:
	customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
	// seconds of current time, we'll use it as starting point in time for data:
	double now = QDateTime::currentDateTime().toTime_t();
	srand(8); // set the random seed, so we always get the same random data
			  // create multiple graphs:
	for (int gi = 0; gi<5; ++gi)
	{
		customPlot->addGraph();
		QPen pen;
		pen.setColor(QColor(0, 0, 255, 200));
		customPlot->graph()->setLineStyle(QCPGraph::lsLine);
		customPlot->graph()->setPen(pen);
		customPlot->graph()->setBrush(QBrush(QColor(255 / 4.0*gi, 160, 50, 150)));
		// generate random walk data:
		QVector<double> time(250), value(250);
		for (int i = 0; i<250; ++i)
		{
			time[i] = now + 24 * 3600 * i;
			if (i == 0)
				value[i] = (i / 50.0 + 1)*(rand() / (double)RAND_MAX - 0.5);
			else
				value[i] = qFabs(value[i - 1])*(1 + 0.02 / 4.0*(4 - gi)) + (i / 50.0 + 1)*(rand() / (double)RAND_MAX - 0.5);
		}
		customPlot->graph()->setData(time, value);
	}
	// configure bottom axis to show date and time instead of number:
	customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
	customPlot->xAxis->setDateTimeFormat("MMMM\nyyyy");
	// set a more compact font size for bottom and left axis tick labels:
	customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
	customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
	// set a fixed tick-step to one tick per month:
	customPlot->xAxis->setAutoTickStep(false);
	customPlot->xAxis->setTickStep(2628000); // one month in seconds
	customPlot->xAxis->setSubTickCount(3);
	// apply manual tick and tick label for left axis:
	customPlot->yAxis->setAutoTicks(false);
	customPlot->yAxis->setAutoTickLabels(false);
	customPlot->yAxis->setTickVector(QVector<double>() << 5 << 55);
	customPlot->yAxis->setTickVectorLabels(QVector<QString>() << "Not so\nhigh" << "Very\nhigh");
	// set axis labels:
	customPlot->xAxis->setLabel("Date");
	customPlot->yAxis->setLabel("Random wobbly lines value");
	// make top and right axes visible but without ticks and labels:
	customPlot->xAxis2->setVisible(true);
	customPlot->yAxis2->setVisible(true);
	customPlot->xAxis2->setTicks(false);
	customPlot->yAxis2->setTicks(false);
	customPlot->xAxis2->setTickLabels(false);
	customPlot->yAxis2->setTickLabels(false);
	// set axis ranges to show all data:
	customPlot->xAxis->setRange(now, now + 24 * 3600 * 249);
	customPlot->yAxis->setRange(0, 60);
	// show legend:
	customPlot->legend->setVisible(true);
}

void MainWindow::setupTextureBrushDemo(QCustomPlot *customPlot)
{
	demoName = "Texture Brush Demo";
	// add two graphs with a textured fill:
	customPlot->addGraph();
	QPen redDotPen;
	redDotPen.setStyle(Qt::DotLine);
	redDotPen.setColor(QColor(170, 100, 100, 180));
	redDotPen.setWidthF(2);
	customPlot->graph(0)->setPen(redDotPen);
	customPlot->graph(0)->setBrush(QBrush(QPixmap("./balboa.jpg"))); // fill with texture of specified image

	customPlot->addGraph();
	customPlot->graph(1)->setPen(QPen(Qt::red));

	// activate channel fill for graph 0 towards graph 1:
	customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));

	// generate data:
	QVector<double> x(250);
	QVector<double> y0(250), y1(250);
	for (int i = 0; i<250; ++i)
	{
		// just playing with numbers, not much to learn here
		x[i] = 3 * i / 250.0;
		y0[i] = 1 + qExp(-x[i] * x[i] * 0.8)*(x[i] * x[i] + x[i]);
		y1[i] = 1 - qExp(-x[i] * x[i] * 0.4)*(x[i] * x[i])*0.1;
	}

	// pass data points to graphs:
	customPlot->graph(0)->setData(x, y0);
	customPlot->graph(1)->setData(x, y1);
	// activate top and right axes, which are invisible by default:
	customPlot->xAxis2->setVisible(true);
	customPlot->yAxis2->setVisible(true);
	// make tick labels invisible on top and right axis:
	customPlot->xAxis2->setTickLabels(false);
	customPlot->yAxis2->setTickLabels(false);
	// set ranges:
	customPlot->xAxis->setRange(0, 2.5);
	customPlot->yAxis->setRange(0.9, 1.6);
	// assign top/right axes same properties as bottom/left:
	customPlot->axisRect()->setupFullAxesBox();
}

void MainWindow::setupMultiAxisDemo(QCustomPlot *customPlot)
{
	customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	demoName = "Multi Axis Demo";

	customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); // period as decimal separator and comma as thousand separator
	customPlot->legend->setVisible(true);
	QFont legendFont = font();  // start out with MainWindow's font..
	legendFont.setPointSize(9); // and make a bit smaller for legend
	customPlot->legend->setFont(legendFont);
	customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));
	// by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
	customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom | Qt::AlignRight);

	// setup for graph 0: key axis left, value axis bottom
	// will contain left maxwell-like function
	customPlot->addGraph(customPlot->yAxis, customPlot->xAxis);
	customPlot->graph(0)->setPen(QPen(QColor(255, 100, 0)));
	customPlot->graph(0)->setBrush(QBrush(QPixmap("./balboa.jpg"))); // fill with texture of specified image
	customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
	customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
	customPlot->graph(0)->setName("Left maxwell function");

	// setup for graph 1: key axis bottom, value axis left (those are the default axes)
	// will contain bottom maxwell-like function
	customPlot->addGraph();
	customPlot->graph(1)->setPen(QPen(Qt::red));
	customPlot->graph(1)->setBrush(QBrush(QPixmap("./balboa.jpg"))); // same fill as we used for graph 0
	customPlot->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
	customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 7));
	customPlot->graph(1)->setErrorType(QCPGraph::etValue);
	customPlot->graph(1)->setName("Bottom maxwell function");

	// setup for graph 2: key axis top, value axis right
	// will contain high frequency sine with low frequency beating:
	customPlot->addGraph(customPlot->xAxis2, customPlot->yAxis2);
	customPlot->graph(2)->setPen(QPen(Qt::blue));
	customPlot->graph(2)->setName("High frequency sine");

	// setup for graph 3: same axes as graph 2
	// will contain low frequency beating envelope of graph 2
	customPlot->addGraph(customPlot->xAxis2, customPlot->yAxis2);
	QPen blueDotPen;
	blueDotPen.setColor(QColor(30, 40, 255, 150));
	blueDotPen.setStyle(Qt::DotLine);
	blueDotPen.setWidthF(4);
	customPlot->graph(3)->setPen(blueDotPen);
	customPlot->graph(3)->setName("Sine envelope");

	// setup for graph 4: key axis right, value axis top
	// will contain parabolically distributed data points with some random perturbance
	customPlot->addGraph(customPlot->yAxis2, customPlot->xAxis2);
	customPlot->graph(4)->setPen(QColor(50, 50, 50, 255));
	customPlot->graph(4)->setLineStyle(QCPGraph::lsNone);
	customPlot->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
	customPlot->graph(4)->setName("Some random data around\na quadratic function");

	// generate data, just playing with numbers, not much to learn here:
	QVector<double> x0(25), y0(25);
	QVector<double> x1(15), y1(15), y1err(15);
	QVector<double> x2(250), y2(250);
	QVector<double> x3(250), y3(250);
	QVector<double> x4(250), y4(250);
	for (int i = 0; i<25; ++i) // data for graph 0
	{
		x0[i] = 3 * i / 25.0;
		y0[i] = qExp(-x0[i] * x0[i] * 0.8)*(x0[i] * x0[i] + x0[i]);
	}
	for (int i = 0; i<15; ++i) // data for graph 1
	{
		x1[i] = 3 * i / 15.0;;
		y1[i] = qExp(-x1[i] * x1[i])*(x1[i] * x1[i])*2.6;
		y1err[i] = y1[i] * 0.25;
	}
	for (int i = 0; i<250; ++i) // data for graphs 2, 3 and 4
	{
		x2[i] = i / 250.0 * 3 * M_PI;
		x3[i] = x2[i];
		x4[i] = i / 250.0 * 100 - 50;
		y2[i] = qSin(x2[i] * 12)*qCos(x2[i]) * 10;
		y3[i] = qCos(x3[i]) * 10;
		y4[i] = 0.01*x4[i] * x4[i] + 1.5*(rand() / (double)RAND_MAX - 0.5) + 1.5*M_PI;
	}

	// pass data points to graphs:
	customPlot->graph(0)->setData(x0, y0);
	customPlot->graph(1)->setDataValueError(x1, y1, y1err);
	customPlot->graph(2)->setData(x2, y2);
	customPlot->graph(3)->setData(x3, y3);
	customPlot->graph(4)->setData(x4, y4);
	// activate top and right axes, which are invisible by default:
	customPlot->xAxis2->setVisible(true);
	customPlot->yAxis2->setVisible(true);
	// set ranges appropriate to show data:
	customPlot->xAxis->setRange(0, 2.7);
	customPlot->yAxis->setRange(0, 2.6);
	customPlot->xAxis2->setRange(0, 3.0*M_PI);
	customPlot->yAxis2->setRange(-70, 35);
	// set pi ticks on top axis:
	QVector<double> piTicks;
	QVector<QString> piLabels;
	piTicks << 0 << 0.5*M_PI << M_PI << 1.5*M_PI << 2 * M_PI << 2.5*M_PI << 3 * M_PI;
	piLabels << "0" << QString::fromUtf8("½π") << QString::fromUtf8("π") << QString::fromUtf8("1½π") << QString::fromUtf8("2π") << QString::fromUtf8("2½π") << QString::fromUtf8("3π");
	customPlot->xAxis2->setAutoTicks(false);
	customPlot->xAxis2->setAutoTickLabels(false);
	customPlot->xAxis2->setTickVector(piTicks);
	customPlot->xAxis2->setTickVectorLabels(piLabels);
	// add title layout element:
	customPlot->plotLayout()->insertRow(0);
	customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, "Way too many graphs in one plot"));
	// set labels:
	customPlot->xAxis->setLabel("Bottom axis with outward ticks");
	customPlot->yAxis->setLabel("Left axis label");
	customPlot->xAxis2->setLabel("Top axis label");
	customPlot->yAxis2->setLabel("Right axis label");
	// make ticks on bottom axis go outward:
	customPlot->xAxis->setTickLength(0, 5);
	customPlot->xAxis->setSubTickLength(0, 3);
	// make ticks on right axis go inward and outward:
	customPlot->yAxis2->setTickLength(3, 3);
	customPlot->yAxis2->setSubTickLength(1, 1);
}

void MainWindow::setupLogarithmicDemo(QCustomPlot *customPlot)
{
	demoName = "Logarithmic Demo";
	customPlot->setNoAntialiasingOnDrag(true); // more performance/responsiveness during dragging
	customPlot->addGraph();
	QPen pen;
	pen.setColor(QColor(255, 170, 100));
	pen.setWidth(2);
	pen.setStyle(Qt::DotLine);
	customPlot->graph(0)->setPen(pen);
	customPlot->graph(0)->setName("x");

	customPlot->addGraph();
	customPlot->graph(1)->setPen(QPen(Qt::red));
	customPlot->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 20)));
	customPlot->graph(1)->setErrorType(QCPGraph::etBoth);
	customPlot->graph(1)->setName("-sin(x)exp(x)");

	customPlot->addGraph();
	customPlot->graph(2)->setPen(QPen(Qt::blue));
	customPlot->graph(2)->setBrush(QBrush(QColor(0, 0, 255, 20)));
	customPlot->graph(2)->setName(" sin(x)exp(x)");

	customPlot->addGraph();
	pen.setColor(QColor(0, 0, 0));
	pen.setWidth(1);
	pen.setStyle(Qt::DashLine);
	customPlot->graph(3)->setPen(pen);
	customPlot->graph(3)->setBrush(QBrush(QColor(0, 0, 0, 15)));
	customPlot->graph(3)->setLineStyle(QCPGraph::lsStepCenter);
	customPlot->graph(3)->setName("x!");

	QVector<double> x0(200), y0(200);
	QVector<double> x1(200), y1(200);
	QVector<double> x2(200), y2(200);
	QVector<double> x3(21), y3(21);
	for (int i = 0; i<200; ++i)
	{
		x0[i] = i / 10.0;
		y0[i] = x0[i];
		x1[i] = i / 10.0;
		y1[i] = -qSin(x1[i])*qExp(x1[i]);
		x2[i] = i / 10.0;
		y2[i] = qSin(x2[i])*qExp(x2[i]);
	}
	for (int i = 0; i<21; ++i)
	{
		x3[i] = i;
		y3[i] = 1;
		for (int k = 1; k <= i; ++k) y3[i] *= k; // factorial
	}
	customPlot->graph(0)->setData(x0, y0);
	customPlot->graph(1)->setData(x1, y1);
	customPlot->graph(2)->setData(x2, y2);
	customPlot->graph(3)->setData(x3, y3);

	customPlot->yAxis->grid()->setSubGridVisible(true);
	customPlot->xAxis->grid()->setSubGridVisible(true);
	customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
	customPlot->yAxis->setScaleLogBase(100);
	customPlot->yAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
	customPlot->yAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
	customPlot->yAxis->setSubTickCount(10);
	customPlot->xAxis->setRange(0, 19.9);
	customPlot->yAxis->setRange(1e-2, 1e10);
	// make range draggable and zoomable:
	customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

	// make top right axes clones of bottom left axes:
	customPlot->axisRect()->setupFullAxesBox();
	// connect signals so top and right axes move in sync with bottom and left axes:
	connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

	customPlot->legend->setVisible(true);
	customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 150)));
	customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop); // make legend align in top left corner or axis rect
}

void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
	QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif
	demoName = "Real Time Data Demo";

	// include this section to fully disable antialiasing for higher performance:
	/*
	customPlot->setNotAntialiasedElements(QCP::aeAll);
	QFont font;
	font.setStyleStrategy(QFont::NoAntialias);
	customPlot->xAxis->setTickLabelFont(font);
	customPlot->yAxis->setTickLabelFont(font);
	customPlot->legend->setFont(font);
	*/
	customPlot->addGraph(); // blue line
	customPlot->graph(0)->setPen(QPen(Qt::blue));
	customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
	customPlot->graph(0)->setAntialiasedFill(false);
	customPlot->addGraph(); // red line
	customPlot->graph(1)->setPen(QPen(Qt::red));
	customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));

	customPlot->addGraph(); // blue dot
	customPlot->graph(2)->setPen(QPen(Qt::blue));
	customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
	customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
	customPlot->addGraph(); // red dot
	customPlot->graph(3)->setPen(QPen(Qt::red));
	customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
	customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

	customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
	customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
	customPlot->xAxis->setAutoTickStep(false);
	customPlot->xAxis->setTickStep(2);
	customPlot->axisRect()->setupFullAxesBox();

	// make left and bottom axes transfer their ranges to right and top axes:
	connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

	// setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
	connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
	dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::setupParametricCurveDemo(QCustomPlot *customPlot)
{
	demoName = "Parametric Curves Demo";

	// create empty curve objects and add them to customPlot:
	QCPCurve *fermatSpiral1 = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
	QCPCurve *fermatSpiral2 = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
	QCPCurve *deltoidRadial = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
	customPlot->addPlottable(fermatSpiral1);
	customPlot->addPlottable(fermatSpiral2);
	customPlot->addPlottable(deltoidRadial);
	// generate the curve data points:
	int pointCount = 500;
	QVector<double> x1(pointCount), y1(pointCount);
	QVector<double> x2(pointCount), y2(pointCount);
	QVector<double> x3(pointCount), y3(pointCount);
	for (int i = 0; i<pointCount; ++i)
	{
		double phi = (i / (double)(pointCount - 1)) * 8 * M_PI;
		x1[i] = qSqrt(phi)*qCos(phi);
		y1[i] = qSqrt(phi)*qSin(phi);
		x2[i] = -x1[i];
		y2[i] = -y1[i];
		double t = i / (double)(pointCount - 1) * 2 * M_PI;
		x3[i] = 2 * qCos(2 * t) + qCos(1 * t) + 2 * qSin(t);
		y3[i] = 2 * qSin(2 * t) - qSin(1 * t);
	}
	// pass the data to the curves:
	fermatSpiral1->setData(x1, y1);
	fermatSpiral2->setData(x2, y2);
	deltoidRadial->setData(x3, y3);
	// color the curves:
	fermatSpiral1->setPen(QPen(Qt::blue));
	fermatSpiral1->setBrush(QBrush(QColor(0, 0, 255, 20)));
	fermatSpiral2->setPen(QPen(QColor(255, 120, 0)));
	fermatSpiral2->setBrush(QBrush(QColor(255, 120, 0, 30)));
	QRadialGradient radialGrad(QPointF(310, 180), 200);
	radialGrad.setColorAt(0, QColor(170, 20, 240, 100));
	radialGrad.setColorAt(0.5, QColor(20, 10, 255, 40));
	radialGrad.setColorAt(1, QColor(120, 20, 240, 10));
	deltoidRadial->setPen(QPen(QColor(170, 20, 240)));
	deltoidRadial->setBrush(QBrush(radialGrad));
	// set some basic customPlot config:
	customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	customPlot->axisRect()->setupFullAxesBox();
	customPlot->rescaleAxes();
}

void MainWindow::setupBarChartDemo(QCustomPlot *customPlot)
{
	demoName = "Bar Chart Demo";
	// create empty bar chart objects:
	QCPBars *regen = new QCPBars(customPlot->xAxis, customPlot->yAxis);
	QCPBars *nuclear = new QCPBars(customPlot->xAxis, customPlot->yAxis);
	QCPBars *fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
	customPlot->addPlottable(regen);
	customPlot->addPlottable(nuclear);
	customPlot->addPlottable(fossil);
	// set names and colors:
	QPen pen;
	pen.setWidthF(1.2);
	fossil->setName("Fossil fuels");
	pen.setColor(QColor(255, 131, 0));
	fossil->setPen(pen);
	fossil->setBrush(QColor(255, 131, 0, 50));
	nuclear->setName("Nuclear");
	pen.setColor(QColor(1, 92, 191));
	nuclear->setPen(pen);
	nuclear->setBrush(QColor(1, 92, 191, 50));
	regen->setName("Regenerative");
	pen.setColor(QColor(150, 222, 0));
	regen->setPen(pen);
	regen->setBrush(QColor(150, 222, 0, 70));
	// stack bars ontop of each other:
	nuclear->moveAbove(fossil);
	regen->moveAbove(nuclear);

	// prepare x axis with country labels:
	QVector<double> ticks;
	QVector<QString> labels;
	ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7;
	labels << "USA" << "Japan" << "Germany" << "France" << "UK" << "Italy" << "Canada";
	customPlot->xAxis->setAutoTicks(false);
	customPlot->xAxis->setAutoTickLabels(false);
	customPlot->xAxis->setTickVector(ticks);
	customPlot->xAxis->setTickVectorLabels(labels);
	customPlot->xAxis->setTickLabelRotation(60);
	customPlot->xAxis->setSubTickCount(0);
	customPlot->xAxis->setTickLength(0, 4);
	customPlot->xAxis->grid()->setVisible(true);
	customPlot->xAxis->setRange(0, 8);

	// prepare y axis:
	customPlot->yAxis->setRange(0, 12.1);
	customPlot->yAxis->setPadding(5); // a bit more space to the left border
	customPlot->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
	customPlot->yAxis->grid()->setSubGridVisible(true);
	QPen gridPen;
	gridPen.setStyle(Qt::SolidLine);
	gridPen.setColor(QColor(0, 0, 0, 25));
	customPlot->yAxis->grid()->setPen(gridPen);
	gridPen.setStyle(Qt::DotLine);
	customPlot->yAxis->grid()->setSubGridPen(gridPen);

	// Add data:
	QVector<double> fossilData, nuclearData, regenData;
	fossilData << 0.86*10.5 << 0.83*5.5 << 0.84*5.5 << 0.52*5.8 << 0.89*5.2 << 0.90*4.2 << 0.67*11.2;
	nuclearData << 0.08*10.5 << 0.12*5.5 << 0.12*5.5 << 0.40*5.8 << 0.09*5.2 << 0.00*4.2 << 0.07*11.2;
	regenData << 0.06*10.5 << 0.05*5.5 << 0.04*5.5 << 0.06*5.8 << 0.02*5.2 << 0.07*4.2 << 0.25*11.2;
	fossil->setData(ticks, fossilData);
	nuclear->setData(ticks, nuclearData);
	regen->setData(ticks, regenData);

	// setup legend:
	customPlot->legend->setVisible(true);
	customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignHCenter);
	customPlot->legend->setBrush(QColor(255, 255, 255, 200));
	QPen legendPen;
	legendPen.setColor(QColor(130, 130, 130, 200));
	customPlot->legend->setBorderPen(legendPen);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	customPlot->legend->setFont(legendFont);
	customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void MainWindow::setupStatisticalDemo(QCustomPlot *customPlot)
{
	demoName = "Statistical Demo";
	// create empty statistical box plottables:
	QCPStatisticalBox *sample1 = new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);
	QCPStatisticalBox *sample2 = new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);
	QCPStatisticalBox *sample3 = new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);
	customPlot->addPlottable(sample1);
	customPlot->addPlottable(sample2);
	customPlot->addPlottable(sample3);
	QBrush boxBrush(QColor(60, 60, 255, 100));
	boxBrush.setStyle(Qt::Dense6Pattern); // make it look oldschool
	sample1->setBrush(boxBrush);
	sample2->setBrush(boxBrush);
	sample3->setBrush(boxBrush);

	// set data:
	sample1->setKey(1);
	sample1->setMinimum(1.1);
	sample1->setLowerQuartile(1.9);
	sample1->setMedian(2.25);
	sample1->setUpperQuartile(2.7);
	sample1->setMaximum(4.2);

	sample2->setKey(2);
	sample2->setMinimum(0.8);
	sample2->setLowerQuartile(1.6);
	sample2->setMedian(2.2);
	sample2->setUpperQuartile(3.2);
	sample2->setMaximum(4.9);
	sample2->setOutliers(QVector<double>() << 0.7 << 0.39 << 0.45 << 6.2 << 5.84);

	sample3->setKey(3);
	sample3->setMinimum(0.2);
	sample3->setLowerQuartile(0.7);
	sample3->setMedian(1.1);
	sample3->setUpperQuartile(1.6);
	sample3->setMaximum(2.9);

	// prepare manual x axis labels:
	customPlot->xAxis->setSubTickCount(0);
	customPlot->xAxis->setTickLength(0, 4);
	customPlot->xAxis->setTickLabelRotation(20);
	customPlot->xAxis->setAutoTicks(false);
	customPlot->xAxis->setAutoTickLabels(false);
	customPlot->xAxis->setTickVector(QVector<double>() << 1 << 2 << 3);
	customPlot->xAxis->setTickVectorLabels(QVector<QString>() << "Sample 1" << "Sample 2" << "Control Group");

	// prepare axes:
	customPlot->yAxis->setLabel(QString::fromUtf8("O₂ Absorption [mg]"));
	customPlot->rescaleAxes();
	customPlot->xAxis->scaleRange(1.7, customPlot->xAxis->range().center());
	customPlot->yAxis->setRange(0, 7);
	customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void MainWindow::setupSimpleItemDemo(QCustomPlot *customPlot)
{
	demoName = "Simple Item Demo";
	customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

	// add the text label at the top:
	QCPItemText *textLabel = new QCPItemText(customPlot);
	customPlot->addItem(textLabel);
	textLabel->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);
	textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
	textLabel->position->setCoords(0.5, 0); // place position at center/top of axis rect
	textLabel->setText("Text Item Demo");
	textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
	textLabel->setPen(QPen(Qt::black)); // show black border around text

										// add the arrow:
	QCPItemLine *arrow = new QCPItemLine(customPlot);
	customPlot->addItem(arrow);
	arrow->start->setParentAnchor(textLabel->bottom);
	arrow->end->setCoords(4, 1.6); // point to (4, 1.6) in x-y-plot coordinates
	arrow->setHead(QCPLineEnding::esSpikeArrow);
}

void MainWindow::setupItemDemo(QCustomPlot *customPlot)
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
	QMessageBox::critical(this, "", "You're using Qt < 4.7, the animation of the item demo needs functions that are available with Qt 4.7 to work properly");
#endif
	demoName = "Item Demo";

	customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	QCPGraph *graph = customPlot->addGraph();
	int n = 500;
	double phase = 0;
	double k = 3;
	QVector<double> x(n), y(n);
	for (int i = 0; i<n; ++i)
	{
		x[i] = i / (double)(n - 1) * 34 - 17;
		y[i] = qExp(-x[i] * x[i] / 20.0)*qSin(k*x[i] + phase);
	}
	graph->setData(x, y);
	graph->setPen(QPen(Qt::blue));
	graph->rescaleKeyAxis();
	customPlot->yAxis->setRange(-1.45, 1.65);
	customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);

	// add the bracket at the top:
	QCPItemBracket *bracket = new QCPItemBracket(customPlot);
	customPlot->addItem(bracket);
	bracket->left->setCoords(-8, 1.1);
	bracket->right->setCoords(8, 1.1);
	bracket->setLength(13);

	// add the text label at the top:
	QCPItemText *wavePacketText = new QCPItemText(customPlot);
	customPlot->addItem(wavePacketText);
	wavePacketText->position->setParentAnchor(bracket->center);
	wavePacketText->position->setCoords(0, -10); // move 10 pixels to the top from bracket center anchor
	wavePacketText->setPositionAlignment(Qt::AlignBottom | Qt::AlignHCenter);
	wavePacketText->setText("Wavepacket");
	wavePacketText->setFont(QFont(font().family(), 10));

	// add the phase tracer (red circle) which sticks to the graph data (and gets updated in bracketDataSlot by timer event):
	QCPItemTracer *phaseTracer = new QCPItemTracer(customPlot);
	customPlot->addItem(phaseTracer);
	itemDemoPhaseTracer = phaseTracer; // so we can access it later in the bracketDataSlot for animation
	phaseTracer->setGraph(graph);
	phaseTracer->setGraphKey((M_PI*1.5 - phase) / k);
	phaseTracer->setInterpolating(true);
	phaseTracer->setStyle(QCPItemTracer::tsCircle);
	phaseTracer->setPen(QPen(Qt::red));
	phaseTracer->setBrush(Qt::red);
	phaseTracer->setSize(7);

	// add label for phase tracer:
	QCPItemText *phaseTracerText = new QCPItemText(customPlot);
	customPlot->addItem(phaseTracerText);
	phaseTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
	phaseTracerText->setPositionAlignment(Qt::AlignRight | Qt::AlignBottom);
	phaseTracerText->position->setCoords(1.0, 0.95); // lower right corner of axis rect
	phaseTracerText->setText("Points of fixed\nphase define\nphase velocity vp");
	phaseTracerText->setTextAlignment(Qt::AlignLeft);
	phaseTracerText->setFont(QFont(font().family(), 9));
	phaseTracerText->setPadding(QMargins(8, 0, 0, 0));

	// add arrow pointing at phase tracer, coming from label:
	QCPItemCurve *phaseTracerArrow = new QCPItemCurve(customPlot);
	customPlot->addItem(phaseTracerArrow);
	phaseTracerArrow->start->setParentAnchor(phaseTracerText->left);
	phaseTracerArrow->startDir->setParentAnchor(phaseTracerArrow->start);
	phaseTracerArrow->startDir->setCoords(-40, 0); // direction 30 pixels to the left of parent anchor (tracerArrow->start)
	phaseTracerArrow->end->setParentAnchor(phaseTracer->position);
	phaseTracerArrow->end->setCoords(10, 10);
	phaseTracerArrow->endDir->setParentAnchor(phaseTracerArrow->end);
	phaseTracerArrow->endDir->setCoords(30, 30);
	phaseTracerArrow->setHead(QCPLineEnding::esSpikeArrow);
	phaseTracerArrow->setTail(QCPLineEnding(QCPLineEnding::esBar, (phaseTracerText->bottom->pixelPoint().y() - phaseTracerText->top->pixelPoint().y())*0.85));

	// add the group velocity tracer (green circle):
	QCPItemTracer *groupTracer = new QCPItemTracer(customPlot);
	customPlot->addItem(groupTracer);
	groupTracer->setGraph(graph);
	groupTracer->setGraphKey(5.5);
	groupTracer->setInterpolating(true);
	groupTracer->setStyle(QCPItemTracer::tsCircle);
	groupTracer->setPen(QPen(Qt::green));
	groupTracer->setBrush(Qt::green);
	groupTracer->setSize(7);

	// add label for group tracer:
	QCPItemText *groupTracerText = new QCPItemText(customPlot);
	customPlot->addItem(groupTracerText);
	groupTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
	groupTracerText->setPositionAlignment(Qt::AlignRight | Qt::AlignTop);
	groupTracerText->position->setCoords(1.0, 0.20); // lower right corner of axis rect
	groupTracerText->setText("Fixed positions in\nwave packet define\ngroup velocity vg");
	groupTracerText->setTextAlignment(Qt::AlignLeft);
	groupTracerText->setFont(QFont(font().family(), 9));
	groupTracerText->setPadding(QMargins(8, 0, 0, 0));

	// add arrow pointing at group tracer, coming from label:
	QCPItemCurve *groupTracerArrow = new QCPItemCurve(customPlot);
	customPlot->addItem(groupTracerArrow);
	groupTracerArrow->start->setParentAnchor(groupTracerText->left);
	groupTracerArrow->startDir->setParentAnchor(groupTracerArrow->start);
	groupTracerArrow->startDir->setCoords(-40, 0); // direction 30 pixels to the left of parent anchor (tracerArrow->start)
	groupTracerArrow->end->setCoords(5.5, 0.4);
	groupTracerArrow->endDir->setParentAnchor(groupTracerArrow->end);
	groupTracerArrow->endDir->setCoords(0, -40);
	groupTracerArrow->setHead(QCPLineEnding::esSpikeArrow);
	groupTracerArrow->setTail(QCPLineEnding(QCPLineEnding::esBar, (groupTracerText->bottom->pixelPoint().y() - groupTracerText->top->pixelPoint().y())*0.85));

	// add dispersion arrow:
	QCPItemCurve *arrow = new QCPItemCurve(customPlot);
	customPlot->addItem(arrow);
	arrow->start->setCoords(1, -1.1);
	arrow->startDir->setCoords(-1, -1.3);
	arrow->endDir->setCoords(-5, -0.3);
	arrow->end->setCoords(-10, -0.2);
	arrow->setHead(QCPLineEnding::esSpikeArrow);

	// add the dispersion arrow label:
	QCPItemText *dispersionText = new QCPItemText(customPlot);
	customPlot->addItem(dispersionText);
	dispersionText->position->setCoords(-6, -0.9);
	dispersionText->setRotation(40);
	dispersionText->setText("Dispersion with\nvp < vg");
	dispersionText->setFont(QFont(font().family(), 10));

	// setup a timer that repeatedly calls MainWindow::bracketDataSlot:
	connect(&dataTimer, SIGNAL(timeout()), this, SLOT(bracketDataSlot()));
	dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::setupStyledDemo(QCustomPlot *customPlot)
{
	demoName = "Styled Demo";

	// prepare data:
	QVector<double> x1(20), y1(20);
	QVector<double> x2(100), y2(100);
	QVector<double> x3(20), y3(20);
	QVector<double> x4(20), y4(20);
	for (int i = 0; i<x1.size(); ++i)
	{
		x1[i] = i / (double)x1.size() * 10;
		y1[i] = qCos(x1[i] * 0.8 + qSin(x1[i] * 0.16 + 1.0))*qSin(x1[i] * 0.54) + 1.4;
	}
	for (int i = 0; i<x2.size(); ++i)
	{
		x2[i] = i / (double)x2.size() * 10;
		y2[i] = qCos(x2[i] * 0.85 + qSin(x2[i] * 0.165 + 1.1))*qSin(x2[i] * 0.50) + 1.7;
	}
	for (int i = 0; i<x3.size(); ++i)
	{
		x3[i] = i / (double)x3.size() * 10;
		y3[i] = 0.05 + 3 * (0.5 + qCos(x3[i] * x3[i] * 0.2 + 2)*0.5) / (double)(x3[i] + 0.7) + qrand() / (double)RAND_MAX*0.01;
	}
	for (int i = 0; i<x4.size(); ++i)
	{
		x4[i] = x3[i];
		y4[i] = (0.5 - y3[i]) + ((x4[i] - 2)*(x4[i] - 2)*0.02);
	}

	// create and configure plottables:
	QCPGraph *graph1 = customPlot->addGraph();
	graph1->setData(x1, y1);
	graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
	graph1->setPen(QPen(QColor(120, 120, 120), 2));

	QCPGraph *graph2 = customPlot->addGraph();
	graph2->setData(x2, y2);
	graph2->setPen(Qt::NoPen);
	graph2->setBrush(QColor(200, 200, 200, 20));
	graph2->setChannelFillGraph(graph1);

	QCPBars *bars1 = new QCPBars(customPlot->xAxis, customPlot->yAxis);
	customPlot->addPlottable(bars1);
	bars1->setWidth(9 / (double)x3.size());
	bars1->setData(x3, y3);
	bars1->setPen(Qt::NoPen);
	bars1->setBrush(QColor(10, 140, 70, 160));

	QCPBars *bars2 = new QCPBars(customPlot->xAxis, customPlot->yAxis);
	customPlot->addPlottable(bars2);
	bars2->setWidth(9 / (double)x4.size());
	bars2->setData(x4, y4);
	bars2->setPen(Qt::NoPen);
	bars2->setBrush(QColor(10, 100, 50, 70));
	bars2->moveAbove(bars1);

	// move bars above graphs and grid below bars:
	customPlot->addLayer("abovemain", customPlot->layer("main"), QCustomPlot::limAbove);
	customPlot->addLayer("belowmain", customPlot->layer("main"), QCustomPlot::limBelow);
	graph1->setLayer("abovemain");
	customPlot->xAxis->grid()->setLayer("belowmain");
	customPlot->yAxis->grid()->setLayer("belowmain");

	// set some pens, brushes and backgrounds:
	customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
	customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
	customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
	customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
	customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
	customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
	customPlot->xAxis->setTickLabelColor(Qt::white);
	customPlot->yAxis->setTickLabelColor(Qt::white);
	customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
	customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
	customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
	customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
	customPlot->xAxis->grid()->setSubGridVisible(true);
	customPlot->yAxis->grid()->setSubGridVisible(true);
	customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
	customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
	customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
	customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
	QLinearGradient plotGradient;
	plotGradient.setStart(0, 0);
	plotGradient.setFinalStop(0, 350);
	plotGradient.setColorAt(0, QColor(80, 80, 80));
	plotGradient.setColorAt(1, QColor(50, 50, 50));
	customPlot->setBackground(plotGradient);
	QLinearGradient axisRectGradient;
	axisRectGradient.setStart(0, 0);
	axisRectGradient.setFinalStop(0, 350);
	axisRectGradient.setColorAt(0, QColor(80, 80, 80));
	axisRectGradient.setColorAt(1, QColor(30, 30, 30));
	customPlot->axisRect()->setBackground(axisRectGradient);

	customPlot->rescaleAxes();
	customPlot->yAxis->setRange(0, 2);
}

void MainWindow::setupAdvancedAxesDemo(QCustomPlot *customPlot)
{
	demoName = "Advanced Axes Demo";

	// configure axis rect:
	customPlot->plotLayout()->clear(); // clear default axis rect so we can start from scratch
	QCPAxisRect *wideAxisRect = new QCPAxisRect(customPlot);
	wideAxisRect->setupFullAxesBox(true);
	wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabels(true);
	wideAxisRect->addAxis(QCPAxis::atLeft)->setTickLabelColor(QColor("#6050F8")); // add an extra axis on the left and color its numbers
	QCPLayoutGrid *subLayout = new QCPLayoutGrid;
	customPlot->plotLayout()->addElement(0, 0, wideAxisRect); // insert axis rect in first row
	customPlot->plotLayout()->addElement(1, 0, subLayout); // sub layout in second row (grid layout will grow accordingly)
														   //customPlot->plotLayout()->setRowStretchFactor(1, 2);
														   // prepare axis rects that will be placed in the sublayout:
	QCPAxisRect *subRectLeft = new QCPAxisRect(customPlot, false); // false means to not setup default axes
	QCPAxisRect *subRectRight = new QCPAxisRect(customPlot, false);
	subLayout->addElement(0, 0, subRectLeft);
	subLayout->addElement(0, 1, subRectRight);
	subRectRight->setMaximumSize(150, 150); // make bottom right axis rect size fixed 150x150
	subRectRight->setMinimumSize(150, 150); // make bottom right axis rect size fixed 150x150
											// setup axes in sub layout axis rects:
	subRectLeft->addAxes(QCPAxis::atBottom | QCPAxis::atLeft);
	subRectRight->addAxes(QCPAxis::atBottom | QCPAxis::atRight);
	subRectLeft->axis(QCPAxis::atLeft)->setAutoTickCount(2);
	subRectRight->axis(QCPAxis::atRight)->setAutoTickCount(2);
	subRectRight->axis(QCPAxis::atBottom)->setAutoTickCount(2);
	subRectLeft->axis(QCPAxis::atBottom)->grid()->setVisible(true);
	// synchronize the left and right margins of the top and bottom axis rects:
	QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
	subRectLeft->setMarginGroup(QCP::msLeft, marginGroup);
	subRectRight->setMarginGroup(QCP::msRight, marginGroup);
	wideAxisRect->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);
	// move newly created axes on "axes" layer and grids on "grid" layer:
	foreach(QCPAxisRect *rect, customPlot->axisRects())
	{
		foreach(QCPAxis *axis, rect->axes())
		{
			axis->setLayer("axes");
			axis->grid()->setLayer("grid");
		}
	}

	// prepare data:
	QVector<double> x1a(20), y1a(20);
	QVector<double> x1b(50), y1b(50);
	QVector<double> x2(100), y2(100);
	QVector<double> x3, y3;
	qsrand(3);
	for (int i = 0; i<x1a.size(); ++i)
	{
		x1a[i] = i / (double)(x1a.size() - 1) * 10 - 5.0;
		y1a[i] = qCos(x1a[i]);
	}
	for (int i = 0; i<x1b.size(); ++i)
	{
		x1b[i] = i / (double)x1b.size() * 10 - 5.0;
		y1b[i] = qExp(-x1b[i] * x1b[i] * 0.2) * 1000;
	}
	for (int i = 0; i<x2.size(); ++i)
	{
		x2[i] = i / (double)x2.size() * 10;
		y2[i] = qrand() / (double)RAND_MAX - 0.5 + y2[qAbs(i - 1)];
	}
	x3 << 1 << 2 << 3 << 4;
	y3 << 2 << 2.5 << 4 << 1.5;

	// create and configure plottables:
	QCPGraph *mainGraph1 = customPlot->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
	mainGraph1->setData(x1a, y1a);
	mainGraph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black), QBrush(Qt::white), 6));
	mainGraph1->setPen(QPen(QColor(120, 120, 120), 2));
	QCPGraph *mainGraph2 = customPlot->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft, 1));
	mainGraph2->setData(x1b, y1b);
	mainGraph2->setPen(QPen(QColor("#8070B8"), 2));
	mainGraph2->setBrush(QColor(110, 170, 110, 30));
	mainGraph1->setChannelFillGraph(mainGraph2);
	mainGraph1->setBrush(QColor(255, 161, 0, 50));

	QCPGraph *graph2 = customPlot->addGraph(subRectLeft->axis(QCPAxis::atBottom), subRectLeft->axis(QCPAxis::atLeft));
	graph2->setData(x2, y2);
	graph2->setLineStyle(QCPGraph::lsImpulse);
	graph2->setPen(QPen(QColor("#FFA100"), 1.5));

	QCPBars *bars1 = new QCPBars(subRectRight->axis(QCPAxis::atBottom), subRectRight->axis(QCPAxis::atRight));
	customPlot->addPlottable(bars1);
	bars1->setWidth(3 / (double)x3.size());
	bars1->setData(x3, y3);
	bars1->setPen(QPen(Qt::black));
	bars1->setAntialiased(false);
	bars1->setAntialiasedFill(false);
	bars1->setBrush(QColor("#705BE8"));
	bars1->keyAxis()->setAutoTicks(false);
	bars1->keyAxis()->setTickVector(x3);
	bars1->keyAxis()->setSubTickCount(0);

	// rescale axes according to graph's data:
	mainGraph1->rescaleAxes();
	mainGraph2->rescaleAxes();
	graph2->rescaleAxes();
	bars1->rescaleAxes();
	wideAxisRect->axis(QCPAxis::atLeft, 1)->setRangeLower(0);
}

void MainWindow::setupColorMapDemo(QCustomPlot *customPlot)
{
	demoName = "Color Map Demo";

	// configure axis rect:
	customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
	customPlot->axisRect()->setupFullAxesBox(true);
	customPlot->xAxis->setLabel("x");
	customPlot->yAxis->setLabel("y");

	// set up the QCPColorMap:
	QCPColorMap *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
	customPlot->addPlottable(colorMap);
	int nx = 200;
	int ny = 200;
	colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
	colorMap->data()->setRange(QCPRange(-4, 4), QCPRange(-4, 4)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
																  // now we assign some data, by accessing the QCPColorMapData instance of the color map:
	double x, y, z;
	for (int xIndex = 0; xIndex<nx; ++xIndex)
	{
		for (int yIndex = 0; yIndex<ny; ++yIndex)
		{
			colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
			double r = 3 * qSqrt(x*x + y*y) + 1e-2;
			z = 2 * x*(qCos(r + 2) / r - qSin(r + 2) / r); // the B field strength of dipole radiation (modulo physical constants)
			colorMap->data()->setCell(xIndex, yIndex, z);
		}
	}

	// add a color scale:
	QCPColorScale *colorScale = new QCPColorScale(customPlot);
	customPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
	colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
	colorMap->setColorScale(colorScale); // associate the color map with the color scale
	colorScale->axis()->setLabel("Magnetic Field Strength");

	// set the color gradient of the color map to one of the presets:
	colorMap->setGradient(QCPColorGradient::gpPolar);
	// we could have also created a QCPColorGradient instance and added own colors to
	// the gradient, see the documentation of QCPColorGradient for what's possible.

	// rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
	colorMap->rescaleDataRange();

	// make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
	QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
	customPlot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);
	colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);

	// rescale the key (x) and value (y) axes so the whole color map is visible:
	customPlot->rescaleAxes();
}

void MainWindow::setupFinancialDemo(QCustomPlot *customPlot)
{
	demoName = "Financial Charts Demo";
	customPlot->legend->setVisible(true);

	// generate two sets of random walk data (one for candlestick and one for ohlc chart):
	int n = 500;
	QVector<double> time(n), value1(n), value2(n);
	QDateTime start = QDateTime(QDate(2014, 6, 11));
	start.setTimeSpec(Qt::UTC);
	double startTime = start.toTime_t();
	double binSize = 3600 * 24; // bin data in 1 day intervals
	time[0] = startTime;
	value1[0] = 60;
	value2[0] = 20;
	qsrand(9);
	for (int i = 1; i<n; ++i)
	{
		time[i] = startTime + 3600 * i;
		value1[i] = value1[i - 1] + (qrand() / (double)RAND_MAX - 0.5) * 10;
		value2[i] = value2[i - 1] + (qrand() / (double)RAND_MAX - 0.5) * 3;
	}

	// create candlestick chart:
	QCPFinancial *candlesticks = new QCPFinancial(customPlot->xAxis, customPlot->yAxis);
	customPlot->addPlottable(candlesticks);
	QCPFinancialDataMap data1 = QCPFinancial::timeSeriesToOhlc(time, value1, binSize, startTime);
	candlesticks->setName("Candlestick");
	candlesticks->setChartStyle(QCPFinancial::csCandlestick);
	candlesticks->setData(&data1, true);
	candlesticks->setWidth(binSize*0.9);
	candlesticks->setTwoColored(true);
	candlesticks->setBrushPositive(QColor(245, 245, 245));
	candlesticks->setBrushNegative(QColor(0, 0, 0));
	candlesticks->setPenPositive(QPen(QColor(0, 0, 0)));
	candlesticks->setPenNegative(QPen(QColor(0, 0, 0)));

	// create ohlc chart:
	QCPFinancial *ohlc = new QCPFinancial(customPlot->xAxis, customPlot->yAxis);
	customPlot->addPlottable(ohlc);
	QCPFinancialDataMap data2 = QCPFinancial::timeSeriesToOhlc(time, value2, binSize / 3.0, startTime); // divide binSize by 3 just to make the ohlc bars a bit denser
	ohlc->setName("OHLC");
	ohlc->setChartStyle(QCPFinancial::csOhlc);
	ohlc->setData(&data2, true);
	ohlc->setWidth(binSize*0.2);
	ohlc->setTwoColored(true);

	// create bottom axis rect for volume bar chart:
	QCPAxisRect *volumeAxisRect = new QCPAxisRect(customPlot);
	customPlot->plotLayout()->addElement(1, 0, volumeAxisRect);
	volumeAxisRect->setMaximumSize(QSize(QWIDGETSIZE_MAX, 100));
	volumeAxisRect->axis(QCPAxis::atBottom)->setLayer("axes");
	volumeAxisRect->axis(QCPAxis::atBottom)->grid()->setLayer("grid");
	// bring bottom and main axis rect closer together:
	customPlot->plotLayout()->setRowSpacing(0);
	volumeAxisRect->setAutoMargins(QCP::msLeft | QCP::msRight | QCP::msBottom);
	volumeAxisRect->setMargins(QMargins(0, 0, 0, 0));
	// create two bar plottables, for positive (green) and negative (red) volume bars:
	QCPBars *volumePos = new QCPBars(volumeAxisRect->axis(QCPAxis::atBottom), volumeAxisRect->axis(QCPAxis::atLeft));
	QCPBars *volumeNeg = new QCPBars(volumeAxisRect->axis(QCPAxis::atBottom), volumeAxisRect->axis(QCPAxis::atLeft));
	for (int i = 0; i<n / 5; ++i)
	{
		int v = qrand() % 20000 + qrand() % 20000 + qrand() % 20000 - 10000 * 3;
		(v < 0 ? volumeNeg : volumePos)->addData(startTime + 3600 * 5.0*i, qAbs(v)); // add data to either volumeNeg or volumePos, depending on sign of v
	}
	customPlot->setAutoAddPlottableToLegend(false);
	customPlot->addPlottable(volumePos);
	customPlot->addPlottable(volumeNeg);
	volumePos->setWidth(3600 * 4);
	volumePos->setPen(Qt::NoPen);
	volumePos->setBrush(QColor(100, 180, 110));
	volumeNeg->setWidth(3600 * 4);
	volumeNeg->setPen(Qt::NoPen);
	volumeNeg->setBrush(QColor(180, 90, 90));

	// interconnect x axis ranges of main and bottom axis rects:
	connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), volumeAxisRect->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
	connect(volumeAxisRect->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis, SLOT(setRange(QCPRange)));
	// configure axes of both main and bottom axis rect:
	volumeAxisRect->axis(QCPAxis::atBottom)->setAutoTickStep(false);
	volumeAxisRect->axis(QCPAxis::atBottom)->setTickStep(3600 * 24 * 4); // 4 day tickstep
	volumeAxisRect->axis(QCPAxis::atBottom)->setTickLabelType(QCPAxis::ltDateTime);
	volumeAxisRect->axis(QCPAxis::atBottom)->setDateTimeSpec(Qt::UTC);
	volumeAxisRect->axis(QCPAxis::atBottom)->setDateTimeFormat("dd. MMM");
	volumeAxisRect->axis(QCPAxis::atBottom)->setTickLabelRotation(15);
	volumeAxisRect->axis(QCPAxis::atLeft)->setAutoTickCount(3);
	customPlot->xAxis->setBasePen(Qt::NoPen);
	customPlot->xAxis->setTickLabels(false);
	customPlot->xAxis->setTicks(false); // only want vertical grid in main axis rect, so hide xAxis backbone, ticks, and labels
	customPlot->xAxis->setAutoTickStep(false);
	customPlot->xAxis->setTickStep(3600 * 24 * 4); // 4 day tickstep
	customPlot->rescaleAxes();
	customPlot->xAxis->scaleRange(1.025, customPlot->xAxis->range().center());
	customPlot->yAxis->scaleRange(1.1, customPlot->yAxis->range().center());

	// make axis rects' left side line up:
	QCPMarginGroup *group = new QCPMarginGroup(customPlot);
	customPlot->axisRect()->setMarginGroup(QCP::msLeft | QCP::msRight, group);
	volumeAxisRect->setMarginGroup(QCP::msLeft | QCP::msRight, group);
}

void MainWindow::realtimeDataSlot()
{
	// calculate two new data points:
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
	double key = 0;
#else
	double key = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
#endif
	static double lastPointKey = 0;
	if (key - lastPointKey > 0.01) // at most add point every 10 ms
	{
		double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
		double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
								   // add data to lines:
		ui->customPlot->graph(0)->addData(key, value0);
		ui->customPlot->graph(1)->addData(key, value1);
		// set data of dots:
		ui->customPlot->graph(2)->clearData();
		ui->customPlot->graph(2)->addData(key, value0);
		ui->customPlot->graph(3)->clearData();
		ui->customPlot->graph(3)->addData(key, value1);
		// remove data of lines that's outside visible range:
		ui->customPlot->graph(0)->removeDataBefore(key - 8);
		ui->customPlot->graph(1)->removeDataBefore(key - 8);
		// rescale value (vertical) axis to fit the current data:
		ui->customPlot->graph(0)->rescaleValueAxis();
		ui->customPlot->graph(1)->rescaleValueAxis(true);
		lastPointKey = key;
	}
	// make key axis range scroll with the data (at a constant range size of 8):
	ui->customPlot->xAxis->setRange(key + 0.25, 8, Qt::AlignRight);
	ui->customPlot->replot();

	// calculate frames per second:
	static double lastFpsKey;
	static int frameCount;
	++frameCount;
	if (key - lastFpsKey > 2) // average fps over 2 seconds
	{
		ui->statusBar->showMessage(
			QString("%1 FPS, Total Data points: %2")
			.arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
			.arg(ui->customPlot->graph(0)->data()->count() + ui->customPlot->graph(1)->data()->count())
			, 0);
		lastFpsKey = key;
		frameCount = 0;
	}
}

void MainWindow::bracketDataSlot()
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
	double secs = 0;
#else
	double secs = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
#endif

	// update data to make phase move:
	int n = 500;
	double phase = secs * 5;
	double k = 3;
	QVector<double> x(n), y(n);
	for (int i = 0; i<n; ++i)
	{
		x[i] = i / (double)(n - 1) * 34 - 17;
		y[i] = qExp(-x[i] * x[i] / 20.0)*qSin(k*x[i] + phase);
	}
	ui->customPlot->graph()->setData(x, y);

	itemDemoPhaseTracer->setGraphKey((8 * M_PI + fmod(M_PI*1.5 - phase, 6 * M_PI)) / k);

	ui->customPlot->replot();

	// calculate frames per second:
	double key = secs;
	static double lastFpsKey;
	static int frameCount;
	++frameCount;
	if (key - lastFpsKey > 2) // average fps over 2 seconds
	{
		ui->statusBar->showMessage(
			QString("%1 FPS, Total Data points: %2")
			.arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
			.arg(ui->customPlot->graph(0)->data()->count())
			, 0);
		lastFpsKey = key;
		frameCount = 0;
	}
}

void MainWindow::setupPlayground(QCustomPlot *customPlot)
{
	Q_UNUSED(customPlot)
}

void MainWindow::screenShot()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	QPixmap pm = QPixmap::grabWindow(qApp->desktop()->winId(), this->x() + 2, this->y() + 2, this->frameGeometry().width() - 4, this->frameGeometry().height() - 4);
#else
	QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x() + 2, this->y() + 2, this->frameGeometry().width() - 4, this->frameGeometry().height() - 4);
#endif
	QString fileName = demoName.toLower() + ".png";
	fileName.replace(" ", "");
	pm.save("./screenshots/" + fileName);
	qApp->quit();
}

void MainWindow::allScreenShots()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	QPixmap pm = QPixmap::grabWindow(qApp->desktop()->winId(), this->x() + 2, this->y() + 2, this->frameGeometry().width() - 4, this->frameGeometry().height() - 4);
#else
	QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x() + 2, this->y() + 2, this->frameGeometry().width() - 4, this->frameGeometry().height() - 4);
#endif
	QString fileName = demoName.toLower() + ".png";
	fileName.replace(" ", "");
	pm.save("./screenshots/" + fileName);

	if (currentDemoIndex < 19)
	{
		if (dataTimer.isActive())
			dataTimer.stop();
		dataTimer.disconnect();
		delete ui->customPlot;
		ui->customPlot = new QCustomPlot(ui->centralWidget);
		//ui->verticalLayout->addWidget(ui->customPlot);      //QT布局学习》》》》》》》》》》》》》》》》》》》》》》》》》》
		setupDemo(currentDemoIndex + 1);
		// setup delay for demos that need time to develop proper look:
		int delay = 250;
		if (currentDemoIndex == 10) // Next is Realtime data demo
			delay = 12000;
		else if (currentDemoIndex == 15) // Next is Item demo
			delay = 5000;
		QTimer::singleShot(delay, this, SLOT(allScreenShots()));
	}
	else
	{
		qApp->quit();
	}
}

