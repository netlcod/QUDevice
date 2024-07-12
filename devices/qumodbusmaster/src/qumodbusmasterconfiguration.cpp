#include "qumodbusmasterconfiguration.h"

QUModbusMasterConfiguration::QUModbusMasterConfiguration(QObject* parent) {
    setMasterType(MasterType::Serial);
    setPortInfo(QSerialPortInfo());
    setPortName(QString());
    setOpenMode(QSerialPort::Direction::AllDirections);
    setBaudRate(115200);
    setDataBits(QSerialPort::DataBits::Data8);
    setParity(QSerialPort::Parity::NoParity);
    setStopBits(QSerialPort::StopBits::OneStop);
    setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    setIp(QString());
    setPort(QString());
}

QUModbusMasterConfiguration::MasterType QUModbusMasterConfiguration::masterType() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QUModbusMasterConfiguration::MasterType>();

    return static_cast<QUModbusMasterConfiguration::MasterType>(metaEnum.keyToValue(m_cfg["masterType"].toString().toStdString().c_str()));
}

QSerialPortInfo QUModbusMasterConfiguration::portInfo() const {
    QString portName = m_cfg["portName"].toString();
    QSerialPortInfo portInfo(portName);

    return portInfo;
}

QString QUModbusMasterConfiguration::portName() const {
    QString portName = m_cfg["portName"].toString();

    return portName;
}

QSerialPort::Direction QUModbusMasterConfiguration::openMode() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Direction>();

    return static_cast<QSerialPort::Direction>(metaEnum.keyToValue(m_cfg["openMode"].toString().toStdString().c_str()));
}

qint32 QUModbusMasterConfiguration::baudRate() const {
    return m_cfg["baudRate"].toInt();
}

QSerialPort::DataBits QUModbusMasterConfiguration::dataBits() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::DataBits>();

    return static_cast<QSerialPort::DataBits>(metaEnum.keyToValue(m_cfg["dataBits"].toString().toStdString().c_str()));
}

QSerialPort::Parity QUModbusMasterConfiguration::parity() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Parity>();

    return static_cast<QSerialPort::Parity>(metaEnum.keyToValue(m_cfg["parity"].toString().toStdString().c_str()));
}

QSerialPort::StopBits QUModbusMasterConfiguration::stopBits() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::StopBits>();

    return static_cast<QSerialPort::StopBits>(metaEnum.keyToValue(m_cfg["stopBits"].toString().toStdString().c_str()));
}

QSerialPort::FlowControl QUModbusMasterConfiguration::flowControl() const {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::FlowControl>();

    return static_cast<QSerialPort::FlowControl>(metaEnum.keyToValue(m_cfg["flowControl"].toString().toStdString().c_str()));
}

QString QUModbusMasterConfiguration::ip() const {
    return m_cfg["ip"].toString();
}

QString QUModbusMasterConfiguration::port() const {
    return m_cfg["port"].toString();
}

int QUModbusMasterConfiguration::responseTime() const {
    return m_cfg["responseTime"].toInt();
}

int QUModbusMasterConfiguration::numberOfRetries() const {
    return m_cfg["numberOfRetries"].toInt();
}

void QUModbusMasterConfiguration::setMasterType(const MasterType masterType) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QUModbusMasterConfiguration::MasterType>();
    m_cfg.insert("masterType", metaEnum.valueToKey(masterType));

    emit masterTypeChanged(masterType);
}

void QUModbusMasterConfiguration::setPortInfo(const QSerialPortInfo portInfo) {
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

void QUModbusMasterConfiguration::setPortName(const QString portName) {
    m_cfg.insert("portName", portName);

    emit portNameChanged(portName);
}

void QUModbusMasterConfiguration::setOpenMode(const QSerialPort::Direction openMode) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Direction>();
    m_cfg.insert("openMode", metaEnum.valueToKey(openMode));

    emit openModeChanged(openMode);
}

void QUModbusMasterConfiguration::setBaudRate(const qint32 baudRate) {
    m_cfg.insert("baudRate", baudRate);

    emit baudRateChanged(baudRate);
}

void QUModbusMasterConfiguration::setDataBits(const QSerialPort::DataBits dataBits) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::DataBits>();
    m_cfg.insert("dataBits", metaEnum.valueToKey(dataBits));

    emit dataBitsChanged(dataBits);
}

void QUModbusMasterConfiguration::setParity(const QSerialPort::Parity parity) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Parity>();
    m_cfg.insert("parity", metaEnum.valueToKey(parity));

    emit parityChanged(parity);
}

void QUModbusMasterConfiguration::setStopBits(const QSerialPort::StopBits stopBits) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::StopBits>();
    m_cfg.insert("stopBits", metaEnum.valueToKey(stopBits));

    emit stopBitsChanged(stopBits);
}

void QUModbusMasterConfiguration::setFlowControl(const QSerialPort::FlowControl flowControl) {
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::FlowControl>();
    m_cfg.insert("flowControl", metaEnum.valueToKey(flowControl));

    emit flowControlChanged(flowControl);
}

void QUModbusMasterConfiguration::setIp(const QString ip) {
    m_cfg.insert("ip", ip);

    emit ipChanged(ip);
}

void QUModbusMasterConfiguration::setPort(const QString port) {
    m_cfg.insert("port", port);

    emit portChanged(port);
}

void QUModbusMasterConfiguration::setResponseTime(const int responseTime) {
    m_cfg.insert("responseTime", responseTime);

    emit responseTimeChanged(responseTime);
}

void QUModbusMasterConfiguration::setNumberOfRetries(const int numberOfRetries) {
    m_cfg.insert("numberOfRetries", numberOfRetries);

    emit numberOfRetriesChanged(numberOfRetries);
}
