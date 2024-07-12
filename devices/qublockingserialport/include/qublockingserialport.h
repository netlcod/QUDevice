#pragma once

#include <QSerialPort>
#include <QVector>

#include "qudevice.h"

class QUDEVICE_EXPORT QUBlockingSerialPort : public QUDevice {
    Q_OBJECT

    static uint32_t device_counter;

public:
    explicit QUBlockingSerialPort(QObject* parent = nullptr);
    ~QUBlockingSerialPort();

public slots:
    bool acquire();
    qint64 write(QByteArray data);
    QByteArray read();
    void release();

private slots:
    void onErrorOccurred(QSerialPort::SerialPortError err);

signals:
    void opened(bool status);
    void closed();
    void error(const QString error);

private:
    QSharedPointer<QSerialPort> m_port;
    int m_waitTimeout;
};