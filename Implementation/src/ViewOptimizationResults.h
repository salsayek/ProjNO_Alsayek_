#pragma once

#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/DrawableString.h>
#include <gui/Font.h>
#include <gui/Image.h>
#include <string>

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>
#include <gui/Application.h>
#include "OptimizationProblem.h"
#include "OptimizationResult.h"
#include "UiImage.h"


class ViewOptimizationResults : public gui::Canvas
{
protected:
    const OptimizationProblem* _pProblem = nullptr;
    const OptimizationResult* _pResult = nullptr;

    bool _imagesLoaded = false;

    gui::Image _imgHeader;
    gui::Image _imgTariff;
    gui::Image _imgSchedule;
    gui::Image _imgPower;
    gui::Image _imgInsights;
    gui::Image _imgHero;

    gui::Image _imgWashingMachine;
    gui::Image _imgDishwasher;
    gui::Image _imgWaterHeater;
    gui::Image _imgStorageHeater;


    const char* L(
        const char* ba,
        const char* en) const
    {
        if (const gui::Application* app = getApplication())
        {
            const td::String& lang =
                app->getLanguage();

            if (lang.compareConstStrCI("EN") ||
                lang.compareConstStrCI("English"))
            {
                return en;
            }

            if (lang.compareConstStrCI("BA") ||
                lang.compareConstStrCI("Bosanski"))
            {
                return ba;
            }
        }

        const td::String& ext =
            getTranslationLang().getExtension();

        return ext.compareConstStrCI("EN")
            ? en
            : ba;
    }
    bool isEnglish() const
    {
        const td::String& ext =
            getTranslationLang().getExtension();

        return ext.compareConstStrCI("EN");
    }


    void formatMoney(
        char* buffer,
        std::size_t bufferSize,
        double value) const
    {
        SNPRINTF(
            buffer,
            bufferSize,
            _TRUNCATE,
            "%.2f KM",
            value
        );

        if (!isEnglish())
        {
            for (char* p = buffer; *p != '\0'; ++p)
            {
                if (*p == '.')
                {
                    *p = ',';
                    break;
                }
            }
        }
    }
protected:

    void drawText(
        const char* text,
        const gui::Point& point,
        gui::Font::ID fontID = gui::Font::ID::SystemNormal,
        td::ColorID color = td::ColorID::DarkSlateGray) const
    {
        gui::DrawableString str(text);
        str.draw(point, fontID, color);
    }


    void drawTextCentered(
        const char* text,
        const gui::Rect& rect,
        gui::Font::ID fontID = gui::Font::ID::SystemNormal,
        td::ColorID color = td::ColorID::DarkSlateGray) const
    {
        gui::DrawableString str(text);
        str.draw(
            rect,
            fontID,
            color,
            td::TextAlignment::Center,
            td::VAlignment::Center
        );
    }


    void ensureImagesLoaded()
    {
        if (_imagesLoaded)
            return;

        ui::loadImage(_imgHeader, "smarthomeenergy.png");
        ui::loadImage(_imgTariff, "electricitytariff.png");
        ui::loadImage(_imgSchedule, "clockschedule.png");
        ui::loadImage(_imgPower, "power consumption.png");
        ui::loadImage(_imgInsights, "lightningbolt.png");
        ui::loadImage(_imgHero, "homeenergymanagement.png");

        ui::loadImage(_imgWashingMachine, "washingmachine.png");
        ui::loadImage(_imgDishwasher, "dishwasher.png");
        ui::loadImage(_imgWaterHeater, "waterheater.png");

        if (!ui::loadImage(_imgStorageHeater, "storageheater.png"))
        {
            ui::loadImage(_imgStorageHeater, "radiator.png");
        }

        _imagesLoaded = true;
    }


    void drawImage(
        const gui::Image& image,
        const gui::Rect& rect) const
    {
        if (image.isOK())
            image.draw(rect);
    }


    void drawSectionHeader(
        const gui::Image& image,
        const char* title,
        const char* subtitle,
        const gui::Rect& panel) const
    {
        gui::Rect iconRect(
            gui::Point(
                panel.left + 14,
                panel.top + 7
            ),
            gui::Size(
                36,
                36
            )
        );

        drawImage(image, iconRect);

        drawText(
            title,
            gui::Point(panel.left + 54, panel.top + 12),
            gui::Font::ID::SystemBold,
            td::ColorID::DarkSlateGray
        );

        if (subtitle && subtitle[0] != '\0')
        {
            drawText(
                subtitle,
                gui::Point(panel.left + 54, panel.top + 32),
                gui::Font::ID::SystemSmallest,
                td::ColorID::SlateGray
            );
        }
    }



