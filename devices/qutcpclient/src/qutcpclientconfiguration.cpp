#include "qutcpclientconfiguration.h"

QUTcpClientConfiguration::QUTcpClientConfiguration(QObject* parent) {
    setHostIp(QString());
    setHostPort(QString());
}

QString QUTcpClientConfiguration::hostIp() const {
    return m_cfg["hostIp"].toString();
}

QString QUTcpClientConfiguration::hostPort() const {
    return m_cfg["hostPort"].toString();
}

void QUTcpClientConfiguration::setHostIp(const QString hostIp) {
    m_cfg.insert("hostIp", hostIp);

    emit hostIpChanged(hostIp);
}

void QUTcpClientConfiguration::setHostPort(const QString hostPort) {
    m_cfg.insert("hostPort", hostPort);

    emit hostPortChanged(hostPort);
}
