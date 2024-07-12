#pragma once

#include "qubuilder.h"

#include "quasyncserialport.h"
#include "quasyncserialportconfiguration.h"
#include "quasyncserialportwidget.h"

class QUDEVICE_EXPORT QUAsyncSerialPortBuilder : public QUBuilder {
    Q_OBJECT

public:
    void buildDevice() override;
    void buildUI() override;
};