    void drawCard(const gui::Rect& rect) const
    {
        gui::Rect shadow(
            gui::Point(rect.left + 3, rect.top + 4),
            gui::Size(rect.right - rect.left, rect.bottom - rect.top)
        );

        gui::Shape::drawRect(
            shadow,
            0.055f,
            td::ColorID::SlateGray
        );

        gui::Shape::drawRect(
            rect,
            td::ColorID::White
        );

        gui::Shape::drawRect(
            rect,
            td::ColorID::Gainsboro,
            0.8f
        );
    }


    void drawPill(
        const gui::Rect& rect,
        const char* text,
        td::ColorID color) const
    {
        gui::Shape::drawRect(
            rect,
            0.13f,
            color
        );

        drawTextCentered(
            text,
            rect,
            gui::Font::ID::SystemSmallerBold,
            color
        );
    }


    void drawHeader(gui::CoordType width) const
    {
        drawImage(
            _imgHeader,
            gui::Rect(
                gui::Point(16, 10),
                gui::Size(56, 56)
            )
        );

        drawText(
            L(
                "Pametna energija",
                "Smart energy"
            ),
            gui::Point(82, 20),
            gui::Font::ID::SystemLargestBold,
            td::ColorID::DodgerBlue
        );

        drawText(
            L(
                "Pregled cijene, potrošnje i rasporeda za naredna 24 sata",
                "Price, load and appliance schedule for the next 24 hours"
            ),
            gui::Point(82, 52),
            gui::Font::ID::SystemSmaller,
            td::ColorID::SlateGray
        );

        (void)width;
    }



    void drawHeroCard(
        const gui::Rect& rect,
        const char* title,
        const char* value,
        const char* subtitle,
        td::ColorID accentColor) const
    {
        drawCard(rect);

        gui::Shape::drawRect(
            rect,
            0.05f,
            accentColor
        );

        gui::Rect accent(
            gui::Point(rect.left, rect.top),
            gui::Size(6, rect.bottom - rect.top)
        );

        gui::Shape::drawRect(
            accent,
            accentColor
        );

        drawImage(
            _imgHero,
            gui::Rect(
                gui::Point(rect.right - 72, rect.top + 8),
                gui::Size(58, 58)
            )
        );

        drawText(
            title,
            gui::Point(rect.left + 22, rect.top + 16),
            gui::Font::ID::SystemSmallerBold,
            td::ColorID::DarkSlateGray
        );
        drawText(
            value,
            gui::Point(rect.left + 22, rect.top + 44),
            gui::Font::ID::SystemLargestBold,
            accentColor
        );

        drawText(
            subtitle,
            gui::Point(rect.left + 22, rect.bottom - 22),
            gui::Font::ID::SystemSmallest,
            td::ColorID::SlateGray
        );
    }


    void drawMetricCard(
        const gui::Rect& rect,
        const char* shortLabel,
        const char* title,
        const char* value,
        const char* subtitle,
        td::ColorID accentColor) const
    {
        drawCard(rect);

        gui::Shape::drawRect(
            rect,
            0.035f,
            accentColor
        );

        gui::Rect iconBox(
            gui::Point(
                rect.left + 12,
                rect.top + 10
            ),
            gui::Size(
                50,
                50
            )
        );

        gui::Shape::drawRect(
            iconBox,
            0.88f,
            accentColor
        );

        const gui::Image* metricIcon = nullptr;

        if (shortLabel[0] == '%')
            metricIcon = &_imgInsights;
        else if (shortLabel[0] == 'k')
            metricIcon = &_imgPower;
        else if (shortLabel[0] == 'E')
            metricIcon = &_imgTariff;

        if (metricIcon && metricIcon->isOK())
        {
            drawImage(
                *metricIcon,
                gui::Rect(
                    gui::Point(
                        iconBox.left + 5,
                        iconBox.top + 5
                    ),
                    gui::Size(
                        40,
                        40
                    )
                )
            );
        }
        else
        {
            drawTextCentered(
                shortLabel,
                iconBox,
                gui::Font::ID::SystemSmallestBold,
                td::ColorID::White
            );
        }

        drawText(
            title,
            gui::Point(rect.left + 74, rect.top + 14),
            gui::Font::ID::SystemSmallerBold,
            td::ColorID::DarkSlateGray
        );

        drawText(
            value,
            gui::Point(rect.left + 74, rect.top + 38),
            gui::Font::ID::SystemLargerBold,
            accentColor
        );

        drawText(
            subtitle,
            gui::Point(rect.left + 16, rect.bottom - 18),
            gui::Font::ID::SystemSmallest,
            td::ColorID::SlateGray
        );
    }


