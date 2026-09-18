#pragma once

#include <gui/MenuBar.h>


class MenuBar : public gui::MenuBar
{
private:

    gui::SubMenu _subApp;

    gui::SubMenu _subOptimization;


protected:


    void populateAppMenu()
    {
        auto& items = _subApp.getItems();


        items[0].initAsActionItem(
            tr("settings"),
            10
        );


        items[1].initAsSeparator();


        items[2].initAsQuitAppActionItem(
            tr("quit"),
            "q"
        );
    }



    void populateOptimizationMenu()
    {
        auto& items =
            _subOptimization.getItems();


        items[0].initAsActionItem(
            tr("optimize"),
            10
        );


        items[1].initAsActionItem(
            tr("reset"),
            20
        );

        items[2].initAsActionItem(
            tr("defaultScenario"),
            30
        );
    }


public:

    MenuBar()
        : gui::MenuBar(2)

        , _subApp(
            10,
            tr("appMenu"),
            3
        )

        , _subOptimization(
            20,
            tr("optimizationMenu"),
            3
        )
    {
        populateAppMenu();

        populateOptimizationMenu();


        _menus[0] =
            &_subApp;

        _menus[1] =
            &_subOptimization;
    }


    ~MenuBar()
    {
    }
};