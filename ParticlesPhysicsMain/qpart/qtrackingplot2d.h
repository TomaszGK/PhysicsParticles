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
     * @param ptrParticles          shared pointer to constant vector of Particle objects
     * @param parent                ponter to parent widget
     */
    explicit QTrackingPlot2D( cptrParticlesContainer ptrParticles, QWidget *parent = nullptr );

private:

    QPen plotPen ; /**< plotting pen */

    /** Holds shared pointer to constant vector of Particle objects. */
    cptrParticlesContainer particles;

    /** Holds constant iterator to tracking particle. */
    citerParticle trackingParticle;

    /**
     * @brief Paints 2D plot.
     */
    void paint() override;

    /**
     * @brief Inits the state of QTrackingPlot2D.
     */
    void init() override;

};
