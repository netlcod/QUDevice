#include "quasyncudpbuilder.h"

void QUAsyncUdpBuilder::buildDevice() {
    m_device        = QSharedPointer<QUAsyncUdp>::create();
    m_configuration = QSharedPointer<QUAsyncUdpConfiguration>::create();
    m_device->setConfiguration(m_configuration);
}

void QUAsyncUdpBuilder::buildUI() {
    m_widget = QSharedPointer<QUAsyncUdpWidget>::create();
    m_widget->setConfiguration(m_configuration);
}
