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
 * Paints bar display using QT framework and BarDisplay class for data manipulation.
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
     * @param parent                ponter to parent widget
     */
    explicit QBarDisplay( MeasurementType type, QWidget* parentWidget = nullptr );

    /**
     * @brief Loads style type from xml file and /to complete/.
     *
     * Loading new parameters (e.g. colors, style sheets) from file corresponding to a given style.
     * @param style                 box style
     * @return true if style has loaded correctly otherwise false
     */
    bool loadStyle( BoxStyles style ) override;

private:    

    /** Holds BarDisplay object. */
    cptrBarDisplay barDisplay;

    /** single bar width */
    int barWidth {0};   

    /**
     * @brief Paints bar charts.
     */
    void paint() override;

    /**
     * @brief Inits and calculates QBarDisplay state.
     */
    void init() override;

};
