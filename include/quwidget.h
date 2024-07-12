#pragma once

#include <QSharedPointer>
#include <QWidget>

#include "quconfiguration.h"
#include "qudevice_export.h"

class QUDEVICE_EXPORT QUWidget : public QWidget {
    Q_OBJECT

public:
    virtual ~QUWidget() = default;

    QSharedPointer<QUConfiguration> configuration() const;
    void setConfiguration(QSharedPointer<QUConfiguration> cfg);

protected slots:
    virtual void restoreUI()         = 0;
    virtual void updateConnections() = 0;

protected:
    QSharedPointer<QUConfiguration> m_cfg;
    QList<QMetaObject::Connection> connections;
};
