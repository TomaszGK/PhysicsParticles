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

private:

    /** Holds Histogram1D object. */
    ptrHistogram1D histogram1D;

    QColor cBackground {145,215,215} ; /** histogram background color */
    QColor cLabelColor {100,20,20}   ; /** histogram label color */

    double barWidth      {0}   ; /** histogram bar width */
    double maxValue      {100} ; /** maximum bin value */
    int    labelPosition {0}   ; /** histogram label position */

    /**
     * @brief Paints histogram.
     */
    void paint() override;

    /**
     * @brief Inits and calculates QHistogram1D state.
     */
    void init() override;

    /**
     * @brief Draws histogram name/label.
     */
    void drawHistogramName();

    /**
     * @brief Draws histogram marking line.
     */
    void drawMarking();

};
