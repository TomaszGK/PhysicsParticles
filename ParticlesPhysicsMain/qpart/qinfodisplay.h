#pragma once

#include "qboxpainter.h"

/** @file
 * @brief Class @ref QInfoDisplay
 */

/**
 * @class QInfoDisplay
 * @brief Implements visualization of several percent values in QT box.
 *
 * Paints four percent values in QT box on its sides.
 * @author Tomasz Gburek
 * @date 2019
 */

class QInfoDisplay : public QBoxPainter
{

    Q_OBJECT

public:       

    /**
     * @brief Constructor
     *
     * @param parentWidget          ponter to parent widget
     */
    explicit QInfoDisplay( QWidget* parentWidget = nullptr );

    /**
     * @brief Sets new value to display for a given box side.
     *
     * @param side                  plane side corresponding to box side
     * @param newValue              new percent value to display
     */
    void setDisplay( PlanePart side, int newValue );

    /**
     * @brief Loads style type from xml file and /to complete/.
     *
     * Loading new parameters (e.g. colors, style sheets) from file corresponding to a given style.
     * @param style                 box style
     * @return true if style has loaded correctly otherwise false
     */
    bool loadStyle( BoxStyles style ) override;

private:    

    /**
     * @brief Paints display.
     */
    void paint() override;

    /**
     * @brief Empty init.
     */
    void init() override {}

    /** Maps percent values to display. */
    std::map<PlanePart,int> value { {PlanePart::UP,0},{PlanePart::DOWN,0},{PlanePart::RIGHT,0},{PlanePart::LEFT,0} };

    /** Maps length of values text in pixels. */
    std::map<PlanePart,int> textWidth { {PlanePart::UP,0},{PlanePart::DOWN,0},{PlanePart::RIGHT,0},{PlanePart::LEFT,0} };

};
