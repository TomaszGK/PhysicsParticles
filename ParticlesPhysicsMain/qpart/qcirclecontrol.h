#pragma once

#include "qboxpainter.h"

/** @file
 * @brief Class @ref QCircleControl
 */

/**
 * @class QCircleControl
 * @brief Implements circle control visualization in QT.
 *
 * Paints circle control in the painter box with draggable small circle
 * inside that can be controled by mouse.
 * @author Tomasz Gburek
 * @date 2019
 */

class QCircleControl : public QBoxPainter
{

    Q_OBJECT

public:

    /**
     * @brief Constructor
     *
     * @param parentWidget          ponter to parent widget
     */
    explicit QCircleControl( QWidget* parentWidget = nullptr );

    /**
     * @brief Gets circle control indicator.
     *
     * Gets vector defined by orgin and small circle positions.
     * @return circle control indicator
     */
    QPoint getIndicator() const noexcept
    {
        return indicatorPos-origin;
    }

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
     * @brief Inits and calculates QCircleControl state.
     */
    void init() override;

    /**
     * @brief Paints circle control.
     */
    void paint() override;

    /**
     * @brief Paints vertical and horizontal line.
     */
    void paintMarks();

    /**
     * @brief Gets information if cursor is hooked to the small circle status.
     *
     * Gets true if cursor is hovered over small circle and left mouse button is holding.
     * @param cursorPos             cursor position
     * @return true if cursor is hooked otherwise false
     */
    bool isCursorHookToSmallCircle( const QPoint& cursorPos );

    QPoint indicatorPos {0,0} ; /** small circle position */
    QPoint origin       {0,0} ; /** circle control orgin position */
    int    bigCircleSize {50} ; /** big circle size */
    int  smallCircleSize {15} ; /** small circle size */

    bool smallCircleHooked  {false} ; /** cursor hooked to small circle flag */
    bool smallCircleHovered {false} ; /** cursor hovered over small circle flag */

};

