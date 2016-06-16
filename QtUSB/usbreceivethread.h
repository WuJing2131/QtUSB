#ifndef USBRECEIVETHREAD_H
#define USBRECEIVETHREAD_H

#include <QMutex>
#include <QThread>

class UsbReceiveThread : public QThread
{
    Q_OBJECT

public:
    UsbReceiveThread(QObject *parent = 0);
    ~UsbReceiveThread();

   bool startProcess();

signals:
   void Sendresponse(const QString &s,int len);
   void SendDataresponse(char &ReadReportBuffer);
public slots:
    void stopProcess();
	void HexAsciiFormatSetting(bool);

protected:
    void run();

private:
    bool m_abort;
	bool isHexFormat;
    //QImage m_image;
    QMutex mutex;
};

#endif
