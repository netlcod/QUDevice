#include "qublockingudpbuilder.h"

void QUBlockingUdpBuilder::buildDevice() {
    m_device        = QSharedPointer<QUBlockingUdp>::create();
    m_configuration = QSharedPointer<QUBlockingUdpConfiguration>::create();

    m_device->setConfiguration(m_configuration);
}

void QUBlockingUdpBuilder::buildUI() {
    m_widget = QSharedPointer<QUBlockingUdpWidget>::create();
    m_widget->setConfiguration(m_configuration);
}
