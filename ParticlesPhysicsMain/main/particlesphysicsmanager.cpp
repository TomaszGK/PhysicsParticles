#include <QtGlobal>
#include "particlesphysicsmanager.h"
#include "particlesmath.h"
#include <cmath>

ParticlesPhysicsManager::ParticlesPhysicsManager( SimulationType type, int planeWidth, int planeHeight )
: simulationType {type}
{
    planeArea = std::make_shared<PlaneArea>(planeWidth,planeHeight,simulationInfo.planeBorderSize[type]);

    calculationStart = time = HRClock::now();

    clustersInfo.clusterSize = static_cast<int>(simulationInfo.maxSizeOfParticle[type]*simulationInfo.clusterRatio[type]);

    particles = std::make_shared<std::vector<Particle>>();
    clusters = std::make_unique<std::vector<Cluster>>();
    particles->reserve(static_cast<size_t>(simulationInfo.maxParticles[type]));
    clusters->reserve(static_cast<size_t>((planeWidth*planeHeight)/(clustersInfo.clusterSize*clustersInfo.clusterSize)+10));

    createClusters();

    createParticles();    

    selectedParticle = particles->begin();

    barCharts["velocity"]     = std::make_shared<BarChart>( 80 , "Average Velocity of Gas Particles" , "Time" , "V" );
    barCharts["velocityBlue"] = std::make_shared<BarChart>( 70 , "Average Velocity of Blue Gas Particles" , "Time" , "V" );
    barCharts["velocityRed"]  = std::make_shared<BarChart>( 70 , "Average Velocity of Red Gas Particles" , "Time" , "V" );
    barCharts["collisions"]   = std::make_shared<BarChart>( 40 , "Collisions" );
    barCharts["kinetic"]      = std::make_shared<BarChart>( 160 , "Energy of Particle Hits" , "Time" , "E" );

    barDisplays["diffiusion"] = std::make_shared<BarDisplay>( 2 );

    histograms1D["velocityDistribution"] = std::make_shared<Histogram1D>( 80 , physicsInfo.minRapidity , physicsInfo.maxRapidity*2.0 , "Velocity Distribution" );
    histograms1D["momentumDistribution"] = std::make_shared<Histogram1D>( 80 , physicsInfo.minRapidity , physicsInfo.maxRapidity*2.0 , "Momentum Distribution" );

    Locator::provide(this);
}

ParticlesPhysicsManager::~ParticlesPhysicsManager()
{

}

void ParticlesPhysicsManager::reset()
{
    simulationInfo.particleSize[ParticleType::BLUE]   = simulationInfo.particleSizeInit[ParticleType::BLUE];
    simulationInfo.particleSize[ParticleType::RED]    = simulationInfo.particleSizeInit[ParticleType::RED];    
    simulationInfo.particleSize[ParticleType::NORMAL] = simulationInfo.particleSizeInit[ParticleType::NORMAL];
    simulationInfo.particleSize[ParticleType::GAS1]  = simulationInfo.particleSizeInit[ParticleType::GAS1];
    simulationInfo.particleSize[ParticleType::GAS2]  = simulationInfo.particleSizeInit[ParticleType::GAS2];
    simulationInfo.particleSize[ParticleType::GAS3]  = simulationInfo.particleSizeInit[ParticleType::GAS3];

    simulationInfo.numberOfParticles[ParticleType::BLUE] = 0;
    simulationInfo.numberOfParticles[ParticleType::RED] = 0;
    simulationInfo.numberOfParticles[ParticleType::NORMAL] = 0;
    simulationInfo.numberOfParticles[ParticleType::GAS1] = 0;
    simulationInfo.numberOfParticles[ParticleType::GAS2] = 0;
    simulationInfo.numberOfParticles[ParticleType::GAS3] = 0;

    physicsInfo = physicsInfoInitial;
    calculationStart = time = HRClock::now();

    if( !pauseByUserFlag ) pause();

    removeAllParticles();
    removeParticlesFromClusters();
    createParticles();

    if( !pauseByUserFlag ) run();
}

