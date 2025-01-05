#include "qutcpclientbuilder.h"

void QUTcpClientBuilder::buildDevice() {
    m_device        = QSharedPointer<QUTcpClient>::create();
    m_configuration = QSharedPointer<QUTcpClientConfiguration>::create();
    m_device->setConfiguration(m_configuration);
}

void QUTcpClientBuilder::buildUI() {
    m_widget = QSharedPointer<QUTcpClientWidget>::create();
    m_widget->setConfiguration(m_configuration);
}
