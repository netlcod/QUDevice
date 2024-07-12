#include "qublockingserialportbuilder.h"

void QUBlockingSerialPortBuilder::buildDevice() {
    m_device        = QSharedPointer<QUBlockingSerialPort>::create();
    m_configuration = QSharedPointer<QUBlockingSerialPortConfiguration>::create();
    m_device->setConfiguration(m_configuration);
}

void QUBlockingSerialPortBuilder::buildUI() {
    m_widget = QSharedPointer<QUBlockingSerialPortWidget>::create();
    m_widget->setConfiguration(m_configuration);
}
