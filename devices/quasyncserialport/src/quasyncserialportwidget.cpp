#include "quasyncserialportwidget.h"

QUAsyncSerialPortWidget::QUAsyncSerialPortWidget(QWidget* parent) {
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(createAvailablePortsGroupBox(), 0, 0);
    mainLayout->addWidget(createSerialPortParametersGroupBox(), 1, 0);

    connect(availablePortsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &QUAsyncSerialPortWidget::showPortInfo);

    setLayout(mainLayout);
}

void QUAsyncSerialPortWidget::showPortInfo(int idx) {
    portDescriptionPlainTextEdit->clear();

    const QStringList list = availablePortsComboBox->itemData(idx).toStringList();
    portDescriptionPlainTextEdit->appendPlainText(QString(tr("Description: ")) + QString("%1").arg(list.count() > 1 ? list.at(1) : "N/A"));
    portDescriptionPlainTextEdit->appendPlainText(QString(tr("Manufacturer: ")) + QString("%1").arg(list.count() > 2 ? list.at(2) : "N/A"));
    portDescriptionPlainTextEdit->appendPlainText(
        QString(tr("QUAsyncSerialPort Number: ")) + QString("%1").arg(list.count() > 3 ? list.at(3) : "N/A"));
    portDescriptionPlainTextEdit->appendPlainText(QString(tr("Location: ")) + QString("%1").arg(list.count() > 4 ? list.at(4) : "N/A"));
    portDescriptionPlainTextEdit->appendPlainText(QString(tr("VID: ")) + QString("%1").arg(list.count() > 5 ? list.at(5) : "N/A"));
    portDescriptionPlainTextEdit->appendPlainText(QString(tr("PID: ")) + QString("%1").arg(list.count() > 6 ? list.at(6) : "N/A"));
}

void QUAsyncSerialPortWidget::fillPortInfo() {
    availablePortsComboBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : infos) {
        QStringList list;
        description  = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName() << (!description.isEmpty() ? description : "N/A") << (!manufacturer.isEmpty() ? manufacturer : "N/A")
             << (!serialNumber.isEmpty() ? serialNumber : "N/A") << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : "N/A")
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : "N/A");

        availablePortsComboBox->addItem(list.first(), list);
    }
}

QGroupBox* QUAsyncSerialPortWidget::createAvailablePortsGroupBox() {
    QGroupBox* availablePortsGroupBox = new QGroupBox(tr("Available Ports"), this);

    availablePortsComboBox = new QComboBox(this);
    availablePortsComboBox->setCurrentIndex(-1);
    connect(availablePortsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index > -1) {
            emit portInfoChanged(QSerialPortInfo::availablePorts().at(index));
            emit portNameChanged(QSerialPortInfo::availablePorts().at(index).portName());
        }
    });

    updateAvailablePortsPushButton = new QPushButton(tr("Update"), this);
    connect(updateAvailablePortsPushButton, &QPushButton::clicked, this, &QUAsyncSerialPortWidget::fillPortInfo);

    portDescriptionPlainTextEdit = new QPlainTextEdit(this);
    portDescriptionPlainTextEdit->setReadOnly(true);
    portDescriptionPlainTextEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QGridLayout* availablePortsLayout = new QGridLayout;
    availablePortsLayout->addWidget(new QLabel("Port:"), 0, 0);
    availablePortsLayout->addWidget(availablePortsComboBox, 0, 1);
    availablePortsLayout->addWidget(updateAvailablePortsPushButton, 0, 2);
    availablePortsLayout->addWidget(portDescriptionPlainTextEdit, 1, 0, 1, 3);

    availablePortsGroupBox->setLayout(availablePortsLayout);

    return availablePortsGroupBox;
}