    void drawSummary(gui::CoordType width) const
    {
        const gui::CoordType margin = 18;
        const gui::CoordType gap = 14;
        const gui::CoordType totalWidth = width - 2 * margin;

        const gui::CoordType heroWidth = totalWidth * 0.34;
        const gui::CoordType smallWidth =
            (totalWidth - heroWidth - 3 * gap) / 3.0;

        
        const gui::CoordType y = 82;
        const gui::CoordType h = 108;

        char cost[64] = "--";
        char costInfo[128];

        char savings[64] = "--";
        char savingsInfo[128];

        char peak[64] = "--";
        char peakInfo[128];

        char energy[64] = "--";
        char energyInfo[128];

        costInfo[0] = 0;
        savingsInfo[0] = 0;
        peakInfo[0] = 0;
        energyInfo[0] = 0;

        if (_pResult && _pResult->isOptimal())
        {
            formatMoney(
                cost,
                sizeof(cost),
                _pResult->getOptimalCost()
            );

            char baselineMoney[64];

            formatMoney(
                baselineMoney,
                sizeof(baselineMoney),
                _pResult->getBaselineCost()
            );

            SNPRINTF(
                costInfo,
                sizeof(costInfo),
                _TRUNCATE,
                "%s %s",
                L(
                    "Bez optimizacije:",
                    "Without optimization:"
                ),
                baselineMoney
            );

            SNPRINTF(
                savings,
                sizeof(savings),
                _TRUNCATE,
                "%.1f%%",
                _pResult->getPercentageSavings()
            );

            char savingsMoney[64];

            formatMoney(
                savingsMoney,
                sizeof(savingsMoney),
                _pResult->getAbsoluteSavings()
            );

            SNPRINTF(
                savingsInfo,
                sizeof(savingsInfo),
                _TRUNCATE,
                "%s %s",
                L(
                    "Ukupna ušteda:",
                    "Total savings:"
                ),
                savingsMoney
            );

            SNPRINTF(
                peak,
                sizeof(peak),
                _TRUNCATE,
                "%.2f kW",
                _pResult->getPeakPower()
            );

            SNPRINTF(
                peakInfo,
                sizeof(peakInfo),
                _TRUNCATE,
                "Limit: %.2f kW",
                _pProblem ? _pProblem->getMaxHouseholdPower() : 0.0
            );

            SNPRINTF(
                energy,
                sizeof(energy),
                _TRUNCATE,
                "%.1f kWh",
                _pResult->getTotalEnergy()
            );
        }

        gui::Rect hero(
            gui::Point(margin, y),
            gui::Size(heroWidth, h)
        );

        gui::Rect card2(
            gui::Point(margin + heroWidth + gap, y),
            gui::Size(smallWidth, h)
        );

        gui::Rect card3(
            gui::Point(margin + heroWidth + 2 * gap + smallWidth, y),
            gui::Size(smallWidth, h)
        );

        gui::Rect card4(
            gui::Point(margin + heroWidth + 3 * gap + 2 * smallWidth, y),
            gui::Size(smallWidth, h)
        );

        drawHeroCard(
            hero,
            L(
                "OPTIMIZIRANI TROŠAK",
                "OPTIMIZED COST"
            ),
            cost,
            costInfo,
            td::ColorID::DodgerBlue
        );

        drawMetricCard(
            card2,
            "%",
            L("UŠTEDA", "SAVINGS"),
            savings,
            savingsInfo,
            td::ColorID::SeaGreen
        );

        drawMetricCard(
            card3,
            "kW",
            L("MAKSIMALNA SNAGA", "PEAK POWER"),
            peak,
            peakInfo,
            td::ColorID::Orange
        );

        drawMetricCard(
            card4,
            "E",
            L("ENERGIJA", "ENERGY"),
            energy,
            energyInfo,
            td::ColorID::MediumPurple
        );
    }


