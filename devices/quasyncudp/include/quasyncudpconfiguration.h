#pragma once

#include <QAbstractSocket>

#include "quconfiguration.h"
#include "qudevice_export.h"

class QUDEVICE_EXPORT QUAsyncUdpConfiguration : public QUConfiguration {
    Q_OBJECT

    Q_PROPERTY(qint32 bindMode READ bindMode WRITE setBindMode NOTIFY bindModeChanged)
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(QString serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(QString clientIp READ clientIp WRITE setClientIp NOTIFY clientIpChanged)
    Q_PROPERTY(QString clientPort READ clientPort WRITE setClientPort NOTIFY clientPortChanged)

public:
    explicit QUAsyncUdpConfiguration(QObject* parent = nullptr);
    ~QUAsyncUdpConfiguration() = default;

    qint32 bindMode() const;
    QString serverIp() const;
    QString serverPort() const;
    QString clientIp() const;
    QString clientPort() const;

public slots:
    void setBindMode(const qint32 bindMode);
    void setServerIp(const QString serverIp);
    void setServerPort(const QString serverPort);
    void setClientIp(const QString clientIp);
    void setClientPort(const QString clientPort);

signals:
    void bindModeChanged(const qint32 bindMode);
    void serverIpChanged(const QString serverIp);
    void serverPortChanged(const QString serverPort);
    void clientIpChanged(const QString clientIp);
    void clientPortChanged(const QString clientPort);
};