void ParticlesPhysicsManager::createParticles()
{
    switch( simulationType )
    {
        case SimulationType::BASIC:         
         visualizationType = VisualizationType::VELOCITY;
         addParticles(ParticleType::NORMAL,visualizationType,simulationInfo.numberOfParticlesInit[ParticleType::NORMAL],simulationInfo.particleSizeInit[ParticleType::NORMAL]);
         planeArea->getPlainDivider().setDividerGap(100);
        break;

        case SimulationType::DIFFUSION:         
         visualizationType = VisualizationType::PARTICLE;
         addParticles(ParticleType::BLUE,visualizationType,simulationInfo.numberOfParticlesInit[ParticleType::BLUE],simulationInfo.particleSizeInit[ParticleType::BLUE]);
         addParticles(ParticleType::RED,visualizationType,simulationInfo.numberOfParticlesInit[ParticleType::RED],simulationInfo.particleSizeInit[ParticleType::RED]);         
         planeArea->getPlainDivider().setDividerGap(0);
        break;

        case SimulationType::BROWNIAN_MOTION:         
         visualizationType = VisualizationType::PARTICLE;
         physicsInfo.temperature = 1.0;
         addParticles(ParticleType::MACROSCOPIC,visualizationType,simulationInfo.numberOfParticlesInit[ParticleType::MACROSCOPIC],simulationInfo.particleSizeInit[ParticleType::MACROSCOPIC]);
         addParticles(ParticleType::MINI,visualizationType,simulationInfo.numberOfParticlesInit[ParticleType::MINI]-1,simulationInfo.particleSizeInit[ParticleType::MINI]);
         planeArea->getPlainDivider().setDividerGap(100);
        break;

        case SimulationType::SANDBOX:         
         visualizationType = VisualizationType::PARTICLE;
         addParticles(ParticleType::GAS1,visualizationType,simulationInfo.numberOfParticlesInit[ParticleType::GAS1],simulationInfo.particleSize[ParticleType::GAS1]);
         addParticles(ParticleType::GAS2,visualizationType,simulationInfo.numberOfParticlesInit[ParticleType::GAS2],simulationInfo.particleSize[ParticleType::GAS2]);
         addParticles(ParticleType::GAS3,visualizationType,simulationInfo.numberOfParticlesInit[ParticleType::GAS3],simulationInfo.particleSize[ParticleType::GAS3]);
         planeArea->getPlainDivider().setDividerGap(100);
        break;
    }
}

void ParticlesPhysicsManager::setVisualizationType( VisualizationType type )
{    
    if( !pauseByUserFlag ) pause();

    visualizationType = type;

    for( auto &particle : *particles )
    {
        particle.visualizationType = type;
        particle.calculateParticleColor();
    }

    if( !pauseByUserFlag ) run();
}

bool ParticlesPhysicsManager::addParticles( ParticleType particleType, VisualizationType visualizationType, int quantity, int particleSize )
{
    if( simulationInfo.numberOfParticles[particleType]+quantity > simulationInfo.maxParticles[simulationType] ) return false;
    if( simulationInfo.maxSizeOfParticle[simulationType] < particleSize ) return false;  // beware of cluster size

    vect2D position,velocity;
    double temperatureMax {0};
    double temperatureMin {0};
    int minX {particleSize+5};
    int maxX {planeArea->getWidth()-particleSize-5};
    int minY {particleSize+5};
    int maxY {planeArea->getHeight()-particleSize-5};

    switch( particleType )
    {
        case ParticleType::GAS1:
        case ParticleType::GAS2:
        case ParticleType::GAS3:
        case ParticleType::NORMAL:
         temperatureMax = physicsInfo.temperature;
        break;

        case ParticleType::BLUE:
         temperatureMax = physicsInfo.temperatureLeft;
         maxX = planeArea->getPlainDivider().getUpperRect().first.x-5;
        break;

        case ParticleType::RED:
         temperatureMax = physicsInfo.temperatureRight;
         minX = planeArea->getPlainDivider().getUpperRect().first.x+planeArea->getPlainDivider().getUpperRect().second.x+5;
        break;

        case ParticleType::MINI:
         temperatureMax = physicsInfo.temperature;
         temperatureMin = physicsInfo.temperature*0.9;
        break;

        case ParticleType::MACROSCOPIC:
         temperatureMax = 0;
         temperatureMin = 0;
         minX = planeArea->getWidth()/2 - 2*particleSize;
         maxX = planeArea->getWidth()/2 + 2*particleSize;
         minY = planeArea->getHeight()/2 - 2*particleSize;
         maxY = planeArea->getHeight()/2 + 2*particleSize;
        break;
    }

    if( !pauseByUserFlag ) pause();

    for( int index=0 ; index<quantity ; ++index )
    {
        if( isParticlePlaneFull() )
        {
            quantity = index;
            break;
        }
        position = isParticleTypeGas(particleType) ? getDisjointRandomParticlePositionTries(minX,maxX,minY,maxY,particleSize/2,100) : getDisjointRandomParticlePosition(minX,maxX,minY,maxY,particleSize/2);

        velocity.x = Random::get<double>(temperatureMin,temperatureMax);
        velocity.y = Random::get<double>(temperatureMin,sqrt(temperatureMax*temperatureMax-velocity.x*velocity.x));
        velocity.x *= Random::get<bool>() ? 1.0 : -1.0;
        velocity.y *= Random::get<bool>() ? 1.0 : -1.0;

        auto iterCluster = getClusterIter(static_cast<size_t>(position.x),static_cast<size_t>(position.y));
        particles->push_back(Particle(particleType,visualizationType,position,velocity,physicsInfo.maxRapidity,particleSize,iterCluster));
        iterCluster->addParticle( std::prev(particles->end()) );
    }

    simulationInfo.numberOfParticles[particleType] += quantity;

    if( !pauseByUserFlag ) run();

    return true;
}

