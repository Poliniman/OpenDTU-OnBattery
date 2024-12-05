// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "SolarChargerStats.h"

class SolarChargerProvider {
public:
    enum class Type : unsigned {
        VEDIRECT = 0
    };

    // returns true if the provider is ready for use, false otherwise
    virtual bool init(bool verboseLogging) = 0;
    virtual void deinit() = 0;
    virtual void loop() = 0;
    virtual std::shared_ptr<SolarChargerStats> getStats() = 0;
};