    void drawTariffGraph(const gui::Rect& panel) const
    {
        drawCard(panel);

        drawSectionHeader(
            _imgTariff,
            L("Cijena elektricne energije", "Electricity price"),
            "",
            panel
        );

        if (!_pProblem)
            return;

        double minPrice = _pProblem->getTariff(0);
        double maxPrice = minPrice;

        for (std::size_t hour = 1;
            hour < OptimizationProblem::NUM_HOURS;
            ++hour)
        {
            minPrice = std::min(minPrice, _pProblem->getTariff(hour));
            maxPrice = std::max(maxPrice, _pProblem->getTariff(hour));
        }

        char lowTxt[64];
        char highTxt[64];

        SNPRINTF(
            lowTxt,
            sizeof(lowTxt),
            _TRUNCATE,
            "%s %.4f",
            L("NIŽA", "LOW"),
            minPrice
        );

        SNPRINTF(
            highTxt,
            sizeof(highTxt),
            _TRUNCATE,
            "%s %.4f",
            L("VIŠA", "HIGH"),
            maxPrice
        );

        gui::Rect lowBadge(
            gui::Point(panel.right - 226, panel.top + 10),
            gui::Size(98, 30)
        );

        gui::Rect highBadge(
            gui::Point(panel.right - 116, panel.top + 10),
            gui::Size(98, 30)
        );

        drawPill(lowBadge, lowTxt, td::ColorID::SeaGreen);
        drawPill(highBadge, highTxt, td::ColorID::Orange);

        const gui::CoordType left = panel.left + 16;
        const gui::CoordType right = panel.right - 16;
        const gui::CoordType top = panel.top + 46;
        const gui::CoordType bottom = panel.bottom - 25;

        const gui::CoordType graphWidth = right - left;
        const gui::CoordType graphHeight = bottom - top;
        const gui::CoordType slotWidth =
            graphWidth / OptimizationProblem::NUM_HOURS;

        double range = maxPrice - minPrice;
        if (range < 1e-12)
            range = 1.0;

        gui::Shape::drawLine(
            gui::Point(left, bottom),
            gui::Point(right, bottom),
            td::ColorID::Gainsboro,
            1.0f
        );

        for (std::size_t hour = 0;
            hour < OptimizationProblem::NUM_HOURS;
            ++hour)
        {
            const double price = _pProblem->getTariff(hour);
            const bool cheap = std::abs(price - minPrice) < 1e-8;

            const double normalized =
                0.34 + 0.66 * ((price - minPrice) / range);

            const gui::CoordType barHeight = normalized * graphHeight;

            gui::Rect bar(
                gui::Point(
                    left + hour * slotWidth + 2,
                    bottom - barHeight
                ),
                gui::Size(
                    std::max(1.0, slotWidth - 4),
                    barHeight
                )
            );

            gui::Shape::drawRect(
                bar,
                0.76f,
                cheap ? td::ColorID::SeaGreen : td::ColorID::Gold
            );
        }

        for (int hour = 0; hour <= 24; hour += 6)
        {
            char txt[16];

            SNPRINTF(
                txt,
                sizeof(txt),
                _TRUNCATE,
                "%02d:00",
                hour
            );

            const gui::CoordType x =
                left + graphWidth * hour / 24.0;

            drawText(
                txt,
                gui::Point(x - 14, bottom + 5),
                gui::Font::ID::SystemSmallest,
                td::ColorID::SlateGray
            );
        }
    }



