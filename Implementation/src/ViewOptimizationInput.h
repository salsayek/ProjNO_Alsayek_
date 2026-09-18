#pragma once

#include <gui/View.h>
#include <gui/Label.h>
#include <gui/NumericEdit.h>
#include <gui/CheckBox.h>
#include <gui/ComboBox.h>
#include <gui/Button.h>
#include <gui/Alert.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/VerticalLayout.h>
#include <gui/HorizontalLayout.h>
#include <gui/Canvas.h>
#include <gui/Image.h>
#include <gui/DrawableString.h>

#include <functional>
#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <gui/Application.h>
#include "OptimizationProblem.h"
#include "UiImage.h"


class IconTextControl : public gui::Canvas
{
protected:
    std::string _fileName;
    const char* _translationKey = nullptr;

    gui::Image _image;
    bool _attemptedLoad = false;


    void ensureImageLoaded()
    {
        if (_attemptedLoad)
            return;

        ui::loadImage(
            _image,
            _fileName.c_str()
        );

        _attemptedLoad = true;
    }


    void onDraw(const gui::Rect& rect) override
    {
        ensureImageLoaded();

        if (_image.isOK())
        {
            _image.draw(
                gui::Rect(
                    gui::Point(0, 2),
                    gui::Size(26, 26)
                )
            );
        }


        gui::DrawableString title(
            tr(_translationKey)
        );

        title.draw(
            gui::Point(35, 5),
            gui::Font::ID::SystemSmallerBold,
            td::ColorID::SysText
        );
    }


public:

    IconTextControl(
        const char* fileName,
        const char* translationKey,
        td::WORD width = 190)
        : _fileName(fileName)
        , _translationKey(translationKey)
    {
        setFixedWidth(width);

        setFixedHeight(36);
    }
};


class ViewOptimizationInput : public gui::View
{
protected:


    OptimizationProblem* _pProblem = nullptr;

    std::function<void()>* _pFnOptimize = nullptr;
    std::function<void()>* _pFnReset = nullptr;
    std::function<void()>* _pFnLoadDefault = nullptr;
    std::function<void()>* _pFnLanguageChanged = nullptr;


    gui::Label _lblModelTitle;
    gui::Label _lblModelSubtitle;

    gui::Label _lblLanguage;
    IconTextControl _languageTitle;
    gui::ComboBox _cmbLanguage;

    gui::Label _lblHouseholdSection;
    IconTextControl _householdTitle;

    gui::Label _lblCapacity;
    gui::NumericEdit _edCapacity;



    gui::Label _lblTariffSection;
    IconTextControl _tariffTitle;

    gui::Label _lblTariffProfile;
    gui::ComboBox _cmbTariffProfile;

    gui::Label _lblLowTariff;
    gui::NumericEdit _edLowTariff;

    gui::Label _lblHighTariff;
    gui::NumericEdit _edHighTariff;



    gui::Label _lblWashingMachineSection;
    IconTextControl _washingMachineTitle;
    gui::CheckBox _cbWashingMachine;

    gui::Label _lblWashingEnergy;
    gui::NumericEdit _edWashingEnergy;

    gui::Label _lblWashingDuration;
    gui::NumericEdit _edWashingDuration;

    gui::Label _lblWashingEarliest;
    gui::NumericEdit _edWashingEarliest;

    gui::Label _lblWashingLatest;
    gui::NumericEdit _edWashingLatest;

    gui::Label _lblDishwasherSection;
    IconTextControl _dishwasherTitle;
    gui::CheckBox _cbDishwasher;

    gui::Label _lblDishwasherEnergy;
    gui::NumericEdit _edDishwasherEnergy;

    gui::Label _lblDishwasherDuration;
    gui::NumericEdit _edDishwasherDuration;

    gui::Label _lblDishwasherEarliest;
    gui::NumericEdit _edDishwasherEarliest;

    gui::Label _lblDishwasherLatest;
    gui::NumericEdit _edDishwasherLatest;


    gui::Label _lblWaterHeaterSection;
    IconTextControl _waterHeaterTitle;
    gui::CheckBox _cbWaterHeater;

    gui::Label _lblWaterHeaterEnergy;
    gui::NumericEdit _edWaterHeaterEnergy;

    gui::Label _lblWaterHeaterDuration;
    gui::NumericEdit _edWaterHeaterDuration;

    gui::Label _lblWaterHeaterEarliest;
    gui::NumericEdit _edWaterHeaterEarliest;

