#pragma once

#include <QAbstractSocket>

#include "quconfiguration.h"
#include "qudevice_export.h"

class QUDEVICE_EXPORT QUBlockingUdpConfiguration : public QUConfiguration {
    Q_OBJECT

    Q_PROPERTY(qint32 bindMode READ bindMode WRITE setBindMode NOTIFY bindModeChanged)
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(QString serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(QString clientIp READ clientIp WRITE setClientIp NOTIFY clientIpChanged)
    Q_PROPERTY(QString clientPort READ clientPort WRITE setClientPort NOTIFY clientPortChanged)
    Q_PROPERTY(int waitTimeout READ waitTimeout WRITE setWaitTimeout NOTIFY waitTimeoutChanged)

public:
    explicit QUBlockingUdpConfiguration(QObject* parent = nullptr);
    ~QUBlockingUdpConfiguration() = default;

    qint32 bindMode() const;
    QString serverIp() const;
    QString serverPort() const;
    QString clientIp() const;
    QString clientPort() const;
    int waitTimeout() const;

public slots:
    void setBindMode(const qint32 bindMode);
    void setServerIp(const QString serverIp);
    void setServerPort(const QString serverPort);
    void setClientIp(const QString clientIp);
    void setClientPort(const QString clientPort);
    void setWaitTimeout(const int waitTimeout);

signals:
    void bindModeChanged(const qint32 bindMode);
    void serverIpChanged(const QString serverIp);
    void serverPortChanged(const QString serverPort);
    void clientIpChanged(const QString clientIp);
    void clientPortChanged(const QString clientPort);
    void waitTimeoutChanged(const int waitTimeout);
};
