#include "quwidget.h"

QSharedPointer<QUConfiguration> QUWidget::configuration() const {
#ifdef QT_DEBUG
    qDebug() << "QUWidget::configuration";
#endif
    return m_cfg;
}

void QUWidget::setConfiguration(QSharedPointer<QUConfiguration> cfg) {
#ifdef QT_DEBUG
    qDebug() << "QUWidget::setConfiguration";
#endif
    m_cfg = cfg;

    foreach (auto var, connections) {
        QObject::disconnect(var);
    }
    connections.clear();

    updateConnections();
    restoreUI();

    connect(m_cfg.get(), &QUConfiguration::loaded, this, &QUWidget::restoreUI);
}
