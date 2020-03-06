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

    /** map of style sheets */
    std::map<QString,QString> sheets  {
                                         {"buttonStyleSelected","QPushButton {background-color: rgb(145,105,125);font-size: 16px;font: bold}"},
                                         {"buttonStyleUnselected","QPushButton {background-color: rgb(200,200,200);font-size: 16px;}"},
                                         {"buttonStyleReset","QPushButton {background-color: rgb(200,200,200);font-size: 16px;}"}
                                      };

    /** map of colors */
    std::map<QString,QColor> colors  {
                                       {"cPlaneBorder",{120,120,120}},
                                       {"cBackground",{145,215,215}},
                                       {"cAxesColor",{120,120,120}},
                                       {"cInnerFrameColor",{100,100,100}},
                                       {"cLabelColor",{100,20,20}},
                                       {"cValue",{100,20,20}},
                                       {"cButtonLabel",{15,15,15}},
                                       {"cUpper",{0,0,255}},
                                       {"cLower",{255,0,0}},
                                       {"cBigCirclePenColor",{80,80,90}},
                                       {"cSmallCircleColor",{155,155,155}},
                                       {"cSmallCircleHookedColor",{100,100,255}}
                                     };

    /** map of integer values */
    std::map<QString,int> values {
                                   {"marginLeft",20},
                                   {"marginRight",20},
                                   {"marginTop",20},
                                   {"marginBottom",20},
                                   {"numberOfHorizontalAxes",5},
                                   {"numberOfVerticalAxes",5},
                                   {"planeBorderWidth",1},
                                   {"buttonWidth",25},
                                   {"buttonHeight",25},
                                   {"buttonIndent",5}
                                 };


    /** map of boolean values */
    std::map<QString,bool> logics {
                                    {"isScalableUp",true},
                                    {"isScalableDown",true},
                                    {"isValueDisplay",true},
                                  };

    /** map of location of xml style files */
    std::map<BoxStyles,QString> styleFiles {
                                            {BoxStyles::BAR_CHART1,"styleBarChartVelocity.xml"},
                                            {BoxStyles::BAR_CHART2,"styleBarChartEnergy.xml"},
                                            {BoxStyles::BAR_CHART3,"styleBarChartDiffusion.xml"},
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
     * Loads value of string,color,int or bool from a given tag name in QDomDocument.
     * If a given tag name does not exist nothing happens.
     * @param element                 element of QDomElement from the search for a given tagName start
     * @param tagName                 name of value tag
     * @param value                   value to upload
     * @return loaded value
     */
    template< typename T >
    void load( const QDomElement& element, const QString& tagName , T& value );

};

