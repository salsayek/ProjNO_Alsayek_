#pragma once

#include <vector>
#include <array>
#include <cmath>
#include <limits>
#include <algorithm>
#include <string>

#include "OptimizationProblem.h"
#include "OptimizationResult.h"


class OptimizationSolver
{
private:

    static constexpr double EPS = 1e-9;
    static constexpr double INF = 1e100;


    class Simplex
    {
    private:

        int _m = 0;
        int _n = 0;

        std::vector<int> _basis;
        std::vector<int> _nonBasis;

        std::vector<std::vector<double>> _tableau;


        void pivot(int row, int column)
        {
            double inverse =
                1.0 / _tableau[row][column];

            for (int i = 0; i < _m + 2; ++i)
            {
                if (i == row)
                    continue;

                for (int j = 0; j < _n + 2; ++j)
                {
                    if (j == column)
                        continue;

                    _tableau[i][j] -=
                        _tableau[row][j] *
                        _tableau[i][column] *
                        inverse;
                }
            }


            for (int j = 0; j < _n + 2; ++j)
            {
                if (j != column)
                {
                    _tableau[row][j] *= inverse;
                }
            }


            for (int i = 0; i < _m + 2; ++i)
            {
                if (i != row)
                {
                    _tableau[i][column] *= -inverse;
                }
            }


            _tableau[row][column] = inverse;

            std::swap(
                _basis[row],
                _nonBasis[column]
            );
        }


        bool runSimplex(int phase)
        {
            const int objectiveRow =
                (phase == 1)
                ? _m + 1
                : _m;


            while (true)
            {
                int entering = -1;

                for (int column = 0;
                    column <= _n;
                    ++column)
                {
                    if (phase == 2 &&
                        _nonBasis[column] == -1)
                    {
                        continue;
                    }


                    if (entering == -1 ||
                        _tableau[objectiveRow][column] <
                        _tableau[objectiveRow][entering] - EPS ||
                        (
                            std::abs(
                                _tableau[objectiveRow][column] -
                                _tableau[objectiveRow][entering]
                            ) <= EPS &&
                            _nonBasis[column] <
                            _nonBasis[entering]
                            ))
                    {
                        entering = column;
                    }
                }


                if (entering == -1 ||
                    _tableau[objectiveRow][entering] >= -EPS)
                {
                    return true;
                }


                int leaving = -1;


                for (int row = 0;
                    row < _m;
                    ++row)
                {
                    if (_tableau[row][entering] <= EPS)
                    {
                        continue;
                    }


                    if (leaving == -1)
                    {
                        leaving = row;
                        continue;
                    }


                    double currentRatio =
                        _tableau[row][_n + 1] /
                        _tableau[row][entering];


                    double bestRatio =
                        _tableau[leaving][_n + 1] /
                        _tableau[leaving][entering];


                    if (currentRatio <
                        bestRatio - EPS ||
                        (
                            std::abs(
                                currentRatio -
                                bestRatio
                            ) <= EPS &&
                            _basis[row] <
                            _basis[leaving]
                            ))
                    {
                        leaving = row;
                    }
                }


                if (leaving == -1)
                {
                    return false;
                }


                pivot(leaving, entering);
            }
        }


    public:

        Simplex(
            const std::vector<std::vector<double>>& A,
            const std::vector<double>& b,
            const std::vector<double>& c)
            : _m(static_cast<int>(b.size()))
            , _n(static_cast<int>(c.size()))
            , _basis(_m)
            , _nonBasis(_n + 1)
            , _tableau(
                _m + 2,
                std::vector<double>(_n + 2, 0.0)
            )
        {

            for (int row = 0;
                row < _m;
                ++row)
            {
                for (int column = 0;
                    column < _n;
                    ++column)
                {
                    _tableau[row][column] =
                        A[row][column];
                }
            }


            for (int row = 0;
                row < _m;
                ++row)
            {
                _basis[row] = _n + row;

                _tableau[row][_n] = -1.0;

                _tableau[row][_n + 1] =
                    b[row];
            }


            for (int column = 0;
                column < _n;
                ++column)
            {
                _nonBasis[column] = column;

                _tableau[_m][column] =
                    -c[column];
            }


            _nonBasis[_n] = -1;

            _tableau[_m + 1][_n] = 1.0;
        }

