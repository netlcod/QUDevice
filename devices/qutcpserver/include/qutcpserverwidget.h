#pragma once

#include <QtWidgets>

#include "qutcpserverconfiguration.h"

#include "qudevice_export.h"
#include "quwidget.h"

class QUDEVICE_EXPORT QUTcpServerWidget : public QUWidget {
    Q_OBJECT

public:
    explicit QUTcpServerWidget(QWidget* parent = nullptr);
    ~QUTcpServerWidget() = default;

private:
    QGroupBox* createTcpServerParametersGroupBox();

private slots:
    void restoreUI() override;
    void updateConnections() override;

private:
    QLineEdit* serverIpLineEdit;
    QLineEdit* serverPortLineEdit;

signals:
    void serverIpChanged(const QString serverIp);
    void serverPortChanged(const QString serverPort);
};
