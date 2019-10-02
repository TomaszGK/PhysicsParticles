#pragma once

#include "qboxpainter.h"
#include "planearea.h"

/** @file
 * @brief Class @ref QPainterManager
 */

/**
 * @class QPainterManager
 * @brief Implements visualization of the particle plane.
 *
 * Paints particles, plane border, particle velocity vector, trace of tracking particle,
 * plane constraint walls with arrows, plane divider and particle hits to the plane.
 * @author Tomasz Gburek
 * @date 2019
 */

class QPainterManager : public QBoxPainter
{

    Q_OBJECT

public:

    /**
     * @brief Constructor
     *
     * @param ptrParticles          shared pointer to constant vector of Particle objects
     * @param ptrPlaneArea          shared pointer to constant PlaneArea object
     * @param parent                ponter to parent widget
     */
    explicit QPainterManager( cptrParticlesContainer ptrParticles, std::shared_ptr<const PlaneArea> ptrPlaneArea, QWidget *parent = nullptr );

    /**
     * @brief Sets a new shared pointer to constant vector of Particle objects
     *
     * @param ptrParticles          shared pointer to constant vector of Particle objects
     */
    void setParticlesContainer( const cptrParticlesContainer& ptrParticles ) { particles = ptrParticles; }

    /**
     * @brief Sets vector paint flag.
     *
     * @param _toVectorPaint        new vector paint flag
     */
    void setVectorPaint( bool _toVectorPaint ) noexcept { toVectorPaint = _toVectorPaint; }

    /**
     * @brief Sets tracking paint flag.
     *
     * @param _toTrackingPaint      new tracking paint flag
     */
    void setTrackingPaint( bool _toTrackingPaint ) noexcept { toTrackingPaint = _toTrackingPaint; }

    /**
     * @brief Sets plane hits flag.
     *
     * @param _toTrackingPaint      new plane hits flag
     */
    void setPlaneHitsPaint( bool _toHandlePlaneHits ) noexcept { toHandlePlaneHits = _toHandlePlaneHits; }

private:

    QPen     particlePen           ; /**< particle pen */
    QColor   particleColor         ; /**< particle color */
    QColor   selectedParticleColor ; /**< selected particle color */

    QPoint   mousePos ; /**< mouse current position */

    /**
     * Stores constant iterator to particle on which hovering mouse cursor.
     * If mouse cursor does not hover of any particles then
     * first pair value is set to false, otherwise is set to true.
     */
    std::pair<bool,std::vector<Particle>::const_iterator> displayVelocityVector;

    /** Holds shared pointer to constant vector of Particle objects. */
    cptrParticlesContainer particles;

    /** Holds shared pointer to constant PlaneArea object. */
    std::shared_ptr<const PlaneArea> planeArea;   

    bool toVectorPaint     {false} ; /**< vector paint flag */
    bool toTrackingPaint   {false} ; /**< tracking paint flag */
    bool toHandlePlaneHits {false} ; /**< plane hits paint flag */

    /**
     * @brief Paints the particle plane.
     *
     * Paint particles nad plane border but may paints also particle velocity vector, trace of tracking particle
     * and particle hits to the plane which is depended on toVectorPaint, toTrackingPaint and toHandlePlaneHits flags.
     */
    void paint() override;

    /**
     * @brief Gets plane border width.
     */
    void init() override;

    /**
     * @brief Paints plane constraint walls along horizontal axis.
     */
    void paintPlaneConstraintWalls();

    /**
     * @brief Paints constraint arrows along horizontal axis.
     */
    void paintConstraintArrows();

    /**
     * @brief Paints constraint arrows along horizontal axis.
     */
    void paintPlaneDivider();

    /**
     * @brief Paints plane hit for a given particle.
     *
     * @param particle              a given particle that hits the plane
     */
    void paintPlaneHit( citerParticle particle );

    /**
     * @brief Paints particle inside the plane
     *
     * @param posx                  particle position along X axis
     * @param posx                  particle position along Y axis
     * @param size                  particle size
     * @param color                 particle color
     */
    inline void paintParticle( int posx , int posy , int size , QColor color );

    /**
     * @brief Paints trace of a given particle.
     *
     * @param particle              a given particle that is tracking
     */
    void paintTracking( citerParticle particle );

    /**
     * @brief Handles by checking if mouse cursor position is hovering on some particle.
     */
    void handleCursorPosition();   

};