        double solve(std::vector<double>& solution)
        {
            if (_m == 0 || _n == 0)
            {
                return -INF;
            }

            int row = 0;

            for (int i = 1;
                i < _m;
                ++i)
            {
                if (_tableau[i][_n + 1] <
                    _tableau[row][_n + 1])
                {
                    row = i;
                }
            }

            if (_tableau[row][_n + 1] < -EPS)
            {
                pivot(row, _n);


                if (!runSimplex(1) ||
                    _tableau[_m + 1][_n + 1] < -EPS)
                {
                    return -INF;
                }


                if (std::abs(
                    _tableau[_m + 1][_n + 1]
                ) > EPS)
                {
                    return -INF;
                }


                for (int i = 0;
                    i < _m;
                    ++i)
                {
                    if (_basis[i] == -1)
                    {
                        int entering = -1;

                        for (int column = 0;
                            column <= _n;
                            ++column)
                        {
                            if (entering == -1 ||
                                _tableau[i][column] <
                                _tableau[i][entering] - EPS ||
                                (
                                    std::abs(
                                        _tableau[i][column] -
                                        _tableau[i][entering]
                                    ) <= EPS &&
                                    _nonBasis[column] <
                                    _nonBasis[entering]
                                    ))
                            {
                                entering = column;
                            }
                        }


                        if (entering != -1)
                        {
                            pivot(i, entering);
                        }
                    }
                }
            }


            if (!runSimplex(2))
            {
                return INF;
            }


            solution.assign(
                _n,
                0.0
            );


            for (int i = 0;
                i < _m;
                ++i)
            {
                if (_basis[i] >= 0 &&
                    _basis[i] < _n)
                {
                    solution[_basis[i]] =
                        _tableau[i][_n + 1];
                }
            }


            return _tableau[_m][_n + 1];
        }
    };


    struct LPModel
    {
        std::vector<std::vector<double>> A;
        std::vector<double> b;

        std::vector<double> c;
        std::array<int, OptimizationProblem::NUM_HOURS>
            capacityRows{};


        std::array<int, OptimizationProblem::NUM_APPLIANCES>
            durationUpperRows{};


        std::array<int, OptimizationProblem::NUM_APPLIANCES>
            durationLowerRows{};
    };

    static std::size_t variableIndex(
        std::size_t appliance,
        std::size_t hour)
    {
        return
            appliance *
            OptimizationProblem::NUM_HOURS +
            hour;
    }


    static int addConstraint(
        LPModel& model,
        const std::vector<double>& row,
        double rhs)
    {
        model.A.push_back(row);
        model.b.push_back(rhs);

        return
            static_cast<int>(
                model.b.size() - 1
                );
    }

    void buildModel(
        const OptimizationProblem& problem,
        LPModel& model) const
    {
        constexpr std::size_t N_APPLIANCES =
            OptimizationProblem::NUM_APPLIANCES;

        constexpr std::size_t N_HOURS =
            OptimizationProblem::NUM_HOURS;

        constexpr std::size_t N_VARIABLES =
            N_APPLIANCES * N_HOURS;


        model.A.clear();
        model.b.clear();

        model.c.assign(
            N_VARIABLES,
            0.0
        );

        for (std::size_t appliance = 0;
            appliance < N_APPLIANCES;
            ++appliance)
        {
            const auto& data =
                problem.getAppliance(appliance);


            for (std::size_t hour = 0;
                hour < N_HOURS;
                ++hour)
            {
                const std::size_t index =
                    variableIndex(
                        appliance,
                        hour
                    );


                const double costCoefficient =
                    problem.getTariff(hour) *
                    data.getPower();


                model.c[index] =
                    -costCoefficient;
            }
        }

        for (std::size_t appliance = 0;
            appliance < N_APPLIANCES;
            ++appliance)
        {
            const auto& data =
                problem.getAppliance(appliance);


            std::vector<double> upperRow(
                N_VARIABLES,
                0.0
            );


            std::vector<double> lowerRow(
                N_VARIABLES,
                0.0
            );


            for (std::size_t hour = 0;
                hour < N_HOURS;
                ++hour)
            {
                std::size_t index =
                    variableIndex(
                        appliance,
                        hour
                    );


                upperRow[index] = 1.0;
                lowerRow[index] = -1.0;
            }


            double requiredDuration =
                data.enabled
                ? data.duration
                : 0.0;


            model.durationUpperRows[appliance] =
                addConstraint(
                    model,
                    upperRow,
                    requiredDuration
                );


            model.durationLowerRows[appliance] =
                addConstraint(
                    model,
                    lowerRow,
                    -requiredDuration
                );
        }

        for (std::size_t hour = 0;
            hour < N_HOURS;
            ++hour)
        {
            std::vector<double> row(
                N_VARIABLES,
                0.0
            );


            for (std::size_t appliance = 0;
                appliance < N_APPLIANCES;
                ++appliance)
            {
                const auto& data =
                    problem.getAppliance(appliance);


                std::size_t index =
                    variableIndex(
                        appliance,
                        hour
                    );


                row[index] =
                    data.enabled
                    ? data.getPower()
                    : 0.0;
            }


            model.capacityRows[hour] =
                addConstraint(
                    model,
                    row,
                    problem.getMaxHouseholdPower()
                );
        }


        for (std::size_t appliance = 0;
            appliance < N_APPLIANCES;
            ++appliance)
        {
            const auto& data =
                problem.getAppliance(appliance);


            for (std::size_t hour = 0;
                hour < N_HOURS;
                ++hour)
            {
                std::vector<double> row(
                    N_VARIABLES,
                    0.0
                );


                std::size_t index =
                    variableIndex(
                        appliance,
                        hour
                    );


                row[index] = 1.0;


                bool insideTimeWindow =
                    static_cast<int>(hour) >=
                    data.earliestStart &&
                    static_cast<int>(hour) <
                    data.latestFinish;


                double upperBound =
                    (
                        data.enabled &&
                        insideTimeWindow
                        )
                    ? 1.0
                    : 0.0;


                addConstraint(
                    model,
                    row,
                    upperBound
                );
            }
        }
    }