    gui::Label _lblWaterHeaterLatest;
    gui::NumericEdit _edWaterHeaterLatest;

    gui::Label _lblStorageHeaterSection;
    IconTextControl _storageHeaterTitle;
    gui::CheckBox _cbStorageHeater;

    gui::Label _lblStorageHeaterEnergy;
    gui::NumericEdit _edStorageHeaterEnergy;

    gui::Label _lblStorageHeaterDuration;
    gui::NumericEdit _edStorageHeaterDuration;

    gui::Label _lblStorageHeaterEarliest;
    gui::NumericEdit _edStorageHeaterEarliest;

    gui::Label _lblStorageHeaterLatest;
    gui::NumericEdit _edStorageHeaterLatest;


    gui::Button _btnOptimize;
    gui::Button _btnReset;
    gui::Button _btnDefault;

    gui::GridLayout _grid;
    gui::HorizontalLayout _buttonsLayout;
    gui::VerticalLayout _mainLayout;


protected:

    const char* L(const char* ba, const char* en) const
    {
        const td::String& ext =
            getTranslationLang().getExtension();

        return ext.compareConstStrCI("EN")
            ? en
            : ba;
    }
    
    
    void refreshLanguageTexts()
    {
        _lblModelTitle.setTitle(tr("optimizationModel"));
        _lblModelSubtitle.setTitle("");
        _lblLanguage.setTitle(tr("language"));

        _lblHouseholdSection.setTitle(tr("householdParameters"));
        _lblCapacity.setTitle(tr("connectionCapacity"));

        _lblTariffSection.setTitle(tr("tariffParameters"));
        _lblTariffProfile.setTitle(tr("tariffProfile"));
        _lblLowTariff.setTitle(tr("lowTariff"));
        _lblHighTariff.setTitle(tr("highTariff"));

        _lblWashingMachineSection.setTitle(tr("washingMachine"));
        _cbWashingMachine.setTitle(tr("enabled"));
        _lblWashingEnergy.setTitle(tr("energyDemand"));
        _lblWashingDuration.setTitle(tr("duration"));
        _lblWashingEarliest.setTitle(tr("earliestStart"));
        _lblWashingLatest.setTitle(tr("latestFinish"));

        _lblDishwasherSection.setTitle(tr("dishwasher"));
        _cbDishwasher.setTitle(tr("enabled"));
        _lblDishwasherEnergy.setTitle(tr("energyDemand"));
        _lblDishwasherDuration.setTitle(tr("duration"));
        _lblDishwasherEarliest.setTitle(tr("earliestStart"));
        _lblDishwasherLatest.setTitle(tr("latestFinish"));

        _lblWaterHeaterSection.setTitle(tr("waterHeater"));
        _cbWaterHeater.setTitle(tr("enabled"));
        _lblWaterHeaterEnergy.setTitle(tr("energyDemand"));
        _lblWaterHeaterDuration.setTitle(tr("duration"));
        _lblWaterHeaterEarliest.setTitle(tr("earliestStart"));
        _lblWaterHeaterLatest.setTitle(tr("latestFinish"));

        _lblStorageHeaterSection.setTitle(tr("storageHeater"));
        _cbStorageHeater.setTitle(tr("enabled"));
        _lblStorageHeaterEnergy.setTitle(tr("energyDemand"));
        _lblStorageHeaterDuration.setTitle(tr("duration"));
        _lblStorageHeaterEarliest.setTitle(tr("earliestStart"));
        _lblStorageHeaterLatest.setTitle(tr("latestFinish"));

        _btnOptimize.setTitle(tr("optimize"));
        _btnReset.setTitle(tr("reset"));
        _btnDefault.setTitle(tr("defaultScenario"));

        _languageTitle.reDraw();
        _householdTitle.reDraw();
        _tariffTitle.reDraw();
        _washingMachineTitle.reDraw();
        _dishwasherTitle.reDraw();
        _waterHeaterTitle.reDraw();
        _storageHeaterTitle.reDraw();


        int tariffIndex =
            _cmbTariffProfile.getSelectedIndex();

        if (tariffIndex < 0)
            tariffIndex = 0;

        _cmbTariffProfile.clean();

        _cmbTariffProfile.addItem(
            tr("summerWorkingDay")
        );

        _cmbTariffProfile.addItem(
            tr("winterWorkingDay")
        );

        _cmbTariffProfile.addItem(
            tr("sundayLowTariff")
        );

        _cmbTariffProfile.selectIndex(
            tariffIndex,
            false
        );
    }


