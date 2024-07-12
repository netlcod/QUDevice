#pragma once

#include <QtWidgets>

#include "quasyncudpconfiguration.h"

#include "qudevice_export.h"
#include "quwidget.h"

class QUDEVICE_EXPORT QUAsyncUdpWidget : public QUWidget {
    Q_OBJECT

public:
    explicit QUAsyncUdpWidget(QWidget* parent = nullptr);
    ~QUAsyncUdpWidget() = default;

private:
    QGroupBox* createBindModeGroupBox();
    QGroupBox* createUdpParametersGroupBox();

private slots:
    void updateBindMode();

    void restoreUI() override;
    void updateConnections() override;

private:
    QCheckBox* shareAddressCheckBox;
    QCheckBox* dontShareAddressCheckBox;
    QCheckBox* reuseAddressHintCheckBox;
    QCheckBox* defaultForPlatformCheckBox;
    QLineEdit* serverIpLineEdit;
    QLineEdit* serverPortLineEdit;
    QLineEdit* clientIpLineEdit;
    QLineEdit* clientPortLineEdit;

signals:
    void bindModeChanged(const qint32 bindMode);
    void serverIpChanged(const QString serverIp);
    void serverPortChanged(const QString serverPort);
    void clientIpChanged(const QString clientIp);
    void clientPortChanged(const QString clientPort);
};
