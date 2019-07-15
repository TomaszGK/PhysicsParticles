#pragma once

#include <QPushButton>
#include "qboxpainter.h"
#include "barchart.h"

/** @file
 * @brief Class @ref QBarChart
 */

/**
 * @class QBarChart
 * @brief Implements bar charts visualization in QT.
 *
 * Paints bar charts using QT framework and BarChart class for data manimulation.
 * @author Tomasz Gburek
 * @date 2019
 */

/** @enum DataVisualization
 *  @brief Represents bar chart visualization type.
 */
enum class DataVisualization
{
    BARS   , /**< painting bars */
    POINTS , /**< painting points */
    LINES    /**< painting lines */
};

class QBarChart : public QBoxPainter
{

    Q_OBJECT

public:

    /**
     * @brief Constructor
     *
     * @param max                   maximum bin value
     * @param scalability           scalability mode
     * @param ptr                   pointer to BarChart object
     * @param parent                ponter to parent widget
     */
    explicit QBarChart( double max = 1, std::pair<bool,bool> scalability = {true,true}, ptrBarChart ptr = nullptr, QWidget* parent = nullptr );

    /**
     * @brief Sets maximum bin value.
     *
     * @param max                   maximum bin value
     */
    void setMaxOY( double max ) { maxValue = max*1.6; }

    /**
     * @brief Sets scalability mode.
     *
     * @param _isScalableUp         scalable up flag
     * @param _isScalableDown       scalable down flag
     */
    void setScalabality( bool _isScalableUp , bool _isScalableDown ) { isScalableUp = _isScalableUp ; isScalableDown = _isScalableDown; }

private:

    /** Holds BarChart object. */
    ptrBarChart barChart;

    /** Maps buttons changing state of data visualization. */
    std::map<DataVisualization,std::unique_ptr<QPushButton>> buttons;

    /** Data visuzalization type */
    DataVisualization dataVisulization { DataVisualization::BARS };

    QString buttonStyleSelected   {""}; /** holds QT style sheet for selected button */
    QString buttonStyleUnselected {""}; /** holds QT style sheet for unselected button */

    QColor cValue        {100,20,20}   ; /** current value color */
    QColor cBackground   {145,215,215} ; /** background color */
    QColor cButtonActive {145,105,125} ; /** active button color */
    QColor cButton       {200,200,200} ; /** button color */
    QColor cButtonLabel  {15,15,15}    ; /** button label color */

    int    barWidth       {0}    ; /** single bar width */
    double maxValue       {1}    ; /** maximum bin value */
    bool   isScalableUp   {true} ; /** scalable up flag - if true then maxValue can increase */
    bool   isScalableDown {true} ; /** scalable down flag - if true then maxValue can decrease */
    int    labelPosition  {0}    ; /** label position */

    /**
     * @brief Paints bar charts.
     */
    void paint() override;

    /**
     * @brief Inits and calculates QBarChart state.
     */
    void init() override;

    /**
     * @brief Calculates label position.
     */
    int calculateLabelPosition();

    /**
     * @brief Draws current bar value value.
     */
    void drawCurrentValue();

    /**
     * @brief Draws QBarChart name.
     */
    void drawChartName();

private slots:

    /**
     * @brief Handles button click event.
     */
    void onButtonClick();

};
