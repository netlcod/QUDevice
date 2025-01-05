#pragma once

#include "qubuilder.h"

#include "qutcpclient.h"
#include "qutcpclientconfiguration.h"
#include "qutcpclientwidget.h"

class QUTcpClientBuilder : public QUBuilder {
    Q_OBJECT

public:
    void buildDevice() override;
    void buildUI() override;
};
