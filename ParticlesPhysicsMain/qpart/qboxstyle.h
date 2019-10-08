#pragma once

#include <QPainter>

/** @file
 * @brief Class @ref QBoxStyle
 */

/**
 * @class QBoxStyle
 * @brief Class defines visualization of boxes implemented by classes derived from QBoxPainter.
 *
 * Stores information about styles and colors of a box.
 * @author Tomasz Gburek
 * @date 2019
 */

class QBoxStyle
{

public:

    /**
     * @brief default constructor
     */
    QBoxStyle();

    /**
     * @brief Loads style from file.
     *
     * Loading new style parameters (e.g. colors, style sheets) from a given file.
     * @param name                 file name in xml format
     * @return true if load style has succeeded otherwise false
     */
    bool loadStyle( QString name );

    // sheet styles

    /** holds QT style sheet for selected button */
    QString buttonStyleSelected  { "QPushButton {""background-color: %1"";font-size: 16px;font: bold}" };

    /** holds QT style sheet for unselected button */
    QString buttonStyleUnselected { "QPushButton {""background-color: %1"";font-size: 16px;}" };

    // colors

    /** box(plane) border color */
    QColor cPlaneBorder {120,120,120} ;

    /** background color */
    QColor cBackground {145,215,215} ;

    /** current value color */
    QColor cValue {100,20,20};

    /** active button color */
    QColor cButtonActive {145,105,125};

    /** button color */
    QColor cButton {200,200,200};

    /** button label color */
    QColor cButtonLabel {15,15,15};

    // other parameters

    /** box margin left in pixels */
    int marginLeft {20};

    /** box margin right in pixels */
    int marginRight {20};

    /** box margin top in pixels */
    int marginTop {20};

    /** box margin bottom in pixels */
    int marginBottom {20};

    /** number of horizontal axes in grid */
    int numberOfHorizontalAxes {5};

    /** number of vertical axes in grid */
    int numberOfVerticalAxes {5};

    /** box(plane) border width */
    int planeBorderWidth {1};

};

