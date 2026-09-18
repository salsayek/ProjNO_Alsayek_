#pragma once

#include <array>
#include <string>
#include <cstddef>



class OptimizationProblem
{
public:


    static constexpr std::size_t NUM_HOURS = 24;
    static constexpr std::size_t NUM_APPLIANCES = 4;

    enum class ApplianceID : std::size_t
    {
        WashingMachine = 0,
        Dishwasher = 1,
        WaterHeater = 2,
        StorageHeater = 3
    };


    struct Appliance
    {
        std::string name;

        double energyDemand = 0.0;

        double duration = 0.0;
        int earliestStart = 0;

        int latestFinish = 24;

        bool enabled = true;


        double getPower() const
        {
            if (duration <= 0.0)
                return 0.0;

            return energyDemand / duration;
        }


        int getAvailableHours() const
        {
            return latestFinish - earliestStart;
        }
    };


protected:


    double _maxHouseholdPower = 7.0;

    std::array<Appliance, NUM_APPLIANCES> _appliances;



    std::array<double, NUM_HOURS> _tariff{};


public:

    OptimizationProblem()
    {
        setDefaultScenario();
    }

    void setDefaultScenario()
    {

        _maxHouseholdPower = 7.0;

        _appliances[static_cast<std::size_t>(
            ApplianceID::WashingMachine)] =
        {
            "Washing Machine",

            2.0,

            2.0,
            8,
            22,

            true
        };


        _appliances[static_cast<std::size_t>(
            ApplianceID::Dishwasher)] =
        {
            "Dishwasher",

            1.5,
            1.5, 

            17,        
            24,         

            true
        };

        _appliances[static_cast<std::size_t>(
            ApplianceID::WaterHeater)] =
        {
            "Water Heater",

            4.0,        
            2.0,        

            5,         
            9, 

            true
        };


        _appliances[static_cast<std::size_t>(
            ApplianceID::StorageHeater)] =
        {
            "Storage Heater",

            9.0,        // kWh
            3.0,        // h

            0,          // earliest start
            8,          // latest finish

            true
        };


        constexpr double LOW_TARIFF = 0.0994;
        constexpr double HIGH_TARIFF = 0.1988;


        for (std::size_t hour = 0; hour < NUM_HOURS; ++hour)
        {
            bool isLowTariff = false;

            if (hour < 8)
            {
                isLowTariff = true;
            }
            if (hour >= 14 && hour < 17)
            {
                isLowTariff = true;
            }
            if (hour >= 23)
            {
                isLowTariff = true;
            }


            _tariff[hour] =
                isLowTariff
                ? LOW_TARIFF
                : HIGH_TARIFF;
        }
    }


    double getMaxHouseholdPower() const
    {
        return _maxHouseholdPower;
    }


    void setMaxHouseholdPower(double power)
    {
        _maxHouseholdPower = power;
    }


    Appliance& getAppliance(ApplianceID id)
    {
        return _appliances[
            static_cast<std::size_t>(id)
        ];
    }


    const Appliance& getAppliance(ApplianceID id) const
    {
        return _appliances[
            static_cast<std::size_t>(id)
        ];
    }


    Appliance& getAppliance(std::size_t index)
    {
        return _appliances[index];
    }


    const Appliance& getAppliance(std::size_t index) const
    {
        return _appliances[index];
    }


    std::array<Appliance, NUM_APPLIANCES>& getAppliances()
    {
        return _appliances;
    }


    const std::array<Appliance, NUM_APPLIANCES>& getAppliances() const
    {
        return _appliances;
    }


    double getTariff(std::size_t hour) const
    {
        return _tariff[hour];
    }


    void setTariff(std::size_t hour, double price)
    {
        if (hour < NUM_HOURS)
        {
            _tariff[hour] = price;
        }
    }


    std::array<double, NUM_HOURS>& getTariffProfile()
    {
        return _tariff;
    }


    const std::array<double, NUM_HOURS>& getTariffProfile() const
    {
        return _tariff;
    }


    double getTotalEnergyDemand() const
    {
        double totalEnergy = 0.0;

        for (const auto& appliance : _appliances)
        {
            if (appliance.enabled)
            {
                totalEnergy += appliance.energyDemand;
            }
        }

        return totalEnergy;
    }


    bool validate(std::string& errorMessage) const
    {
        errorMessage.clear();

        if (_maxHouseholdPower <= 0.0)
        {
            errorMessage =
                "Maximum household power must be greater than zero.";

            return false;
        }



        for (std::size_t hour = 0; hour < NUM_HOURS; ++hour)
        {
            if (_tariff[hour] < 0.0)
            {
                errorMessage =
                    "Electricity tariff values cannot be negative.";

                return false;
            }
        }


        for (const auto& appliance : _appliances)
        {
            if (!appliance.enabled)
                continue;


            if (appliance.energyDemand <= 0.0)
            {
                errorMessage =
                    appliance.name +
                    ": energy demand must be greater than zero.";

                return false;
            }


            if (appliance.duration <= 0.0)
            {
                errorMessage =
                    appliance.name +
                    ": operating duration must be greater than zero.";

                return false;
            }


            if (appliance.earliestStart < 0 ||
                appliance.earliestStart >= 24)
            {
                errorMessage =
                    appliance.name +
                    ": earliest start must be between 0 and 23.";

                return false;
            }


            if (appliance.latestFinish <= 0 ||
                appliance.latestFinish > 24)
            {
                errorMessage =
                    appliance.name +
                    ": latest finish must be between 1 and 24.";

                return false;
            }


            if (appliance.earliestStart >= appliance.latestFinish)
            {
                errorMessage =
                    appliance.name +
                    ": earliest start must be before latest finish.";

                return false;
            }

            if (appliance.duration >
                static_cast<double>(appliance.getAvailableHours()))
            {
                errorMessage =
                    appliance.name +
                    ": operating duration exceeds its available time window.";

                return false;
            }


            if (appliance.getPower() > _maxHouseholdPower)
            {
                errorMessage =
                    appliance.name +
                    ": required power exceeds household connection capacity.";

                return false;
            }
        }


        return true;
    }


    bool isValid() const
    {
        std::string errorMessage;

        return validate(errorMessage);
    }
};