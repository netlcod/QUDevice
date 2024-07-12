#include "qublockingudpconfiguration.h"

QUBlockingUdpConfiguration::QUBlockingUdpConfiguration(QObject* parent) {
    setBindMode(0x00);
    setServerIp(QString());
    setServerPort(QString());
    setClientIp(QString());
    setClientPort(QString());
}

qint32 QUBlockingUdpConfiguration::bindMode() const {
    return m_cfg["bindMode"].toInt();
}

QString QUBlockingUdpConfiguration::serverIp() const {
    return m_cfg["serverIp"].toString();
}

QString QUBlockingUdpConfiguration::serverPort() const {
    return m_cfg["serverPort"].toString();
}

QString QUBlockingUdpConfiguration::clientIp() const {
    return m_cfg["clientIp"].toString();
}

QString QUBlockingUdpConfiguration::clientPort() const {
    return m_cfg["clientPort"].toString();
}

int QUBlockingUdpConfiguration::waitTimeout() const {
    int waitTimeout = m_cfg["waitTimeout"].toInt();

    return waitTimeout;
}

void QUBlockingUdpConfiguration::setBindMode(const qint32 bindMode) {
    m_cfg.insert("bindMode", bindMode);

    emit bindModeChanged(bindMode);
}

void QUBlockingUdpConfiguration::setServerIp(const QString serverIp) {
    m_cfg.insert("serverIp", serverIp);

    emit serverIpChanged(serverIp);
}

void QUBlockingUdpConfiguration::setServerPort(const QString serverPort) {
    m_cfg.insert("serverPort", serverPort);

    emit serverPortChanged(serverPort);
}

void QUBlockingUdpConfiguration::setClientIp(const QString clientIp) {
    m_cfg.insert("clientIp", clientIp);

    emit clientIpChanged(clientIp);
}

void QUBlockingUdpConfiguration::setClientPort(const QString clientPort) {
    m_cfg.insert("clientPort", clientPort);

    emit clientPortChanged(clientPort);
}

void QUBlockingUdpConfiguration::setWaitTimeout(const int waitTimeout) {
    m_cfg.insert("waitTimeout", waitTimeout);

    emit waitTimeoutChanged(waitTimeout);
}
