#include "qudevice.h"

QUDevice::QUDevice(QObject* parent) : QObject(parent) {
#ifdef QT_DEBUG
    qDebug() << "QUDevice::QUDevice" << m_name;
#endif
    m_uiid = QUuid::createUuid().toString();
}

QString QUDevice::uiid() const {
#ifdef QT_DEBUG
    qDebug() << "QUDevice::uiid" << m_name;
#endif
    return m_uiid;
}

QString QUDevice::name() const {
#ifdef QT_DEBUG
    qDebug() << "QUDevice::name" << m_name;
#endif
    return m_name;
}

void QUDevice::setName(QString name) {
#ifdef QT_DEBUG
    qDebug() << "QUDevice::setName" << m_name;
#endif
    m_name = name;
}

QSharedPointer<QUConfiguration> QUDevice::configuration() const {
#ifdef QT_DEBUG
    qDebug() << "QUDevice::configuration" << m_name;
#endif
    return m_cfg;
}

void QUDevice::setConfiguration(QSharedPointer<QUConfiguration> cfg) {
#ifdef QT_DEBUG
    qDebug() << "QUDevice::setConfiguration" << m_name;
#endif
    m_cfg = cfg;
}