    void changeLanguage()
    {
        gui::Application* app = getApplication();
        if (!app)
            return;

        int selected = _cmbLanguage.getSelectedIndex();

        if (selected < 0)
            selected = 0;

        const char* languageCode =
            (selected == 1) ? "EN" : "BA";


        const td::String& currentExt =
            getTranslationLang().getExtension();

        const bool alreadySelected =
            (selected == 1)
            ? currentExt.compareConstStrCI("EN")
            : currentExt.compareConstStrCI("BA");

        if (alreadySelected)
            return;


        if (auto* properties = app->getProperties())
        {
            properties->setValue(
                "translation",
                languageCode
            );

            properties->save();
        }

        app->restart();
    }



    void configureControls()
    {
        _cmbLanguage.setSizeLimitForNChars(12, gui::Control::Limit::Fixed);
        _cmbTariffProfile.setSizeLimitForNChars(18, gui::Control::Limit::Fixed);

        const std::array<gui::NumericEdit*, 19> edits =
        {
            &_edCapacity,
            &_edLowTariff, &_edHighTariff,
            &_edWashingEnergy, &_edWashingDuration,
            &_edWashingEarliest, &_edWashingLatest,
            &_edDishwasherEnergy, &_edDishwasherDuration,
            &_edDishwasherEarliest, &_edDishwasherLatest,
            &_edWaterHeaterEnergy, &_edWaterHeaterDuration,
            &_edWaterHeaterEarliest, &_edWaterHeaterLatest,
            &_edStorageHeaterEnergy, &_edStorageHeaterDuration,
            &_edStorageHeaterEarliest, &_edStorageHeaterLatest
        };

        for (gui::NumericEdit* edit : edits)
        {
            edit->setSizeLimitForNChars(8, gui::Control::Limit::Fixed);
        }

        _edCapacity.showThSep(false);
        _edLowTariff.showThSep(false);
        _edHighTariff.showThSep(false);

        _edWashingEnergy.showThSep(false);
        _edWashingDuration.showThSep(false);
        _edWashingEarliest.showThSep(false);
        _edWashingLatest.showThSep(false);

        _edDishwasherEnergy.showThSep(false);
        _edDishwasherDuration.showThSep(false);
        _edDishwasherEarliest.showThSep(false);
        _edDishwasherLatest.showThSep(false);

        _edWaterHeaterEnergy.showThSep(false);
        _edWaterHeaterDuration.showThSep(false);
        _edWaterHeaterEarliest.showThSep(false);
        _edWaterHeaterLatest.showThSep(false);

        _edStorageHeaterEnergy.showThSep(false);
        _edStorageHeaterDuration.showThSep(false);
        _edStorageHeaterEarliest.showThSep(false);
        _edStorageHeaterLatest.showThSep(false);

        _edCapacity.setMinValue(0.1);
        _edCapacity.setMaxValue(50.0);
        _edCapacity.setNumberOfDigitsAfterDecimalPoint(2);


        _edLowTariff.setMinValue(0.0);
        _edLowTariff.setMaxValue(5.0);
        _edLowTariff.setNumberOfDigitsAfterDecimalPoint(4);

        _edHighTariff.setMinValue(0.0);
        _edHighTariff.setMaxValue(5.0);
        _edHighTariff.setNumberOfDigitsAfterDecimalPoint(4);


        configureApplianceControls(
            _edWashingEnergy,
            _edWashingDuration,
            _edWashingEarliest,
            _edWashingLatest
        );

        configureApplianceControls(
            _edDishwasherEnergy,
            _edDishwasherDuration,
            _edDishwasherEarliest,
            _edDishwasherLatest
        );

        configureApplianceControls(
            _edWaterHeaterEnergy,
            _edWaterHeaterDuration,
            _edWaterHeaterEarliest,
            _edWaterHeaterLatest
        );

        configureApplianceControls(
            _edStorageHeaterEnergy,
            _edStorageHeaterDuration,
            _edStorageHeaterEarliest,
            _edStorageHeaterLatest
        );
    }


