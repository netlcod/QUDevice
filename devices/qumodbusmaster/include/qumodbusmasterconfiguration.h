#pragma once

#include <QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

#include "quconfiguration.h"
#include "qudevice_export.h"

class QUDEVICE_EXPORT QUModbusMasterConfiguration : public QUConfiguration {
    Q_OBJECT

    Q_PROPERTY(MasterType masterType READ masterType WRITE setMasterType NOTIFY masterTypeChanged)
    Q_PROPERTY(QSerialPortInfo portInfo READ portInfo WRITE setPortInfo NOTIFY portInfoChanged)
    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(QSerialPort::Direction openMode READ openMode WRITE setOpenMode NOTIFY openModeChanged)
    Q_PROPERTY(qint32 baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(QSerialPort::DataBits dataBits READ dataBits WRITE setDataBits NOTIFY dataBitsChanged)
    Q_PROPERTY(QSerialPort::Parity parity READ parity WRITE setParity NOTIFY parityChanged)
    Q_PROPERTY(QSerialPort::StopBits stopBits READ stopBits WRITE setStopBits NOTIFY stopBitsChanged)
    Q_PROPERTY(QSerialPort::FlowControl flowControl READ flowControl WRITE setFlowControl NOTIFY flowControlChanged)
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(QString port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(int responseTime READ responseTime WRITE setResponseTime NOTIFY responseTimeChanged)
    Q_PROPERTY(int numberOfRetries READ numberOfRetries WRITE setNumberOfRetries NOTIFY numberOfRetriesChanged)

public:
    enum MasterType { Serial, Tcp };
    Q_ENUM(MasterType)

    explicit QUModbusMasterConfiguration(QObject* parent = nullptr);
    ~QUModbusMasterConfiguration() = default;

    MasterType masterType() const;
    QSerialPortInfo portInfo() const;
    QString portName() const;
    QSerialPort::Direction openMode() const;
    qint32 baudRate() const;
    QSerialPort::DataBits dataBits() const;
    QSerialPort::Parity parity() const;
    QSerialPort::StopBits stopBits() const;
    QSerialPort::FlowControl flowControl() const;
    QString ip() const;
    QString port() const;
    int responseTime() const;
    int numberOfRetries() const;

public slots:
    void setMasterType(const QUModbusMasterConfiguration::MasterType masterType);
    void setPortInfo(const QSerialPortInfo portInfo);
    void setPortName(const QString portName);
    void setOpenMode(const QSerialPort::Direction openMode);
    void setBaudRate(const qint32 baudRate);
    void setDataBits(const QSerialPort::DataBits dataBits);
    void setParity(const QSerialPort::Parity parity);
    void setStopBits(const QSerialPort::StopBits stopBits);
    void setFlowControl(const QSerialPort::FlowControl flowControl);
    void setIp(const QString ip);
    void setPort(const QString port);
    void setResponseTime(const int responseTime);
    void setNumberOfRetries(const int numberOfRetries);

signals:
    void masterTypeChanged(const QUModbusMasterConfiguration::MasterType masterType);
    void portInfoChanged(const QSerialPortInfo portInfo);
    void portNameChanged(const QString portName);
    void openModeChanged(const QSerialPort::Direction openMode);
    void baudRateChanged(const qint32 baudRate);
    void dataBitsChanged(const QSerialPort::DataBits dataBits);
    void parityChanged(const QSerialPort::Parity parity);
    void stopBitsChanged(const QSerialPort::StopBits stopBits);
    void flowControlChanged(const QSerialPort::FlowControl flowControl);
    void ipChanged(const QString ip);
    void portChanged(const QString port);
    void responseTimeChanged(const int responseTime);
    void numberOfRetriesChanged(const int numberOfRetries);
};
