#pragma once

#include "qboxpainter.h"
#include "particle.h"

/** @file
 * @brief Class @ref QTrackingPlot2D
 */

/**
 * @class QTrackingPlot2D
 * @brief Implements visualization of two dimentional plot in QT.
 *
 * Paints 2D plot of tracking particle using QT paint box.
 * @author Tomasz Gburek
 * @date 2019
 */

class QTrackingPlot2D: public QBoxPainter
{

    Q_OBJECT

public:

    /**
     * @brief Constructor
     *     
     * @param parent                ponter to parent widget
     */
    explicit QTrackingPlot2D( QString _title = "", QWidget *parent = nullptr );

    /**
     * @brief Loads style type from xml file and /to complete/.
     *
     * Loading new parameters (e.g. colors, style sheets) from file corresponding to a given style.
     * @param style                 box style
     * @return true if style has loaded correctly otherwise false
     */
    bool loadStyle( BoxStyles style ) override;

private:

    QPen plotPen ; /**< plotting pen */

    /** Holds shared pointer to constant vector of Particle objects. */
    cptrParticlesContainer particles;

    /** Holds constant iterator to tracking particle. */
    citerParticle trackingParticle;

    /** Tracking plot title. */
    QString title;

    /**
     * @brief Paints 2D plot.
     */
    void paint() override;

    /**
     * @brief Inits the state of QTrackingPlot2D.
     */
    void init() override;

};