    void configureApplianceControls(
        gui::NumericEdit& energy,
        gui::NumericEdit& duration,
        gui::NumericEdit& earliest,
        gui::NumericEdit& latest)
    {
        energy.setMinValue(0.01);
        energy.setMaxValue(100.0);
        energy.setNumberOfDigitsAfterDecimalPoint(2);


        duration.setMinValue(0.1);
        duration.setMaxValue(24.0);
        duration.setNumberOfDigitsAfterDecimalPoint(2);


        earliest.setMinValue(0.0);
        earliest.setMaxValue(23.0);
        earliest.setNumberOfDigitsAfterDecimalPoint(0);


        latest.setMinValue(1.0);
        latest.setMaxValue(24.0);
        latest.setNumberOfDigitsAfterDecimalPoint(0);
    }


    static void setDecimal2(
        gui::NumericEdit& edit,
        double value)
    {
        td::Variant val(value);
        edit.setValue(val);
    }


    static void setDecimal4(
        gui::NumericEdit& edit,
        double value)
    {
        td::Variant val(value);
        edit.setValue(val);
    }



    void refreshAppliance(
        const OptimizationProblem::Appliance& appliance,
        gui::CheckBox& enabled,
        gui::NumericEdit& energy,
        gui::NumericEdit& duration,
        gui::NumericEdit& earliest,
        gui::NumericEdit& latest)
    {
        enabled.setValue(appliance.enabled);

        setDecimal2(
            energy,
            appliance.energyDemand
        );

        setDecimal2(
            duration,
            appliance.duration
        );

        setDecimal2(
            earliest,
            static_cast<double>(
                appliance.earliestStart
                )
        );

        setDecimal2(
            latest,
            static_cast<double>(
                appliance.latestFinish
                )
        );
    }


    void commitAppliance(
        OptimizationProblem::Appliance& appliance,
        gui::CheckBox& enabled,
        gui::NumericEdit& energy,
        gui::NumericEdit& duration,
        gui::NumericEdit& earliest,
        gui::NumericEdit& latest)
    {
        double energyValue = 0.0;
        double durationValue = 0.0;
        double earliestValue = 0.0;
        double latestValue = 0.0;

        energy.getValue(energyValue);
        duration.getValue(durationValue);
        earliest.getValue(earliestValue);
        latest.getValue(latestValue);


        appliance.enabled =
            enabled.getValue();

        appliance.energyDemand =
            energyValue;

        appliance.duration =
            durationValue;

        appliance.earliestStart =
            static_cast<int>(
                std::lround(earliestValue)
                );

        appliance.latestFinish =
            static_cast<int>(
                std::lround(latestValue)
                );
    }


    void applyTariffProfile(
        double lowTariff,
        double highTariff)
    {
        if (!_pProblem)
            return;


        const int profile =
            _cmbTariffProfile.getSelectedIndex();


        for (std::size_t hour = 0;
            hour < OptimizationProblem::NUM_HOURS;
            ++hour)
        {
            bool lowPrice = false;

            if (profile == 0)
            {
                if (hour < 8)
                    lowPrice = true;

                if (hour >= 14 && hour < 17)
                    lowPrice = true;

                if (hour >= 23)
                    lowPrice = true;
            }


            else if (profile == 1)
            {
                if (hour < 7)
                    lowPrice = true;

                if (hour >= 13 && hour < 16)
                    lowPrice = true;

                if (hour >= 22)
                    lowPrice = true;
            }

            else
            {
                lowPrice = true;
            }


            _pProblem->setTariff(
                hour,
                lowPrice
                ? lowTariff
                : highTariff
            );
        }
    }


    void inferTariffProfile()
    {
        if (!_pProblem)
            return;


        const auto& tariff =
            _pProblem->getTariffProfile();


        auto minmax =
            std::minmax_element(
                tariff.begin(),
                tariff.end()
            );


        const double low =
            *minmax.first;

        const double high =
            *minmax.second;


        setDecimal4(
            _edLowTariff,
            low
        );

        setDecimal4(
            _edHighTariff,
            high
        );


        if (std::abs(high - low) < 1e-8)
        {
            _cmbTariffProfile.selectIndex(2);
            return;
        }


        bool summer = true;
        bool winter = true;


        for (std::size_t hour = 0;
            hour < OptimizationProblem::NUM_HOURS;
            ++hour)
        {
            bool expectedSummerLow =
                hour < 8 ||
                (hour >= 14 && hour < 17) ||
                hour >= 23;


            bool expectedWinterLow =
                hour < 7 ||
                (hour >= 13 && hour < 16) ||
                hour >= 22;


            const bool actualLow =
                std::abs(
                    tariff[hour] - low
                ) < 1e-8;


            if (actualLow != expectedSummerLow)
            {
                summer = false;
            }


            if (actualLow != expectedWinterLow)
            {
                winter = false;
            }
        }


        if (summer)
        {
            _cmbTariffProfile.selectIndex(0);
        }
        else if (winter)
        {
            _cmbTariffProfile.selectIndex(1);
        }
    }


