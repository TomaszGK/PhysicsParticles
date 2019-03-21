#pragma once

#include "definitions.h"


class Particle
{    

    bool isMacroscopic {false}; // true if is macroscopic - may have large size

public:

    Particle( ParticleType,VisualizationType,vect2D,vect2D,double,int,iterCluster );
    Particle( const Particle& ) = default;
    Particle( Particle&& ) = default;

    Particle& operator=( const Particle& ) = default;
    Particle& operator=( Particle&& ) = default;

    inline vect2D calculateNextPosition( const double& time = 1.0 ) const { return position+(velocity*time); }

    void setParticleSize( int size );

    void setParticleMassInPercent( int percent );

    void calculateColor();

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

    void updateParticleColor();

    void savePosition();

    // stores last positions of particle
    std::list<vect2D> particlePositionsTracking;

    // maximum positions in particlePositionTracking list
    size_t maxPositionsInTracking {200};

    ParticleType      particleType {ParticleType::NORMAL};
    VisualizationType visualizationType {VisualizationType::VELOCITY};

    vect2D       position {0.0,0.0};
    vect2D       velocity {0.0,0.0};
    double       mass {0};
    double       maxRapidity {1};
    int          size {0};
    double       radius {0};
    colorRGB     color;
    iterCluster  cluster;

    bool modifiedVelocity {false};
    bool isTracking {false}; // true if position of particle is tracking

    inline int getCurrentVelocityPercent() const { return static_cast<int>(100*velocity()/maxRapidity); }

    inline bool IsMacroscopic() const noexcept { return isMacroscopic; }

    void setMacroscopic( bool flag )
    {
        isMacroscopic = flag;
        if( flag ) color = {50,10,95};
        else calculateColor();
    }

};
