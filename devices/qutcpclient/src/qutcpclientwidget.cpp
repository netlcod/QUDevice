#include "qutcpclientwidget.h"

QUTcpClientWidget::QUTcpClientWidget(QWidget* parent) {
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(createTcpSocketParametersGroupBox());

    setLayout(mainLayout);
}

QGroupBox* QUTcpClientWidget::createTcpSocketParametersGroupBox() {
    QGroupBox* tcpSocketParameterGroupBox = new QGroupBox(tr("TCP Socket"), this);

    hostIpLineEdit = new QLineEdit(this);
    connect(hostIpLineEdit, &QLineEdit::textChanged, this, &QUTcpClientWidget::hostIpChanged);

    hostPortLineEdit = new QLineEdit(this);
    connect(hostPortLineEdit, &QLineEdit::textChanged, this, &QUTcpClientWidget::hostPortChanged);

    QFormLayout* tcpSocketParameterLayout = new QFormLayout;
    tcpSocketParameterLayout->addRow(tr("Ip:"), hostIpLineEdit);
    tcpSocketParameterLayout->addRow(tr("Port:"), hostPortLineEdit);
    tcpSocketParameterGroupBox->setLayout(tcpSocketParameterLayout);

    return tcpSocketParameterGroupBox;
}

void QUTcpClientWidget::restoreUI() {
    auto cfg = qSharedPointerCast<QUTcpClientConfiguration>(m_cfg);

    {
        QSignalBlocker b(hostIpLineEdit);
        hostIpLineEdit->setText(cfg->hostIp());
    }

    {
        QSignalBlocker b(hostPortLineEdit);
        hostPortLineEdit->setText(cfg->hostPort());
    }
}

void QUTcpClientWidget::updateConnections() {
    connections << connect(this,
        &QUTcpClientWidget::hostIpChanged,
        qSharedPointerCast<QUTcpClientConfiguration>(m_cfg).get(),
        &QUTcpClientConfiguration::setHostIp);
    connections << connect(this,
        &QUTcpClientWidget::hostPortChanged,
        qSharedPointerCast<QUTcpClientConfiguration>(m_cfg).get(),
        &QUTcpClientConfiguration::setHostPort);
}
