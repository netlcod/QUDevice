#pragma once

#include <QNetworkDatagram>
#include <QTcpSocket>

#include "qudevice.h"

class QUDEVICE_EXPORT QUTcpClient : public QUDevice {
    Q_OBJECT

    static uint32_t device_counter;

public:
    explicit QUTcpClient(QObject* parent = nullptr);
    ~QUTcpClient();

public:
    QHostAddress clientAddress() const;
    int clientPort() const;
    QHostAddress serverAddress() const;
    int serverPort() const;

public slots:
    bool acquire();
    qint64 write(QByteArray data);
    void release();

private slots:
    void onReceiveData();
    void onErrorOccurred(QAbstractSocket::SocketError error);

signals:
    void opened(bool status);
    void closed();
    void error(const QString error);
    void outputBytes(const QByteArray data);

private:
    QSharedPointer<QTcpSocket> m_tcpSocket;
    QHostAddress m_hostIp;
    int m_hostPort;
};
