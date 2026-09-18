#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <cmath>

#include "OptimizationProblem.h"


class OptimizationResult
{
public:

    static constexpr std::size_t NUM_HOURS =
        OptimizationProblem::NUM_HOURS;

    static constexpr std::size_t NUM_APPLIANCES =
        OptimizationProblem::NUM_APPLIANCES;


    enum class Status
    {
        NotSolved,
        Optimal,
        Infeasible,
        Unbounded,
        InvalidInput,
        SolverError
    };


protected:

    std::array<
        std::array<double, NUM_HOURS>,
        NUM_APPLIANCES
    > _schedule{};


    std::array<double, NUM_HOURS> _totalPower{};


    std::array<double, NUM_HOURS> _capacityShadowPrice{};


    std::array<double, NUM_APPLIANCES> _applianceShadowPrice{};

    std::array<bool, NUM_HOURS> _capacityBinding{};

    double _optimalCost = 0.0;
    double _baselineCost = 0.0;
    double _absoluteSavings = 0.0;

    double _percentageSavings = 0.0;

    double _peakPower = 0.0;

    double _totalEnergy = 0.0;


    Status _status = Status::NotSolved;

    std::string _message;


public:

    OptimizationResult()
    {
        clear();
    }
    void clear()
    {
        for (auto& applianceSchedule : _schedule)
        {
            applianceSchedule.fill(0.0);
        }

        _totalPower.fill(0.0);

        _capacityShadowPrice.fill(0.0);

        _applianceShadowPrice.fill(0.0);

        _capacityBinding.fill(false);


        _optimalCost = 0.0;

        _baselineCost = 0.0;

        _absoluteSavings = 0.0;

        _percentageSavings = 0.0;

        _peakPower = 0.0;

        _totalEnergy = 0.0;


        _status = Status::NotSolved;

        _message.clear();
    }


    void setStatus(Status status)
    {
        _status = status;
    }


    Status getStatus() const
    {
        return _status;
    }


    bool isOptimal() const
    {
        return _status == Status::Optimal;
    }


    bool isSolved() const
    {
        return
            _status == Status::Optimal ||
            _status == Status::Infeasible ||
            _status == Status::Unbounded ||
            _status == Status::SolverError;
    }


    void setInvalidInput()
    {
        _status = Status::InvalidInput;

        _message =
            "Invalid optimization problem parameters.";
    }


    void setInfeasible()
    {
        _status = Status::Infeasible;

        _message =
            "The optimization problem is infeasible.";
    }


    void setUnbounded()
    {
        _status = Status::Unbounded;

        _message =
            "The optimization problem is unbounded.";
    }


    void setSolverError()
    {
        _status = Status::SolverError;

        _message =
            "The optimization solver could not obtain a solution.";
    }


    void setOptimal()
    {
        _status = Status::Optimal;

        _message =
            "Optimal solution found.";
    }



    void setMessage(const std::string& message)
    {
        _message = message;
    }


    const std::string& getMessage() const
    {
        return _message;
    }


    const char* getStatusText() const
    {
        switch (_status)
        {
        case Status::NotSolved:
            return "Not solved";

        case Status::Optimal:
            return "Optimal";

        case Status::Infeasible:
            return "Infeasible";

        case Status::Unbounded:
            return "Unbounded";

        case Status::InvalidInput:
            return "Invalid input";

        case Status::SolverError:
            return "Solver error";

        default:
            return "Unknown";
        }
    }

    void setScheduleValue(
        std::size_t appliance,
        std::size_t hour,
        double value)
    {
        if (appliance >= NUM_APPLIANCES ||
            hour >= NUM_HOURS)
        {
            return;
        }

        if (std::abs(value) < 1e-9)
        {
            value = 0.0;
        }

        if (std::abs(value - 1.0) < 1e-9)
        {
            value = 1.0;
        }

        _schedule[appliance][hour] = value;
    }


    double getScheduleValue(
        std::size_t appliance,
        std::size_t hour) const
    {
        if (appliance >= NUM_APPLIANCES ||
            hour >= NUM_HOURS)
        {
            return 0.0;
        }

        return _schedule[appliance][hour];
    }


    const std::array<
        std::array<double, NUM_HOURS>,
        NUM_APPLIANCES
    >& getSchedule() const
    {
        return _schedule;
    }


    void setTotalPower(
        std::size_t hour,
        double power)
    {
        if (hour < NUM_HOURS)
        {
            _totalPower[hour] = power;
        }
    }


