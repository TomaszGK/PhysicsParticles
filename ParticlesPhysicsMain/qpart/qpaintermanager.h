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
     * @param parent                ponter to parent widget
     */
    explicit QPainterManager( QWidget *parent = nullptr );

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

    /**
     * @brief Loads style type from xml file and /to complete/.
     *
     * Loading new parameters (e.g. colors, style sheets) from file corresponding to a given style.
     * @param style                 box style
     * @return true if style has loaded correctly otherwise false
     */
    bool loadStyle( BoxStyles style ) override;

private:

    /**< particle pen */
    QPen particlePen;

    /**< particle color */
    QColor particleColor;

    /**< selected particle color */
    QColor selectedParticleColor;

    /**< mouse current position */
    QPoint mousePos;

    /**
     * Stores constant iterator to particle on which hovering mouse cursor.
     * If mouse cursor does not hover of any particles then
     * first pair value is set to false, otherwise is set to true.
     */
    std::pair<bool,std::vector<Particle>::const_iterator> displayVelocityVector;

    /** Holds shared pointer to constant vector of Particle objects. */
    cptrParticlesContainer particles {nullptr};

    /** Holds shared pointer to constant PlaneArea object. */
    cptrPlaneArea planeArea {nullptr};

    /** vector paint flag */
    bool toVectorPaint {false};

    /** tracking paint flag */
    bool toTrackingPaint {false};

    /** plane hits paint flag */
    bool toHandlePlaneHits {false};

    /** map of particle colors */
    std::map<ParticleType,QColor> color;

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


    /**
     * @brief Updates a given particle color.
     *
     * Updates color corresponding to particle velocity.
     * Using when @ref VisualizationType is set on VisualizationType::VELOCITY.
     *
     * @param particle              a given particle
     */
    void updateParticleColor( citerParticle particle );

};