    double solveModel(
        const LPModel& model,
        std::vector<double>& solution) const
    {
        Simplex simplex(
            model.A,
            model.b,
            model.c
        );


        double maximumObjective =
            simplex.solve(solution);


        if (maximumObjective <= -INF / 2.0)
        {
            return INF;
        }


        if (maximumObjective >= INF / 2.0)
        {
            return -INF;
        }


        return -maximumObjective;
    }


    double calculateBaselineCost(
        const OptimizationProblem& problem,
        double optimalCost) const
    {
        constexpr std::size_t N_HOURS =
            OptimizationProblem::NUM_HOURS;

        constexpr std::size_t N_APPLIANCES =
            OptimizationProblem::NUM_APPLIANCES;


        std::array<double, N_HOURS>
            remainingCapacity{};


        remainingCapacity.fill(
            problem.getMaxHouseholdPower()
        );


        double baselineCost = 0.0;


        for (std::size_t appliance = 0;
            appliance < N_APPLIANCES;
            ++appliance)
        {
            const auto& data =
                problem.getAppliance(appliance);


            if (!data.enabled)
            {
                continue;
            }


            double remainingDuration =
                data.duration;


            const double power =
                data.getPower();


            for (int hour = data.earliestStart;
                hour < data.latestFinish &&
                remainingDuration > EPS;
                ++hour)
            {
                if (hour < 0 ||
                    hour >= static_cast<int>(N_HOURS))
                {
                    continue;
                }


                if (power <= EPS)
                {
                    continue;
                }


                double capacityFraction =
                    remainingCapacity[hour] /
                    power;


                double amount =
                    std::min(
                        1.0,
                        remainingDuration
                    );


                amount =
                    std::min(
                        amount,
                        capacityFraction
                    );


                if (amount <= EPS)
                {
                    continue;
                }


                baselineCost +=
                    problem.getTariff(hour) *
                    power *
                    amount;


                remainingCapacity[hour] -=
                    power * amount;


                remainingDuration -=
                    amount;
            }
            if (remainingDuration > 1e-7)
            {
                return optimalCost;
            }
        }


        return baselineCost;
    }


    void calculateCapacityShadowPrices(
        const LPModel& baseModel,
        double baseCost,
        OptimizationResult& result) const
    {
        const double delta = 1e-4;


        for (std::size_t hour = 0;
            hour < OptimizationProblem::NUM_HOURS;
            ++hour)
        {
            LPModel perturbed =
                baseModel;


            const int row =
                baseModel.capacityRows[hour];


            perturbed.b[row] += delta;


            std::vector<double>
                temporarySolution;


            double perturbedCost =
                solveModel(
                    perturbed,
                    temporarySolution
                );


            if (std::abs(perturbedCost) >= INF / 2.0)
            {
                result.setCapacityShadowPrice(
                    hour,
                    0.0
                );

                continue;
            }


            double shadowPrice =
                (
                    perturbedCost -
                    baseCost
                    ) /
                delta;


            if (std::abs(shadowPrice) < 1e-7)
            {
                shadowPrice = 0.0;
            }


            result.setCapacityShadowPrice(
                hour,
                shadowPrice
            );
        }
    }

