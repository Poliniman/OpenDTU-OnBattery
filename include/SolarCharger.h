// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <memory>
#include <mutex>
#include <TaskSchedulerDeclarations.h>

#include "SolarChargerProvider.h"
#include "SolarChargerStats.h"

class SolarChargerClass {
public:
    void init(Scheduler&);
    void updateSettings();

    std::shared_ptr<SolarChargerStats const> getStats() const;

    SolarChargerProvider* getProvider() const { return _upProvider.get(); }

private:
    void loop();

    Task _loopTask;
    mutable std::mutex _mutex;
    std::unique_ptr<SolarChargerProvider> _upProvider = nullptr;
};

extern SolarChargerClass SolarCharger;
