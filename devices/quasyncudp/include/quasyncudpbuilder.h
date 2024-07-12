#pragma once

#include "qubuilder.h"

#include "quasyncudp.h"
#include "quasyncudpconfiguration.h"
#include "quasyncudpwidget.h"

class QUAsyncUdpBuilder : public QUBuilder {
    Q_OBJECT

public:
    void buildDevice() override;
    void buildUI() override;
};
