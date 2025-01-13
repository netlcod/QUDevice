#include "quasyncudp.h"

#include "quasyncudpconfiguration.h"

uint32_t QUAsyncUdp::device_counter = 0;

QUAsyncUdp::QUAsyncUdp(QObject* parent) {
    device_counter++;
    setName("QUAsyncUdp " + QString::number(device_counter));
#ifdef QT_DEBUG
    qDebug() << "QUAsyncUdp::QUAsyncUdp" << m_name;
#endif
}

QUAsyncUdp::~QUAsyncUdp() {
#ifdef QT_DEBUG
    qDebug() << "QUAsyncUdp::~QUAsyncUdp" << m_name;
#endif
    device_counter--;
}

bool QUAsyncUdp::acquire() {
#ifdef QT_DEBUG
    qDebug() << "QUAsyncUdp::acquire" << m_name;
#endif
    QSharedPointer<QUAsyncUdpConfiguration> cfg = qSharedPointerCast<QUAsyncUdpConfiguration>(m_cfg);

    m_udpSocket = QSharedPointer<QUdpSocket>::create();

    if (!m_udpSocket) {
        emit opened(false);
        emit error(QString("Cannot create QUdpSocket!"));
        return false;
    }

    bindMode = QAbstractSocket::BindMode(cfg->bindMode());
    m_serverIp.setAddress(cfg->serverIp());
    m_serverPort = cfg->serverPort().toInt();
    m_clientIp.setAddress(cfg->clientIp());
    m_clientPort = cfg->clientPort().toInt();

    connect(m_udpSocket.get(), &QUdpSocket::readyRead, this, &QUAsyncUdp::onReceiveData);
    connect(m_udpSocket.get(), &QUdpSocket::errorOccurred, this, &QUAsyncUdp::onErrorOccurred);

    bool status = false;
    if (m_udpSocket->state() != m_udpSocket->BoundState) {
        status = m_udpSocket->bind(m_serverIp, m_serverPort, bindMode);
    }
    emit opened(status);

    return status;
}

qint64 QUAsyncUdp::write(QByteArray data) {
#ifdef QT_DEBUG
    qDebug() << "QUAsyncUdp::write" << m_name;
#endif
    if (m_udpSocket) {
        return m_udpSocket->writeDatagram(data, data.size(), m_clientIp, m_clientPort);
    } else {
        emit error(QString("QUdpSocket nullptr"));
        return -1;
    }
}

bool QUAsyncUdp::release() {
#ifdef QT_DEBUG
    qDebug() << "QUAsyncUdp::release" << m_name;
#endif
    if (m_udpSocket && m_udpSocket->isOpen()) {
        m_udpSocket->close();
        emit closed(true);
        return true;
    } else {
        emit error("QUdpSocket release error!");
        emit closed(false);
        return false;
    }
}

void QUAsyncUdp::onReceiveData() {
#ifdef QT_DEBUG
    qDebug() << "QUAsyncUdp::onReceiveData" << m_name;
#endif
    QByteArray data;
    while (m_udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
        if (datagram.isValid()) {
            data += datagram.data();
        }
    }

    emit outputBytes(data);
}

void QUAsyncUdp::onErrorOccurred(QAbstractSocket::SocketError err) {
    QString errString = m_udpSocket->errorString();
#ifdef QT_DEBUG
    qDebug() << "QUAsyncUdp::onErrorOccurred" << m_name << errString;
#endif
    emit error(errString);
}
