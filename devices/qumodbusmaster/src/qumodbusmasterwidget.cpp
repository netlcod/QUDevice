#include "qumodbusmasterwidget.h"

QUModbusMasterWidget::QUModbusMasterWidget(QWidget* parent) {
    QVBoxLayout* serialPortLayout = new QVBoxLayout;
    serialPortLayout->addWidget(createAvailablePortsGroupBox());
    serialPortLayout->addWidget(createSerialPortParametersGroupBox());
    QWidget* serialPortWidget = new QWidget(this);
    serialPortWidget->setLayout(serialPortLayout);

    masterTypeWidget = new QStackedWidget;
    masterTypeWidget->addWidget(serialPortWidget);
    masterTypeWidget->addWidget(createTcpParametersGroupBox());

    connect(availablePortsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &QUModbusMasterWidget::showPortInfo);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(createModbusParametersGroupBox());
    mainLayout->addWidget(masterTypeWidget);
    connect(masterTypeComboBox, QOverload<int>::of(&QComboBox::activated), masterTypeWidget, &QStackedWidget::setCurrentIndex);

    setLayout(mainLayout);
}

QUModbusMasterWidget::~QUModbusMasterWidget() {
    qDebug() << "~QUModbusMasterWidget()";
}

void QUModbusMasterWidget::showPortInfo(int idx) {
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

void QUModbusMasterWidget::fillPortInfo() {
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

QGroupBox* QUModbusMasterWidget::createModbusParametersGroupBox() {
    QGroupBox* modbusParametersGroupBox = new QGroupBox(tr("Modbus Parameters"));

    masterTypeComboBox = new QComboBox(this);
    masterTypeComboBox->addItem(tr("Serial"), QUModbusMasterConfiguration::MasterType::Serial);
    masterTypeComboBox->addItem(tr("Tcp"), QUModbusMasterConfiguration::MasterType::Tcp);
    connect(masterTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        emit masterTypeChanged(QUModbusMasterConfiguration::MasterType(index));
    });

    responseTimeSpinBox = new QSpinBox(this);
    responseTimeSpinBox->setRange(1, 10000);
    connect(responseTimeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &QUModbusMasterWidget::responseTimeSpinBoxChanged);

    numberOfRetriesSpinBox = new QSpinBox(this);
    numberOfRetriesSpinBox->setRange(1, 10);
    connect(
        numberOfRetriesSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &QUModbusMasterWidget::numberOfRetriesSpinBoxChanged);

    QFormLayout* filterSetupLayout = new QFormLayout;
    filterSetupLayout->addRow(tr("Master Type:"), masterTypeComboBox);
    filterSetupLayout->addRow(tr("Response Time:"), responseTimeSpinBox);
    filterSetupLayout->addRow(tr("Number Of Retries:"), numberOfRetriesSpinBox);

    modbusParametersGroupBox->setLayout(filterSetupLayout);

    return modbusParametersGroupBox;
}

QGroupBox* QUModbusMasterWidget::createAvailablePortsGroupBox() {
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
    connect(updateAvailablePortsPushButton, &QPushButton::clicked, this, &QUModbusMasterWidget::fillPortInfo);

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

QGroupBox* QUModbusMasterWidget::createSerialPortParametersGroupBox() {
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
    connect(baudRateSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &QUModbusMasterWidget::baudRateChanged);

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

QGroupBox* QUModbusMasterWidget::createTcpParametersGroupBox() {
    QGroupBox* portParametersGroupBox = new QGroupBox(tr("Port Parameters"));

    ipLineEdit = new QLineEdit(this);
    connect(ipLineEdit, &QLineEdit::textChanged, this, &QUModbusMasterWidget::ipChanged);

    portLineEdit = new QLineEdit(this);
    connect(portLineEdit, &QLineEdit::textChanged, this, &QUModbusMasterWidget::portChanged);

    QFormLayout* portParametersLayout = new QFormLayout;
    portParametersLayout->addRow(tr("Ip:"), ipLineEdit);
    portParametersLayout->addRow(tr("Port:"), portLineEdit);

    portParametersGroupBox->setLayout(portParametersLayout);

    return portParametersGroupBox;
}

void QUModbusMasterWidget::restoreUI() {
    auto cfg = qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg);

    {
        QSignalBlocker b(masterTypeComboBox);
        masterTypeComboBox->setCurrentIndex(cfg->masterType());
        masterTypeWidget->setCurrentIndex(cfg->masterType());
    }
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
    {
        QSignalBlocker b(ipLineEdit);
        ipLineEdit->setText(cfg->ip());
    }
    {
        QSignalBlocker b(portLineEdit);
        portLineEdit->setText(cfg->port());
    }

    {
        QSignalBlocker b(responseTimeSpinBox);
        responseTimeSpinBox->setValue(cfg->responseTime());
    }
    {
        QSignalBlocker b(numberOfRetriesSpinBox);
        numberOfRetriesSpinBox->setValue(cfg->numberOfRetries());
    }
}

void QUModbusMasterWidget::updateConnections() {
    connections << connect(this,
        &QUModbusMasterWidget::masterTypeChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setMasterType);
    connections << connect(this,
        &QUModbusMasterWidget::portInfoChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setPortInfo);
    connections << connect(this,
        &QUModbusMasterWidget::portNameChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setPortName);
    connections << connect(this,
        &QUModbusMasterWidget::openModeChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setOpenMode);
    connections << connect(this,
        &QUModbusMasterWidget::baudRateChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setBaudRate);
    connections << connect(this,
        &QUModbusMasterWidget::dataBitsChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setDataBits);
    connections << connect(this,
        &QUModbusMasterWidget::parityChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setParity);
    connections << connect(this,
        &QUModbusMasterWidget::stopBitsChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setStopBits);
    connections << connect(this,
        &QUModbusMasterWidget::flowControlChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setFlowControl);
    connections << connect(this,
        &QUModbusMasterWidget::ipChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setIp);
    connections << connect(this,
        &QUModbusMasterWidget::portChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setPort);
    connections << connect(this,
        &QUModbusMasterWidget::responseTimeSpinBoxChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setResponseTime);
    connections << connect(this,
        &QUModbusMasterWidget::numberOfRetriesSpinBoxChanged,
        qSharedPointerCast<QUModbusMasterConfiguration>(m_cfg).get(),
        &QUModbusMasterConfiguration::setNumberOfRetries);
}
