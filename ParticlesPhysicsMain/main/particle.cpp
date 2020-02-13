#include "particle.h"

Particle::Particle( ParticleType particleType, VisualizationType visualizationType, vect2D position, vect2D velocity, int size, iterCluster cluster )
: particleType {particleType}, visualizationType {visualizationType}, position {position}, velocity {velocity},
  size {size}, cluster {cluster}
{
    radius = static_cast<double>(size)*0.5;
    setParticleMassInPercent(100);
    if( particleType == ParticleType::MACROSCOPIC )
    {
        isTracking = isMacroscopic = true;
    }    
}

void Particle::setParticleSize( int newSize )
{
    size = newSize;
    mass = 4.0/3.0*3.141592*newSize*newSize*newSize*0.001;
    radius = static_cast<double>(newSize)*0.5;
}

void Particle::setParticleMassInPercent( int percent )
{
    Ensures( percent>=0 );
    mass = (4.0/3.0*3.141592*size*size*size*0.001)*percent*0.01;
}


void Particle::savePosition()
{
    if( !particlePositionsTracking.empty() )
    {
        if( (*prev(particlePositionsTracking.end())-position)()<=radius ) return;
    }

    if( particlePositionsTracking.size()>maxPositionsInTracking )
    {
        particlePositionsTracking.pop_front();
    }

    particlePositionsTracking.push_back(position);
}
