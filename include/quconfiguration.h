#pragma once

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>
#include <QObject>

#include "qudevice_export.h"

class QUDEVICE_EXPORT QUConfiguration : public QObject {
    Q_OBJECT

    Q_PROPERTY(QJsonObject json READ json CONSTANT)

public:
    virtual ~QUConfiguration() = default;

    QJsonObject json() const;

    void setValue(QString key, QJsonValue value);
    QJsonValue getValue(QString key);

    void setArray(QString key, QJsonArray array);
    QJsonArray getArray(QString key);

public slots:
    void writeToFile(QString filePath);
    void readFrom(QString cfgpath);
    void readFrom(QJsonObject cfg);

protected:
    QJsonObject m_cfg;

signals:
    void loaded();
    void changed(QString key, QJsonValue value);
    void changed(QString key, QJsonArray array);
};
