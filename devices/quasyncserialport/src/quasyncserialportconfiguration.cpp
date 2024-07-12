#include "quasyncserialportconfiguration.h"

QUAsyncSerialPortConfiguration::QUAsyncSerialPortConfiguration(QObject* parent) {
    setPortInfo(QSerialPortInfo());
    setPortName(QString());
    setOpenMode(QSerialPort::Direction::AllDirections);
    setBaudRate(115200);
    setDataBits(QSerialPort::DataBits::Data8);
    setParity(QSerialPort::Parity::NoParity);
    setStopBits(QSerialPort::StopBits::OneStop);
    setFlowControl(QSerialPort::FlowControl::NoFlowControl);
}

QSerialPortInfo QUAsyncSerialPortConfiguration::portInfo() const {
    QString portName = m_cfg["portName"].toString();

    return QSerialPortInfo(portName);
}

QString QUAsyncSerialPortConfiguration::portName() const {
    QString portName = m_cfg["portName"].toString();

    return portName;
}

QSerialPort::Direction QUAsyncSerialPortConfiguration::openMode() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Direction>();

    return static_cast<QSerialPort::Direction>(metaEnum.keyToValue(m_cfg["openMode"].toString().toStdString().c_str()));
}

qint32 QUAsyncSerialPortConfiguration::baudRate() const {
    return m_cfg["baudRate"].toInt();
}

QSerialPort::DataBits QUAsyncSerialPortConfiguration::dataBits() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::DataBits>();

    return static_cast<QSerialPort::DataBits>(metaEnum.keyToValue(m_cfg["dataBits"].toString().toStdString().c_str()));
}

QSerialPort::Parity QUAsyncSerialPortConfiguration::parity() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Parity>();

    return static_cast<QSerialPort::Parity>(metaEnum.keyToValue(m_cfg["parity"].toString().toStdString().c_str()));
}

QSerialPort::StopBits QUAsyncSerialPortConfiguration::stopBits() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::StopBits>();

    return static_cast<QSerialPort::StopBits>(metaEnum.keyToValue(m_cfg["stopBits"].toString().toStdString().c_str()));
}

QSerialPort::FlowControl QUAsyncSerialPortConfiguration::flowControl() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::FlowControl>();

    return static_cast<QSerialPort::FlowControl>(metaEnum.keyToValue(m_cfg["flowControl"].toString().toStdString().c_str()));
}

void QUAsyncSerialPortConfiguration::setPortInfo(const QSerialPortInfo portInfo) {
    QJsonObject info;
    info.insert("description", portInfo.description());
    info.insert("manufacturer", portInfo.manufacturer());
    info.insert("portName", portInfo.portName());
    info.insert("productIdentifier", portInfo.productIdentifier());
    info.insert("serialNumber", portInfo.serialNumber());
    info.insert("vendorIdentifier", portInfo.vendorIdentifier());
    m_cfg.insert("portInfo", info);

    emit portInfoChanged(portInfo);
}

void QUAsyncSerialPortConfiguration::setPortName(const QString portName) {
    m_cfg.insert("portName", portName);

    emit portNameChanged(portName);
}

void QUAsyncSerialPortConfiguration::setOpenMode(const QSerialPort::Direction openMode) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Direction>();
    m_cfg.insert("openMode", metaEnum.valueToKey(openMode));

    emit openModeChanged(openMode);
}

void QUAsyncSerialPortConfiguration::setBaudRate(const qint32 baudRate) {
    m_cfg.insert("baudRate", baudRate);

    emit baudRateChanged(baudRate);
}

void QUAsyncSerialPortConfiguration::setDataBits(const QSerialPort::DataBits dataBits) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::DataBits>();
    m_cfg.insert("dataBits", metaEnum.valueToKey(dataBits));

    emit dataBitsChanged(dataBits);
}

void QUAsyncSerialPortConfiguration::setParity(const QSerialPort::Parity parity) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Parity>();
    m_cfg.insert("parity", metaEnum.valueToKey(parity));

    emit parityChanged(parity);
}

void QUAsyncSerialPortConfiguration::setStopBits(const QSerialPort::StopBits stopBits) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::StopBits>();
    m_cfg.insert("stopBits", metaEnum.valueToKey(stopBits));

    emit stopBitsChanged(stopBits);
}

void QUAsyncSerialPortConfiguration::setFlowControl(const QSerialPort::FlowControl flowControl) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::FlowControl>();
    m_cfg.insert("flowControl", metaEnum.valueToKey(flowControl));

    emit flowControlChanged(flowControl);
}