bool ParticlesPhysicsManager::removeParticles( ParticleType particleType, int quantity )
{    

    if( simulationInfo.numberOfParticles[particleType] < quantity ) return false;

    if( !pauseByUserFlag ) pause();

    auto iter = particles->begin();    
    bool erase = false;
    bool isTrackingFlag = selectedParticle->isTracking;

    simulationInfo.numberOfParticles[particleType] -= quantity;

    while( quantity>0 )
    {
        if( iter->particleType == particleType )
        {            
            particles->erase(iter);
            iter = particles->begin();
            erase = true;
            --quantity;
        }
        if( !erase ) ++iter;
        else erase = false;
    }

    recalculateParticlesInClusters();
    selectedParticle = particles->begin();
    selectedParticle->isTracking = isTrackingFlag;

    if( !pauseByUserFlag ) run();

    return true;
}

void ParticlesPhysicsManager::removeAllParticles()
{
    particles->clear();
}

void ParticlesPhysicsManager::shuffleParticle( const iterParticle& particle )
{
    double minX {0};
    double maxX {0};

    if( particle->particleType == ParticleType::NORMAL || planeArea->getPlainDivider().getDividerGap()>0 )
    {
        minX = particle->size+5;
        maxX = planeArea->getWidth()-particle->size-5;
    }
    else if( particle->particleType == ParticleType::BLUE )
    {
        minX = particle->size+5;
        maxX = planeArea->getPlainDivider().getUpperRect().first.x-5;
    }
    else if( particle->particleType == ParticleType::RED )
    {
        minX = planeArea->getPlainDivider().getUpperRect().first.x+planeArea->getPlainDivider().getUpperRect().second.x+5;
        maxX = planeArea->getWidth()-particle->size-5;
    }

    particle->position = getDisjointRandomParticlePosition(minX,maxX,particle->size+5,planeArea->getHeight()-particle->size-5,particle->radius);

    handleParticleClusterTransition(particle);
}

bool ParticlesPhysicsManager::isParticlePlaneFull()
{
    double particlesVolume {0};
    for( const auto& particle : *particles )
    {
        particlesVolume += 3.1415*particle.radius*particle.radius;
    }

    return ( planeArea->getPlaneField() < simulationInfo.planeFillCoefficient*particlesVolume ) ? true : false;
}

bool ParticlesPhysicsManager::isParticlePlaneFull( ParticleType particleType, int newSize )
{
    double particlesVolume {0};
    for( const auto& particle : *particles )
    {
        particlesVolume += 3.1415 * ( ( particle.particleType == particleType ) ? newSize*newSize*0.25 : particle.radius*particle.radius );
    }

    return ( planeArea->getPlaneField() < simulationInfo.planeFillCoefficient*particlesVolume ) ? true : false;
}

bool ParticlesPhysicsManager::setParticlesInPlane( ParticleType particleType, int quantity )
{
    if( simulationInfo.maxParticles[simulationType] < quantity ) return false;
    if( simulationInfo.numberOfParticles[particleType] == quantity ) return true;

    int diff = std::abs( quantity - simulationInfo.numberOfParticles[particleType] );

    return ( simulationInfo.numberOfParticles[particleType] < quantity ) ? addParticles( particleType, visualizationType, diff, simulationInfo.particleSize[particleType] ) : removeParticles( particleType , diff );
}

void ParticlesPhysicsManager::setSizeOfParticlesInPercent( ParticleType type, int quantity )
{
    Ensures( quantity>=0 && quantity<=100 );
    int newSize = simulationInfo.minSizeOfParticle + static_cast<int>(quantity*0.01*(simulationInfo.maxSizeOfParticle[simulationType]-simulationInfo.minSizeOfParticle));

    if( isParticlePlaneFull(type,newSize) ) return;

    simulationInfo.particleSize[type] = newSize;

    if( !pauseByUserFlag ) pause();

    for( auto iter=particles->begin() ; iter!=particles->end() ; ++iter )
    {
        if( iter->particleType == type )
        {
            iter->setParticleSize(simulationInfo.particleSize[type]);
            disjoint(iter);
        }
    }

    if( !pauseByUserFlag ) run();
}

void ParticlesPhysicsManager::setAttractionForceInPercent( int quantity )
{
    Ensures( quantity>=-100 && quantity<=100 );
    physicsInfo.attractionForce = quantity*0.01*physicsInfo.maxAttractionForce;
}

void ParticlesPhysicsManager::setPlaneWidthInPercent( int quantity )
{
    Ensures( quantity>=0 && quantity<=100 );
    planeArea->setXConstraint( static_cast<int>(0.5*0.01*simulationInfo.maxPlaneXConstraint*planeArea->getWidth()*quantity*0.01) );
}

void ParticlesPhysicsManager::enableTracking()
{
    selectedParticle->isTracking = true;
}

void ParticlesPhysicsManager::disableTracking()
{
    selectedParticle->isTracking = false;
    selectedParticle->particlePositionsTracking.clear();
}

bool ParticlesPhysicsManager::disjoint( const iterParticle& particle )
{
    double distance {0};

    for( auto &cluster : *particle->cluster->getAdjoinClusters() )
    {
        for( auto &otherParticle : *cluster->getParticlesInCluster() )
        {
            if( otherParticle == particle ) continue ;
            distance = (otherParticle->position - particle->position)();

            if( distance < (particle->radius + otherParticle->radius - 1) )
            {                
                shuffleParticle(particle);
                return true;
            }
        }
    }

    return false;
}

