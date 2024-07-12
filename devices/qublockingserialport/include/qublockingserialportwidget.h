#pragma once

#include <QtSerialPort/QSerialPort>
#include <QtWidgets>

#include "qublockingserialportconfiguration.h"

#include "qudevice_export.h"
#include "quwidget.h"

class QUDEVICE_EXPORT QUBlockingSerialPortWidget : public QUWidget {
    Q_OBJECT

public:
    explicit QUBlockingSerialPortWidget(QWidget* parent = nullptr);
    ~QUBlockingSerialPortWidget() = default;

private slots:
    void showPortInfo(int idx);
    void fillPortInfo();

private:
    QGroupBox* createAvailablePortsGroupBox();
    QGroupBox* createSerialPortParametersGroupBox();
    QGroupBox* createAdditionalSettingsGroupBox();

private slots:
    void restoreUI() override;
    void updateConnections() override;

private:
    QComboBox* availablePortsComboBox;
    QPushButton* updateAvailablePortsPushButton;
    QPlainTextEdit* portDescriptionPlainTextEdit;
    QComboBox* directionComboBox;
    QSpinBox* baudRateSpinBox;
    QComboBox* dataBitsComboBox;
    QComboBox* parityComboBox;
    QComboBox* stopBitsComboBox;
    QComboBox* flowControlComboBox;
    QSpinBox* waitTimeoutSpinBox;

signals:
    void portInfoChanged(const QSerialPortInfo portInfo);
    void portNameChanged(const QString portName);
    void openModeChanged(const QSerialPort::Direction openMode);
    void baudRateChanged(const qint32 baudRate);
    void dataBitsChanged(const QSerialPort::DataBits dataBits);
    void parityChanged(const QSerialPort::Parity parity);
    void stopBitsChanged(const QSerialPort::StopBits stopBits);
    void flowControlChanged(const QSerialPort::FlowControl flowControl);
    void waitTimeoutChanged(const int waitTimeout);
};
