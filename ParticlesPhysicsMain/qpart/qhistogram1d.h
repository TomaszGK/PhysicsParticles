#pragma once

#include "qboxpainter.h"
#include "histogram1d.h"

/** @file
 * @brief Class @ref QHistogram1D
 */

/**
 * @class QHistogram1D
 * @brief Implements visualization of 1D histogram in QT.
 *
 * Paints 1D histogram using QT framework and Histogram1D class for data manimulation.
 * @author Tomasz Gburek
 * @date 2019
 */

class QHistogram1D : public QBoxPainter
{

    Q_OBJECT

public:

    /**
     * @brief Constructor
     *
     * @param max                   maximum bin value
     * @param ptr                   pointer to Histogram1D object
     * @param parent                ponter to parent widget
     */
    explicit QHistogram1D( double maxValue = 100, ptrHistogram1D ptr = nullptr, QWidget* parent = nullptr );

    /**
     * @brief Loads style type from xml file and /to complete/.
     *
     * Loading new parameters (e.g. colors, style sheets) from file corresponding to a given style.
     * @param style                 box style
     * @return true if style has loaded correctly otherwise false
     */
    bool loadStyle( BoxStyles style ) override;

private:

    /** Holds Histogram1D object. */
    ptrHistogram1D histogram1D;

    /** histogram bar width */
    double barWidth {0};

    /** maximum bin value */
    double maxValue {100};

    /** histogram label position */
    int labelPosition {0};

    /**
     * @brief Paints histogram.
     */
    void paint() override;

    /**
     * @brief Inits and calculates QHistogram1D state.
     */
    void init() override;

    /**
     * @brief Calculates label position.
     */
    int calculateLabelPosition();

    /**
     * @brief Draws histogram name/label.
     */
    void drawHistogramName();

    /**
     * @brief Draws histogram marking line.
     */
    void drawMarking();

};
