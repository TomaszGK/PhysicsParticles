#pragma once

#include "qboxpainter.h"
#include "planearea.h"

/** @file
 * @brief Class @ref QPainterManager
 */

/** @enum PaintMode
 *  @brief Represents painting mode.
 */
enum class PaintMode
{
   TRACKING,         /**< paint tracking particle */
   PLANE_CONSTRAINT, /**< paint plane wall constraints */
   DIVIDER,          /**< paint plane divider */
   PLANE_HITS,       /**< paint plane hits */
   VECTOR,           /**< paint velocity vector of selected particle */
   EDIT              /**< paint selected particle in edit mode */
};

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
     * @param parent                pointer to parent widget
     */
    explicit QPainterManager( QWidget *parent = nullptr );

    /**
     * @brief Overload operator[]
     *
     * @param mode                  painting mode
     * @return referenced value of a given painting mode
     */
    bool& operator []( PaintMode mode ){ return paintMode[mode]; }

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

    /** Map of painting modes. */
    std::map<PaintMode,bool> paintMode {
                                        { PaintMode::TRACKING,false },
                                        { PaintMode::PLANE_CONSTRAINT,false },
                                        { PaintMode::DIVIDER,false },
                                        { PaintMode::PLANE_HITS,false },
                                        { PaintMode::VECTOR,false },
                                        { PaintMode::EDIT,false }
                                       };

    /** translation map from ParticleType to BoxColors */
    inline static const std::map< const ParticleType , const BoxColors > translation {
                                                   {ParticleType::BLUE,BoxColors::BLUE_PARTICLE},
                                                   {ParticleType::RED,BoxColors::RED_PARTICLE},
                                                   {ParticleType::NORMAL,BoxColors::NORMAL_PARTICLE},
                                                   {ParticleType::GAS1,BoxColors::GAS1_PARTICLE},
                                                   {ParticleType::GAS2,BoxColors::GAS2_PARTICLE},
                                                   {ParticleType::GAS3,BoxColors::GAS3_PARTICLE},
                                                   {ParticleType::MINI,BoxColors::MINI_PARTICLE},
                                                   {ParticleType::MACROSCOPIC,BoxColors::MACROSCOPIC_PARTICLE},
                                                 };

    /**
     * @brief Paints the particle plane.
     *
     * Paint particles nad plane border but may paints also particle velocity vector, trace of tracking particle
     * and particle hits to the plane which is depended on paintMode map.
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
     * @brief Paints particle
     *
     * @param posx                  particle position along X axis
     * @param posx                  particle position along Y axis
     * @param size                  particle size
     * @param color                 particle color
     */
    inline void paintParticle( int posx , int posy , int size , const QColor& color );

    /**
     * @brief Paints particle using its iterartor
     *
     * @param particle              particle iterator
     * @param color                 particle color
     */
    inline void paintParticle( citerParticle particle , const QColor& color );

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
     * @brief Attaches color for a given particle.
     *
     * Using color corresponding to particle velocity when @ref VisualizationType
     * is set on VisualizationType::VELOCITY. Otherwise using colors defined in box style.
     *
     * @param particle              particle iterator    
     */
    void attachParticleColor( citerParticle particle );

};
