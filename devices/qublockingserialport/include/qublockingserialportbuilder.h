#pragma once

#include "qubuilder.h"

#include "qublockingserialport.h"
#include "qublockingserialportconfiguration.h"
#include "qublockingserialportwidget.h"

class QUBlockingSerialPortBuilder : public QUBuilder {
    Q_OBJECT

public:
    void buildDevice() override;
    void buildUI() override;
};
