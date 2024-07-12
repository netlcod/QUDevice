#include "qublockingudpwidget.h"

QUBlockingUdpWidget::QUBlockingUdpWidget(QWidget* parent) {
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(createBindModeWidget(), 0, 0);
    mainLayout->addWidget(createAdditionalSettingsGroupBox(), 1, 0);
    mainLayout->addWidget(createUdpParametersGroupBox(), 0, 1, 2, 1);

    setLayout(mainLayout);
}

QGroupBox* QUBlockingUdpWidget::createBindModeWidget() {
    QGroupBox* bindModeGroupBox = new QGroupBox(tr("Bind Mode"), this);

    shareAddressCheckBox = new QCheckBox(tr("Share Address"), this);
    connect(shareAddressCheckBox, &QCheckBox::stateChanged, this, &QUBlockingUdpWidget::updateBindMode);

    dontShareAddressCheckBox = new QCheckBox(tr("Don't Share Address"), this);
    connect(dontShareAddressCheckBox, &QCheckBox::stateChanged, this, &QUBlockingUdpWidget::updateBindMode);

    reuseAddressHintCheckBox = new QCheckBox(tr("Reuse Address Hint"), this);
    connect(reuseAddressHintCheckBox, &QCheckBox::stateChanged, this, &QUBlockingUdpWidget::updateBindMode);

    defaultForPlatformCheckBox = new QCheckBox(tr("Default For Platform"), this);
    connect(defaultForPlatformCheckBox, &QCheckBox::stateChanged, this, &QUBlockingUdpWidget::updateBindMode);

    QVBoxLayout* bindModeLayout = new QVBoxLayout;
    bindModeLayout->addWidget(shareAddressCheckBox);
    bindModeLayout->addWidget(dontShareAddressCheckBox);
    bindModeLayout->addWidget(reuseAddressHintCheckBox);
    bindModeLayout->addWidget(defaultForPlatformCheckBox);
    bindModeGroupBox->setLayout(bindModeLayout);

    return bindModeGroupBox;
}

QGroupBox* QUBlockingUdpWidget::createUdpParametersGroupBox() {
    QGroupBox* serverParameterGroupBox = new QGroupBox(tr("Server"), this);

    serverIpLineEdit = new QLineEdit(this);
    connect(serverIpLineEdit, &QLineEdit::textChanged, this, &QUBlockingUdpWidget::serverIpChanged);

    serverPortLineEdit = new QLineEdit(this);
    connect(serverPortLineEdit, &QLineEdit::textChanged, this, &QUBlockingUdpWidget::serverPortChanged);

    QFormLayout* serverParameterLayout = new QFormLayout;
    serverParameterLayout->addRow(tr("Ip:"), serverIpLineEdit);
    serverParameterLayout->addRow(tr("Port:"), serverPortLineEdit);
    serverParameterGroupBox->setLayout(serverParameterLayout);

    QGroupBox* clientParametersGroupBox = new QGroupBox(tr("Client"), this);

    clientIpLineEdit = new QLineEdit(this);
    connect(clientIpLineEdit, &QLineEdit::textChanged, this, &QUBlockingUdpWidget::clientIpChanged);

    clientPortLineEdit = new QLineEdit(this);
    connect(clientPortLineEdit, &QLineEdit::textChanged, this, &QUBlockingUdpWidget::clientPortChanged);

    QFormLayout* clientParametersLayout = new QFormLayout;
    clientParametersLayout->addRow(tr("Ip:"), clientIpLineEdit);
    clientParametersLayout->addRow(tr("Port:"), clientPortLineEdit);
    clientParametersGroupBox->setLayout(clientParametersLayout);

    QGroupBox* udpSocketGroupBox = new QGroupBox(tr("UDP Socket"), this);
    QVBoxLayout* udpSocketLayout = new QVBoxLayout;
    udpSocketLayout->addWidget(serverParameterGroupBox);
    udpSocketLayout->addWidget(clientParametersGroupBox);
    udpSocketGroupBox->setLayout(udpSocketLayout);

    return udpSocketGroupBox;
}

QGroupBox* QUBlockingUdpWidget::createAdditionalSettingsGroupBox() {
    QGroupBox* additionalSettingsGroupBox = new QGroupBox(tr("Additional"), this);

    waitTimeoutSpinBox = new QSpinBox(this);
    waitTimeoutSpinBox->setRange(1, 10000);
    waitTimeoutSpinBox->setValue(1000);
    connect(waitTimeoutSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &QUBlockingUdpWidget::waitTimeoutChanged);

    QFormLayout* additionalSettingsLayout = new QFormLayout();
    additionalSettingsLayout->addRow(tr("Wait Timeout:"), waitTimeoutSpinBox);

    additionalSettingsGroupBox->setLayout(additionalSettingsLayout);

    return additionalSettingsGroupBox;
}