    void drawSchedule(const gui::Rect& panel) const
    {
        drawCard(panel);

        drawText(
            L("Raspored uređaja", "Appliance schedule"),
            gui::Point(panel.left + 16, panel.top + 12),
            gui::Font::ID::SystemBold,
            td::ColorID::DarkSlateGray
        );
        drawText(
            L("Svijetlo: dozvoljeni period   |   Puno: planirani rad", "Light: allowed period   |   Solid: scheduled activity"),
            gui::Point(panel.left + 16, panel.top + 32),
            gui::Font::ID::SystemSmallest,
            td::ColorID::SlateGray
        );

        if (!_pProblem)
            return;

        const bool solved =
            _pResult &&
            _pResult->isOptimal();

        const gui::CoordType labelWidth = 190;

        const gui::CoordType left =
            panel.left + labelWidth;

        const gui::CoordType right =
            panel.right - 16;

        const gui::CoordType top =
            panel.top + 58;

        const gui::CoordType bottom =
            panel.bottom - 24;

        const gui::CoordType timelineWidth = right - left;
        const gui::CoordType rowHeight =
            (bottom - top) / OptimizationProblem::NUM_APPLIANCES;
        const gui::CoordType slotWidth =
            timelineWidth / OptimizationProblem::NUM_HOURS;

        const td::ColorID colors[OptimizationProblem::NUM_APPLIANCES] =
        {
            td::ColorID::Cerulean,     
            td::ColorID::MediumPurple,  
            td::ColorID::Coral,          
            td::ColorID::SeaGreen        
        };

        const char* applianceNames[
            OptimizationProblem::NUM_APPLIANCES
        ] =
            {
                L(
                    "Veš mašina",
                    "Washing machine"
                ),

                L(
                    "Mašina za suđe",
                    "Dishwasher"
                ),

                L(
                    "Bojler",
                    "Water heater"
                ),

                L(
                    "Peć",
                    "Storage heater"
                )
            };

        const gui::Image* applianceIcons[OptimizationProblem::NUM_APPLIANCES] =
        {
            &_imgWashingMachine,
            &_imgDishwasher,
            &_imgWaterHeater,
            &_imgStorageHeater
        };

        for (int hour = 0; hour <= 24; hour += 3)
        {
            const gui::CoordType x =
                left + timelineWidth * hour / 24.0;

            const bool major = hour % 6 == 0;

            gui::Shape::drawLine(
                gui::Point(x, top),
                gui::Point(x, bottom),
                td::ColorID::SlateGray,
                major ? 0.8f : 0.4f,
                td::LinePattern::Solid,
                major ? 0.25f : 0.10f
            );
        }

        for (std::size_t appliance = 0;
            appliance < OptimizationProblem::NUM_APPLIANCES;
            ++appliance)
        {
            const auto& data = _pProblem->getAppliance(appliance);
            const td::ColorID color = colors[appliance];

            const gui::CoordType rowTop =
                top + appliance * rowHeight;


            gui::Rect applianceCard(
                gui::Point(
                    panel.left + 14,
                    rowTop + 2
                ),
                gui::Size(
                    labelWidth - 28,
                    rowHeight - 4
                )
            );


            gui::Shape::drawRect(
                applianceCard,
                0.035f,
                color
            );


            gui::Shape::drawRect(
                applianceCard,
                td::ColorID::Gainsboro,
                0.6f
            );


            gui::Rect accentBar(
                gui::Point(
                    applianceCard.left,
                    applianceCard.top
                ),
                gui::Size(
                    4,
                    applianceCard.bottom - applianceCard.top
                )
            );

            gui::Shape::drawRect(
                accentBar,
                color
            );


            gui::Rect iconBox(
                gui::Point(
                    applianceCard.left + 8,
                    applianceCard.top + 1
                ),
                gui::Size(
                    34,
                    34
                )
            );

            gui::Shape::drawRect(
                iconBox,
                0.12f,
                color
            );


            drawImage(
                *applianceIcons[appliance],
                gui::Rect(
                    gui::Point(
                        iconBox.left + 2,
                        iconBox.top + 2
                    ),
                    gui::Size(
                        30,
                        30
                    )
                )
            );

            drawText(
                applianceNames[appliance],
                gui::Point(
                    applianceCard.left + 50,
                    applianceCard.top + 11
                ),
                gui::Font::ID::SystemSmallestBold,
                data.enabled
                ? td::ColorID::DarkSlateGray
                : td::ColorID::LightSlateGray
            );


            gui::Rect track(
                gui::Point(
                    left,
                    rowTop + 5
                ),
                gui::Size(
                    timelineWidth,
                    std::max(
                        1.0,
                        rowHeight - 10
                    )
                )
            );

            gui::Shape::drawRect(
                track,
                0.022f,
                td::ColorID::SlateGray
            );

            if (data.enabled)
            {
                const gui::CoordType windowX =
                    left + data.earliestStart * slotWidth;

                const gui::CoordType windowWidth =
                    (data.latestFinish - data.earliestStart) * slotWidth;

                gui::Rect allowed(
                    gui::Point(
                        windowX,
                        rowTop + 6
                    ),
                    gui::Size(
                        windowWidth,
                        std::max(
                            1.0,
                            rowHeight - 12
                        )
                    )
                );

                gui::Shape::drawRect(
                    allowed,
                    0.14f,
                    color
                );
            }

            if (solved)
            {
                for (std::size_t hour = 0;
                    hour < OptimizationProblem::NUM_HOURS;
                    ++hour)
                {
                    const double value =
                        _pResult->getScheduleValue(appliance, hour);

                    if (value <= 1e-8)
                        continue;

                    gui::Rect activity(
                        gui::Point(
                            left + hour * slotWidth + 1,
                            rowTop + 8
                        ),
                        gui::Size(
                            std::max(
                                1.0,
                                slotWidth - 2
                            ),
                            std::max(
                                1.0,
                                rowHeight - 16
                            )
                        )
                    );

                    const float activityOpacity =
                        static_cast<float>(
                            0.72 +
                            0.25 * std::min(1.0, value)
                            );

                    gui::Shape::drawRect(
                        activity,
                        activityOpacity,
                        color
                    );

                    if (value < 0.999)
                    {
                        char fraction[16];

                        SNPRINTF(
                            fraction,
                            sizeof(fraction),
                            _TRUNCATE,
                            "%.2f",
                            value
                        );

                        drawTextCentered(
                            fraction,
                            activity,
                            gui::Font::ID::SystemSmallestBold,
                            td::ColorID::White
                        );
                    }
                }
            }
        }

        for (int hour = 0; hour <= 24; hour += 6)
        {
            char txt[16];

            SNPRINTF(
                txt,
                sizeof(txt),
                _TRUNCATE,
                "%02d:00",
                hour
            );

            const gui::CoordType x =
                left + timelineWidth * hour / 24.0;

            drawText(
                txt,
                gui::Point(x - 14, bottom + 5),
                gui::Font::ID::SystemSmallest,
                td::ColorID::SlateGray
            );
        }
    }


