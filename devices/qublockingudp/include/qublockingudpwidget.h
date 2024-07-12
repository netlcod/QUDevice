#pragma once

#include <QtWidgets>

#include "qublockingudpconfiguration.h"

#include "qudevice_export.h"
#include "quwidget.h"

class QUDEVICE_EXPORT QUBlockingUdpWidget : public QUWidget {
    Q_OBJECT

public:
    explicit QUBlockingUdpWidget(QWidget* parent = nullptr);
    ~QUBlockingUdpWidget() = default;

private:
    QGroupBox* createBindModeWidget();
    QGroupBox* createUdpParametersGroupBox();
    QGroupBox* createAdditionalSettingsGroupBox();

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
    QSpinBox* waitTimeoutSpinBox;

signals:
    void bindModeChanged(const qint32 bindMode);
    void serverIpChanged(const QString serverIp);
    void serverPortChanged(const QString serverPort);
    void clientIpChanged(const QString clientIp);
    void clientPortChanged(const QString clientPort);
    void waitTimeoutChanged(const int waitTimeout);
};
