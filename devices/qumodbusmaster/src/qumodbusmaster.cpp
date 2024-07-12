#include "qumodbusmaster.h"
#include "qumodbusmasterconfiguration.h"

#include <QDataStream>
#include <QTime>

uint32_t QUModbusMaster::device_counter = 0;

QUModbusMaster::QUModbusMaster(QObject* parent) {
    device_counter++;
    setName("QUModbusMaster " + QString::number(device_counter));
#ifdef QT_DEBUG
    qDebug() << "QUModbusMaster::QUModbusMaster" << m_name;
#endif
}

QUModbusMaster::~QUModbusMaster() {
#ifdef QT_DEBUG
    qDebug() << "QUModbusMaster::~QUModbusMaster" << m_name;
#endif
    device_counter--;
}

bool QUModbusMaster::acquire() {
#ifdef QT_DEBUG
    qDebug() << "QUModbusMaster::acquire" << m_name;
#endif
    QSharedPointer<QUModbusMasterConfiguration> cfg = qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg);

    if (cfg->masterType() == QUModbusMasterConfiguration::MasterType::Serial) {
        m_modbusDevice = QSharedPointer<QModbusRtuSerialMaster>::create();
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, cfg->portName());
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, cfg->baudRate());
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, cfg->dataBits());
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, cfg->parity());
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, cfg->stopBits());
    } else if (cfg->masterType() == QUModbusMasterConfiguration::MasterType::Tcp) {
        m_modbusDevice = QSharedPointer<QModbusTcpClient>::create();
        m_modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, cfg->ip());
        m_modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, cfg->port());
    }
    m_modbusDevice->setTimeout(cfg->responseTime());
    m_modbusDevice->setNumberOfRetries(cfg->numberOfRetries());

    connect(m_modbusDevice.get(), &QModbusClient::errorOccurred, this, &QUModbusMaster::onErrorOccured);
    connect(m_modbusDevice.get(), &QModbusClient::stateChanged, this, &QUModbusMaster::onStateChanged);

    bool status = m_modbusDevice->connectDevice();
    emit opened(status);

    return status;
}

bool QUModbusMaster::release() {
#ifdef QT_DEBUG
    qDebug() << "QUModbusMaster::release" << m_name;
#endif
    if (m_modbusDevice->state() == QModbusDevice::ConnectedState) {
        m_modbusDevice->disconnectDevice();
        emit closed();
    }

    return true;
}

void QUModbusMaster::readRegisters(QModbusDataUnit::RegisterType registerType, qint8 id, qint16 registerAddress, qint16 registerCount) {
    QModbusDataUnit readUnit = QModbusDataUnit(registerType, registerAddress, registerCount);

    if (auto* reply = m_modbusDevice->sendReadRequest(readUnit, id)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, registerType, reply]() { onReadReady(registerType, reply); });
        } else {
            reply->deleteLater();
        }
    }
}

void QUModbusMaster::writeCoilRegisters(qint8 id, qint16 registerAddress, qint16 registerCount, QBitArray data) {
    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, registerAddress, registerCount);
    for (uint i = 0; i < writeUnit.valueCount(); ++i) {
        writeUnit.setValue(i, data[i]);
    }

    if (auto* reply = m_modbusDevice->sendWriteRequest(writeUnit, id)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() { reply->deleteLater(); });
        } else {
            reply->deleteLater();
        }
    }
}

void QUModbusMaster::writeHoldingRegisters(qint8 id, qint16 registerAddress, qint16 registerCount, QList<quint16> data) {
    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, registerAddress, registerCount);
    for (uint i = 0; i < writeUnit.valueCount(); ++i) {
        writeUnit.setValue(i, data[i]);
    }

    if (auto* reply = m_modbusDevice->sendWriteRequest(writeUnit, id)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() { reply->deleteLater(); });
        } else {
            reply->deleteLater();
        }
    }
}

void QUModbusMaster::readWriteHoldingRegisters(qint8 id,
    qint16 registerAddressRead,
    qint16 registerCountRead,
    qint16 registerAddressWrite,
    qint16 registerCountWrite,
    QList<quint16> data) {
    QModbusDataUnit readUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, registerAddressRead, registerCountRead);

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, registerAddressWrite, registerCountWrite);
    for (uint i = 0; i < writeUnit.valueCount(); ++i) {
        writeUnit.setValue(i, data[i]);
    }

    if (auto* reply = m_modbusDevice->sendReadWriteRequest(readUnit, writeUnit, id)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() { onReadReady(QModbusDataUnit::HoldingRegisters, reply); });
        } else {
            reply->deleteLater();
        }
    }
}

void QUModbusMaster::onErrorOccured(QModbusDevice::Error err) {
    QString errString = m_modbusDevice->errorString();
#ifdef QT_DEBUG
    qDebug() << "QUModbusMaster::onErrorOccurred" << m_name << err;
#endif
    emit error(errString);
}

void QUModbusMaster::onStateChanged(QModbusDevice::State state) {
    emit readFinished(QStringList(QMetaEnum::fromType<QModbusDevice::Error>().valueToKey(state)));
}

void QUModbusMaster::onReadReady(QModbusDataUnit::RegisterType registerType, QModbusReply* reply) {
    QModbusDevice::Error error = reply->error();
    if (!error) {
        const QModbusDataUnit unit = reply->result();
        QStringList resultsList;
        ushort count = unit.valueCount();
        if (registerType == QModbusDataUnit::Coils || registerType == QModbusDataUnit::DiscreteInputs) {
            QBitArray values(count);
            for (uint i = 0; i < count; ++i) {
                values[i]     = unit.value(i);
                QString entry = tr("%1: Address: %2, Value: %3")
                                    .arg(QTime::currentTime().toString())
                                    .arg(unit.startAddress() + i)
                                    .arg(QString::number(unit.value(i), 2));
                resultsList += entry;
            }
            emit readFinished(unit.startAddress(), values);
        } else {
            QList<quint16> values;
            for (uint i = 0; i < count; ++i) {
                values.append(unit.value(i));
                QString entry = tr("%1: Address: %2, Value: 0x%3")
                                    .arg(QTime::currentTime().toString())
                                    .arg(unit.startAddress() + i)
                                    .arg(QString::number(unit.value(i), 16));
                resultsList += entry;
            }
            emit readFinished(unit.startAddress(), values);
        }
        emit readFinished(resultsList);
    }
    reply->deleteLater();
}
