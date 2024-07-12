#pragma once

#include <QObject>
#include <QSharedPointer>

#include "quconfiguration.h"
#include "qudevice_export.h"

#ifdef QT_DEBUG
#    include <QDebug>
#endif

class QUDEVICE_EXPORT QUDevice : public QObject {
    Q_OBJECT

    QUDevice(QUDevice&&)                 = delete;
    QUDevice(const QUDevice&)            = delete;
    QUDevice& operator=(QUDevice&&)      = delete;
    QUDevice& operator=(const QUDevice&) = delete;

public:
    QUDevice(QObject* parent = nullptr);
    virtual ~QUDevice() = default;

    QString uiid() const;

    QString name() const;
    void setName(QString name);

    QSharedPointer<QUConfiguration> configuration() const;
    void setConfiguration(QSharedPointer<QUConfiguration> cfg);

protected:
    QString m_uiid;
    QString m_name;
    QSharedPointer<QUConfiguration> m_cfg;
};