    void initializeCallbacks()
    {
        _btnOptimize.onClick(
            [this]()
            {
                if (_pFnOptimize)
                {
                    (*_pFnOptimize)();
                }
            }
        );


        _btnReset.onClick(
            [this]()
            {
                if (_pFnReset)
                {
                    (*_pFnReset)();
                }
            }
        );


        _btnDefault.onClick(
            [this]()
            {
                if (_pFnLoadDefault)
                {
                    (*_pFnLoadDefault)();
                }
            }
        );
        _cmbLanguage.onChangedSelection(
            [this]()
            {
                changeLanguage();
            }
        );
    }


    td::String localizeValidationError(const std::string& errorMessage) const
    {
        if (errorMessage.empty())
            return td::String();

        const char* applianceName = nullptr;

        if (errorMessage.rfind("Washing Machine:", 0) == 0)
            applianceName = L("Ves masina", "Washing machine");
        else if (errorMessage.rfind("Dishwasher:", 0) == 0)
            applianceName = L("Masina za sude", "Dishwasher");
        else if (errorMessage.rfind("Water Heater:", 0) == 0)
            applianceName = L("Bojler", "Water heater");
        else if (errorMessage.rfind("Storage Heater:", 0) == 0)
            applianceName = L("Peć", "Storage heater");

        const char* detail = nullptr;

        if (errorMessage.find("energy demand must be greater than zero") != std::string::npos)
            detail = L("energija mora biti veca od nule.", "energy must be greater than zero.");
        else if (errorMessage.find("operating duration must be greater than zero") != std::string::npos)
            detail = L("trajanje mora biti vece od nule.", "duration must be greater than zero.");
        else if (errorMessage.find("earliest start must be between 0 and 23") != std::string::npos)
            detail = L("vrijeme 'Od' mora biti izmedju 0 i 23.", "'From' must be between 0 and 23.");
        else if (errorMessage.find("latest finish must be between 1 and 24") != std::string::npos)
            detail = L("vrijeme 'Do' mora biti izmedju 1 i 24.", "'To' must be between 1 and 24.");
        else if (errorMessage.find("earliest start must be before latest finish") != std::string::npos)
            detail = L("vrijeme 'Od' mora biti prije vremena 'Do'.", "'From' must be earlier than 'To'.");
        else if (errorMessage.find("operating duration exceeds its available time window") != std::string::npos)
            detail = L("trajanje je duze od raspolozivog vremenskog perioda.", "duration is longer than the available time window.");
        else if (errorMessage.find("required power exceeds household connection capacity") != std::string::npos)
            detail = L("potrebna snaga prelazi maksimalnu snagu prikljucka.", "required power exceeds the household capacity.");

        if (applianceName && detail)
        {
            char buffer[256];
            SNPRINTF(
                buffer,
                sizeof(buffer),
                _TRUNCATE,
                "%s: %s",
                applianceName,
                detail
            );
            return td::String(buffer);
        }

        if (errorMessage.find("Maximum household power") != std::string::npos)
            return td::String(L("Maksimalna snaga mora biti veca od nule.", "Maximum power must be greater than zero."));

        if (errorMessage.find("Electricity tariff values cannot be negative") != std::string::npos)
            return td::String(L("Tarifa ne moze biti negativna.", "Tariff values cannot be negative."));

        return td::String(errorMessage.c_str());
    }


public:


    ViewOptimizationInput()
        : _lblModelTitle(
            tr("optimizationModel")
        )
        , _lblModelSubtitle("")
        , _lblLanguage(
            tr("language")
        )
        , _languageTitle(
            "language.png",
            "language",
            150
        )
        , _lblHouseholdSection(
            tr("householdParameters")
        )
        , _householdTitle(
            "homeenergymanagement.png",
            "householdParameters",
            280
        )
        , _lblCapacity(
            tr("connectionCapacity")
        )
        , _edCapacity(td::real8)


