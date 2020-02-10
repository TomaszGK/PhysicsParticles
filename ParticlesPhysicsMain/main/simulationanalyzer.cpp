#include "simulationanalyzer.h"


SimulationAnalyzer::SimulationAnalyzer()
{
    barDisplays = std::make_shared<MapBarDisplay>();
    barCharts = std::make_shared<MapBarChart>();
    histograms1D = std::make_shared<MapHistogram1D>();

    (*barCharts)[ActionType::M_VELOCITY]      = std::make_shared<BarChart>( 80 , "Average Velocity of Gas Particles" , "Time" , "V" );
    (*barCharts)[ActionType::M_VELOCITY_BLUE] = std::make_shared<BarChart>( 70 , "Average Velocity of Blue Gas Particles" , "Time" , "V" );
    (*barCharts)[ActionType::M_VELOCITY_RED]  = std::make_shared<BarChart>( 70 , "Average Velocity of Red Gas Particles" , "Time" , "V" );
    (*barCharts)[ActionType::M_COLLISIONS]    = std::make_shared<BarChart>( 40 , "Collisions" );
    (*barCharts)[ActionType::M_KINETIC]       = std::make_shared<BarChart>( 160 , "Energy of Particle Hits" , "Time" , "E" );

    (*barDisplays)[ActionType::M_DIFFIUSION] = std::make_shared<BarDisplay>(2);

    (*histograms1D)[ActionType::M_VELOCITY_DIST] = std::make_shared<Histogram1D>( 80 , physicsInfo.minRapidity , physicsInfo.maxRapidity*2.0 , "Velocity Distribution" );
    (*histograms1D)[ActionType::M_MOMENTUM_DIST] = std::make_shared<Histogram1D>( 80 , physicsInfo.minRapidity , physicsInfo.maxRapidity*2.0 , "Momentum Distribution" );
}

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

void SimulationAnalyzer::update( SimulationType simulationType )
{

    switch( simulationType )
    {

     case SimulationType::DIFFUSION :
        if( velocityCounter[ParticleType::BLUE]>0 ) physicsInfo.avgVelocityBlue = velocitySum[ParticleType::BLUE]/velocityCounter[ParticleType::BLUE];
        if( velocityCounter[ParticleType::RED]>0 ) physicsInfo.avgVelocityRed = velocitySum[ParticleType::RED]/velocityCounter[ParticleType::RED];
     break;

     case SimulationType::BROWNIAN_MOTION :
        (*histograms1D)[ActionType::M_VELOCITY_DIST]->markBin( Locator::getParticles()->begin()->velocity() );
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

void SimulationAnalyzer::updateBars( SimulationType simulationType )
{

    switch( simulationType )
    {

     case SimulationType::BASIC :
        (*barCharts)[ActionType::M_VELOCITY]->add(physicsInfo.avgVelocity);
        (*barCharts)[ActionType::M_COLLISIONS]->add(physicsInfo.numOfCollisionTP);
        (*barCharts)[ActionType::M_KINETIC]->add(physicsInfo.kineticEnergySumTP);
     break;

     case SimulationType::DIFFUSION :
        (*barCharts)[ActionType::M_VELOCITY_BLUE]->add(physicsInfo.avgVelocityBlue);
        (*barCharts)[ActionType::M_VELOCITY_RED]->add(physicsInfo.avgVelocityRed);
        (*barDisplays)[ActionType::M_DIFFIUSION]->setUpperBox(0,physicsInfo.numBlueParticlesLeft);
        (*barDisplays)[ActionType::M_DIFFIUSION]->setUpperBox(1,physicsInfo.numBlueParticlesRight);
        (*barDisplays)[ActionType::M_DIFFIUSION]->setLowerBox(0,physicsInfo.numRedParticlesLeft);
        (*barDisplays)[ActionType::M_DIFFIUSION]->setLowerBox(1,physicsInfo.numRedParticlesRight);
     break;

     default :
     break;

    }

}

