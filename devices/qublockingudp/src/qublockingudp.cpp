#include "qublockingudp.h"

#include "qublockingudpconfiguration.h"

uint32_t QUBlockingUdp::device_counter = 0;

QUBlockingUdp::QUBlockingUdp(QObject* parent) {
    device_counter++;
    setName("QUBlockingUdp " + QString::number(device_counter));
#ifdef QT_DEBUG
    qDebug() << "QUBlockingUdp::QUBlockingUdp" << m_name;
#endif
}

QUBlockingUdp::~QUBlockingUdp() {
#ifdef QT_DEBUG
    qDebug() << "QUBlockingUdp::~QUBlockingUdp" << m_name;
#endif
    device_counter--;
}

bool QUBlockingUdp::acquire() {
#ifdef QT_DEBUG
    qDebug() << "QUBlockingUdp::acquire" << m_name;
#endif
    QSharedPointer<QUBlockingUdpConfiguration> cfg = qSharedPointerCast<QUBlockingUdpConfiguration>(m_cfg);

    m_udpSocket = QSharedPointer<QUdpSocket>::create();

    if (!m_udpSocket) {
        emit opened(false);
        emit error(QString("Cannot create QUdpSocket!"));
        return false;
    }

    m_waitTimeout = cfg->waitTimeout();
    bindMode      = QAbstractSocket::BindMode(cfg->bindMode());
    m_serverIp.setAddress(cfg->serverIp());
    m_serverPort = cfg->serverPort().toInt();
    m_clientIp.setAddress(cfg->clientIp());
    m_clientPort = cfg->clientPort().toInt();

    connect(m_udpSocket.get(), &QUdpSocket::errorOccurred, this, &QUBlockingUdp::onErrorOccurred);

    bool status = false;
    if (m_udpSocket->state() != m_udpSocket->BoundState) {
        status = m_udpSocket->bind(m_serverIp, m_serverPort, bindMode);
    }
    emit opened(status);

    return status;
}

qint64 QUBlockingUdp::write(QByteArray data) {
#ifdef QT_DEBUG
    qDebug() << "QUBlockingSerialPort::write" << m_name;
#endif
    if (m_udpSocket) {
        qint64 size = m_udpSocket->writeDatagram(data, data.size(), m_clientIp, m_clientPort);
        m_udpSocket->waitForBytesWritten(m_waitTimeout);
        return size;
    } else {
        emit error(QString("QUdpSocket nullptr"));
        return -1;
    }
}

QByteArray QUBlockingUdp::read() {
#ifdef QT_DEBUG
    qDebug() << "QUBlockingUdp::read" << m_name;
#endif
    if (m_udpSocket) {
        QByteArray data;
        if (m_udpSocket->waitForReadyRead(m_waitTimeout)) {
            while (m_udpSocket->hasPendingDatagrams()) {
                QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
                if (datagram.isValid()) {
                    data += datagram.data();
                }
            }
        }
        return data;
    } else {
        emit error(QString("QUdpSocket nullptr"));
        return QByteArray();
    }
}

bool QUBlockingUdp::release() {
#ifdef QT_DEBUG
    qDebug() << "QUBlockingUdp::release" << m_name;
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

void QUBlockingUdp::onErrorOccurred(QAbstractSocket::SocketError err) {
    QString errString = m_udpSocket->errorString();
#ifdef QT_DEBUG
    qDebug() << "QUBlockingUdp::onErrorOccurred" << m_name << err;
#endif
    emit error(errString);
}
