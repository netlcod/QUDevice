#include "quasyncserialportbuilder.h"

void QUAsyncSerialPortBuilder::buildDevice() {
    m_device        = QSharedPointer<QUAsyncSerialPort>::create();
    m_configuration = QSharedPointer<QUAsyncSerialPortConfiguration>::create();
    m_device->setConfiguration(m_configuration);
}

void QUAsyncSerialPortBuilder::buildUI() {
    m_widget = QSharedPointer<QUAsyncSerialPortWidget>::create();
    m_widget->setConfiguration(m_configuration);
}
