#pragma once

#include <QAbstractSocket>

#include "quconfiguration.h"
#include "qudevice_export.h"

class QUDEVICE_EXPORT QUTcpClientConfiguration : public QUConfiguration {
    Q_OBJECT

    Q_PROPERTY(QString hostIp READ hostIp WRITE setHostIp NOTIFY hostIpChanged)
    Q_PROPERTY(QString hostPort READ hostPort WRITE setHostPort NOTIFY hostPortChanged)

public:
    explicit QUTcpClientConfiguration(QObject* parent = nullptr);
    ~QUTcpClientConfiguration() = default;

    QString hostIp() const;
    QString hostPort() const;

public slots:
    void setHostIp(const QString hostIp);
    void setHostPort(const QString hostPort);

signals:
    void hostIpChanged(const QString hostIp);
    void hostPortChanged(const QString hostPort);
};
