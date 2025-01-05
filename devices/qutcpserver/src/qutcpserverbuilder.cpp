#include "qutcpserverbuilder.h"

void QUTcpServerBuilder::buildDevice() {
    m_device        = QSharedPointer<QUTcpServer>::create();
    m_configuration = QSharedPointer<QUTcpServerConfiguration>::create();
    m_device->setConfiguration(m_configuration);
}

void QUTcpServerBuilder::buildUI() {
    m_widget = QSharedPointer<QUTcpServerWidget>::create();
    m_widget->setConfiguration(m_configuration);
}
