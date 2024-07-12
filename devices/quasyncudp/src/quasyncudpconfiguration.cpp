#include "quasyncudpconfiguration.h"

QUAsyncUdpConfiguration::QUAsyncUdpConfiguration(QObject* parent) {
    setBindMode(0x00);
    setServerIp(QString());
    setServerPort(QString());
    setClientIp(QString());
    setClientPort(QString());
}

qint32 QUAsyncUdpConfiguration::bindMode() const {
    return m_cfg["bindMode"].toInt();
}

QString QUAsyncUdpConfiguration::serverIp() const {
    return m_cfg["serverIp"].toString();
}

QString QUAsyncUdpConfiguration::serverPort() const {
    return m_cfg["serverPort"].toString();
}

QString QUAsyncUdpConfiguration::clientIp() const {
    return m_cfg["clientIp"].toString();
}

QString QUAsyncUdpConfiguration::clientPort() const {
    return m_cfg["clientPort"].toString();
}

void QUAsyncUdpConfiguration::setBindMode(const qint32 bindMode) {
    m_cfg.insert("bindMode", bindMode);

    emit bindModeChanged(bindMode);
}

void QUAsyncUdpConfiguration::setServerIp(const QString serverIp) {
    m_cfg.insert("serverIp", serverIp);

    emit serverIpChanged(serverIp);
}

void QUAsyncUdpConfiguration::setServerPort(const QString serverPort) {
    m_cfg.insert("serverPort", serverPort);

    emit serverPortChanged(serverPort);
}

void QUAsyncUdpConfiguration::setClientIp(const QString clientIp) {
    m_cfg.insert("clientIp", clientIp);

    emit clientIpChanged(clientIp);
}

void QUAsyncUdpConfiguration::setClientPort(const QString clientPort) {
    m_cfg.insert("clientPort", clientPort);

    emit clientPortChanged(clientPort);
}