void ParticlesPhysicsManager::correctParticlesInvalidParameters()
{
    for( auto particle=particles->begin() ; particle!=particles->end() ; ++particle )
    {
        if( particle->position.x - particle->radius <= planeArea->getXConstraint() ||
            particle->position.x + particle->radius >= planeArea->getWidth() - planeArea->getXConstraint() ||
            particle->position.y - particle->radius <= 0 ||
            particle->position.y + particle->radius >= planeArea->getHeight() )
        {
           particle->position = getDisjointRandomParticlePosition(particle->radius,planeArea->getWidth()-particle->radius,particle->radius,planeArea->getHeight()-particle->radius,particle->radius);
           particle->velocity.set(0.0,0.0);
           handleParticleClusterTransition(particle);
        }
    }
}

void ParticlesPhysicsManager::disjointPositions( double impactFactor )
{
    for( auto particle=particles->begin() ; particle!=particles->end() ; ++particle )
    {
        for( auto &cluster : *particle->cluster->getAdjoinClusters() )
        {
            for( auto &otherParticle : *cluster->getParticlesInCluster() )
            {
                if( otherParticle == particle ) continue ;

                auto distance = (otherParticle->position - particle->position)();

                if( distance < impactFactor*(particle->radius + otherParticle->radius) )
                {                    
                    particle->position += (particle->position - otherParticle->position).setLength(distance*0.5+1);
                    otherParticle->position += (otherParticle->position - particle->position).setLength(distance*0.5+1);
                    preserveParticleInPlane(particle);
                    preserveParticleInPlane(otherParticle);
                    handleParticleClusterTransition(particle);
                    handleParticleClusterTransition(otherParticle);
                }
            }
        }
    }
}

void ParticlesPhysicsManager::preserveParticleInPlane( const iterParticle& particle )
{
    if( particle->position.x-particle->radius<=0 ) particle->position.x = particle->radius+1;
    else if( particle->position.x+particle->radius>=planeArea->getWidth() ) particle->position.x = static_cast<double>(planeArea->getWidth())-particle->radius-1;

    if( particle->position.y-particle->radius<=0 ) particle->position.y = particle->radius+1;
    else if( particle->position.y+particle->radius>=planeArea->getHeight() ) particle->position.y = static_cast<double>(planeArea->getHeight())-particle->radius-1;
}

void ParticlesPhysicsManager::removeParticlesFromClusters()
{
    for( auto &cluster : *clusters )
    {
        cluster.getParticlesInCluster()->clear();
    }
}

void ParticlesPhysicsManager::recalculateParticlesInClusters()
{
    removeParticlesFromClusters();

    for( auto particle=particles->begin() ; particle!=particles->end() ; ++particle )
    {
        auto iterCurrent = getClusterIter(static_cast<size_t>(particle->position.x),static_cast<size_t>(particle->position.y));
        iterCurrent->addParticle( particle );
        particle->cluster = iterCurrent;
    }
}

iterCluster ParticlesPhysicsManager::getClusterIter( const size_t& posx, const size_t& posy )
{
    // if position out of range then throw exception
    return clusterIters.at(posx).at(posy);
}

void ParticlesPhysicsManager::update()
{
    calculationPeriod = std::chrono::duration_cast<std::chrono::microseconds>(HRClock::now() - calculationStart).count()*0.001;
    calculationStart = HRClock::now();

    timeContribution = calculationPeriod<simulationInfo.maxTimeContribution ? calculationPeriod : simulationInfo.maxTimeContribution;

    double velocitySum {0};
    double kineticEnergy {0};
    double velocity {0};
    double velocityBlueSum {0};
    double velocityRedSum {0};

    simulationInfo.calculationCount++;    

    for( auto particle=particles->begin() ; particle!=particles->end() ; ++particle )
    {

        if( !physicsInfo.pushForce.isZero() )
        {
            if( !particle->isMacroscopic ) particle->velocity += physicsInfo.pushForce*calculationPeriod;
        }

        handleParticleCollisions(particle);

        if( particle->cluster->PLANE_BOUNDRY || planeArea->getXConstraint()>0 )
        {
            kineticEnergy = handleParticleCollisionWithPlaneBoundries(particle);
            if( kineticEnergy>0 )
            {
                ++physicsInfo.numOfCollision;
                physicsInfo.kineticEnergySum += kineticEnergy;
            }
        }

        if( planeArea->getPlainDivider().isDividerInPlane() && particle->cluster->PLANE_DIVIDER )
        {
            planeArea->getPlainDivider().handleParticleCollision(particle);
        }

        particle->moveToNextPosition(timeContribution);

        handleParticleClusterTransition(particle);

        velocity = particle->velocity();
        histograms1D["velocityDistribution"]->fill(velocity);
        histograms1D["momentumDistribution"]->fill(velocity*particle->mass);

        if( particle->particleType == ParticleType::NORMAL )
        {
            velocitySum += velocity;
        }
        else if( particle->particleType == ParticleType::BLUE )
        {
            velocityBlueSum += velocity;
        }
        else if( particle->particleType == ParticleType::RED )
        {
            velocityRedSum += velocity;
        }

    }        

    if( simulationType == SimulationType::BROWNIAN_MOTION ) histograms1D["velocityDistribution"]->markBin( getMoleculeVelocity() );

    if( simulationType == SimulationType::BASIC )
    {
        physicsInfo.avgVelocity = velocitySum/particles->size();
    }
    else if( simulationType == SimulationType::DIFFUSION )
    {
        physicsInfo.avgVelocityBlue = velocityBlueSum/simulationInfo.numberOfParticlesInit[ParticleType::BLUE];
        physicsInfo.avgVelocityRed = velocityRedSum/simulationInfo.numberOfParticlesInit[ParticleType::RED];
    }
    else if( simulationType == SimulationType::SANDBOX )
    {
        physicsInfo.avgVelocity = velocitySum/particles->size();
    }

    if( std::chrono::duration_cast<Milliseconds>(HRClock::now() - time) > physicsInfo.timePeriod )
    {        
        time = HRClock::now();       
        physicsInfo.kineticEnergySumTP = physicsInfo.kineticEnergySum/simulationInfo.calculationCount;
        physicsInfo.kineticEnergySum = 0;
        physicsInfo.numOfCollisionTP = static_cast<double>(physicsInfo.numOfCollision)/static_cast<double>(simulationInfo.calculationCount);
        physicsInfo.numOfCollision = 0;
        simulationInfo.avgCalculationCount = simulationInfo.calculationCount;
        simulationInfo.calculationCount = 0;
        if( simulationInfo.disjointParticles[simulationType] ) disjointPositions(0.9);
        physicsInfo.averageKineticEnergy = getAverageKineticEnergyOfParticles();
    }

}

