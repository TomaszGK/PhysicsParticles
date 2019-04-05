#pragma once

#include "definitions.h"


class Particle
{    

public:

    Particle( ParticleType _particleType, VisualizationType _visualizationType, vect2D _position, vect2D _velocity, double _maxRapidity, int _size, iterCluster _cluster );
    Particle( const Particle& ) = default;
    Particle( Particle&& ) = default;

    Particle& operator=( const Particle& ) = default;
    Particle& operator=( Particle&& ) = default;

    inline vect2D calculateNextPosition( const double& time = 1.0 ) const { return position+(velocity*time); }

    void setParticleSize( int size );

    void setParticleMassInPercent( int percent );

    void calculateParticleColor();

    void updateParticleColor();

    void savePosition();

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

    inline int getCurrentVelocityPercent() const { return static_cast<int>(100*velocity()/maxRapidity); }

    // stores last positions of particle
    std::list<vect2D> particlePositionsTracking;

    // maximum positions in particlePositionTracking list
    size_t maxPositionsInTracking {200};

    ParticleType      particleType {ParticleType::NORMAL};
    VisualizationType visualizationType {VisualizationType::VELOCITY};

    vect2D      position {0.0,0.0};
    vect2D      velocity {0.0,0.0};
    double      mass {0};
    double      maxRapidity {1.0};
    int         size {0};
    double      radius {0};
    colorRGB    color;
    iterCluster cluster;

    bool modifiedVelocity {false};
    bool isTracking       {false}; // true if position of particle is tracking
    bool isMacroscopic    {false}; // true if particle is macroscopic

};
