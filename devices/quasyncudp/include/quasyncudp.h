#pragma once

#include <QNetworkDatagram>
#include <QUdpSocket>

#include "qudevice.h"

class QUDEVICE_EXPORT QUAsyncUdp : public QUDevice {
    Q_OBJECT

    static uint32_t device_counter;

public:
    explicit QUAsyncUdp(QObject* parent = nullptr);
    ~QUAsyncUdp();

public slots:
    bool acquire();
    qint64 write(QByteArray data);
    bool release();

private slots:
    void onReceiveData();
    void onErrorOccurred(QAbstractSocket::SocketError error);

signals:
    void opened(bool status);
    void closed();
    void error(const QString error) const;
    void outputBytes(const QByteArray data);

private:
    QSharedPointer<QUdpSocket> m_udpSocket;
    QAbstractSocket::BindMode bindMode;
    QHostAddress m_serverIp;
    int m_serverPort;
    QHostAddress m_clientIp;
    int m_clientPort;
};
