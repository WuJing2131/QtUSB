#include "UsbReceivethread.h"
#include "libusb\include\lusb0_usb.h"
UsbReceiveThread::UsbReceiveThread(QObject *parent)
    : QThread(parent)
{
    m_abort = false;
}

UsbReceiveThread::~UsbReceiveThread()
{
    mutex.lock();
    m_abort = true;
    mutex.unlock();

    wait();
}


bool UsbReceiveThread::startProcess()
{
    //if (image.isNull())
    //    return;

    //m_image = image;
    m_abort = false;
	if (!isRunning())
	{
		start();
		return true;
	}
	else
	{
		return false;
	}
}

extern usb_dev_handle *gusb_handle;
//CWinThread *m_pReadReportThread = NULL;
extern OVERLAPPED ReadOverlapped;
extern char ReadReportBuffer[1024];
extern volatile BOOL m_bReadReportRunFlag;
extern uint  EP_IN;
extern uint  EP_OUT;
void UsbReceiveThread::run()
{
	ReadOverlapped.Offset = 0;
	ReadOverlapped.OffsetHigh = 0;
	ReadOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_bReadReportRunFlag = TRUE;
	while (TRUE)
	{
		if (!m_bReadReportRunFlag)
		{
			if (ReadOverlapped.hEvent != NULL)
			{
				CloseHandle(ReadOverlapped.hEvent);
				ReadOverlapped.hEvent = NULL;
			}
			return;
		}
		ResetEvent(ReadOverlapped.hEvent);
		//usb_bulk_read(gusb_handle, 0x81, ReadReportBuffer, sizeof(ReadReportBuffer), 100);
		int ret = usb_interrupt_read(gusb_handle, EP_IN, ReadReportBuffer, sizeof(ReadReportBuffer), 0);

		if (isHexFormat&&ret!=-116)
		{
			QString szStr;
			for (int i = 0; i < ret; i++)
			{
				szStr += QString::number(ReadReportBuffer[i] & 0xFF, 16) + " ";
			}
			emit Sendresponse(szStr, ret);
		}
		else if(!isHexFormat&&ret!=-116)
		{
			QString szStr = QString(QLatin1String(ReadReportBuffer));
			emit Sendresponse(szStr, ret);
		}
		
		/*if ((unsigned char)ReadReportBuffer[0] == 0x01 && (unsigned char)ReadReportBuffer[3] == 0xFF && (unsigned char)ReadReportBuffer[4] == 0xFC
			&& (unsigned char)ReadReportBuffer[5] == 0xFF && (unsigned char)ReadReportBuffer[6] == 0xFF)
		{
			emit Sendresponse(tr("UsbReceiveThread Receive Right!"));
			
		}*/
			
		ResetEvent(ReadOverlapped.hEvent);
		if (m_abort)
			return;
	}
}


void UsbReceiveThread::stopProcess()
{
    mutex.lock();
    m_abort = true;
    mutex.unlock();
}


void UsbReceiveThread::HexAsciiFormatSetting(bool tf)
{
	mutex.lock();
	isHexFormat = tf;
	mutex.unlock();
}