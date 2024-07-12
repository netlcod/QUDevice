#pragma once

#include <QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

#include "quconfiguration.h"
#include "qudevice_export.h"

class QUDEVICE_EXPORT QUBlockingSerialPortConfiguration : public QUConfiguration {
    Q_OBJECT

    Q_PROPERTY(QSerialPortInfo portInfo READ portInfo WRITE setPortInfo NOTIFY portInfoChanged)
    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(QSerialPort::Direction openMode READ openMode WRITE setOpenMode NOTIFY openModeChanged)
    Q_PROPERTY(qint32 baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(QSerialPort::DataBits dataBits READ dataBits WRITE setDataBits NOTIFY dataBitsChanged)
    Q_PROPERTY(QSerialPort::Parity parity READ parity WRITE setParity NOTIFY parityChanged)
    Q_PROPERTY(QSerialPort::StopBits stopBits READ stopBits WRITE setStopBits NOTIFY stopBitsChanged)
    Q_PROPERTY(QSerialPort::FlowControl flowControl READ flowControl WRITE setFlowControl NOTIFY flowControlChanged)
    Q_PROPERTY(int waitTimeout READ waitTimeout WRITE setWaitTimeout NOTIFY waitTimeoutChanged)

public:
    explicit QUBlockingSerialPortConfiguration(QObject* parent = nullptr);
    ~QUBlockingSerialPortConfiguration() = default;

    QSerialPortInfo portInfo() const;
    QString portName() const;
    QSerialPort::Direction openMode() const;
    qint32 baudRate() const;
    QSerialPort::DataBits dataBits() const;
    QSerialPort::Parity parity() const;
    QSerialPort::StopBits stopBits() const;
    QSerialPort::FlowControl flowControl() const;
    int waitTimeout() const;

public slots:
    void setPortInfo(const QSerialPortInfo portInfo);
    void setPortName(const QString portName);
    void setOpenMode(const QSerialPort::Direction openMode);
    void setBaudRate(const qint32 baudRate);
    void setDataBits(const QSerialPort::DataBits dataBits);
    void setParity(const QSerialPort::Parity parity);
    void setStopBits(const QSerialPort::StopBits stopBits);
    void setFlowControl(const QSerialPort::FlowControl flowControl);
    void setWaitTimeout(const int waitTimeout);

signals:
    void portInfoChanged(const QSerialPortInfo portInfo);
    void portNameChanged(const QString portName);
    void openModeChanged(const QSerialPort::Direction openMode);
    void baudRateChanged(const qint32 baudRate);
    void dataBitsChanged(const QSerialPort::DataBits dataBits);
    void parityChanged(const QSerialPort::Parity parity);
    void stopBitsChanged(const QSerialPort::StopBits stopBits);
    void flowControlChanged(const QSerialPort::FlowControl flowControl);
    void waitTimeoutChanged(const int waitTimeout);
};
