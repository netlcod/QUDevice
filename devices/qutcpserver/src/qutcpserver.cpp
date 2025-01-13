#include "qutcpserver.h"

#include "qutcpserverconfiguration.h"

uint32_t QUTcpServer::device_counter = 0;

QUTcpServer::QUTcpServer(QObject* parent) {
    device_counter++;
    setName("QUTcpServer " + QString::number(device_counter));
#ifdef QT_DEBUG
    qDebug() << "QUTcpServer::QUTcpServer" << m_name;
#endif
}

QUTcpServer::~QUTcpServer() {
#ifdef QT_DEBUG
    qDebug() << "QUTcpServer::~QUTcpServer" << m_name;
#endif
    device_counter--;
}

QList<QString> QUTcpServer::availableClients() const {
#ifdef QT_DEBUG
    qDebug() << "QUTcpServer::getAvailableClients" << m_name;
#endif
    return m_clients.keys();
}

bool QUTcpServer::acquire() {
#ifdef QT_DEBUG
    qDebug() << "QUTcpServer::acquire" << m_name;
#endif
    QSharedPointer<QUTcpServerConfiguration> cfg = qSharedPointerCast<QUTcpServerConfiguration>(m_cfg);

    m_tcpServer = QSharedPointer<QTcpServer>::create();

    if (!m_tcpServer) {
        emit error(QString("Cannot create QTcpServer!"));
        return false;
    }

    m_serverIp.setAddress(cfg->serverIp());
    m_serverPort = cfg->serverPort().toInt();

    connect(m_tcpServer.get(), &QTcpServer::newConnection, this, &QUTcpServer::onNewConnection);
    connect(m_tcpServer.get(), &QTcpServer::acceptError, this, &QUTcpServer::onErrorOccurred);

    bool status = m_tcpServer->listen(m_serverIp, m_serverPort);
    emit opened(status);

    return status;
}

qint64 QUTcpServer::write(QString client, QByteArray data) {
#ifdef QT_DEBUG
    qDebug() << "QUTcpServer::write" << m_name;
#endif
    if (m_clients.contains(client)) {
        return m_clients[client]->write(data);
    } else {
        emit error(QString("Client disconnected: %1").arg(client));
        return -1;
    }
}

bool QUTcpServer::release() {
#ifdef QT_DEBUG
    qDebug() << "QUTcpServer::release" << m_name;
#endif
    if (m_tcpServer && m_tcpServer->isListening()) {
        m_clients.clear();
        m_tcpServer->close();
        emit closed(true);
        return true;
    } else {
        emit error("QTcpServer release error!");
        emit closed(false);
        return false;
    }
}

void QUTcpServer::onNewConnection() {
#ifdef QT_DEBUG
    qDebug() << "QUTcpServer::onNewConnection" << m_name;
#endif
    while (m_tcpServer->hasPendingConnections()) {
        QSharedPointer<QTcpSocket> client =
            QSharedPointer<QTcpSocket>(m_tcpServer->nextPendingConnection(), [](QTcpSocket* obj) { obj->deleteLater(); });

        connect(client.get(), &QTcpSocket::disconnected, this, &QUTcpServer::onClientDisconnect);
        connect(client.get(), &QTcpSocket::readyRead, this, &QUTcpServer::onReceiveData);

        m_clients.insert(QString("%1:%2").arg(client->peerAddress().toString()).arg(client->peerPort()), client);
    }
}

void QUTcpServer::onReceiveData() {
#ifdef QT_DEBUG
    qDebug() << "QUTcpServer::onReceiveData" << m_name;
#endif
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        QByteArray data    = client->readAll();
        qintptr descriptor = client->socketDescriptor();
        client->write(data);
        emit outputBytes(descriptor, data);
    }
}

void QUTcpServer::onClientDisconnect() {
#ifdef QT_DEBUG
    qDebug() << "QUTcpServer::onClientDisconnect" << m_name;
#endif
    QSharedPointer<QTcpSocket> client = QSharedPointer<QTcpSocket>(qobject_cast<QTcpSocket*>(sender()));
    if (client) {
        m_clients.remove(QString("%1:%2").arg(client->peerAddress().toString()).arg(client->peerPort()));
    }
}

void QUTcpServer::onErrorOccurred(QAbstractSocket::SocketError err) {
    QString errString = m_tcpServer->errorString();
#ifdef QT_DEBUG
    qDebug() << "QUTcpServer::onErrorOccurred" << m_name << errString;
#endif
    emit error(errString);
}