QGroupBox* QUAsyncSerialPortWidget::createSerialPortParametersGroupBox() {
    QGroupBox* serialPortGroupBox = new QGroupBox(tr("Port Info"), this);

    directionComboBox = new QComboBox(this);
    directionComboBox->addItem(tr("Input"), QSerialPort::Input);
    directionComboBox->addItem(tr("Output"), QSerialPort::Output);
    directionComboBox->addItem(tr("AllDirections"), QSerialPort::AllDirections);
    directionComboBox->setCurrentIndex(-1);
    connect(directionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index > -1) {
            emit openModeChanged(static_cast<QSerialPort::Direction>(directionComboBox->currentData().toInt()));
        }
    });

    baudRateSpinBox = new QSpinBox(this);
    baudRateSpinBox->setRange(0, 4000000);
    baudRateSpinBox->setValue(0);
    connect(baudRateSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &QUAsyncSerialPortWidget::baudRateChanged);

    dataBitsComboBox = new QComboBox(this);
    dataBitsComboBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    dataBitsComboBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    dataBitsComboBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    dataBitsComboBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    dataBitsComboBox->setCurrentIndex(-1);
    connect(dataBitsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index > -1) {
            emit dataBitsChanged(static_cast<QSerialPort::DataBits>(dataBitsComboBox->currentData().toInt()));
        }
    });

    parityComboBox = new QComboBox(this);
    parityComboBox->addItem(tr("None"), QSerialPort::NoParity);
    parityComboBox->addItem(tr("Even"), QSerialPort::EvenParity);
    parityComboBox->addItem(tr("Odd"), QSerialPort::OddParity);
    parityComboBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    parityComboBox->addItem(tr("Space"), QSerialPort::SpaceParity);
    parityComboBox->setCurrentIndex(-1);
    connect(parityComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index > -1) {
            emit parityChanged(static_cast<QSerialPort::Parity>(parityComboBox->currentData().toInt()));
        }
    });

    stopBitsComboBox = new QComboBox(this);
    stopBitsComboBox->addItem(tr("OneStop"), QSerialPort::OneStop);
    stopBitsComboBox->addItem(tr("OneAndHalfStop"), QSerialPort::OneAndHalfStop);
    stopBitsComboBox->addItem(tr("TwoStop"), QSerialPort::TwoStop);
    stopBitsComboBox->setCurrentIndex(-1);
    connect(stopBitsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index > -1) {
            emit stopBitsChanged(static_cast<QSerialPort::StopBits>(stopBitsComboBox->currentData().toInt()));
        }
    });

    flowControlComboBox = new QComboBox(this);
    flowControlComboBox->addItem(tr("NoFlowControl"), QSerialPort::NoFlowControl);
    flowControlComboBox->addItem(tr("HardwareControl"), QSerialPort::HardwareControl);
    flowControlComboBox->addItem(tr("SoftwareControl"), QSerialPort::SoftwareControl);
    flowControlComboBox->setCurrentIndex(-1);
    connect(flowControlComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index > -1) {
            emit flowControlChanged(static_cast<QSerialPort::FlowControl>(flowControlComboBox->currentData().toInt()));
        }
    });

    QFormLayout* serialPortParametersLayout = new QFormLayout();
    serialPortParametersLayout->addRow(tr("Direction:"), directionComboBox);
    serialPortParametersLayout->addRow(tr("Baud rate:"), baudRateSpinBox);
    serialPortParametersLayout->addRow(tr("Data bits:"), dataBitsComboBox);
    serialPortParametersLayout->addRow(tr("Parity:"), parityComboBox);
    serialPortParametersLayout->addRow(tr("Stop bits:"), stopBitsComboBox);
    serialPortParametersLayout->addRow(tr("Flow control:"), flowControlComboBox);

    serialPortGroupBox->setLayout(serialPortParametersLayout);

    return serialPortGroupBox;
}

void QUAsyncSerialPortWidget::restoreUI() {
    auto cfg = qSharedPointerCast<QUAsyncSerialPortConfiguration>(m_cfg);

    {
        QSignalBlocker b(directionComboBox);
        directionComboBox->setCurrentIndex(directionComboBox->findData(cfg->openMode()));
    }
    {
        QSignalBlocker b(baudRateSpinBox);
        baudRateSpinBox->setValue(cfg->baudRate());
    }
    {
        QSignalBlocker b(dataBitsComboBox);
        dataBitsComboBox->setCurrentIndex(dataBitsComboBox->findData(cfg->dataBits()));
    }
    {
        QSignalBlocker b(parityComboBox);
        parityComboBox->setCurrentIndex(parityComboBox->findData(cfg->parity()));
    }
    {
        QSignalBlocker b(stopBitsComboBox);
        stopBitsComboBox->setCurrentIndex(stopBitsComboBox->findData(cfg->stopBits()));
    }
    {
        QSignalBlocker b(flowControlComboBox);
        flowControlComboBox->setCurrentIndex(flowControlComboBox->findData(cfg->flowControl()));
    }
}

void QUAsyncSerialPortWidget::updateConnections() {
    connections << connect(this,
        &QUAsyncSerialPortWidget::portInfoChanged,
        qSharedPointerCast<QUAsyncSerialPortConfiguration>(m_cfg).get(),
        &QUAsyncSerialPortConfiguration::setPortInfo);
    connections << connect(this,
        &QUAsyncSerialPortWidget::portNameChanged,
        qSharedPointerCast<QUAsyncSerialPortConfiguration>(m_cfg).get(),
        &QUAsyncSerialPortConfiguration::setPortName);
    connections << connect(this,
        &QUAsyncSerialPortWidget::openModeChanged,
        qSharedPointerCast<QUAsyncSerialPortConfiguration>(m_cfg).get(),
        &QUAsyncSerialPortConfiguration::setOpenMode);
    connections << connect(this,
        &QUAsyncSerialPortWidget::baudRateChanged,
        qSharedPointerCast<QUAsyncSerialPortConfiguration>(m_cfg).get(),
        &QUAsyncSerialPortConfiguration::setBaudRate);
    connections << connect(this,
        &QUAsyncSerialPortWidget::dataBitsChanged,
        qSharedPointerCast<QUAsyncSerialPortConfiguration>(m_cfg).get(),
        &QUAsyncSerialPortConfiguration::setDataBits);
    connections << connect(this,
        &QUAsyncSerialPortWidget::parityChanged,
        qSharedPointerCast<QUAsyncSerialPortConfiguration>(m_cfg).get(),
        &QUAsyncSerialPortConfiguration::setParity);
    connections << connect(this,
        &QUAsyncSerialPortWidget::stopBitsChanged,
        qSharedPointerCast<QUAsyncSerialPortConfiguration>(m_cfg).get(),
        &QUAsyncSerialPortConfiguration::setStopBits);
    connections << connect(this,
        &QUAsyncSerialPortWidget::flowControlChanged,
        qSharedPointerCast<QUAsyncSerialPortConfiguration>(m_cfg).get(),
        &QUAsyncSerialPortConfiguration::setFlowControl);
}
