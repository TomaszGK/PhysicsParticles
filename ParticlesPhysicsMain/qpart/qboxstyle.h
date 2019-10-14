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
    QString buttonStyleSelected { "QPushButton {background-color: rgb(145,105,125);font-size: 16px;font: bold}" };

    /** holds QT style sheet for unselected button */
    QString buttonStyleUnselected { "QPushButton {background-color: rgb(200,200,200);font-size: 16px;}" };

    // colors

    /** box(plane) border color */
    QColor cPlaneBorder {120,120,120};

    /** background color */
    QColor cBackground {145,215,215};

    /** axes color */
    QColor cAxesColor {120,120,120};

    /** inner frame color */
    QColor cInnerFrameColor {100,100,100};

    /** Label color in QHistogram1D */
    QColor cLabelColor {100,20,20};

    /** current value color in QBarChart */
    QColor cValue {100,20,20};

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

    /** button width in box chart */
    int buttonWidth {25};

    /** button height in box chart */
    int buttonHeight {25};

    /** selected button indent in box chart */
    int buttonIndent {5};

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
    bool loadStyleFromFile( BoxStyles style );

private:

    /**
     * @brief Loads value (string,color,int or bool) from a given tag name in QDomDocument.
     *
     * @param element                 element of QDomElement from the search for a given tagName start
     * @param tagName                 name of value tag
     * @return loaded value
     */
    template< typename T >
    T load( const QDomElement& element, const QString& tagName );

};

