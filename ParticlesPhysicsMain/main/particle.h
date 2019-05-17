#pragma once

#include "definitions.h"

/** @file
 * @brief Class @ref Particle
 */

/**
 * @class Particle
 * @brief Implements 2D particle.
 *
 * Particle is a two dimentional circle shaped object having physical properties like
 * position, velocity, size, radius and mass. Each particle provides information about belonging cluster,
 * visualization type and own color.
 * @author Tomasz Gburek
 * @date 2019
 */

class Particle
{    

public:

    /**
     * @brief Constructor
     *
     * @param particleType      particle type
     * @param visualizationType visualization type
     * @param position          particle position
     * @param velocity          particle velocity
     * @param maxRapidity       particle maximum velocity
     * @param size              particle size = 2*radius
     * @param cluster           cluster  where particle is located
     */
    Particle( ParticleType particleType, VisualizationType visualizationType, vect2D position, vect2D velocity, double maxRapidity, int size, iterCluster cluster );

    /**
     * @brief Calculates the next particle position.
     *
     * Calculates the next position adding velocity multipling by time period.
     * @param time time period
     * @return next particle position
     */
    inline vect2D calculateNextPosition( const double& time = 1.0 ) const { return position+(velocity*time); }

    /**
     * @brief Set particle size.
     * @param size particle size
     */
    void setParticleSize( int size );

    /**
     * @brief Set particle mass in percent value.
     * @param percent new particle mass in percent value
     */
    void setParticleMassInPercent( int percent );

    /**
     * @brief Calculates particle color.
     *
     * Particle color depends on @ref VisualizationType and @ref ParticleType.
     */
    void calculateParticleColor();

    /**
     * @brief Updates particle color.
     *
     * Updates color corresponding to particle velocity.
     * Using when @ref VisualizationType is set on VisualizationType::VELOCITY.
     */
    void updateParticleColor();

    /**
     * @brief Save current particle position.
     *
     * Current particle position is saved in the list particlePositionsTracking using push_back().
     * If size of particlePositionsTracking excessing maxPositionsInTracking then
     * last saved particle position is removed from the list by using pop_front().
     */
    void savePosition();

    /**
     * @brief Moves to the next particle position.
     *
     * Calculates next particle position and assign it to the current particle position.
     * Updates paarticle color if @ref VisualizationType is set on VisualizationType::VELOCITY.
     * Save particle position if isTracking flag set to true.
     * Set modifiedVelocity flag to true.
     * @param time time period
     */
    void moveToNextPosition( const double& time = 1.0 )
    {
        if( visualizationType == VisualizationType::VELOCITY && modifiedVelocity ) updateParticleColor();
        position += (velocity*time);
        modifiedVelocity = false;
        if( isTracking )
        {
            savePosition();
        }      
    }

    /**
     * @brief Gets current particle velocity in percent value with respect to maximum particle rapidity.
     * @return current particle velocity in percent value
     */
    inline int getCurrentVelocityPercent() const { return static_cast<int>(100*velocity()/maxRapidity); }


    std::list<vect2D> particlePositionsTracking; /**< stores particle saved positions */

    size_t maxPositionsInTracking {200}; /**< maximum saved positions in particlePositionTracking list */

    ParticleType      particleType {ParticleType::NORMAL}             ; /**< particle type @ref ParticleType */
    VisualizationType visualizationType {VisualizationType::VELOCITY} ; /**< visualization type @ref VisualizationType */

    vect2D      position    {0.0,0.0} ; /**< particle position */
    vect2D      velocity    {0.0,0.0} ; /**< particle velocity */
    double      mass        {0}       ; /**< particle mass */
    double      maxRapidity {1.0}     ; /**< particle maximum rapidity */
    int         size        {0}       ; /**< particle size */
    double      radius      {0}       ; /**< particle radius */
    colorRGB    color                 ; /**< particle color */
    iterCluster cluster               ; /**< particle cluster */

    bool modifiedVelocity {false} ; /**< true if particle velocity have been modified */
    bool isTracking       {false} ; /**< true if position of particle is tracking */
    bool isMacroscopic    {false} ; /**< true if particle is macroscopic */

};