    void drawPowerGraph(const gui::Rect& panel) const
    {
        drawCard(panel);

        drawText(
            tr("householdPower").c_str(),
            gui::Point(panel.left + 16, panel.top + 12),
            gui::Font::ID::SystemBold,
            td::ColorID::DarkSlateGray
        );
        drawText(
            L("Opterecenje po satima u odnosu na ugovoreni limit", "Hourly load compared with the contracted capacity"),
            gui::Point(panel.left + 16, panel.top + 32),
            gui::Font::ID::SystemSmallest,
            td::ColorID::SlateGray
        );

        if (!_pProblem)
            return;

        const double capacity = _pProblem->getMaxHouseholdPower();

        char capacityTxt[64];
        SNPRINTF(
            capacityTxt,
            sizeof(capacityTxt),
            _TRUNCATE,
            L("LIMIT %.2f kW", "LIMIT %.2f kW"),
            capacity
        );

        gui::Rect limitBadge(
            gui::Point(panel.right - 126, panel.top + 10),
            gui::Size(108, 30)
        );

        drawPill(limitBadge, capacityTxt, td::ColorID::Coral);

        const gui::CoordType left = panel.left + 34;
        const gui::CoordType right = panel.right - 16;
        const gui::CoordType top = panel.top + 62;
        const gui::CoordType bottom = panel.bottom - 26;

        const gui::CoordType graphWidth = right - left;
        const gui::CoordType graphHeight = bottom - top;
        const gui::CoordType slotWidth =
            graphWidth / OptimizationProblem::NUM_HOURS;

        double maximumY = std::max(1.0, capacity * 1.15);

        if (_pResult && _pResult->isOptimal())
        {
            maximumY = std::max(
                maximumY,
                _pResult->getPeakPower() * 1.15
            );
        }

        const gui::CoordType capacityY =
            bottom - (capacity / maximumY) * graphHeight;

        gui::Shape::drawLine(
            gui::Point(left, capacityY),
            gui::Point(right, capacityY),
            td::ColorID::Coral,
            1.4f,
            td::LinePattern::Dash
        );

        gui::Shape::drawLine(
            gui::Point(left, bottom),
            gui::Point(right, bottom),
            td::ColorID::Gainsboro,
            1.0f
        );

        if (_pResult && _pResult->isOptimal())
        {
            gui::Point previousPoint;

            for (std::size_t hour = 0;
                hour < OptimizationProblem::NUM_HOURS;
                ++hour)
            {
                const double power = _pResult->getTotalPower(hour);
                const gui::CoordType barHeight =
                    (power / maximumY) * graphHeight;

                gui::Rect bar(
                    gui::Point(
                        left + hour * slotWidth + 1,
                        bottom - barHeight
                    ),
                    gui::Size(
                        std::max(1.0, slotWidth - 2),
                        barHeight
                    )
                );

                const bool binding =
                    _pResult->isCapacityBinding(hour);

                gui::Shape::drawRect(
                    bar,
                    binding ? 0.88f : 0.62f,
                    binding ? td::ColorID::Coral : td::ColorID::Cerulean
                );

                const gui::Point currentPoint(
                    left + (hour + 0.5) * slotWidth,
                    bottom - barHeight
                );

                if (hour > 0)
                {
                    gui::Shape::drawLine(
                        previousPoint,
                        currentPoint,
                        td::ColorID::Cerulean,
                        1.8f
                    );
                }

                previousPoint = currentPoint;
            }
        }

        for (int hour = 0; hour <= 24; hour += 6)
        {
            char txt[16];

            SNPRINTF(
                txt,
                sizeof(txt),
                _TRUNCATE,
                "%02d",
                hour
            );

            const gui::CoordType x =
                left + graphWidth * hour / 24.0;

            drawText(
                txt,
                gui::Point(x - 7, bottom + 5),
                gui::Font::ID::SystemSmallest,
                td::ColorID::SlateGray
            );
        }
    }