void QUBlockingUdpWidget::updateBindMode() {
    qint32 bindMode = 0;
    if (shareAddressCheckBox->isChecked()) {
        bindMode |= 0x01;
    } else {
        bindMode &= ~0x01;
    }
    if (dontShareAddressCheckBox->isChecked()) {
        bindMode |= 0x02;
    } else {
        bindMode &= ~0x02;
    }
    if (reuseAddressHintCheckBox->isChecked()) {
        bindMode |= 0x04;
    } else {
        bindMode &= ~0x04;
    }
    if (defaultForPlatformCheckBox->isChecked()) {
        bindMode |= 0x00;
    } else {
        bindMode &= ~0x00;
    }

    emit bindModeChanged(bindMode);
}

void QUBlockingUdpWidget::restoreUI() {
    auto cfg = qSharedPointerCast<QUBlockingUdpConfiguration>(m_cfg);

    {
        QSignalBlocker b1(shareAddressCheckBox);
        QSignalBlocker b2(dontShareAddressCheckBox);
        QSignalBlocker b3(reuseAddressHintCheckBox);
        QSignalBlocker b4(defaultForPlatformCheckBox);
        qint32 mode = cfg->bindMode();

        switch (mode) {
            case 0x00: {
                shareAddressCheckBox->setChecked(false);
                dontShareAddressCheckBox->setChecked(false);
                reuseAddressHintCheckBox->setChecked(false);
                defaultForPlatformCheckBox->setChecked(true);
                break;
            }
            case 0x01: {
                shareAddressCheckBox->setChecked(true);
                dontShareAddressCheckBox->setChecked(false);
                reuseAddressHintCheckBox->setChecked(false);
                defaultForPlatformCheckBox->setChecked(false);
                break;
            }
            case 0x02: {
                shareAddressCheckBox->setChecked(false);
                dontShareAddressCheckBox->setChecked(true);
                reuseAddressHintCheckBox->setChecked(false);
                defaultForPlatformCheckBox->setChecked(false);
                break;
            }
            case 0x04: {
                shareAddressCheckBox->setChecked(false);
                dontShareAddressCheckBox->setChecked(false);
                reuseAddressHintCheckBox->setChecked(true);
                defaultForPlatformCheckBox->setChecked(false);
                break;
            }
            case 0x05: {
                shareAddressCheckBox->setChecked(true);
                dontShareAddressCheckBox->setChecked(false);
                reuseAddressHintCheckBox->setChecked(true);
                defaultForPlatformCheckBox->setChecked(false);
                break;
            }
            case 0x06: {
                shareAddressCheckBox->setChecked(false);
                dontShareAddressCheckBox->setChecked(true);
                reuseAddressHintCheckBox->setChecked(true);
                defaultForPlatformCheckBox->setChecked(false);
                break;
            }
            default: {
                shareAddressCheckBox->setChecked(false);
                dontShareAddressCheckBox->setChecked(false);
                reuseAddressHintCheckBox->setChecked(false);
                defaultForPlatformCheckBox->setChecked(true);
                break;
            }
        }
    }

    {
        QSignalBlocker b(serverIpLineEdit);
        serverIpLineEdit->setText(cfg->serverIp());
    }

    {
        QSignalBlocker b(serverPortLineEdit);
        serverPortLineEdit->setText(cfg->serverPort());
    }

    {
        QSignalBlocker b(clientIpLineEdit);
        clientIpLineEdit->setText(cfg->clientIp());
    }

    {
        QSignalBlocker b(clientPortLineEdit);
        clientPortLineEdit->setText(cfg->clientPort());
    }
}
void QUBlockingUdpWidget::updateConnections() {
    connections << connect(this,
        &QUBlockingUdpWidget::bindModeChanged,
        qSharedPointerCast<QUBlockingUdpConfiguration>(m_cfg).get(),
        &QUBlockingUdpConfiguration::setBindMode);
    connections << connect(this,
        &QUBlockingUdpWidget::serverIpChanged,
        qSharedPointerCast<QUBlockingUdpConfiguration>(m_cfg).get(),
        &QUBlockingUdpConfiguration::setServerIp);
    connections << connect(this,
        &QUBlockingUdpWidget::serverPortChanged,
        qSharedPointerCast<QUBlockingUdpConfiguration>(m_cfg).get(),
        &QUBlockingUdpConfiguration::setServerPort);
    connections << connect(this,
        &QUBlockingUdpWidget::clientIpChanged,
        qSharedPointerCast<QUBlockingUdpConfiguration>(m_cfg).get(),
        &QUBlockingUdpConfiguration::setClientIp);
    connections << connect(this,
        &QUBlockingUdpWidget::clientPortChanged,
        qSharedPointerCast<QUBlockingUdpConfiguration>(m_cfg).get(),
        &QUBlockingUdpConfiguration::setClientPort);
}