void ParticlesPhysicsManager::mainLoop()
{
    while( calculationState.load() != ThreadCalculationState::END )
    {
        if( calculationState.load() != ThreadCalculationState::PAUSE )
        {
            try
            {
                calculateNextPositionFlag.store(true);
                update();
                calculateNextPositionFlag.store(false);
            }
            catch( const std::exception& ex )
            {
                std::cout << "1: " << ex.what() << "\n";
                correctParticlesInvalidParameters();
            }
            catch( const std::string& ex )
            {
                std::cout << "2: " << ex << "\n";
            }
            catch(...)
            {
                std::cout << "3: Cought undefined exception \n";
            }

        }
    }
}

void ParticlesPhysicsManager::updateBars()
{
    if( calculationState != ThreadCalculationState::PAUSE )
    {
        if( simulationType == SimulationType::BASIC )
        {
            barCharts["velocity"]->add(physicsInfo.avgVelocity);
            barCharts["collisions"]->add(physicsInfo.numOfCollisionTP);
            barCharts["kinetic"]->add(physicsInfo.kineticEnergySumTP);
        }
        if( simulationType == SimulationType::DIFFUSION )
        {
            barCharts["velocityBlue"]->add(physicsInfo.avgVelocityBlue);
            barCharts["velocityRed"]->add(physicsInfo.avgVelocityRed);

            updateParticlesLocationInPlane();
            barDisplays["diffiusion"]->setUpperBox(0,physicsInfo.numBlueParticlesLeft);
            barDisplays["diffiusion"]->setUpperBox(1,physicsInfo.numBlueParticlesRight);
            barDisplays["diffiusion"]->setLowerBox(0,physicsInfo.numRedParticlesLeft);
            barDisplays["diffiusion"]->setLowerBox(1,physicsInfo.numRedParticlesRight);
        }
    }
}

void ParticlesPhysicsManager::setDividerGap( int dividerGap )
{
    Ensures( dividerGap>=0 && dividerGap<=100 );
    planeArea->getPlainDivider().setDividerGap(dividerGap);
}

void ParticlesPhysicsManager::setTemperatureInPercent( int temperature )
{
    Ensures( temperature>=0 && temperature<=100 );
    physicsInfo.temperature = (sqrt(physicsInfo.maxRapidity*2)/2)*temperature*0.01;
}

void ParticlesPhysicsManager::setTemperatureLeftInPercent( int temperature )
{
    Ensures( temperature>=0 && temperature<=100 );
    physicsInfo.temperatureLeft = (sqrt(physicsInfo.maxRapidity*2)/2)*temperature*0.01;
}

void ParticlesPhysicsManager::setTemperatureRightInPercent( int temperature )
{
    Ensures( temperature>=0 && temperature<=100 );
    physicsInfo.temperatureRight = (sqrt(physicsInfo.maxRapidity*2)/2)*temperature*0.01;
}

void ParticlesPhysicsManager::setSideTemperatureInPercent( PlaneSide side, int temperature )
{
    Ensures( temperature>=0 && temperature<=100 );
    physicsInfo.planeSideTemperature[side] = (sqrt(physicsInfo.maxRapidity*2)/2)*temperature*0.01;
}

void ParticlesPhysicsManager::setHorizontalForceInPercent( int force )
{
    Ensures( force>=-100 && force<=100 );
    physicsInfo.pushForce.x = 0.01*force*physicsInfo.maxSideForce;
}