    void drawInsights(const gui::Rect& panel) const
    {
        drawCard(panel);

        drawText(
            L("Optimizacijski uvidi", "Optimization insights"),
            gui::Point(panel.left + 16, panel.top + 14),
            gui::Font::ID::SystemLargerBold,
            td::ColorID::DarkSlateGray
        );
        drawText(
            L("Ograničenja i dualne vrijednosti", "Constraints and dual values"),
            gui::Point(panel.left + 16, panel.top + 38),
            gui::Font::ID::SystemSmaller,
            td::ColorID::SlateGray
        );

        if (!_pResult || !_pResult->isOptimal())
        {
            gui::CoordType y = panel.top + 68;

            struct HintRow
            {
                const char* tag;
                const char* title;
                const char* subtitle;
                td::ColorID color;
            };

            const HintRow rows[] =
            {
                { "kW", L("Kapacitet", "Capacity"), L("Vrijednost dodatnog 1 kW", "Value of +1 kW"), td::ColorID::DodgerBlue },
                { "LP", L("Dualnost", "Duality"), L("Uticaj ograničenja na trosak", "Cost impact of constraints"), td::ColorID::MediumPurple },
                { "!", L("Aktivni sati", "Active hours"), L("Sati kada je limit snage dostignut", "Hours when the capacity limit binds"), td::ColorID::Orange }
            };

            for (const auto& row : rows)
            {
                gui::Rect icon(
                    gui::Point(panel.left + 16, y),
                    gui::Size(30, 30)
                );

                gui::Shape::drawRect(icon, 0.14f, row.color);

                drawTextCentered(
                    row.tag,
                    icon,
                    gui::Font::ID::SystemSmallerBold,
                    row.color
                );

                drawText(
                    row.title,
                    gui::Point(panel.left + 56, y),
                    gui::Font::ID::SystemSmallerBold,
                    td::ColorID::DarkSlateGray
                );

                drawText(
                    row.subtitle,
                    gui::Point(panel.left + 56, y + 16),
                    gui::Font::ID::SystemSmallest,
                    td::ColorID::SlateGray
                );

                y += 49;
            }

            return;
        }

        const std::size_t bindingCount =
            _pResult->getNumberOfBindingCapacityConstraints();

        char bindingTxt[64];

        if (bindingCount == 0)
        {
            SNPRINTF(
                bindingTxt,
                sizeof(bindingTxt),
                _TRUNCATE,
                "%s",
                L(
                    "LIMIT OK",
                    "CAPACITY OK"
                )
            );
        }
        else
        {
            SNPRINTF(
                bindingTxt,
                sizeof(bindingTxt),
                _TRUNCATE,
                L(
                    SIZE_T_FMT " aktivnih ograničenja",
                    SIZE_T_FMT " active constraints"
                ),
                bindingCount
            );
        }


        gui::Rect bindingBadge(
            gui::Point(
                panel.right - 158,
                panel.top + 10
            ),
            gui::Size(
                140,
                30
            )
        );


        drawPill(
            bindingBadge,
            bindingTxt,

            bindingCount == 0
            ? td::ColorID::SeaGreen
            : td::ColorID::Coral
        );

        gui::CoordType y = panel.top + 62;

        drawText(
            L("Dualne vrijednosti uređaja", "Appliance dual values"),
            gui::Point(panel.left + 16, y),
            gui::Font::ID::SystemSmallerBold,
            td::ColorID::MediumPurple
        );

        y += 24;

        const char* names[OptimizationProblem::NUM_APPLIANCES] =
        {
            L("Veš mašina", "Washing machine"),
            L("Mašina za suđe", "Dishwasher"),
            L("Bojler", "Water heater"),
            L("Peć", "Storage heater")
        };

        const td::ColorID colors[OptimizationProblem::NUM_APPLIANCES] =
        {
            td::ColorID::DodgerBlue,
            td::ColorID::MediumPurple,
            td::ColorID::Orange,
            td::ColorID::SeaGreen
        };

        for (std::size_t i = 0;
            i < OptimizationProblem::NUM_APPLIANCES;
            ++i)
        {
            char value[96];

            SNPRINTF(
                value,
                sizeof(value),
                _TRUNCATE,
                "%+.5f",
                _pResult->getApplianceShadowPrice(i)
            );

            gui::Rect colorTag(
                gui::Point(panel.left + 16, y + 3),
                gui::Size(8, 8)
            );

            gui::Shape::drawRect(colorTag, colors[i]);

            drawText(
                names[i],
                gui::Point(panel.left + 32, y),
                gui::Font::ID::SystemSmallest,
                td::ColorID::DarkSlateGray
            );

            drawText(
                value,
                gui::Point(panel.right - 86, y),
                gui::Font::ID::SystemSmallerBold,
                colors[i]
            );

            y += 23;
        }

        y += 8;

        drawText(
            L(
                "Status ograničenja snage",
                "Power limit status"
            ),
            gui::Point(
                panel.left + 16,
                y
            ),
            gui::Font::ID::SystemSmallerBold,
            td::ColorID::DodgerBlue
        );

        y += 22;

        struct ShadowEntry
        {
            std::size_t hour;
            double value;
        };

        std::vector<ShadowEntry> entries;

        for (std::size_t hour = 0;
            hour < OptimizationProblem::NUM_HOURS;
            ++hour)
        {
            const double value =
                _pResult->getCapacityShadowPrice(hour);

            if (std::abs(value) > 1e-8 ||
                _pResult->isCapacityBinding(hour))
            {
                entries.push_back({ hour, value });
            }
        }

        std::sort(
            entries.begin(),
            entries.end(),
            [](const ShadowEntry& a, const ShadowEntry& b)
            {
                return std::abs(a.value) > std::abs(b.value);
            }
        );

        if (entries.empty())
        {
            char statusTxt[160];

            const double peakPower =
                _pResult->getPeakPower();

            const double powerLimit =
                _pProblem
                ? _pProblem->getMaxHouseholdPower()
                : 0.0;


            SNPRINTF(
                statusTxt,
                sizeof(statusTxt),
                _TRUNCATE,
                L(
                    "Limit nije dostignut: vrh %.2f kW od %.2f kW.",
                    "Limit not reached: peak %.2f kW of %.2f kW."
                ),
                peakPower,
                powerLimit
            );


            drawText(
                statusTxt,
                gui::Point(
                    panel.left + 16,
                    y
                ),
                gui::Font::ID::SystemSmallest,
                td::ColorID::SeaGreen
            );
        }

        else
        {
            const std::size_t maxRows =
                std::min<std::size_t>(3, entries.size());

            for (std::size_t i = 0; i < maxRows; ++i)
            {
                char txt[128];

                SNPRINTF(
                    txt,
                    sizeof(txt),
                    _TRUNCATE,
                    "%02d:00   dual %+.5f%s",
                    static_cast<int>(entries[i].hour),
                    entries[i].value,
                    _pResult->isCapacityBinding(entries[i].hour)
                        ? L("   AKTIVNO", "   ACTIVE")
                        : ""
                );

                drawText(
                    txt,
                    gui::Point(panel.left + 16, y),
                    gui::Font::ID::SystemSmallest,
                    _pResult->isCapacityBinding(entries[i].hour)
                        ? td::ColorID::Coral
                        : td::ColorID::DarkSlateGray
                );

                y += 20;
            }
        }
    }



