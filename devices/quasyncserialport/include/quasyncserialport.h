#pragma once

#include <QSerialPort>
#include <QVector>

#include "qudevice.h"

class QUDEVICE_EXPORT QUAsyncSerialPort : public QUDevice {
    Q_OBJECT

    static uint32_t device_counter;

public:
    explicit QUAsyncSerialPort(QObject* parent = nullptr);
    ~QUAsyncSerialPort();

public slots:
    bool acquire();
    qint64 write(QByteArray data);
    void release();

private slots:
    void onReceiveData();
    void onErrorOccurred(QSerialPort::SerialPortError err);

signals:
    void opened(bool status);
    void closed();
    void error(const QString error);
    void outputBytes(const QByteArray data);

private:
    QSharedPointer<QSerialPort> m_port;
};
