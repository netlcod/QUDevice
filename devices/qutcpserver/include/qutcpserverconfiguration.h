#pragma once

#include <QAbstractSocket>

#include "quconfiguration.h"
#include "qudevice_export.h"

class QUDEVICE_EXPORT QUTcpServerConfiguration : public QUConfiguration {
    Q_OBJECT

    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(QString serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)

public:
    explicit QUTcpServerConfiguration(QObject* parent = nullptr);
    ~QUTcpServerConfiguration() = default;

    QString serverIp() const;
    QString serverPort() const;

public slots:
    void setServerIp(const QString serverIp);
    void setServerPort(const QString serverPort);

signals:
    void serverIpChanged(const QString serverIp);
    void serverPortChanged(const QString serverPort);
};
