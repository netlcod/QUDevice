#pragma once

#include <QNetworkDatagram>
#include <QUdpSocket>

#include "qudevice.h"

class QUDEVICE_EXPORT QUBlockingUdp : public QUDevice {
    Q_OBJECT

    static uint32_t device_counter;

public:
    explicit QUBlockingUdp(QObject* parent = nullptr);
    ~QUBlockingUdp();

public slots:
    bool acquire();
    qint64 write(QByteArray data);
    QByteArray read();
    bool release();

private slots:
    void onErrorOccurred(QAbstractSocket::SocketError error);

signals:
    void opened(bool status);
    void closed();
    void error(const QString error) const;

private:
    QSharedPointer<QUdpSocket> m_udpSocket;
    int m_waitTimeout;
    QAbstractSocket::BindMode bindMode;
    QHostAddress m_serverIp;
    int m_serverPort;
    QHostAddress m_clientIp;
    int m_clientPort;
};
