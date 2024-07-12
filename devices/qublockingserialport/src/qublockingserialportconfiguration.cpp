#include "qublockingserialportconfiguration.h"

QUBlockingSerialPortConfiguration::QUBlockingSerialPortConfiguration(QObject* parent) {
    setPortInfo(QSerialPortInfo());
    setPortName(QString());
    setOpenMode(QSerialPort::Direction::AllDirections);
    setBaudRate(115200);
    setDataBits(QSerialPort::DataBits::Data8);
    setParity(QSerialPort::Parity::NoParity);
    setStopBits(QSerialPort::StopBits::OneStop);
    setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    setWaitTimeout(1000);
}

QSerialPortInfo QUBlockingSerialPortConfiguration::portInfo() const {
    QString portName = m_cfg["portName"].toString();

    return QSerialPortInfo(portName);
}

QString QUBlockingSerialPortConfiguration::portName() const {
    QString portName = m_cfg["portName"].toString();

    return portName;
}

QSerialPort::Direction QUBlockingSerialPortConfiguration::openMode() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Direction>();

    return static_cast<QSerialPort::Direction>(metaEnum.keyToValue(m_cfg["openMode"].toString().toStdString().c_str()));
}

qint32 QUBlockingSerialPortConfiguration::baudRate() const {
    return m_cfg["baudRate"].toInt();
}

QSerialPort::DataBits QUBlockingSerialPortConfiguration::dataBits() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::DataBits>();

    return static_cast<QSerialPort::DataBits>(metaEnum.keyToValue(m_cfg["dataBits"].toString().toStdString().c_str()));
}

QSerialPort::Parity QUBlockingSerialPortConfiguration::parity() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Parity>();

    return static_cast<QSerialPort::Parity>(metaEnum.keyToValue(m_cfg["parity"].toString().toStdString().c_str()));
}

QSerialPort::StopBits QUBlockingSerialPortConfiguration::stopBits() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::StopBits>();

    return static_cast<QSerialPort::StopBits>(metaEnum.keyToValue(m_cfg["stopBits"].toString().toStdString().c_str()));
}

QSerialPort::FlowControl QUBlockingSerialPortConfiguration::flowControl() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::FlowControl>();

    return static_cast<QSerialPort::FlowControl>(metaEnum.keyToValue(m_cfg["flowControl"].toString().toStdString().c_str()));
}

int QUBlockingSerialPortConfiguration::waitTimeout() const {
    int waitTimeout = m_cfg["waitTimeout"].toInt();

    return waitTimeout;
}

void QUBlockingSerialPortConfiguration::setPortInfo(const QSerialPortInfo portInfo) {
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

void QUBlockingSerialPortConfiguration::setPortName(const QString portName) {
    m_cfg.insert("portName", portName);

    emit portNameChanged(portName);
}

void QUBlockingSerialPortConfiguration::setOpenMode(const QSerialPort::Direction openMode) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Direction>();
    m_cfg.insert("openMode", metaEnum.valueToKey(openMode));

    emit openModeChanged(openMode);
}

void QUBlockingSerialPortConfiguration::setBaudRate(const qint32 baudRate) {
    m_cfg.insert("baudRate", baudRate);

    emit baudRateChanged(baudRate);
}

void QUBlockingSerialPortConfiguration::setDataBits(const QSerialPort::DataBits dataBits) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::DataBits>();
    m_cfg.insert("dataBits", metaEnum.valueToKey(dataBits));

    emit dataBitsChanged(dataBits);
}

void QUBlockingSerialPortConfiguration::setParity(const QSerialPort::Parity parity) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Parity>();
    m_cfg.insert("parity", metaEnum.valueToKey(parity));

    emit parityChanged(parity);
}

void QUBlockingSerialPortConfiguration::setStopBits(const QSerialPort::StopBits stopBits) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::StopBits>();
    m_cfg.insert("stopBits", metaEnum.valueToKey(stopBits));

    emit stopBitsChanged(stopBits);
}

void QUBlockingSerialPortConfiguration::setFlowControl(const QSerialPort::FlowControl flowControl) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::FlowControl>();
    m_cfg.insert("flowControl", metaEnum.valueToKey(flowControl));

    emit flowControlChanged(flowControl);
}

void QUBlockingSerialPortConfiguration::setWaitTimeout(const int waitTimeout) {
    m_cfg.insert("waitTimeout", waitTimeout);

    emit waitTimeoutChanged(waitTimeout);
}
