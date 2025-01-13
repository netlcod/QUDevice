#pragma once

#include <QBitArray>
#include <QByteArray>
#include <QList>
#include <QModbusClient>
#include <QModbusRtuSerialMaster>
#include <QModbusTcpClient>

#include "qudevice.h"

class QUDEVICE_EXPORT QUModbusMaster : public QUDevice {
    Q_OBJECT

    static uint32_t device_counter;

public:
    explicit QUModbusMaster(QObject* parent = nullptr);
    ~QUModbusMaster();

public slots:
    bool acquire();
    bool release();
    void readRegisters(QModbusDataUnit::RegisterType registerType, qint8 id, qint16 registerAddress, qint16 registerCount);
    void writeCoilRegisters(qint8 id, qint16 registerAddress, qint16 registerCount, QBitArray data);
    void writeHoldingRegisters(qint8 id, qint16 registerAddress, qint16 registerCount, QList<quint16> data);
    void readWriteHoldingRegisters(qint8 id,
        qint16 registerAddressRead,
        qint16 registerCountRead,
        qint16 registerAddressWrite,
        qint16 registerCountWrite,
        QList<quint16> data);
private slots:
    void onErrorOccured(QModbusDevice::Error error);
    void onStateChanged(QModbusDevice::State state);
    void onReadReady(QModbusDataUnit::RegisterType registerType, QModbusReply* reply);

signals:
    void opened(bool status);
    void closed(bool status);
    void error(const QString error) const;
    void readFinished(QStringList values);
    void readFinished(qint16 registerAddress, QBitArray values);
    void readFinished(qint16 registerAddress, QList<quint16> values);

private:
    QSharedPointer<QModbusClient> m_modbusDevice;
};
