#include "qutcpclient.h"

#include "qutcpclientconfiguration.h"

uint32_t QUTcpClient::device_counter = 0;

QUTcpClient::QUTcpClient(QObject* parent) {
    device_counter++;
    setName("QUTcpClient " + QString::number(device_counter));
#ifdef QT_DEBUG
    qDebug() << "QUTcpClient::QUTcpClient" << m_name;
#endif
}

QUTcpClient::~QUTcpClient() {
#ifdef QT_DEBUG
    qDebug() << "QUTcpClient::~QUTcpClient" << m_name;
#endif
    device_counter--;
}

QHostAddress QUTcpClient::clientAddress() const {
#ifdef QT_DEBUG
    qDebug() << "QUTcpClient::clientAddress" << m_name;
#endif
    if (m_tcpSocket) {
        return m_tcpSocket->localAddress();
    } else {
        emit error(QString("QTcpSocket nullptr"));
        return QHostAddress();
    }
}

int QUTcpClient::clientPort() const {
#ifdef QT_DEBUG
    qDebug() << "QUTcpClient::clientPort" << m_name;
#endif
    if (m_tcpSocket) {
        return m_tcpSocket->localPort();
    } else {
        emit error(QString("QTcpSocket nullptr"));
        return -1;
    }
}

QHostAddress QUTcpClient::serverAddress() const {
#ifdef QT_DEBUG
    qDebug() << "QUTcpClient::serverAddress" << m_name;
#endif
    if (m_tcpSocket) {
        return m_tcpSocket->peerAddress();
    } else {
        emit error(QString("QTcpSocket nullptr"));
        return QHostAddress();
    }
}

int QUTcpClient::serverPort() const {
#ifdef QT_DEBUG
    qDebug() << "QUTcpClient::serverPort" << m_name;
#endif
    if (m_tcpSocket) {
        return m_tcpSocket->peerPort();
    } else {
        emit error(QString("QTcpSocket nullptr"));
        return -1;
    }
}

bool QUTcpClient::acquire() {
#ifdef QT_DEBUG
    qDebug() << "QUTcpClient::acquire" << m_name;
#endif
    QSharedPointer<QUTcpClientConfiguration> cfg = qSharedPointerCast<QUTcpClientConfiguration>(m_cfg);

    m_tcpSocket = QSharedPointer<QTcpSocket>(new QTcpSocket, [](QTcpSocket* obj) { obj->deleteLater(); });

    if (!m_tcpSocket) {
        emit error(QString("Cannot create QTcpSocket!"));
        return false;
    }

    m_hostIp.setAddress(cfg->hostIp());
    m_hostPort = cfg->hostPort().toInt();

    connect(m_tcpSocket.get(), &QTcpSocket::readyRead, this, &QUTcpClient::onReceiveData);
    connect(m_tcpSocket.get(), &QTcpSocket::errorOccurred, this, &QUTcpClient::onErrorOccurred);

    m_tcpSocket->connectToHost(m_hostIp, m_hostPort);

    bool status = m_tcpSocket->waitForConnected();
    emit opened(status);

    return status;
}

qint64 QUTcpClient::write(QByteArray data) {
#ifdef QT_DEBUG
    qDebug() << "QUTcpClient::write" << m_name;
#endif
    if (m_tcpSocket) {
        return m_tcpSocket->write(data, data.size());
    } else {
        emit error(QString("QTcpSocket nullptr"));
        return -1;
    }
}

bool QUTcpClient::release()
{
#ifdef QT_DEBUG
    qDebug() << "QUTcpClient::release" << m_name;
#endif
    if (m_tcpSocket && m_tcpSocket->isOpen()) {
        m_tcpSocket->close();
        emit closed();
        return true;
    } else {
        emit error("QUTcpClient release error!");
        return false;
    }
}

void QUTcpClient::onReceiveData() {
#ifdef QT_DEBUG
    qDebug() << "QUTcpClient::onReceiveData" << m_name;
#endif
    QByteArray data;
    while (m_tcpSocket->bytesAvailable()) {
        data += m_tcpSocket->readAll();
    }

    emit outputBytes(data);
}

void QUTcpClient::onErrorOccurred(QAbstractSocket::SocketError err) {
    QString errString = m_tcpSocket->errorString();
#ifdef QT_DEBUG
    qDebug() << "QUTcpClient::onErrorOccurred" << m_name << errString;
#endif
    emit error(errString);
}