    void drawSolverMessage(
        gui::CoordType width,
        gui::CoordType height) const
    {
        if (!_pResult)
            return;

        if (_pResult->getStatus() == OptimizationResult::Status::NotSolved ||
            _pResult->getStatus() == OptimizationResult::Status::Optimal)
        {
            return;
        }

        gui::Rect r(
            gui::Point(24, height - 43),
            gui::Size(width - 48, 27)
        );

        gui::Shape::drawRect(
            r,
            0.14f,
            td::ColorID::Red
        );

        gui::Shape::drawRect(
            r,
            td::ColorID::Red,
            1.0f
        );

        drawTextCentered(
            _pResult->getMessage().c_str(),
            r,
            gui::Font::ID::SystemSmallerBold,
            td::ColorID::Red
        );
    }


protected:


    void onDraw(const gui::Rect& rect) override
    {
        gui::Size size;
        getSize(size);

        ensureImagesLoaded();

        gui::Shape::drawRect(
            rect,
            td::ColorID::Honeydew
        );

        const gui::CoordType width = size.width;
        const gui::CoordType height = size.height;

        if (width < 650 || height < 780)
        {
            drawTextCentered(
                L("Povečaj prozor za prikaz.", "Enlarge the window to continue."),
                rect,
                gui::Font::ID::SystemBold,
                td::ColorID::DarkSlateGray
            );
            return;
        }

        drawHeader(width);
        drawSummary(width);

        const gui::CoordType margin = 18;
        const gui::CoordType gap = 14;

        const gui::CoordType tariffTop = 204;
        const gui::CoordType tariffHeight = 154;

        gui::Rect tariffPanel(
            gui::Point(margin, tariffTop),
            gui::Size(width - 2 * margin, tariffHeight)
        );

        drawTariffGraph(tariffPanel);

        const gui::CoordType scheduleTop =
            tariffTop + tariffHeight + gap;
        const gui::CoordType scheduleHeight = 268;

        gui::Rect schedulePanel(
            gui::Point(margin, scheduleTop),
            gui::Size(width - 2 * margin, scheduleHeight)
        );

        drawSchedule(schedulePanel);

        const gui::CoordType bottomTop =
            scheduleTop + scheduleHeight + gap;

        const gui::CoordType bottomHeight =
            std::max(
                170.0,
                height - bottomTop - margin
            );

        const gui::CoordType totalBottomWidth =
            width - 2 * margin;

        const gui::CoordType powerWidth =
            totalBottomWidth * 0.54;

        const gui::CoordType insightsWidth =
            totalBottomWidth - powerWidth - gap;

        gui::Rect powerPanel(
            gui::Point(margin, bottomTop),
            gui::Size(powerWidth, bottomHeight)
        );

        gui::Rect insightsPanel(
            gui::Point(margin + powerWidth + gap, bottomTop),
            gui::Size(insightsWidth, bottomHeight)
        );

        drawPowerGraph(powerPanel);
        drawInsights(insightsPanel);

        drawSolverMessage(width, height);
    }


public:
    ViewOptimizationResults()
    {
    }


    void setData(
        const OptimizationProblem* pProblem,
        const OptimizationResult* pResult)
    {
        _pProblem = pProblem;
        _pResult = pResult;
        reDraw();
    }


    void refresh()
    {
        reDraw();
    }
};
