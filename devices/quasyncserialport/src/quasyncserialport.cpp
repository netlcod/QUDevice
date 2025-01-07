#include "quasyncserialport.h"
#include "quasyncserialportconfiguration.h"

uint32_t QUAsyncSerialPort::device_counter = 0;

QUAsyncSerialPort::QUAsyncSerialPort(QObject* parent) {
    device_counter++;
    setName("QUAsyncSerialPort " + QString::number(device_counter));
#ifdef QT_DEBUG
    qDebug() << "QUAsyncSerialPort::QUAsyncSerialPort" << m_name;
#endif
}

QUAsyncSerialPort::~QUAsyncSerialPort() {
#ifdef QT_DEBUG
    qDebug() << "QUAsyncSerialPort::~QUAsyncSerialPort" << m_name;
#endif
    device_counter--;
}

bool QUAsyncSerialPort::acquire() {
#ifdef QT_DEBUG
    qDebug() << "QUAsyncSerialPort::acquire" << m_name;
#endif
    QSharedPointer<QUAsyncSerialPortConfiguration> cfg = qSharedPointerCast<QUAsyncSerialPortConfiguration>(m_cfg);

    m_port = QSharedPointer<QSerialPort>::create();

    if (!m_port) {
        emit opened(false);
        emit error(QString("Cannot create QSerialPort!"));
        return false;
    }

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

    connect(m_port.get(), &QSerialPort::readyRead, this, &QUAsyncSerialPort::onReceiveData);
    connect(m_port.get(), &QSerialPort::errorOccurred, this, &QUAsyncSerialPort::onErrorOccurred);

    bool status = m_port->open(mode);
    emit opened(status);

    return status;
}

qint64 QUAsyncSerialPort::write(QByteArray data) {
#ifdef QT_DEBUG
    qDebug() << "QUAsyncSerialPort::write" << m_name;
#endif
    if (m_port) {
        return m_port->write(data);
    } else {
        emit error(QString("QSerialPort nullptr"));
        return -1;
    }
}

bool QUAsyncSerialPort::release()
{
#ifdef QT_DEBUG
    qDebug() << "QUAsyncSerialPort::release" << m_name;
#endif
    if (m_port && m_port->isOpen()) {
        m_port->close();
        emit closed();
        return true;
    } else {
        emit error("QSerialPort release error!");
        return false;
    }
}

void QUAsyncSerialPort::onReceiveData() {
#ifdef QT_DEBUG
    qDebug() << "QUAsyncSerialPort::onReceiveData" << m_name;
#endif
    QByteArray data = m_port->readAll();
    emit outputBytes(data);
}

void QUAsyncSerialPort::onErrorOccurred(QSerialPort::SerialPortError err) {
    QString errString = m_port->errorString();
#ifdef QT_DEBUG
    qDebug() << "QUAsyncSerialPort::onErrorOccurred" << m_name << errString;
#endif
    emit error(errString);
}
