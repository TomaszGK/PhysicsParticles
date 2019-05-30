#pragma once

#include "qboxpainter.h"
#include "bardisplay.h"

/** @file
 * @brief Class @ref QBarDisplay
 */

/**
 * @class QBarDisplay
 * @brief Implements visualization of bar display in QT.
 *
 * Paints bar display using QT framework and BarDisplay class for data manimulation.
 * @author Tomasz Gburek
 * @date 2019
 */

class QBarDisplay : public QBoxPainter
{

    Q_OBJECT

public:

    /**
     * @brief Constructor
     *
     * @param max                   maximum value in bin
     * @param ptr                   pointer to BarDisplay object
     * @param parent                ponter to parent widget
     */
    explicit QBarDisplay( int max = 100, ptrBarDisplay ptr = nullptr, QWidget* parentWidget = nullptr );

private:    

    /** Holds BarDisplay object. */
    ptrBarDisplay barDisplay;

    int    barWidth    {0} ; /**< single bar width */
    int    max       {100} ; /**< maximum bin balue */
    QColor upper {0,0,255} ; /**< upper bins color */
    QColor lower {255,0,0} ; /**< lower bins color */

    /**
     * @brief Paints bar charts.
     */
    void paint() override;

    /**
     * @brief Inits and calculates QBarDisplay state.
     */
    void init() override;

    /**
     * @brief Gets centered text position in a single bar.
     *
     * @param number                number to display in bar
     * @return centered position
     */
    int  getCenteredTextPosition( int number );
};