    void calculateDurationShadowPrices(
        const OptimizationProblem& problem,
        const LPModel& baseModel,
        double baseCost,
        OptimizationResult& result) const
    {
        const double delta = 1e-4;


        for (std::size_t appliance = 0;
            appliance <
            OptimizationProblem::NUM_APPLIANCES;
            ++appliance)
        {
            const auto& data =
                problem.getAppliance(appliance);


            if (!data.enabled)
            {
                result.setApplianceShadowPrice(
                    appliance,
                    0.0
                );

                continue;
            }


            LPModel perturbed =
                baseModel;


            const int upperRow =
                baseModel.durationUpperRows[
                    appliance
                ];


            const int lowerRow =
                baseModel.durationLowerRows[
                    appliance
                ];


            perturbed.b[upperRow] += delta;

            perturbed.b[lowerRow] -= delta;


            std::vector<double>
                temporarySolution;


            double perturbedCost =
                solveModel(
                    perturbed,
                    temporarySolution
                );


            if (std::abs(perturbedCost) >= INF / 2.0)
            {
                result.setApplianceShadowPrice(
                    appliance,
                    0.0
                );

                continue;
            }


            double shadowPrice =
                (
                    perturbedCost -
                    baseCost
                    ) /
                delta;


            if (std::abs(shadowPrice) < 1e-7)
            {
                shadowPrice = 0.0;
            }


            result.setApplianceShadowPrice(
                appliance,
                shadowPrice
            );
        }
    }


public:

    OptimizationSolver() = default;


    bool solve(
        const OptimizationProblem& problem,
        OptimizationResult& result)
    {
        result.clear();

        std::string errorMessage;


        if (!problem.validate(errorMessage))
        {
            result.setInvalidInput();

            result.setMessage(
                errorMessage
            );

            return false;
        }

        LPModel model;

        buildModel(
            problem,
            model
        );

        std::vector<double> solution;


        double optimalCost =
            solveModel(
                model,
                solution
            );

        if (optimalCost >= INF / 2.0)
        {
            result.setInfeasible();

            return false;
        }


        if (optimalCost <= -INF / 2.0)
        {
            result.setUnbounded();

            return false;
        }


        const std::size_t expectedVariables =
            OptimizationProblem::NUM_APPLIANCES *
            OptimizationProblem::NUM_HOURS;


        if (solution.size() != expectedVariables)
        {
            result.setSolverError();

            result.setMessage(
                "Unexpected number of optimization variables."
            );

            return false;
        }

        for (std::size_t appliance = 0;
            appliance <
            OptimizationProblem::NUM_APPLIANCES;
            ++appliance)
        {
            for (std::size_t hour = 0;
                hour <
                OptimizationProblem::NUM_HOURS;
                ++hour)
            {
                const std::size_t index =
                    variableIndex(
                        appliance,
                        hour
                    );


                double value =
                    solution[index];


                if (std::abs(value) < 1e-8)
                {
                    value = 0.0;
                }


                if (std::abs(value - 1.0) < 1e-8)
                {
                    value = 1.0;
                }


                result.setScheduleValue(
                    appliance,
                    hour,
                    value
                );
            }
        }


        double objectiveValue = 0.0;


        for (std::size_t appliance = 0;
            appliance <
            OptimizationProblem::NUM_APPLIANCES;
            ++appliance)
        {
            const auto& data =
                problem.getAppliance(appliance);


            for (std::size_t hour = 0;
                hour <
                OptimizationProblem::NUM_HOURS;
                ++hour)
            {
                objectiveValue +=
                    problem.getTariff(hour) *
                    data.getPower() *
                    result.getScheduleValue(
                        appliance,
                        hour
                    );
            }
        }


        result.setOptimalCost(
            objectiveValue
        );


        double baselineCost =
            calculateBaselineCost(
                problem,
                objectiveValue
            );


        result.setBaselineCost(
            baselineCost
        );


        result.calculateDerivedValues(
            problem
        );


        calculateCapacityShadowPrices(
            model,
            objectiveValue,
            result
        );


        calculateDurationShadowPrices(
            problem,
            model,
            objectiveValue,
            result
        );

        result.setOptimal();

        result.setMessage(
            "Optimal LP solution found using the two-phase simplex method."
        );


        return true;
    }
};