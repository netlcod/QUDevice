#pragma once

#include <QModbusDataUnit>
#include <QtWidgets>

#include "qumodbusmasterconfiguration.h"

#include "qudevice_export.h"
#include "quwidget.h"

class QUDEVICE_EXPORT QUModbusMasterWidget : public QUWidget {
    Q_OBJECT

public:
    explicit QUModbusMasterWidget(QWidget* parent = nullptr);
    ~QUModbusMasterWidget();

private slots:
    void showPortInfo(int idx);
    void fillPortInfo();

private:
    QGroupBox* createModbusParametersGroupBox();
    QGroupBox* createAvailablePortsGroupBox();
    QGroupBox* createSerialPortParametersGroupBox();
    QGroupBox* createTcpParametersGroupBox();

private slots:
    void restoreUI() override;
    void updateConnections() override;

private:
    QStackedWidget* masterTypeWidget;
    QComboBox* masterTypeComboBox;
    QComboBox* availablePortsComboBox;
    QPushButton* updateAvailablePortsPushButton;
    QPlainTextEdit* portDescriptionPlainTextEdit;
    QComboBox* directionComboBox;
    QSpinBox* baudRateSpinBox;
    QComboBox* dataBitsComboBox;
    QComboBox* parityComboBox;
    QComboBox* stopBitsComboBox;
    QComboBox* flowControlComboBox;
    QLineEdit* ipLineEdit;
    QLineEdit* portLineEdit;
    QSpinBox* responseTimeSpinBox;
    QSpinBox* numberOfRetriesSpinBox;

signals:
    void masterTypeChanged(const QUModbusMasterConfiguration::MasterType masterType);
    void portInfoChanged(const QSerialPortInfo portInfo);
    void portNameChanged(const QString portName);
    void openModeChanged(const QSerialPort::Direction openMode);
    void baudRateChanged(const qint32 baudRate);
    void dataBitsChanged(const QSerialPort::DataBits dataBits);
    void parityChanged(const QSerialPort::Parity parity);
    void stopBitsChanged(const QSerialPort::StopBits stopBits);
    void flowControlChanged(const QSerialPort::FlowControl flowControl);
    void ipChanged(const QString ip);
    void portChanged(const QString port);
    void responseTimeSpinBoxChanged(const int responseTimeSpinBox);
    void numberOfRetriesSpinBoxChanged(const int numberOfRetriesSpinBox);
};
