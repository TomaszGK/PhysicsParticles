#pragma once

#include <QPainter>
#include <QtXml>

/** @file
 * @brief Class @ref QBoxStyle
 */

/** @enum BoxStyles
 *  @brief Represents various box styles.
 */
enum class BoxStyles
{
    BAR_CHART1 , /**< style for bar chart */
    BAR_CHART2 , /**< style for bar chart */
    BAR_CHART3 , /**< style for bar chart */
    HISTOGRAM1 , /**< style for histogram */
    HISTOGRAM2 , /**< style for histogram */
    HISTOGRAM3 , /**< style for histogram */
    DEFAULT      /**< default style */
};

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

    // sheet styles

    /** holds QT style sheet for selected button */
    QString buttonStyleSelected  { "QPushButton {background-color: %1;font-size: 16px;font: bold}" };

    /** holds QT style sheet for unselected button */
    QString buttonStyleUnselected { "QPushButton {background-color: %1;font-size: 16px;}" };

    // colors

    /** box(plane) border color */
    QColor cPlaneBorder {120,120,120} ;

    /** background color */
    QColor cBackground {145,215,215} ;

    /** Label color in QHistogram1D */
    QColor cLabelColor {100,20,20};

    /** current value color in QBarChart */
    QColor cValue {100,20,20};

    /** active button color in QBarChart */
    QColor cButtonActive {145,105,125};

    /** button color in QBarChart */
    QColor cButton {200,200,200};

    /** button label color in QBarChart */
    QColor cButtonLabel {15,15,15};

    /**< upper bins color in QBarDisplay */
    QColor cUpper {0,0,255};

    /**< lower bins color in QBarDisplay */
    QColor cLower {255,0,0};

    /** big circle border color in QCircleControl */
    QColor cBigCirclePenColor {80,80,90};

    /** small circle color in QCircleControl */
    QColor cSmallCircleColor {155,155,155};

    /** small circle hooked color in QCircleControl */
    QColor cSmallCircleHookedColor {100,100,255};

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

    /** scalable up flag - if true then maxValue can increase , in QBarChart */
    bool isScalableUp {true};

    /** scalable down flag - if true then maxValue can decrease , in QBarChart */
    bool isScalableDown {true};

    /** map of location of xml style files */
    std::map<BoxStyles,QString> styleFiles {
                                            {BoxStyles::BAR_CHART1,"styleBarChart.xml"},
                                            {BoxStyles::BAR_CHART2,"styleBarChart.xml"},
                                            {BoxStyles::BAR_CHART3,"styleBarChart.xml"},
                                            {BoxStyles::HISTOGRAM1,"styleBarChart.xml"},
                                            {BoxStyles::HISTOGRAM2,"styleBarChart.xml"},
                                            {BoxStyles::HISTOGRAM3,"styleBarChart.xml"},
                                            {BoxStyles::DEFAULT,""}
                                           };

    /**
     * @brief Loads style type from xml file.
     *
     * Loading new parameters (e.g. colors, style sheets) from file corresponding to a given style.
     * @param style                 box style
     * @return true if style has loaded correctly otherwise false
     */
    bool loadStyle( BoxStyles style );

private:

    /**
     * @brief Loads color from a given tag name in QDomDocument.
     *
     * @param element                 element of QDomElement from the search for a given tagName start
     * @param tagName                 name of color tag
     * @return loaded color or QColor {0,0,0} if such tag does not exits
     */
    QColor loadColor( const QDomElement& element, const QString& tagName );

    /**
     * @brief Loads string from a given tag name in QDomDocument.
     *
     * @param element                 element of QDomElement from the search for a given tagName start
     * @param tagName                 name of string tag
     * @return loaded string or empty string if such tag does not exits
     */
    QString loadString( const QDomElement& element, const QString& tagName );

    /**
     * @brief Loads integer number from a given tag name in QDomDocument.
     *
     * @param element                 element of QDomElement from the search for a given tagName start
     * @param tagName                 name of integer tag
     * @return loaded integer or zero if such tag does not exits
     */
    int loadInt( const QDomElement& element, const QString& tagName );

    /**
     * @brief Loads boolean value from a given tag name in QDomDocument.
     *
     * @param element                 element of QDomElement from the search for a given tagName start
     * @param tagName                 name of boolean value tag
     * @return loaded boolean value or false if such tag does not exits
     */
    bool loadBool( const QDomElement& element, const QString& tagName );

};