    double getTotalPower(
        std::size_t hour) const
    {
        if (hour >= NUM_HOURS)
        {
            return 0.0;
        }

        return _totalPower[hour];
    }


    const std::array<double, NUM_HOURS>&
        getTotalPowerProfile() const
    {
        return _totalPower;
    }


    void setCapacityShadowPrice(
        std::size_t hour,
        double value)
    {
        if (hour < NUM_HOURS)
        {
            if (std::abs(value) < 1e-9)
            {
                value = 0.0;
            }

            _capacityShadowPrice[hour] = value;
        }
    }


    double getCapacityShadowPrice(
        std::size_t hour) const
    {
        if (hour >= NUM_HOURS)
        {
            return 0.0;
        }

        return _capacityShadowPrice[hour];
    }


    const std::array<double, NUM_HOURS>&
        getCapacityShadowPrices() const
    {
        return _capacityShadowPrice;
    }

    void setApplianceShadowPrice(
        std::size_t appliance,
        double value)
    {
        if (appliance < NUM_APPLIANCES)
        {
            if (std::abs(value) < 1e-9)
            {
                value = 0.0;
            }

            _applianceShadowPrice[appliance] = value;
        }
    }


    double getApplianceShadowPrice(
        std::size_t appliance) const
    {
        if (appliance >= NUM_APPLIANCES)
        {
            return 0.0;
        }

        return _applianceShadowPrice[appliance];
    }


    const std::array<double, NUM_APPLIANCES>&
        getApplianceShadowPrices() const
    {
        return _applianceShadowPrice;
    }


    void setCapacityBinding(
        std::size_t hour,
        bool binding)
    {
        if (hour < NUM_HOURS)
        {
            _capacityBinding[hour] = binding;
        }
    }


    bool isCapacityBinding(
        std::size_t hour) const
    {
        if (hour >= NUM_HOURS)
        {
            return false;
        }

        return _capacityBinding[hour];
    }


    const std::array<bool, NUM_HOURS>&
        getBindingCapacityConstraints() const
    {
        return _capacityBinding;
    }


    std::size_t getNumberOfBindingCapacityConstraints() const
    {
        std::size_t count = 0;

        for (bool binding : _capacityBinding)
        {
            if (binding)
            {
                ++count;
            }
        }

        return count;
    }

    void setOptimalCost(double value)
    {
        _optimalCost = value;
    }


    double getOptimalCost() const
    {
        return _optimalCost;
    }

    void setBaselineCost(double value)
    {
        _baselineCost = value;

        calculateSavings();
    }


    double getBaselineCost() const
    {
        return _baselineCost;
    }

    void calculateSavings()
    {
        _absoluteSavings =
            _baselineCost - _optimalCost;

        if (_baselineCost > 0.0)
        {
            _percentageSavings =
                100.0 *
                _absoluteSavings /
                _baselineCost;
        }
        else
        {
            _percentageSavings = 0.0;
        }
    }


    double getAbsoluteSavings() const
    {
        return _absoluteSavings;
    }


    double getPercentageSavings() const
    {
        return _percentageSavings;
    }

    void setPeakPower(double value)
    {
        _peakPower = value;
    }


    double getPeakPower() const
    {
        return _peakPower;
    }

    void setTotalEnergy(double value)
    {
        _totalEnergy = value;
    }


    double getTotalEnergy() const
    {
        return _totalEnergy;
    }


    void calculateDerivedValues(
        const OptimizationProblem& problem)
    {
        _totalPower.fill(0.0);

        _peakPower = 0.0;

        _totalEnergy = 0.0;

        for (std::size_t hour = 0;
            hour < NUM_HOURS;
            ++hour)
        {
            double powerAtHour = 0.0;

            for (std::size_t appliance = 0;
                appliance < NUM_APPLIANCES;
                ++appliance)
            {
                const auto& applianceData =
                    problem.getAppliance(appliance);

                if (!applianceData.enabled)
                {
                    continue;
                }

                powerAtHour +=
                    applianceData.getPower() *
                    _schedule[appliance][hour];
            }

            _totalPower[hour] = powerAtHour;

            if (powerAtHour > _peakPower)
            {
                _peakPower = powerAtHour;
            }

            _capacityBinding[hour] =
                std::abs(
                    powerAtHour -
                    problem.getMaxHouseholdPower()
                ) < 1e-6;
        }

        for (std::size_t hour = 0;
            hour < NUM_HOURS;
            ++hour)
        {
            _totalEnergy += _totalPower[hour];
        }


        calculateSavings();
    }
};