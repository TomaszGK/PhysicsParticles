#pragma once

#include "qboxpainter.h"

/** @file
 * @brief Class @ref QInfoDisplay
 */

/**
 * @class QInfoDisplay
 * @brief Implements visualization of four percent values in QT box.
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
     * @brief Sets new value for a given box side.
     *
     * @param side                  plane side corresponding to box side
     * @param newValue              new percent value to display
     */
    void setDisplay( PlaneSide side, int newValue );

    /**
     * @brief Paints display.
     */
    void paint() override;

    /**
     * @brief Empty init.
     */
    void init() override {}


private:    

    /** Maps percent values to display. */
    std::map<PlaneSide,int> value { {PlaneSide::UP,0},{PlaneSide::DOWN,0},{PlaneSide::RIGHT,0},{PlaneSide::LEFT,0} };

    /** Maps length of values text in pixels. */
    std::map<PlaneSide,int> textWidth { {PlaneSide::UP,0},{PlaneSide::DOWN,0},{PlaneSide::RIGHT,0},{PlaneSide::LEFT,0} };

};
