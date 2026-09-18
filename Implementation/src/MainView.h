#pragma once

#include <gui/View.h>
#include <gui/SplitterLayout.h>
#include <functional>

#include "OptimizationProblem.h"
#include "OptimizationResult.h"
#include "OptimizationSolver.h"
#include "ViewOptimizationInput.h"
#include "ViewOptimizationResults.h"


class MainView : public gui::View
{
protected:

    gui::SplitterLayout _splitter;
    OptimizationProblem _problem;
    OptimizationResult _result;

    OptimizationSolver _solver;

    ViewOptimizationResults _resultsView;
    ViewOptimizationInput _inputView;


    std::function<void()>* _pUpdateMenuAndTB = nullptr;
    std::function<void()> _fnOptimize;
    std::function<void()> _fnReset;
    std::function<void()> _fnLoadDefault;
    std::function<void()> _fnLanguageChanged;

protected:

    void updateMenuAndToolbar()
    {
        if (_pUpdateMenuAndTB)
            (*_pUpdateMenuAndTB)();
    }


public:

    MainView(std::function<void()>* pUpdateMenuAndTB)
        : _splitter(
            gui::SplitterLayout::Orientation::Horizontal,
            gui::SplitterLayout::AuxiliaryCell::Second
        )
        , _pUpdateMenuAndTB(pUpdateMenuAndTB)
        , _fnOptimize(
            std::bind(&MainView::optimizeSchedule, this)
        )
        , _fnReset(
            std::bind(&MainView::resetOptimization, this)
        )
        , _fnLoadDefault(
            std::bind(&MainView::loadDefaultScenario, this)
        )
        , _fnLanguageChanged(
            [this]()
            {
                _resultsView.refresh();
            }
        )
    {
        setMargins(0, 0, 0, 0);
        _inputView.setProblem(&_problem);

        _resultsView.setData(&_problem, &_result);
        _inputView.setCallbacks(
            &_fnOptimize,
            &_fnReset,
            &_fnLoadDefault,
            &_fnLanguageChanged
        );

        _splitter.setContent(
            _resultsView,
            _inputView
        );

        setLayout(&_splitter);

        _problem.setDefaultScenario();

        _inputView.refresh();
        _resultsView.refresh();
    }


    void optimizeSchedule()
    {
        if (!_inputView.commitChanges())
        {
            _result.clear();

            _result.setInvalidInput();

            _resultsView.refresh();
            updateMenuAndToolbar();

            return;
        }


        _result.clear();

        _solver.solve(
            _problem,
            _result
        );


        _resultsView.refresh();


        updateMenuAndToolbar();
    }

    void resetOptimization()
    {
        _result.clear();

        _resultsView.refresh();

        updateMenuAndToolbar();
    }


    void loadDefaultScenario()
    {
        _problem.setDefaultScenario();

        _result.clear();

        _inputView.refresh();

        _resultsView.refresh();

        updateMenuAndToolbar();
    }



    bool hasOptimalSolution() const
    {
        return _result.isOptimal();
    }


    const OptimizationProblem& getProblem() const
    {
        return _problem;
    }


    const OptimizationResult& getResult() const
    {
        return _result;
    }
};