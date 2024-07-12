#include "quasyncudpwidget.h"

QUAsyncUdpWidget::QUAsyncUdpWidget(QWidget* parent) {
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(createBindModeGroupBox());
    mainLayout->addWidget(createUdpParametersGroupBox());

    setLayout(mainLayout);
}

QGroupBox* QUAsyncUdpWidget::createBindModeGroupBox() {
    QGroupBox* bindModeGroupBox = new QGroupBox(tr("Bind Mode"), this);

    shareAddressCheckBox = new QCheckBox(tr("Share Address"), this);
    connect(shareAddressCheckBox, &QCheckBox::stateChanged, this, &QUAsyncUdpWidget::updateBindMode);

    dontShareAddressCheckBox = new QCheckBox(tr("Don't Share Address"), this);
    connect(dontShareAddressCheckBox, &QCheckBox::stateChanged, this, &QUAsyncUdpWidget::updateBindMode);

    reuseAddressHintCheckBox = new QCheckBox(tr("Reuse Address Hint"), this);
    connect(reuseAddressHintCheckBox, &QCheckBox::stateChanged, this, &QUAsyncUdpWidget::updateBindMode);

    defaultForPlatformCheckBox = new QCheckBox(tr("Default For Platform"), this);
    connect(defaultForPlatformCheckBox, &QCheckBox::stateChanged, this, &QUAsyncUdpWidget::updateBindMode);

    QVBoxLayout* bindModeLayout = new QVBoxLayout;
    bindModeLayout->addWidget(shareAddressCheckBox);
    bindModeLayout->addWidget(dontShareAddressCheckBox);
    bindModeLayout->addWidget(reuseAddressHintCheckBox);
    bindModeLayout->addWidget(defaultForPlatformCheckBox);
    bindModeGroupBox->setLayout(bindModeLayout);

    return bindModeGroupBox;
}

QGroupBox* QUAsyncUdpWidget::createUdpParametersGroupBox() {
    QGroupBox* serverParameterGroupBox = new QGroupBox(tr("Server"), this);

    serverIpLineEdit = new QLineEdit(this);
    connect(serverIpLineEdit, &QLineEdit::textChanged, this, &QUAsyncUdpWidget::serverIpChanged);

    serverPortLineEdit = new QLineEdit(this);
    connect(serverPortLineEdit, &QLineEdit::textChanged, this, &QUAsyncUdpWidget::serverPortChanged);

    QFormLayout* serverParameterLayout = new QFormLayout;
    serverParameterLayout->addRow(tr("Ip:"), serverIpLineEdit);
    serverParameterLayout->addRow(tr("Port:"), serverPortLineEdit);
    serverParameterGroupBox->setLayout(serverParameterLayout);

    QGroupBox* clientParametersGroupBox = new QGroupBox(tr("Client"), this);

    clientIpLineEdit = new QLineEdit(this);
    connect(clientIpLineEdit, &QLineEdit::textChanged, this, &QUAsyncUdpWidget::clientIpChanged);

    clientPortLineEdit = new QLineEdit(this);
    connect(clientPortLineEdit, &QLineEdit::textChanged, this, &QUAsyncUdpWidget::clientPortChanged);

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

void QUAsyncUdpWidget::updateBindMode() {
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

void QUAsyncUdpWidget::restoreUI() {
    auto cfg = qSharedPointerCast<QUAsyncUdpConfiguration>(m_cfg);

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
void QUAsyncUdpWidget::updateConnections() {
    connections << connect(this,
        &QUAsyncUdpWidget::bindModeChanged,
        qSharedPointerCast<QUAsyncUdpConfiguration>(m_cfg).get(),
        &QUAsyncUdpConfiguration::setBindMode);
    connections << connect(this,
        &QUAsyncUdpWidget::serverIpChanged,
        qSharedPointerCast<QUAsyncUdpConfiguration>(m_cfg).get(),
        &QUAsyncUdpConfiguration::setServerIp);
    connections << connect(this,
        &QUAsyncUdpWidget::serverPortChanged,
        qSharedPointerCast<QUAsyncUdpConfiguration>(m_cfg).get(),
        &QUAsyncUdpConfiguration::setServerPort);
    connections << connect(this,
        &QUAsyncUdpWidget::clientIpChanged,
        qSharedPointerCast<QUAsyncUdpConfiguration>(m_cfg).get(),
        &QUAsyncUdpConfiguration::setClientIp);
    connections << connect(this,
        &QUAsyncUdpWidget::clientPortChanged,
        qSharedPointerCast<QUAsyncUdpConfiguration>(m_cfg).get(),
        &QUAsyncUdpConfiguration::setClientPort);
}
