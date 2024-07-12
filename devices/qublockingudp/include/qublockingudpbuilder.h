#pragma once

#include "qubuilder.h"

#include "qublockingudp.h"
#include "qublockingudpconfiguration.h"
#include "qublockingudpwidget.h"

class QUBlockingUdpBuilder : public QUBuilder {
    Q_OBJECT

public:
    void buildDevice() override;
    void buildUI() override;
};
