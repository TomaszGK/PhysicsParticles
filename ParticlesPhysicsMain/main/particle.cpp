#include "particle.h"

Particle::Particle( ParticleType _particleType, VisualizationType _visualizationType, vect2D _position, vect2D _velocity, double _maxRapidity, int _size, iterCluster _cluster )
: particleType {_particleType}, visualizationType {_visualizationType}, position {_position}, velocity {_velocity},
  maxRapidity {_maxRapidity}, size {_size}, cluster {_cluster}
{
    radius = static_cast<double>(size)*0.5;
    setParticleMassInPercent(100);
    if( _particleType == ParticleType::MACROSCOPIC )
    {
        isTracking = isMacroscopic = true;
    }
    calculateParticleColor();
}

void Particle::setParticleSize( int newSize )
{
    size = newSize;
    mass = 4/3*3.141592*newSize*newSize*newSize*0.001;
    radius = static_cast<double>(newSize)*0.5;
}

void Particle::setParticleMassInPercent( int percent )
{
    mass = (4/3*3.141592*size*size*size*0.001)*percent*0.01;
}

void Particle::calculateParticleColor()
{
    if( visualizationType == VisualizationType::VELOCITY ) updateParticleColor();
    else
    {
        if( particleType == ParticleType::BLUE             ) color = {0,0,255};
        else if( particleType == ParticleType::RED         ) color = {255,0,0};
        else if( particleType == ParticleType::NORMAL      ) color = {255,0,255};
        else if( particleType == ParticleType::GAS1        ) color = {255,0,0};
        else if( particleType == ParticleType::GAS2        ) color = {0,85,80};
        else if( particleType == ParticleType::GAS3        ) color = {0,0,255};
        else if( particleType == ParticleType::MINI        ) color = {50,10,255};
        else if( particleType == ParticleType::MACROSCOPIC ) color = {50,10,95};
    }
}

void Particle::updateParticleColor()
{
    int intensity = static_cast<int>(255.0*velocity()/maxRapidity);
    if( intensity>255 ) intensity = 255;
    color.R = intensity;
    color.G = 0;
    color.B = 255-intensity;
}

void Particle::savePosition()
{
    if( particlePositionsTracking.size()>maxPositionsInTracking ) particlePositionsTracking.pop_front();
    if( particlePositionsTracking.size()>0 )
    {
        if( (*prev(particlePositionsTracking.end())-position)()>radius ) particlePositionsTracking.push_back(position);
    }
    else
    {
        particlePositionsTracking.push_back(position);
    }
}
