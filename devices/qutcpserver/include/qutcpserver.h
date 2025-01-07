#pragma once

#include <QTcpServer>
#include <QTcpSocket>

#include "qudevice.h"

class QUDEVICE_EXPORT QUTcpServer : public QUDevice {
    Q_OBJECT

    static uint32_t device_counter;

public:
    explicit QUTcpServer(QObject* parent = nullptr);
    ~QUTcpServer();

    QList<QString> availableClients() const;

public slots:
    bool acquire();
    qint64 write(QString client, QByteArray data);
    bool release();

private slots:
    void onNewConnection();
    void onReceiveData();
    void onClientDisconnect();
    void onErrorOccurred(QAbstractSocket::SocketError error);

signals:
    void opened(bool status);
    void closed();
    void error(const QString error) const;
    void outputBytes(qintptr descriptor, const QByteArray data);

private:
    QSharedPointer<QTcpServer> m_tcpServer;
    QHostAddress m_serverIp;
    int m_serverPort;
    QMap<QString, QSharedPointer<QTcpSocket>> m_clients;
};