void ParticlesPhysicsManager::setVerticalForceInPercent( int force )
{
    Ensures( force>=-100 && force<=100 );
    physicsInfo.pushForce.y = 0.01*force*physicsInfo.maxSideForce;
}

void ParticlesPhysicsManager::setMassOfMoleculeInPercent( int percent )
{
    if( simulationType == SimulationType::BROWNIAN_MOTION )
    {
        particles->begin()->setParticleMassInPercent(percent);
    }
}

colorRGB ParticlesPhysicsManager::getRandomColor()
{
    return { static_cast<unsigned char>(Random::get<int>(0,255)),
             static_cast<unsigned char>(Random::get<int>(0,255)),
                static_cast<unsigned char>(Random::get<int>(0,255))   };
}

double ParticlesPhysicsManager::getAverageKineticEnergyOfParticles()
{
    double average {0};

    for( const auto& particle : *particles )
    {
        average += 0.5*particle.mass*(~particle.velocity);
    }

    return average/particles->size();
}

void ParticlesPhysicsManager::updateParticlesLocationInPlane()
{
    physicsInfo.numBlueParticlesRight = physicsInfo.numBlueParticlesLeft = 0;
    physicsInfo.numRedParticlesRight = physicsInfo.numRedParticlesLeft = 0;

    for( auto &particle : *particles )
    {
        if( particle.particleType == ParticleType::BLUE )
        {
            if( particle.position.x>planeArea->getPlainDivider().getDividerPosX() ) physicsInfo.numBlueParticlesRight++;
            else physicsInfo.numBlueParticlesLeft++;
        }
        else if( particle.particleType == ParticleType::RED )
        {
            if( particle.position.x>planeArea->getPlainDivider().getDividerPosX() ) physicsInfo.numRedParticlesRight++;
            else physicsInfo.numRedParticlesLeft++;
        }
    }
}

vect2D ParticlesPhysicsManager::getDisjointRandomParticlePosition( double minx, double maxx, double miny, double maxy, double radius )
{
    vect2D position;
    int maxTries = static_cast<int>((maxx-minx)*(maxy-miny)/(3.14*radius*radius));

    position.x = Random::get<double>(minx,maxx);
    position.y = Random::get<double>(miny,maxy);

    while( isParticlesOverlap(position,radius) && --maxTries>0 )
    {
        position.x += 2*radius;
        if( position.x>maxx )
        {
            position.x = minx;
            position.y += 2*radius;
            if( position.y>maxy ) position.y = miny;
        }
    }    

    return position;
}

vect2D ParticlesPhysicsManager::getDisjointRandomParticlePositionTries( double minx, double maxx, double miny, double maxy, double radius, int tries )
{
    vect2D position;

    do
    {
        position.x = Random::get<double>(minx,maxx);
        position.y = Random::get<double>(miny,maxy);
    }
    while( isParticlesOverlap(position,radius) && --tries>0 );

    return position;
}

void ParticlesPhysicsManager::handleParticleClusterTransition( const iterParticle& particle )
{
    auto iterCurrent = getClusterIter(static_cast<size_t>(particle->position.x),static_cast<size_t>(particle->position.y));
    if( iterCurrent != particle->cluster )
    {
        iterCurrent->addParticle( particle );
        particle->cluster->removeParticle( particle );
        particle->cluster = iterCurrent;
    }
}

void ParticlesPhysicsManager::handleParticleCollisions( const iterParticle& particle )
{

    double dotProductV1V2 {0};
    double dotProductV2V1 {0};
    double distance {0};
    double minDistance {0};

    for( auto &cluster : *particle->cluster->getAdjoinClusters() )
    {
        for( auto &otherParticle : *cluster->getParticlesInCluster() )
        {
            if( otherParticle == particle ) continue ;

            distance = (otherParticle->position - particle->calculateNextPosition(timeContribution))();
            minDistance = otherParticle->radius+particle->radius;

            if( (otherParticle->position - particle->position)() <= minDistance ) continue;

            if( distance < minDistance )
            {
                particle->modifiedVelocity = otherParticle->modifiedVelocity = true;

                dotProductV1V2 = (particle->velocity-otherParticle->velocity)*(particle->position-otherParticle->position);
                dotProductV2V1 = (otherParticle->velocity-particle->velocity)*(otherParticle->position-particle->position);

                particle->velocity -= (particle->position-otherParticle->position) *
                                        ((2*otherParticle->mass/(otherParticle->mass+particle->mass)) *
                                         dotProductV1V2/~(particle->position-otherParticle->position)) ;

                otherParticle->velocity -= (otherParticle->position-particle->position) *
                                             ((2*particle->mass/(otherParticle->mass+particle->mass)) *
                                              dotProductV2V1/~(otherParticle->position-particle->position)) ;


            }
            else if( simulationType == SimulationType::SANDBOX && physicsInfo.attractionForce!=0.0 )
            {
                particle->modifiedVelocity = otherParticle->modifiedVelocity = true;                
                vect2D direction = (particle->position-otherParticle->position).setLength(1.0);
                double factor    = physicsInfo.attractionForce*calculationPeriod/distance;

                particle->velocity += direction*factor*(-1.0)*otherParticle->mass;
                otherParticle->velocity += direction*factor*particle->mass;
            }

        }
    }

}

