#pragma once

#include "qubuilder.h"

#include "qutcpserver.h"
#include "qutcpserverconfiguration.h"
#include "qutcpserverwidget.h"

class QUTcpServerBuilder : public QUBuilder {
    Q_OBJECT

public:
    void buildDevice() override;
    void buildUI() override;
};