        , _lblTariffSection(
            tr("tariffParameters")
        )
        , _tariffTitle(
            "electricitytariff.png",
            "tariffParameters"
        )
        , _lblTariffProfile(
            tr("tariffProfile")
        )
        , _lblLowTariff(
            tr("lowTariff")
        )
        , _edLowTariff(td::real8)
        , _lblHighTariff(
            tr("highTariff")
        )
        , _edHighTariff(td::real8)



        , _lblWashingMachineSection(
            tr("washingMachine")
        )
        , _washingMachineTitle(
            "washingmachine.png",
            "washingMachine"
        )
        , _cbWashingMachine(
            tr("enabled")
        )
        , _lblWashingEnergy(
            tr("energyDemand")
        )
        , _edWashingEnergy(td::real8)
        , _lblWashingDuration(
            tr("duration")
        )
        , _edWashingDuration(td::real8)
        , _lblWashingEarliest(
            tr("earliestStart")
        )
        , _edWashingEarliest(td::real8)
        , _lblWashingLatest(
            tr("latestFinish")
        )
        , _edWashingLatest(td::real8)



        , _lblDishwasherSection(
            tr("dishwasher")
        )
        , _dishwasherTitle(
            "dishwasher.png",
            "dishwasher"
        )
        , _cbDishwasher(
            tr("enabled")
        )
        , _lblDishwasherEnergy(
            tr("energyDemand")
        )
        , _edDishwasherEnergy(td::real8)
        , _lblDishwasherDuration(
            tr("duration")
        )
        , _edDishwasherDuration(td::real8)
        , _lblDishwasherEarliest(
            tr("earliestStart")
        )
        , _edDishwasherEarliest(td::real8)
        , _lblDishwasherLatest(
            tr("latestFinish")
        )
        , _edDishwasherLatest(td::real8)


        , _lblWaterHeaterSection(
            tr("waterHeater")
        )
        , _waterHeaterTitle(
            "waterheater.png",
            "waterHeater"
        )
        , _cbWaterHeater(
            tr("enabled")
        )
        , _lblWaterHeaterEnergy(
            tr("energyDemand")
        )
        , _edWaterHeaterEnergy(td::real8)
        , _lblWaterHeaterDuration(
            tr("duration")
        )
        , _edWaterHeaterDuration(td::real8)
        , _lblWaterHeaterEarliest(
            tr("earliestStart")
        )
        , _edWaterHeaterEarliest(td::real8)
        , _lblWaterHeaterLatest(
            tr("latestFinish")
        )
        , _edWaterHeaterLatest(td::real8)



        , _lblStorageHeaterSection(
            tr("storageHeater")
        )
        , _storageHeaterTitle(
            "radiator.png",
            "storageHeater"
        )
        , _cbStorageHeater(
            tr("enabled")
        )
        , _lblStorageHeaterEnergy(
            tr("energyDemand")
        )
        , _edStorageHeaterEnergy(td::real8)
        , _lblStorageHeaterDuration(
            tr("duration")
        )
        , _edStorageHeaterDuration(td::real8)
        , _lblStorageHeaterEarliest(
            tr("earliestStart")
        )
        , _edStorageHeaterEarliest(td::real8)
        , _lblStorageHeaterLatest(
            tr("latestFinish")
        )
        , _edStorageHeaterLatest(td::real8)

        , _btnOptimize(
            tr("optimize")
        )
        , _btnReset(
            tr("reset")
        )
        , _btnDefault(
            tr("defaultScenario")
        )


