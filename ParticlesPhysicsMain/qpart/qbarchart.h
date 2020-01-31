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
    LINES  , /**< painting lines */
};

class QBarChart : public QBoxPainter
{

    Q_OBJECT

public:

    /**
     * @brief Constructor
     *
     * @param type                  measurement type
     * @param max                   maximum barchart bin values     
     * @param parent                ponter to parent widget
     */
    explicit QBarChart( MeasurementType type, double max = 1, QWidget* parent = nullptr );

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
     * Recalculate box display values by adjusting marigins and align bar width.
     * @param style                 box style
     * @return true if style has loaded correctly otherwise false
     */
    bool loadStyle( BoxStyles style ) override;

protected:

    /**
     * @brief Handles event changing signal.
     *
     * @param event                 event to handle
     */
    void changeEvent( QEvent *event ) override;

private:

    /** Holds BarChart object. */
    ptrBarChart barChart;

    /** Maps buttons changing state of data visualization. */
    std::map<DataVisualization,std::unique_ptr<QPushButton>> buttons;

    /** Resets bar chart bin. */
    std::unique_ptr<QPushButton> resetButton;

    /** Data visuzalization type */
    DataVisualization dataVisulization { DataVisualization::BARS };

    /** single bar width */
    int barWidth {0};

    /** maximum bin value */
    double maxValue {1};

    /** label position */
    int labelPosition {0};

    /**< barchart name - Qt version string of label */
    QString qLabel {""};

    /**< barchart label for axis X - Qt version string of labelX */
    QString qLabelX {""};

    /**< barchart label for axis Y - Qt version string of labelY */
    QString qLabelY {""};

    /**
     * @brief Paints bar charts.
     */
    void paint() override;

    /**
     * @brief Inits and calculates QBarChart state.
     */
    void init() override;

    /**
     * @brief Draws current bar value value.
     */
    void drawCurrentValue();

    /**
     * @brief Draws name of QBarChart and X(Y) axis names.
     */
    void drawChartLabels();

    /**
     * @brief Reconfigurates (adjusts) left-right box margins and bar width.
     */
    void reconfigurateBarChartLayout();

    /**
     * @brief Configuring button position, size and style.
     */
    void configureButtons();

private slots:

    /**
     * @brief Handles button click event.
     */
    void onButtonClick();

};
