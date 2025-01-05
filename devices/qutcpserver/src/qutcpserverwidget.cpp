#include "qutcpserverwidget.h"

QUTcpServerWidget::QUTcpServerWidget(QWidget* parent) {
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(createTcpServerParametersGroupBox());

    setLayout(mainLayout);
}

QGroupBox* QUTcpServerWidget::createTcpServerParametersGroupBox() {
    QGroupBox* serverParameterGroupBox = new QGroupBox(tr("TCP Server"), this);

    serverIpLineEdit = new QLineEdit(this);
    connect(serverIpLineEdit, &QLineEdit::textChanged, this, &QUTcpServerWidget::serverIpChanged);

    serverPortLineEdit = new QLineEdit(this);
    connect(serverPortLineEdit, &QLineEdit::textChanged, this, &QUTcpServerWidget::serverPortChanged);

    QFormLayout* serverParameterLayout = new QFormLayout;
    serverParameterLayout->addRow(tr("Ip:"), serverIpLineEdit);
    serverParameterLayout->addRow(tr("Port:"), serverPortLineEdit);
    serverParameterGroupBox->setLayout(serverParameterLayout);

    return serverParameterGroupBox;
}

void QUTcpServerWidget::restoreUI() {
    auto cfg = qSharedPointerCast<QUTcpServerConfiguration>(m_cfg);

    {
        QSignalBlocker b(serverIpLineEdit);
        serverIpLineEdit->setText(cfg->serverIp());
    }

    {
        QSignalBlocker b(serverPortLineEdit);
        serverPortLineEdit->setText(cfg->serverPort());
    }
}

void QUTcpServerWidget::updateConnections() {
    connections << connect(this,
        &QUTcpServerWidget::serverIpChanged,
        qSharedPointerCast<QUTcpServerConfiguration>(m_cfg).get(),
        &QUTcpServerConfiguration::setServerIp);
    connections << connect(this,
        &QUTcpServerWidget::serverPortChanged,
        qSharedPointerCast<QUTcpServerConfiguration>(m_cfg).get(),
        &QUTcpServerConfiguration::setServerPort);
}
