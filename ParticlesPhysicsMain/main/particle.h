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
     * @param size              particle size = 2*radius
     * @param cluster           cluster  where particle is located
     */
    Particle( ParticleType particleType, VisualizationType visualizationType, vect2D position, vect2D velocity, int size, iterCluster cluster );

    /**
     * @brief Calculates and returns the next particle position.
     *
     * Calculates the next position by adding velocity multipling by a given period of time.
     * @param time time period
     * @return next particle position
     */
    vect2D calculateNextPosition( const double& time = 1.0 ) const { return position+(velocity*time); }

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
     * @brief Save current particle position.
     *
     * Current particle position is saved in the list particlePositionsTracking using push_back().
     * If size of particlePositionsTracking excessing maxPositionsInTracking then
     * last saved particle position is removed from the list by using pop_front().
     */
    void savePosition();

    /**
     * @brief Adds velocity.
     *
     * @param newVelocity       a given velocity
     */
    void addVelocity( const vect2D& newVelocity )
    {
        velocity += newVelocity;
        if( velocity() > 1.0 ) velocity.setLength(1.0);
    }

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
        position += (velocity*time);
        modifiedVelocity = false;
        if( isTracking )
        {
            savePosition();
        }      
    }

    /** stores particle saved positions */
    std::list<vect2D> particlePositionsTracking;

    /** maximum saved positions in particlePositionTracking list */
    size_t maxPositionsInTracking {200};

    /** particle type @ref ParticleType */
    ParticleType particleType {ParticleType::NORMAL};

    /** visualization type @ref VisualizationType */
    VisualizationType visualizationType {VisualizationType::VELOCITY};

    /** particle position */
    vect2D position {0.0,0.0};

    /** particle velocity */
    vect2D velocity {0.0,0.0};

    /** particle mass */
    double mass {0};

    /** particle size */
    int size {0};

    /** particle radius */
    double radius {0};

    /** particle color */
    colorRGB color;

    /** particle cluster */
    iterCluster cluster;

    /** true if particle velocity have been modified */
    bool modifiedVelocity {false};

    /** true if position of particle is tracking */
    bool isTracking {false};

    /** true if particle is macroscopic */
    bool isMacroscopic {false};  

};