void ParticlesPhysicsManager::handleParticleCollisionsAlternative( const iterParticle& particle )
{

    vect2D velocityChangeP1; // velocity change for particle
    vect2D velocityChangeP2; // velocity change for other particle

    double v1 {0};
    double v2 {0};
    double saveVelocity {0};
    double diffVelocity {0};
    double distance {0};
    double minDistance {0};

    for( auto &cluster : *particle->cluster->getAdjoinClusters() )
    {
        for( auto &otherParticle : *cluster->getParticlesInCluster() )
        {
            if( otherParticle == particle ) continue ;

            distance = (otherParticle->position - particle->calculateNextPosition(timeContribution))();
            minDistance = otherParticle->radius+particle->radius;

            if( distance < minDistance )
            {
                particle->modifiedVelocity = otherParticle->modifiedVelocity = true;

                if( ParticlesMath::cosAngle( particle->velocity , otherParticle->position - particle->position )>0 )
                {
                    velocityChangeP1 = ParticlesMath::projectionVector( otherParticle->position , particle->position , particle->calculateNextPosition() );
                }
                else velocityChangeP1.set(0.0,0.0);

                if( ParticlesMath::cosAngle( otherParticle->velocity , particle->position - otherParticle->position )>0 )
                {
                    velocityChangeP2 = ParticlesMath::projectionVector( particle->position , otherParticle->position , otherParticle->calculateNextPosition() );
                }
                else velocityChangeP2.set(0.0,0.0);

                saveVelocity = particle->velocity() + otherParticle->velocity();

                particle->velocity      += (velocityChangeP2-velocityChangeP1);
                otherParticle->velocity += (velocityChangeP1-velocityChangeP2);

                // preserve the sum of velocites
                v1 = particle->velocity();
                v2 = otherParticle->velocity();
                diffVelocity = ( (v1+v2) - saveVelocity )*0.5;

                particle->velocity.setLength(v1-diffVelocity);
                otherParticle->velocity.setLength(v2-diffVelocity);

            }
            else if( simulationType == SimulationType::SANDBOX && physicsInfo.attractionForce!=0.0 )
            {
                particle->modifiedVelocity = otherParticle->modifiedVelocity = true;
                double distanceCut = ( distance<minDistance ) ? minDistance : distance ;
                vect2D direction = (particle->position-otherParticle->position).setLength(1.0);
                double factor    = physicsInfo.attractionForce*calculationPeriod/distanceCut;

                particle->velocity += direction*factor*(-1.0)*otherParticle->mass;
                otherParticle->velocity += direction*factor*particle->mass;
            }

        }
    }

}

double ParticlesPhysicsManager::handleParticleCollisionWithPlaneBoundries( const iterParticle &particle )
{
    vect2D newPosition = particle->calculateNextPosition(timeContribution);
    double kineticEnergy {0.0};
    double temperature {0};

    if( simulationType == SimulationType::BASIC )
    {
        temperature = physicsInfo.temperature;
    }   
    else if( simulationType == SimulationType::DIFFUSION )
    {
        if( newPosition.x < planeArea->getPlainDivider().getDividerPosX() ) temperature = physicsInfo.temperatureLeft;
        else temperature =  physicsInfo.temperatureRight;
    }

    if( newPosition.x - particle->radius <= planeArea->getXConstraint() )
    {
        if( simulationType == SimulationType::SANDBOX ) temperature = physicsInfo.planeSideTemperature[PlaneSide::LEFT];
        particle->position.x = particle->radius + planeArea->getXConstraint();
        kineticEnergy = abs(particle->velocity.x)*particle->mass*0.5;
        particle->velocity.x = simulationType == SimulationType::BROWNIAN_MOTION ? (-1.0)*particle->velocity.x : temperature;
        particle->modifiedVelocity = true;
        return kineticEnergy;
    }
    if( newPosition.x + particle->radius >= planeArea->getWidth() - planeArea->getXConstraint() )
    {
        if( simulationType == SimulationType::SANDBOX ) temperature = physicsInfo.planeSideTemperature[PlaneSide::RIGHT];
        particle->position.x = static_cast<double>(planeArea->getWidth()) - particle->radius - planeArea->getXConstraint();
        kineticEnergy = abs(particle->velocity.x)*particle->mass*0.5;
        particle->velocity.x = (-1.0)*( simulationType == SimulationType::BROWNIAN_MOTION ? particle->velocity.x : temperature);
        particle->modifiedVelocity = true;
        return kineticEnergy;
    }

    if( newPosition.y - particle->radius <= 0 )
    {
        if( simulationType == SimulationType::SANDBOX ) temperature = physicsInfo.planeSideTemperature[PlaneSide::UP];
        particle->position.y = particle->radius;
        kineticEnergy = abs(particle->velocity.y)*particle->mass*0.5;
        particle->velocity.y = simulationType == SimulationType::BROWNIAN_MOTION ? (-1.0)*particle->velocity.y : temperature;
        particle->modifiedVelocity = true;
        return kineticEnergy;
    }
    if( newPosition.y + particle->radius >= planeArea->getHeight() )
    {
        if( simulationType == SimulationType::SANDBOX ) temperature = physicsInfo.planeSideTemperature[PlaneSide::DOWN];
        particle->position.y = static_cast<double>(planeArea->getHeight()) - particle->radius;
        kineticEnergy = abs(particle->velocity.y)*particle->mass*0.5;
        particle->velocity.y = (-1.0)*( simulationType == SimulationType::BROWNIAN_MOTION ? particle->velocity.y : temperature);
        particle->modifiedVelocity = true;
        return kineticEnergy;
    }

    return 0;
}

