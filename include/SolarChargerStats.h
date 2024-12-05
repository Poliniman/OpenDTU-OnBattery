// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <cstddef>
#include <cstdint>

class SolarChargerStats {

    public:
        class SolarChargerControllerStats {
            public:
                virtual int32_t getDataAgeMillis() const = 0;
                virtual int32_t getOutputPowerWatts() const = 0;
                virtual int32_t getPanelPowerWatts() const = 0;
                virtual float getYieldTotal() const = 0;
                virtual float getYieldDay() const = 0;
                virtual float getOutputVoltage() const = 0;
        };

        virtual uint32_t getDataAgeMillis() const = 0;

        // minimum of all MPPT charge controllers' output voltages in V
        virtual float getOutputVoltage() const = 0;
        virtual bool isOuputVoltageValid() const = 0;

        // total output of all MPPT charge controllers in Watts
        virtual int32_t getOutputPowerWatts() const = 0;
        virtual bool isOutputPowerWattsValid() const = 0;

        // total panel input power of all MPPT charge controllers in Watts
        virtual int32_t getPanelPowerWatts() const = 0;
        virtual bool isPanelPowerWattsValid() const = 0;

        // sum of total yield of all MPPT charge controllers in kWh
        virtual float getYieldTotal() const = 0;

        // sum of today's yield of all MPPT charge controllers in kWh
        virtual float getYieldDay() const = 0;

        virtual size_t controllerAmount() const = 0;

        virtual std::vector<std::unique_ptr<SolarChargerControllerStats>> getControllerStats(size_t idx = 0) const = 0;
};

class SolarChargerStatsDummy : public SolarChargerStats {
    public:
        uint32_t getDataAgeMillis() const final { return INT32_MAX; }
        float getOutputVoltage() const final { return 0; }
        bool isOuputVoltageValid() const final { return false; }
        int32_t getOutputPowerWatts() const final { return 0; }
        bool isOutputPowerWattsValid() const final { return false; }
        int32_t getPanelPowerWatts() const final { return 0; }
        bool isPanelPowerWattsValid() const final { return false; }
        float getYieldTotal() const final { return 0; }
        float getYieldDay() const final { return 0; }
        size_t controllerAmount() const final { return 0; };
};

class VictronMpptStats : public SolarChargerStats {
    friend class VictronMppt;

    class VictronMpptControllerStats : public SolarChargerControllerStats {
        friend class VictronMppt;

         public:
            int32_t getDataAgeMillis() const final { return _dataAgeMillis; };
            int32_t getOutputPowerWatts() const final { return _outputPowerWatts; };
            int32_t getPanelPowerWatts() const final { return _panelPowerWatts; };
            float getYieldTotal() const final { return _yieldTotal; };
            float getYieldDay() const final { return _yieldDay; };
            float getOutputVoltage() const final { return _outputVoltage; };

        protected:
            uint32_t _dataAgeMillis;
            float _outputVoltage;
            int32_t _outputPowerWatts;
            int32_t _panelPowerWatts;
            float _yieldTotal;
            float _yieldDay;
    };

    public:
        uint32_t getDataAgeMillis() const final { return _dataAgeMillis; };
        float getOutputVoltage() const final { return _outputVoltage; };
        bool isOuputVoltageValid() const final { return _isOuputVoltageValid; };
        int32_t getOutputPowerWatts() const final { return _outputPowerWatts; };
        bool isOutputPowerWattsValid() const final { return _isOutputPowerWattsValid; };
        int32_t getPanelPowerWatts() const final { return _panelPowerWatts; };
        bool isPanelPowerWattsValid() const final { return _isPanelPowerWattsValid; };
        float getYieldTotal() const final { return _yieldTotal; };
        float getYieldDay() const final { return _yieldDay; };

        size_t controllerAmount() const final { return _controllerStats.size(); };

    protected:
        uint32_t _dataAgeMillis;
        float _outputVoltage;
        bool _isOuputVoltageValid;
        int32_t _outputPowerWatts;
        bool _isOutputPowerWattsValid;
        int32_t _panelPowerWatts;
        bool _isPanelPowerWattsValid;
        float _yieldTotal;
        float _yieldDay;

        std::vector<std::unique_ptr<VictronMpptControllerStats>> _controllerStats;
};
