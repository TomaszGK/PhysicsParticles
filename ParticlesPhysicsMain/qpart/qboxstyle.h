#pragma once

#include <QPainter>
#include <QtXml>

/** @file
 * @brief Class @ref QBoxStyle
 */

/** @struct boxValue
 *  @brief Defines box value with tag name used in QDomDocument.
 */
template< typename T >
struct boxValue
{
    operator T() { return value; }
    operator std::string() { return tag; }
    boxValue& operator =( T _value ) { value=_value; return *this; }

    T value;
    std::string tag;
};

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

/** @enum BoxSheets
 *  @brief Represents box sheet styles.
 */
enum class BoxSheets
{
   BUTTON_SELECTED,   /**< style sheet for selected button in QBarChart */
   BUTTON_UNSELECTED, /**< style sheet for unselected button in QBarChart */
   BUTTON_RESET       /**< style sheet for reset button in QBarChart */
};

/** @enum BoxColors
 *  @brief Represents box colors.
 */
enum class BoxColors
{
    PLANE_BORDER,            /**< box(plane) border color */
    BACKGROUND,              /**< background color */
    AXES,                    /**< axes color */
    INNER_FRAME,             /**< inner frame color */
    LABEL,                   /**< label color in QHistogram1D */
    VALUE,                   /**< current value color in QBarChart */
    BUTTON_LABEL,            /**< button label color in QBarChart */
    UPPER,                   /**< upper bins color in QBarDisplay */
    LOWER,                   /**< lower bins color in QBarDisplay */
    BIG_CIRCLE_PEN,          /**< big circle border color in QCircleControl */
    SMALL_CIRCLE,            /**< small circle color in QCircleControl */
    SMALL_CIRCLE_HOOKED,     /**< small circle hooked color in QCircleControl */
    DIM_PARTCILE,            /**< dim particle color in QPainterManager */
    CONSTRAINT_ARROW,        /**< constraint arrow color in QPainterManager */
    PLANE_HIT,               /**< plane hit color in QPainterManager */
    SELECTED_PARTICLE,       /**< selected particle color in QPainterManager */
    SELECTED_PARTICLE_LABEL, /**< selected particle label color in QPainterManager */
    EDIT_SELECTED_PARTICLE,  /**< selected and edited particle color in QPainterManager */
    BLUE_PARTICLE,           /**< blue particle type color in QPainterManager */
    RED_PARTICLE,            /**< red particle type color in QPainterManager */
    NORMAL_PARTICLE,         /**< normal particle type color in QPainterManager */
    GAS1_PARTICLE,           /**< gas1 particle type color in QPainterManager */
    GAS2_PARTICLE,           /**< gas2 particle type color in QPainterManager */
    GAS3_PARTICLE,           /**< gas3 particle type color in QPainterManager */
    MINI_PARTICLE,           /**< mini particle type color in QPainterManager */
    MACROSCOPIC_PARTICLE     /**< macroscopic particle type color in QPainterManager */
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

    std::map<BoxSheets,boxValue<QString>> sheets {
                                         { BoxSheets::BUTTON_SELECTED , {"QPushButton {background-color: rgb(145,105,125);font-size: 16px;font: bold}","buttonStyleSelected"} },
                                         { BoxSheets::BUTTON_UNSELECTED , {"QPushButton {background-color: rgb(200,200,200);font-size: 16px;}","buttonStyleUnselected"} },
                                         { BoxSheets::BUTTON_RESET , {"QPushButton {background-color: rgb(200,200,200);font-size: 16px;}","buttonStyleReset"} }
                                      };

    /** map of colors */
    std::map<BoxColors,boxValue<QColor>> colors {
                                                 { BoxColors::PLANE_BORDER , {{120,120,120},"cPlaneBorder"} },
                                                 { BoxColors::BACKGROUND , {{145,215,215},"cBackground"} },
                                                 { BoxColors::AXES , {{120,120,120},"cAxesColor"} },
                                                 { BoxColors::INNER_FRAME , {{100,100,100},"cInnerFrameColor"} },
                                                 { BoxColors::LABEL , {{100,20,20},"cLabelColor"} },
                                                 { BoxColors::VALUE , {{100,20,20},"cValue"} },
                                                 { BoxColors::BUTTON_LABEL , {{15,15,15},"cButtonLabel"} },
                                                 { BoxColors::UPPER , {{0,0,255},"cUpper"} },
                                                 { BoxColors::LOWER , {{255,0,0},"cLower"} },
                                                 { BoxColors::BIG_CIRCLE_PEN , {{80,80,90},"cBigCirclePenColor"} },
                                                 { BoxColors::SMALL_CIRCLE , {{155,155,155},"cSmallCircleColor"} },
                                                 { BoxColors::SMALL_CIRCLE_HOOKED , {{100,100,255},"cSmallCircleHookedColor"} },
                                                 { BoxColors::DIM_PARTCILE , {{200,200,200},"cDimParticle"} },
                                                 { BoxColors::CONSTRAINT_ARROW , {{100,60,250},"cConstraintArrow"} },
                                                 { BoxColors::PLANE_HIT , {{210,50,50},"cPlaneHit"} },
                                                 { BoxColors::SELECTED_PARTICLE , {{32,165,56},"cSelectedParticle"} },
                                                 { BoxColors::SELECTED_PARTICLE_LABEL , {{10,15,10},"cSelectedParticleLabel"} },
                                                 { BoxColors::EDIT_SELECTED_PARTICLE , {{150,150,145},"cEditSelectedParticle"} },
                                                 { BoxColors::BLUE_PARTICLE , {{0,0,255},"cBlueParticle"} },
                                                 { BoxColors::RED_PARTICLE , {{255,0,0},"cRedParticle"} },
                                                 { BoxColors::NORMAL_PARTICLE , {{255,0,255},"cNormalParticle"} },
                                                 { BoxColors::GAS1_PARTICLE , {{255,0,0},"cGas1Particle"} },
                                                 { BoxColors::GAS2_PARTICLE , {{0,85,80},"cGas2Particle"} },
                                                 { BoxColors::GAS3_PARTICLE , {{0,0,255},"cGas3Particle"} },
                                                 { BoxColors::MINI_PARTICLE , {{50,10,255},"cMiniParticle"} },
                                                 { BoxColors::MACROSCOPIC_PARTICLE , {{50,10,95},"cMacroscopicParticle"} },
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
    void load( const QDomElement& element, const std::string& tagName , T& value );

};

