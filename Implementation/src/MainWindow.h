#pragma once

#include <gui/Window.h>
#include <functional>

#include "MainView.h"


class MainWindow : public gui::Window
{
protected:

    std::function<void()> _fnUpdateUI;

    MainView _mainView;


public:

    MainWindow()
        : gui::Window(gui::Size(1600, 930))
        , _fnUpdateUI([]() {})
        , _mainView(&_fnUpdateUI)
    {
        setTitle(
            tr("appTitle")
        );

        setCentralView(
            &_mainView
        );
    }


    ~MainWindow()
    {
    }


protected:

    bool shouldClose() override
    {
        return true;
    }
};