        , _grid(29, 2)
        , _buttonsLayout(10)
        , _mainLayout(10)
    {
        setMargins(
            14,
            12,
            14,
            12
        );


        _lblModelTitle.setBold();

        _lblHouseholdSection.setBold();
        _lblTariffSection.setBold();

        _lblWashingMachineSection.setBold();
        _lblDishwasherSection.setBold();
        _lblWaterHeaterSection.setBold();
        _lblStorageHeaterSection.setBold();


        _btnOptimize.setAsDefault();

        _btnOptimize.setType(
            gui::Button::Type::Constructive
        );



        _cmbTariffProfile.addItem(
            tr("summerWorkingDay")
        );

        _cmbTariffProfile.addItem(
            tr("winterWorkingDay")
        );

        _cmbTariffProfile.addItem(
            tr("sundayLowTariff")
        );

        _cmbTariffProfile.selectIndex(0);

        _cmbLanguage.addItem(
            "Bosanski"
        );

        _cmbLanguage.addItem(
            "English"
        );


        const td::String& activeLanguage =
            getTranslationLang().getExtension();

        _cmbLanguage.selectIndex(
            activeLanguage.compareConstStrCI("EN") ? 1 : 0,
            false
        );

        configureControls();


        _grid.setSpaceBetweenCells(4, 10);

        gui::GridComposer gc(_grid);

        gc.appendRow(
            _lblModelTitle,
            0
        );

        gc.appendRow(
            _lblModelSubtitle,
            0
        );

        gc.appendRow(
            _languageTitle
        );

        gc.appendCol(
            _cmbLanguage,
            td::HAlignment::Right
        );

        gc.appendRow(
            _householdTitle,
            0
        );

        gc.appendRow(_lblCapacity);

        gc.appendCol(
            _edCapacity,
            td::HAlignment::Right
        );


        gc.appendRow(
            _tariffTitle,
            0
        );

        gc.appendRow(_lblTariffProfile);

        gc.appendCol(
            _cmbTariffProfile,
            td::HAlignment::Right
        );


        gc.appendRow(_lblLowTariff);

        gc.appendCol(
            _edLowTariff,
            td::HAlignment::Right
        );


        gc.appendRow(_lblHighTariff);

        gc.appendCol(
            _edHighTariff,
            td::HAlignment::Right
        );

        gc.appendRow(
            _washingMachineTitle
        );

        gc.appendCol(
            _cbWashingMachine,
            td::HAlignment::Right
        );


        gc.appendRow(_lblWashingEnergy);

        gc.appendCol(
            _edWashingEnergy,
            td::HAlignment::Right
        );


        gc.appendRow(_lblWashingDuration);

        gc.appendCol(
            _edWashingDuration,
            td::HAlignment::Right
        );


        gc.appendRow(_lblWashingEarliest);

        gc.appendCol(
            _edWashingEarliest,
            td::HAlignment::Right
        );


        gc.appendRow(_lblWashingLatest);

        gc.appendCol(
            _edWashingLatest,
            td::HAlignment::Right
        );



        gc.appendRow(
            _dishwasherTitle
        );

        gc.appendCol(
            _cbDishwasher,
            td::HAlignment::Right
        );


        gc.appendRow(_lblDishwasherEnergy);

        gc.appendCol(
            _edDishwasherEnergy,
            td::HAlignment::Right
        );


        gc.appendRow(_lblDishwasherDuration);

        gc.appendCol(
            _edDishwasherDuration,
            td::HAlignment::Right
        );


        gc.appendRow(_lblDishwasherEarliest);

        gc.appendCol(
            _edDishwasherEarliest,
            td::HAlignment::Right
        );


        gc.appendRow(_lblDishwasherLatest);

        gc.appendCol(
            _edDishwasherLatest,
            td::HAlignment::Right
        );


        gc.appendRow(
            _waterHeaterTitle
        );

        gc.appendCol(
            _cbWaterHeater,
            td::HAlignment::Right
        );


        gc.appendRow(_lblWaterHeaterEnergy);

        gc.appendCol(
            _edWaterHeaterEnergy,
            td::HAlignment::Right
        );


        gc.appendRow(_lblWaterHeaterDuration);

        gc.appendCol(
            _edWaterHeaterDuration,
            td::HAlignment::Right
        );


        gc.appendRow(_lblWaterHeaterEarliest);

        gc.appendCol(
            _edWaterHeaterEarliest,
            td::HAlignment::Right
        );


        gc.appendRow(_lblWaterHeaterLatest);

        gc.appendCol(
            _edWaterHeaterLatest,
            td::HAlignment::Right
        );


        gc.appendRow(
            _storageHeaterTitle
        );

        gc.appendCol(
            _cbStorageHeater,
            td::HAlignment::Right
        );


        gc.appendRow(_lblStorageHeaterEnergy);

        gc.appendCol(
            _edStorageHeaterEnergy,
            td::HAlignment::Right
        );


        gc.appendRow(_lblStorageHeaterDuration);

        gc.appendCol(
            _edStorageHeaterDuration,
            td::HAlignment::Right
        );


        gc.appendRow(_lblStorageHeaterEarliest);

        gc.appendCol(
            _edStorageHeaterEarliest,
            td::HAlignment::Right
        );


        gc.appendRow(_lblStorageHeaterLatest);

        gc.appendCol(
            _edStorageHeaterLatest,
            td::HAlignment::Right
        );


        _buttonsLayout
            << _btnOptimize
            << _btnReset
            << _btnDefault;

        _mainLayout << _grid;

        _mainLayout << _buttonsLayout;


        setLayout(
            &_mainLayout
        );


        initializeCallbacks();
    }


