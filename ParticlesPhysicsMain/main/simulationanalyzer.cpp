#include "simulationanalyzer.h"
#include "particlesphysicsmanager.h"


void SimulationAnalyzer::resetPhysicsData()
{
    physicsInfo = PhysicsInfo();
}

void SimulationAnalyzer::collect( double kineticEnergy )
{
    if( kineticEnergy>0 ) ++physicsInfo.numOfCollision;
    physicsInfo.kineticEnergySum += kineticEnergy;
}

void SimulationAnalyzer::collect( iterParticle particle )
{
    auto velocity = particle->velocity();
    velocitySum[particle->particleType] += velocity;
    ++velocityCounter[particle->particleType];

    (*histograms1D)[ActionType::M_VELOCITY_DIST]->fill(velocity);
    (*histograms1D)[ActionType::M_MOMENTUM_DIST]->fill(velocity*particle->mass);
}

void SimulationAnalyzer::update()
{

    switch( simulationType )
    {

     case SimulationType::DIFFUSION :
        if( velocityCounter[ParticleType::BLUE]>0 ) physicsInfo.avgVelocityBlue = velocitySum[ParticleType::BLUE]/velocityCounter[ParticleType::BLUE];
        if( velocityCounter[ParticleType::RED]>0 ) physicsInfo.avgVelocityRed = velocitySum[ParticleType::RED]/velocityCounter[ParticleType::RED];
     break;

     case SimulationType::BROWNIAN_MOTION :
        (*histograms1D)[ActionType::M_VELOCITY_DIST]->markBin( ParticlesPhysicsManager::Locator::getParticles()->begin()->velocity() );
     [[fallthrough]];

     default :
        double sum {0};
        int counter {0};
        for( auto& [type,value] : velocitySum )
        {
           sum += value;
           counter += velocityCounter[type];
        }
        if( counter>0 ) physicsInfo.avgVelocity = sum/counter;
     break;

    }

    for( auto& [type,value] : velocitySum )
    {
       value = 0.0;
       velocityCounter[type] = 0;
    }

}

