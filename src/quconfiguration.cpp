#include "quconfiguration.h"

QJsonObject QUConfiguration::json() const {
#ifdef QT_DEBUG
    qDebug() << "QUConfiguration::json";
#endif
    return m_cfg;
}

void QUConfiguration::setValue(QString key, QJsonValue value) {
    m_cfg.insert(key, value);

    emit changed(key, value);
}

QJsonValue QUConfiguration::getValue(QString key) {
    return m_cfg[key];
}

void QUConfiguration::setArray(QString key, QJsonArray array) {
    m_cfg.insert(key, array);

    emit changed(key, array);
}

QJsonArray QUConfiguration::getArray(QString key) {
    return m_cfg[key].toArray();
}

void QUConfiguration::writeToFile(QString filePath) {
    QJsonDocument jsonDoc(m_cfg);
    QFile jsonFile(filePath);

    if (jsonFile.open(QFile::WriteOnly)) {
        jsonFile.write(jsonDoc.toJson());
        jsonFile.close();
    }
}

void QUConfiguration::readFrom(QString cfgpath) {
    QFile jsonFile(cfgpath);
    if (jsonFile.open(QFile::ReadOnly)) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile.readAll());
        jsonFile.close();

        m_cfg = jsonDoc.object();

        emit loaded();
    }
}

void QUConfiguration::readFrom(QJsonObject cfg) {
    m_cfg = cfg;

    emit loaded();
}