    void setProblem(
        OptimizationProblem* pProblem)
    {
        _pProblem = pProblem;
    }

    void setCallbacks(
        std::function<void()>* pOptimize,
        std::function<void()>* pReset,
        std::function<void()>* pLoadDefault,
        std::function<void()>* pLanguageChanged)
    {
        _pFnOptimize = pOptimize;
        _pFnReset = pReset;
        _pFnLoadDefault = pLoadDefault;
        _pFnLanguageChanged = pLanguageChanged;
    }


    void refresh()
    {
        if (!_pProblem)
            return;


        setDecimal2(
            _edCapacity,
            _pProblem->getMaxHouseholdPower()
        );


        inferTariffProfile();


        refreshAppliance(
            _pProblem->getAppliance(
                OptimizationProblem::ApplianceID::WashingMachine
            ),
            _cbWashingMachine,
            _edWashingEnergy,
            _edWashingDuration,
            _edWashingEarliest,
            _edWashingLatest
        );


        refreshAppliance(
            _pProblem->getAppliance(
                OptimizationProblem::ApplianceID::Dishwasher
            ),
            _cbDishwasher,
            _edDishwasherEnergy,
            _edDishwasherDuration,
            _edDishwasherEarliest,
            _edDishwasherLatest
        );


        refreshAppliance(
            _pProblem->getAppliance(
                OptimizationProblem::ApplianceID::WaterHeater
            ),
            _cbWaterHeater,
            _edWaterHeaterEnergy,
            _edWaterHeaterDuration,
            _edWaterHeaterEarliest,
            _edWaterHeaterLatest
        );


        refreshAppliance(
            _pProblem->getAppliance(
                OptimizationProblem::ApplianceID::StorageHeater
            ),
            _cbStorageHeater,
            _edStorageHeaterEnergy,
            _edStorageHeaterDuration,
            _edStorageHeaterEarliest,
            _edStorageHeaterLatest
        );
    }


    bool commitChanges()
    {
        if (!_pProblem)
        {
            return false;
        }



        double capacity = 0.0;

        _edCapacity.getValue(
            capacity
        );

        _pProblem->setMaxHouseholdPower(
            capacity
        );

        double lowTariff = 0.0;
        double highTariff = 0.0;

        _edLowTariff.getValue(
            lowTariff
        );

        _edHighTariff.getValue(
            highTariff
        );


        if (highTariff < lowTariff)
        {
            gui::Alert::show(
                tr("invalidInput"),
                tr("tariffOrderError")
            );

            return false;
        }


        applyTariffProfile(
            lowTariff,
            highTariff
        );

        commitAppliance(
            _pProblem->getAppliance(
                OptimizationProblem::ApplianceID::WashingMachine
            ),
            _cbWashingMachine,
            _edWashingEnergy,
            _edWashingDuration,
            _edWashingEarliest,
            _edWashingLatest
        );


        commitAppliance(
            _pProblem->getAppliance(
                OptimizationProblem::ApplianceID::Dishwasher
            ),
            _cbDishwasher,
            _edDishwasherEnergy,
            _edDishwasherDuration,
            _edDishwasherEarliest,
            _edDishwasherLatest
        );


        commitAppliance(
            _pProblem->getAppliance(
                OptimizationProblem::ApplianceID::WaterHeater
            ),
            _cbWaterHeater,
            _edWaterHeaterEnergy,
            _edWaterHeaterDuration,
            _edWaterHeaterEarliest,
            _edWaterHeaterLatest
        );


        commitAppliance(
            _pProblem->getAppliance(
                OptimizationProblem::ApplianceID::StorageHeater
            ),
            _cbStorageHeater,
            _edStorageHeaterEnergy,
            _edStorageHeaterDuration,
            _edStorageHeaterEarliest,
            _edStorageHeaterLatest
        );

        std::string errorMessage;


        if (!_pProblem->validate(
            errorMessage
        ))
        {
            gui::Alert::show(
                tr("invalidOptimizationProblem"),
                localizeValidationError(errorMessage)
            );

            return false;
        }


        return true;
    }
};