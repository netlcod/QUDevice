#pragma once

#include <QtWidgets>

#include "qutcpclientconfiguration.h"

#include "qudevice_export.h"
#include "quwidget.h"

class QUDEVICE_EXPORT QUTcpClientWidget : public QUWidget {
    Q_OBJECT

public:
    explicit QUTcpClientWidget(QWidget* parent = nullptr);
    ~QUTcpClientWidget() = default;

private:
    QGroupBox* createTcpSocketParametersGroupBox();

private slots:
    void restoreUI() override;
    void updateConnections() override;

private:
    QLineEdit* hostIpLineEdit;
    QLineEdit* hostPortLineEdit;

signals:
    void hostIpChanged(const QString hostIp);
    void hostPortChanged(const QString hostPort);
};
