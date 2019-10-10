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
    explicit QBarChart( double max = 1, ptrBarChart ptr = nullptr, QWidget* parent = nullptr );

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
    void setScalabality( bool isScalableUp , bool isScalableDown ) { boxStyle.isScalableUp = isScalableUp ; boxStyle.isScalableDown = isScalableDown; }

    /**
     * @brief Loads style type from xml file and recalulate box display values.
     *
     * Loading new parameters (e.g. colors, style sheets) from file corresponding to a given style.
     * Recalculate box display values by adjusting marigins and bar width.
     * @param style                 box style
     * @return true if style has loaded correctly otherwise false
     */
    bool loadStyle( BoxStyles style ) override;

private:

    /** Holds BarChart object. */
    ptrBarChart barChart;

    /** Maps buttons changing state of data visualization. */
    std::map<DataVisualization,std::unique_ptr<QPushButton>> buttons;

    /** Data visuzalization type */
    DataVisualization dataVisulization { DataVisualization::BARS };

    /** single bar width */
    int barWidth {0};

    /** maximum bin value */
    double maxValue {1};

    /** label position */
    int labelPosition {0};

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

    /**
     * @brief Adjusting left-right box margins and bar width.
     */
    void adjustBoxDisplayValues();

private slots:

    /**
     * @brief Handles button click event.
     */
    void onButtonClick();

};
