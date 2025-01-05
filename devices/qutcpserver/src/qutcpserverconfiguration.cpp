#include "qutcpserverconfiguration.h"

QUTcpServerConfiguration::QUTcpServerConfiguration(QObject* parent) {
    setServerIp(QString());
    setServerPort(QString());
}

QString QUTcpServerConfiguration::serverIp() const {
    return m_cfg["serverIp"].toString();
}

QString QUTcpServerConfiguration::serverPort() const {
    return m_cfg["serverPort"].toString();
}

void QUTcpServerConfiguration::setServerIp(const QString serverIp) {
    m_cfg.insert("serverIp", serverIp);

    emit serverIpChanged(serverIp);
}

void QUTcpServerConfiguration::setServerPort(const QString serverPort) {
    m_cfg.insert("serverPort", serverPort);

    emit serverPortChanged(serverPort);
}
