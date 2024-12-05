// SPDX-License-Identifier: GPL-2.0-or-later
#include "SolarCharger.h"
#include <Configuration.h>
#include <MessageOutput.h>
#include <VictronMppt.h>

SolarChargerClass SolarCharger;

void SolarChargerClass::init(Scheduler& scheduler)
{
    scheduler.addTask(_loopTask);
    _loopTask.setCallback(std::bind(&SolarChargerClass::loop, this));
    _loopTask.setIterations(TASK_FOREVER);
    _loopTask.enable();

    this->updateSettings();
}

void SolarChargerClass::updateSettings()
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_upProvider) {
        _upProvider->deinit();
        _upProvider = nullptr;
    }

    auto const& config = Configuration.get();
    if (!config.SolarCharger.Enabled) { return; }

    bool verboseLogging = config.SolarCharger.VerboseLogging;

    switch (static_cast<SolarChargerProvider::Type>(config.SolarCharger.Provider)) {
        case SolarChargerProvider::Type::VEDIRECT:
            _upProvider = std::make_unique<VictronMppt>();
            break;
        default:
            MessageOutput.printf("[SolarCharger] Unknown provider: %d\r\n", config.SolarCharger.Provider);
            return;
    }

    if (!_upProvider->init(verboseLogging)) { _upProvider = nullptr; }
}

std::shared_ptr<SolarChargerStats const> SolarChargerClass::getStats() const
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_upProvider) {
        return _upProvider->getStats();
    }

    static auto sspDummyStats = std::make_shared<SolarChargerStatsDummy>();
    return sspDummyStats;
}

void SolarChargerClass::loop()
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_upProvider) {
        _upProvider->loop();
    }
}