bool ParticlesPhysicsManager::isParticlesOverlap( const vect2D& particlePosition, double radius )
{    
    double distance {0};

    auto iterCluster = getClusterIter(static_cast<size_t>(particlePosition.x),static_cast<size_t>(particlePosition.y));
    for( auto &cluster : *iterCluster->getAdjoinClusters() )
    {
        for( auto &particle : *cluster->getParticlesInCluster() )
        {
            distance = (particlePosition-particle->position)();
            if( distance<(radius+particle->radius+2) ) return true;
        }
    }

    return false;
}

void ParticlesPhysicsManager::connectClusters()
{
    if( clusters->size()>1 )
    {
        coord2D coordinates {0,0};

        for( auto &cluster : *clusters )
        {
            coordinates = cluster.getCoordinates();

            if(  coordinates.x == 1 || coordinates.y == 1 ||
                 coordinates.x == clustersInfo.clusterColumns ||
                 coordinates.y == clustersInfo.clusterRows  ) cluster.PLANE_BOUNDRY = true;

            for( int a=-1 ; a<2 ; ++a)
            {
                for( int b=-1 ; b<2 ; ++b)
                {
                    if( coordinates.x+a<1 || coordinates.x+a>clustersInfo.clusterColumns )  continue;
                    if( coordinates.y+b<1 || coordinates.y+b>clustersInfo.clusterRows )  continue;
                    cluster.getAdjoinClusters()->push_back( clusterCoordinatesMap[ coordinates + coord2D(a,b) ] );
                }
            }

        }
    }
}

void ParticlesPhysicsManager::createClusters()
{
    clusterIters.resize(static_cast<size_t>(planeArea->getWidth()));
    for( auto &column : clusterIters )
    {
        column.resize(static_cast<size_t>(planeArea->getHeight()));
    }

    int id {1};
    int clusterSizeX, clusterSizeY;

    for( int x=0 ; x<planeArea->getWidth()-clustersInfo.clusterSize ; x+=clustersInfo.clusterSize)
    {
        ++clustersInfo.clusterColumns;
        clustersInfo.clusterRows = 0;
        for( int y=0 ; y<planeArea->getHeight()-clustersInfo.clusterSize ; y+=clustersInfo.clusterSize)
        {
           ++clustersInfo.clusterRows;
           clusterSizeX = clusterSizeY = clustersInfo.clusterSize;
           if( (x+2*clustersInfo.clusterSize)>planeArea->getWidth() )
           {
               clusterSizeX = planeArea->getWidth()-x;
           }
           if( (y+2*clustersInfo.clusterSize)>planeArea->getHeight() )
           {
               clusterSizeY = planeArea->getHeight()-y;
           }           

           clusters->emplace_back(Cluster(id,clustersInfo.clusterColumns,clustersInfo.clusterRows));
           populateClusterID( x,y,clusterSizeX,clusterSizeY,std::prev(clusters->end()) );
           ++id;
        }
    }

    for( auto cluster=clusters->begin() ; cluster!=clusters->end() ; ++cluster )
    {        
        clusterCoordinatesMap[ cluster->getCoordinates() ] = cluster;
    }

    connectClusters();

    // tag clusters close to plane divider
    for( int y=0 ; y<planeArea->getHeight() ; y+=clustersInfo.clusterSize )
    {
        clusterIters.at(static_cast<size_t>(planeArea->getPlainDivider().getDividerPosX())).at(static_cast<size_t>(y))->PLANE_DIVIDER = true;
        clusterIters.at(static_cast<size_t>(planeArea->getPlainDivider().getDividerPosX()-clustersInfo.clusterSize+1)).at(static_cast<size_t>(y))->PLANE_DIVIDER = true;
        clusterIters.at(static_cast<size_t>(planeArea->getPlainDivider().getDividerPosX()+clustersInfo.clusterSize-1)).at(static_cast<size_t>(y))->PLANE_DIVIDER = true;
    }

    clustersInfo.numberOfClusters = static_cast<int>(clusters->size());
}

void ParticlesPhysicsManager::populateClusterID( int xstart, int ystart, int xlength, int ylength, iterCluster cluster )
{
    for( int x=xstart ; x<xstart+xlength ; ++x)
    {
        for( int y=ystart ; y<ystart+ylength ; ++y)
        {
            clusterIters.at(static_cast<size_t>(x)).at(static_cast<size_t>(y)) = cluster;
        }
    }
}


