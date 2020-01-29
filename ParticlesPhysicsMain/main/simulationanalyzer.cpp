#include "simulationanalyzer.h"


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

    histograms1D["velocityDistribution"]->fill(velocity);
    histograms1D["momentumDistribution"]->fill(velocity*particle->mass);
}

void SimulationAnalyzer::update()
{

    //if( simulationType == SimulationType::BROWNIAN_MOTION ) histograms1D["velocityDistribution"]->markBin( getMoleculeVelocity() );

    switch( simulationType )
    {

     case SimulationType::DIFFUSION :
        if( velocityCounter[ParticleType::BLUE]>0 ) physicsInfo.avgVelocityBlue = velocitySum[ParticleType::BLUE]/velocityCounter[ParticleType::BLUE];
        if( velocityCounter[ParticleType::RED]>0 ) physicsInfo.avgVelocityRed = velocitySum[ParticleType::RED]/velocityCounter[ParticleType::RED];
      break;

     default :
        double sum {0};
        int counter {0};
        for( auto& [type,value] : velocitySum )
        {
           sum += velocitySum[type];
           counter += velocityCounter[type];
        }
        if( counter>0 ) physicsInfo.avgVelocity = sum/counter;
      break;

    }

    for( auto& [type,value] : velocitySum )
    {
       velocitySum[type] = 0.0;
       velocityCounter[type] = 0;
    }

}

