#include "qublockingserialport.h"
#include "qublockingserialportconfiguration.h"

uint32_t QUBlockingSerialPort::device_counter = 0;

QUBlockingSerialPort::QUBlockingSerialPort(QObject* parent) {
    device_counter++;
    setName("QUBlockingSerialPort " + QString::number(device_counter));
#ifdef QT_DEBUG
    qDebug() << "QUBlockingSerialPort::QUBlockingSerialPort" << m_name;
#endif
}

QUBlockingSerialPort::~QUBlockingSerialPort() {
#ifdef QT_DEBUG
    qDebug() << "QUBlockingSerialPort::~QUBlockingSerialPort" << m_name;
#endif
    device_counter--;
}

bool QUBlockingSerialPort::acquire() {
#ifdef QT_DEBUG
    qDebug() << "QUBlockingSerialPort::acquire" << m_name;
#endif
    QSharedPointer<QUBlockingSerialPortConfiguration> cfg = qSharedPointerCast<QUBlockingSerialPortConfiguration>(m_cfg);

    m_port = QSharedPointer<QSerialPort>::create();

    if (!m_port) {
        emit opened(false);
        emit error(QString("Cannot create QSerialPort!"));
        return false;
    }

    m_waitTimeout = cfg->waitTimeout();
    m_port->setPort(cfg->portInfo());
    m_port->setPortName(cfg->portName());
    m_port->setParity(cfg->parity());
    m_port->setBaudRate(cfg->baudRate(), cfg->openMode());
    m_port->setDataBits(cfg->dataBits());
    m_port->setStopBits(cfg->stopBits());
    m_port->setFlowControl(cfg->flowControl());

    QIODevice::OpenMode mode;
    switch (cfg->openMode()) {
        case QSerialPort::Direction::Input:
            mode = QIODevice::ReadOnly;
            break;
        case QSerialPort::Direction::Output:
            mode = QIODevice::WriteOnly;
            break;
        case QSerialPort::Direction::AllDirections:
            mode = QIODevice::ReadWrite;
            break;
    }

    connect(m_port.get(), &QSerialPort::errorOccurred, this, &QUBlockingSerialPort::onErrorOccurred);

    bool status = m_port->open(mode);
    emit opened(status);

    return status;
}

qint64 QUBlockingSerialPort::write(QByteArray data) {
#ifdef QT_DEBUG
    qDebug() << "QUBlockingSerialPort::write" << m_name;
#endif
    if (m_port) {
        qint64 size = m_port->write(data);
        m_port->waitForBytesWritten(m_waitTimeout);
        return size;
    } else {
        emit error(QString("QSerialPort nullptr"));
        return -1;
    }
}

QByteArray QUBlockingSerialPort::read() {
#ifdef QT_DEBUG
    qDebug() << "QUBlockingSerialPort::read" << m_name;
#endif
    if (m_port) {
        QByteArray data;
        if (m_port->waitForReadyRead(m_waitTimeout)) {
            data += m_port->readAll();
        }
        return data;
    } else {
        emit error(QString("QSerialPort nullptr"));
        return QByteArray();
    }
}

bool QUBlockingSerialPort::release() {
#ifdef QT_DEBUG
    qDebug() << "QUBlockingSerialPort::release" << m_name;
#endif
    if (m_port && m_port->isOpen()) {
        m_port->close();
        emit closed(true);
        return true;
    } else {
        emit error("QSerialPort release error!");
        emit closed(false);
        return false;
    }
}

void QUBlockingSerialPort::onErrorOccurred(QSerialPort::SerialPortError err) {
    QString errString = m_port->errorString();
#ifdef QT_DEBUG
    qDebug() << "QUBlockingSerialPort::onErrorOccurred" << m_name << errString;
#endif
    emit error(errString);
}
