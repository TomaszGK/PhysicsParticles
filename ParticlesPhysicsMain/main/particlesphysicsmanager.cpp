#include <QtGlobal>
#include "particlesphysicsmanager.h"
#include "particlesmath.h"
#include <cmath>

ParticlesPhysicsManager::ParticlesPhysicsManager( SimulationType type, int planeWidth, int planeHeight )
: simulationType {type}
{
    analyzer = std::make_unique<SimulationAnalyzer>();

    planeArea = std::make_shared<PlaneArea>(planeWidth,planeHeight,analyzer->simulationInfo.planeBorderSize[type]);

    calculationStart = time = HRClock::now();

    clustersInfo.clusterSize = static_cast<int>(analyzer->simulationInfo.maxSizeOfParticle[type]*analyzer->simulationInfo.clusterRatio[type]);

    particles = std::make_shared<std::vector<Particle>>();
    clusters = std::make_unique<std::vector<Cluster>>();
    particles->reserve(static_cast<size_t>(analyzer->simulationInfo.maxParticles[type]));
    clusters->reserve(static_cast<size_t>((planeWidth*planeHeight)/(clustersInfo.clusterSize*clustersInfo.clusterSize)+10));

    createClusters();

    createParticles();    

    selectedParticle = particles->begin();

    Locator::provide(this);

    // explicit instantiations of template function to know its definitions in other translate units - linker purpose
    mainLoop<SimulationType::BASIC>();
    mainLoop<SimulationType::DIFFUSION>();
    mainLoop<SimulationType::BROWNIAN_MOTION>();
    mainLoop<SimulationType::SANDBOX>();

}

ParticlesPhysicsManager::~ParticlesPhysicsManager()
{

}

void ParticlesPhysicsManager::reset()
{
    analyzer->reset();
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
         addParticles(ParticleType::NORMAL,visualizationType,analyzer->simulationInfo.numberOfParticlesInit[ParticleType::NORMAL],analyzer->simulationInfo.particleSizeInit[ParticleType::NORMAL]);
         planeArea->getPlainDivider().setDividerGap(100);
        break;

        case SimulationType::DIFFUSION:         
         visualizationType = VisualizationType::PARTICLE;
         addParticles(ParticleType::BLUE,visualizationType,analyzer->simulationInfo.numberOfParticlesInit[ParticleType::BLUE],analyzer->simulationInfo.particleSizeInit[ParticleType::BLUE]);
         addParticles(ParticleType::RED,visualizationType,analyzer->simulationInfo.numberOfParticlesInit[ParticleType::RED],analyzer->simulationInfo.particleSizeInit[ParticleType::RED]);
         planeArea->getPlainDivider().setDividerGap(0);
        break;

        case SimulationType::BROWNIAN_MOTION:         
         visualizationType = VisualizationType::PARTICLE;
         analyzer->physicsInfo.temperature[PlanePart::WHOLE] = 1.0;
         addParticles(ParticleType::MACROSCOPIC,visualizationType,analyzer->simulationInfo.numberOfParticlesInit[ParticleType::MACROSCOPIC],analyzer->simulationInfo.particleSizeInit[ParticleType::MACROSCOPIC]);
         addParticles(ParticleType::MINI,visualizationType,analyzer->simulationInfo.numberOfParticlesInit[ParticleType::MINI]-1,analyzer->simulationInfo.particleSizeInit[ParticleType::MINI]);
         planeArea->getPlainDivider().setDividerGap(100);
        break;

        case SimulationType::SANDBOX:         
         visualizationType = VisualizationType::PARTICLE;
         addParticles(ParticleType::GAS1,visualizationType,analyzer->simulationInfo.numberOfParticlesInit[ParticleType::GAS1],analyzer->simulationInfo.particleSize[ParticleType::GAS1]);
         addParticles(ParticleType::GAS2,visualizationType,analyzer->simulationInfo.numberOfParticlesInit[ParticleType::GAS2],analyzer->simulationInfo.particleSize[ParticleType::GAS2]);
         addParticles(ParticleType::GAS3,visualizationType,analyzer->simulationInfo.numberOfParticlesInit[ParticleType::GAS3],analyzer->simulationInfo.particleSize[ParticleType::GAS3]);
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
    }

    if( !pauseByUserFlag ) run();
}

bool ParticlesPhysicsManager::addParticles( ParticleType particleType, VisualizationType visualizationType, int quantity, int particleSize )
{
    if( analyzer->simulationInfo.numberOfParticles[particleType]+quantity > analyzer->simulationInfo.maxParticles[simulationType] ) return false;
    if( analyzer->simulationInfo.maxSizeOfParticle[simulationType] < particleSize ) return false;  // beware of cluster size

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
         temperatureMax = analyzer->physicsInfo.temperature[PlanePart::WHOLE];
        break;

        case ParticleType::BLUE:
         temperatureMax = analyzer->physicsInfo.temperature[PlanePart::LEFTBOX];
         maxX = planeArea->getPlainDivider().getUpperRect().first.x-5;
        break;

        case ParticleType::RED:
         temperatureMax = analyzer->physicsInfo.temperature[PlanePart::RIGHTBOX];
         minX = planeArea->getPlainDivider().getUpperRect().first.x+planeArea->getPlainDivider().getUpperRect().second.x+5;
        break;

        case ParticleType::MINI:
         temperatureMax = analyzer->physicsInfo.temperature[PlanePart::WHOLE];
         temperatureMin = analyzer->physicsInfo.temperature[PlanePart::WHOLE]*0.9;
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
        particles->push_back(Particle(particleType,visualizationType,position,velocity,particleSize,iterCluster));
        iterCluster->addParticle( std::prev(particles->end()) );
    }

    analyzer->simulationInfo.numberOfParticles[particleType] += quantity;

    if( !pauseByUserFlag ) run();

    return true;
}

bool ParticlesPhysicsManager::removeParticles( ParticleType particleType, int quantity )
{    

    if( analyzer->simulationInfo.numberOfParticles[particleType] < quantity ) return false;

    if( !pauseByUserFlag ) pause();

    auto iter = particles->begin();    
    bool erase = false;
    bool isTrackingFlag = selectedParticle->isTracking;

    analyzer->simulationInfo.numberOfParticles[particleType] -= quantity;

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

void ParticlesPhysicsManager::shuffleParticle( const iterParticle particle )
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

    return ( planeArea->getPlaneField() < analyzer->simulationInfo.planeFillCoefficient*particlesVolume ) ? true : false;
}

bool ParticlesPhysicsManager::isParticlePlaneFull( ParticleType particleType, int newSize )
{
    double particlesVolume {0};
    for( const auto& particle : *particles )
    {
        particlesVolume += 3.1415 * ( ( particle.particleType == particleType ) ? newSize*newSize*0.25 : particle.radius*particle.radius );
    }

    return ( planeArea->getPlaneField() < analyzer->simulationInfo.planeFillCoefficient*particlesVolume ) ? true : false;
}

bool ParticlesPhysicsManager::setNumberOfParticlesInPlane( ParticleType particleType, DataFormat format, int quantity )
{
    if( format == DataFormat::PERCENT ) quantity = static_cast<int>(analyzer->simulationInfo.maxParticles[simulationType]*static_cast<int>(quantity)*0.01);

    if( analyzer->simulationInfo.maxParticles[simulationType] < quantity ) return false;
    if( analyzer->simulationInfo.numberOfParticles[particleType] == quantity ) return true;

    int diff = std::abs( quantity - analyzer->simulationInfo.numberOfParticles[particleType] );

    return ( analyzer->simulationInfo.numberOfParticles[particleType] < quantity ) ? addParticles( particleType, visualizationType, diff, analyzer->simulationInfo.particleSize[particleType] ) : removeParticles( particleType , diff );
}

void ParticlesPhysicsManager::setParticleSize( ParticleType type, DataFormat format, int quantity )
{
    Ensures( quantity>=0 && quantity<=100 );

    int newSize {quantity};
    if( format == DataFormat::PERCENT ) newSize = analyzer->simulationInfo.minSizeOfParticle + static_cast<int>(quantity*0.01*(analyzer->simulationInfo.maxSizeOfParticle[simulationType]-analyzer->simulationInfo.minSizeOfParticle));

    if( isParticlePlaneFull(type,newSize) ) return;

    analyzer->simulationInfo.particleSize[type] = newSize;

    if( !pauseByUserFlag ) pause();

    for( auto iter=particles->begin() ; iter!=particles->end() ; ++iter )
    {
        if( iter->particleType == type )
        {
            iter->setParticleSize(analyzer->simulationInfo.particleSize[type]);
            disjoint(iter);
        }
    }

    if( !pauseByUserFlag ) run();
}

void ParticlesPhysicsManager::setAttractionForceInPercent( int quantity )
{
    Ensures( quantity>=-100 && quantity<=100 );
    analyzer->physicsInfo.attractionForce = quantity*0.01*analyzer->physicsInfo.maxAttractionForce;
}

void ParticlesPhysicsManager::setPlaneWidthInPercent( int quantity )
{
    Ensures( quantity>=0 && quantity<=100 );
    planeArea->setXConstraint( static_cast<int>(0.5*0.01*analyzer->simulationInfo.maxPlaneXConstraint*planeArea->getWidth()*quantity*0.01) );
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

bool ParticlesPhysicsManager::disjoint( const iterParticle particle )
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

void ParticlesPhysicsManager::preserveParticleInPlane( const iterParticle particle )
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
    // if position is out of range then exception is thrown
    return clusterIters.at(posx).at(posy);
}

template< SimulationType type >
void ParticlesPhysicsManager::update()
{
    calculationPeriod = std::chrono::duration_cast<std::chrono::microseconds>(HRClock::now() - calculationStart).count()*0.001;
    calculationStart = HRClock::now();

    timeContribution = calculationPeriod;//analyzer->adjustTimeContribution(calculationPeriod);

    analyzer->simulationInfo.calculationCount++;

    for( auto particle=particles->begin() ; particle!=particles->end() ; ++particle )
    {       

        handleParticleCollisions<type>(particle);

        if( particle->cluster->PLANE_BOUNDRY || planeArea->getXConstraint()>0 )
        {
            analyzer->collect( handleParticleCollisionWithPlaneBoundries<type>(particle) );
        }

        if constexpr( type == SimulationType::DIFFUSION ) planeArea->handleParticleCollisionWithPlaneDivider(particle);

        particle->moveToNextPosition(timeContribution);

        handleParticleClusterTransition(particle);

        if constexpr( type == SimulationType::BROWNIAN_MOTION || type == SimulationType::SANDBOX )
        {
            if( !particle->isMacroscopic ) particle->velocity += analyzer->physicsInfo.pushForce*calculationPeriod;
        }

        analyzer->collect(particle);

    }        

    if( analyzer->isTimePeriodUp(time) )
    {        
        time = HRClock::now();       
        analyzer->update(simulationType);
        if( analyzer->simulationInfo.disjointParticles[simulationType] ) disjointPositions(0.9);        
    }    

}

template< SimulationType type >
void ParticlesPhysicsManager::mainLoop()
{
    while( calculationState.load() != ThreadCalculationState::END )
    {
        if( calculationState.load() != ThreadCalculationState::PAUSE )
        {
            try
            {                
                update<type>();                
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
                return;
            }

        }

        if( calculationState.load() == ThreadCalculationState::PAUSE )
        {
            if( fUpdate.wait_for(std::chrono::seconds(0)) != std::future_status::ready ) pUpdate.set_value(true);
        }
    }
}

void ParticlesPhysicsManager::updateBars()
{
    if( calculationState != ThreadCalculationState::PAUSE )
    {
        if( simulationType == SimulationType::DIFFUSION ) updateParticlesLocationInPlane();
        analyzer->updateBars(simulationType);
    }
}

void ParticlesPhysicsManager::setDividerGap( int dividerGap )
{
    Ensures( dividerGap>=0 && dividerGap<=100 );
    planeArea->getPlainDivider().setDividerGap(dividerGap);
}

void ParticlesPhysicsManager::setTemperature( PlanePart part, DataFormat format, double temperature )
{
    if( format == DataFormat::PERCENT )
    {
        Ensures( temperature>=0 && temperature<=100 );

        analyzer->physicsInfo.temperature[part] = (sqrt(analyzer->physicsInfo.maxRapidity*2.0)/2.0)*temperature*0.01;
    }
    else analyzer->physicsInfo.temperature[part] = temperature;
}

void ParticlesPhysicsManager::setForce( Axis type, DataFormat format, double force )
{
    if( format == DataFormat::PERCENT )
    {
        Ensures( force>=-100 && force<=100 );
        force = 0.01*force*analyzer->physicsInfo.maxSideForce;
    }

    if( type == Axis::HORIZONTAL ) analyzer->physicsInfo.pushForce.x = force;
    else analyzer->physicsInfo.pushForce.y = force;
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

void ParticlesPhysicsManager::updateParticlesLocationInPlane()
{
    analyzer->physicsInfo.numBlueParticlesRight = analyzer->physicsInfo.numBlueParticlesLeft = 0;
    analyzer->physicsInfo.numRedParticlesRight = analyzer->physicsInfo.numRedParticlesLeft = 0;

    for( auto &particle : *particles )
    {
        if( particle.particleType == ParticleType::BLUE )
        {
            if( particle.position.x>planeArea->getPlainDivider().getDividerPosX() ) analyzer->physicsInfo.numBlueParticlesRight++;
            else analyzer->physicsInfo.numBlueParticlesLeft++;
        }
        else if( particle.particleType == ParticleType::RED )
        {
            if( particle.position.x>planeArea->getPlainDivider().getDividerPosX() ) analyzer->physicsInfo.numRedParticlesRight++;
            else analyzer->physicsInfo.numRedParticlesLeft++;
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

void ParticlesPhysicsManager::handleParticleClusterTransition( const iterParticle particle )
{
    auto iterCurrent = getClusterIter(static_cast<size_t>(particle->position.x),static_cast<size_t>(particle->position.y));
    if( iterCurrent != particle->cluster )
    {
        iterCurrent->addParticle( particle );
        particle->cluster->removeParticle( particle );
        particle->cluster = iterCurrent;
    }
}

template< SimulationType type >
void ParticlesPhysicsManager::handleParticleCollisions( const iterParticle particle )
{

    double dotProductV1V2 {0.0};
    double dotProductV2V1 {0.0};
    double distance {0.0};
    double minDistance {0.0};

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

                dotProductV1V2 = (particle->velocity-otherParticle->velocity)*(particle->position-otherParticle->position);
                dotProductV2V1 = (otherParticle->velocity-particle->velocity)*(otherParticle->position-particle->position);

                particle->velocity -= (particle->position-otherParticle->position) *
                                        ((2*otherParticle->mass/(otherParticle->mass+particle->mass)) *
                                         dotProductV1V2/~(particle->position-otherParticle->position)) ;

                otherParticle->velocity -= (otherParticle->position-particle->position) *
                                             ((2*particle->mass/(otherParticle->mass+particle->mass)) *
                                              dotProductV2V1/~(otherParticle->position-particle->position)) ;


            }

            if constexpr ( type == SimulationType::SANDBOX )
            {                
                if( analyzer->physicsInfo.attractionForce!=0.0 )
                {
                    vect2D direction = (particle->position-otherParticle->position).setLength(1.0);
                    double factor    = analyzer->physicsInfo.attractionForce*calculationPeriod/distance;

                    particle->velocity += direction*factor*(-1.0)*otherParticle->mass;
                    otherParticle->velocity += direction*factor*particle->mass;
                }
            }

        }
    }

}

void ParticlesPhysicsManager::handleParticleCollisionsAlternative( const iterParticle particle )
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
            else if( simulationType == SimulationType::SANDBOX && analyzer->physicsInfo.attractionForce!=0.0 )
            {                
                double distanceCut = ( distance<minDistance ) ? minDistance : distance ;
                vect2D direction = (particle->position-otherParticle->position).setLength(1.0);
                double factor    = analyzer->physicsInfo.attractionForce*calculationPeriod/distanceCut;

                particle->velocity += direction*factor*(-1.0)*otherParticle->mass;
                otherParticle->velocity += direction*factor*particle->mass;
            }

        }
    }

}

template< SimulationType type >
double ParticlesPhysicsManager::handleParticleCollisionWithPlaneBoundries( const iterParticle particle )
{
    vect2D newPosition = particle->calculateNextPosition(timeContribution);
    double kineticEnergy {0.0};
    double temperature {0};

    if constexpr( type == SimulationType::BASIC )
    {
        temperature = analyzer->physicsInfo.temperature[PlanePart::WHOLE];
    }   
    if constexpr( type == SimulationType::DIFFUSION )
    {
        temperature = ( newPosition.x < planeArea->getPlainDivider().getDividerPosX() ) ? analyzer->physicsInfo.temperature[PlanePart::LEFTBOX] : analyzer->physicsInfo.temperature[PlanePart::RIGHTBOX];
    }

    if( newPosition.x - particle->radius <= planeArea->getXConstraint() )
    {
        if constexpr ( type == SimulationType::SANDBOX ) temperature = analyzer->physicsInfo.temperature[PlanePart::LEFT];
        particle->position.x = particle->radius + planeArea->getXConstraint();
        kineticEnergy = abs(particle->velocity.x)*particle->mass*0.5;
        if constexpr ( type == SimulationType::BROWNIAN_MOTION ) particle->velocity.x = (-1.0)*particle->velocity.x;
        else particle->velocity.x = temperature;
    }
    if( newPosition.x + particle->radius >= planeArea->getWidth() - planeArea->getXConstraint() )
    {
        if constexpr ( type == SimulationType::SANDBOX ) temperature = analyzer->physicsInfo.temperature[PlanePart::RIGHT];
        particle->position.x = static_cast<double>(planeArea->getWidth()) - particle->radius - planeArea->getXConstraint();
        kineticEnergy = abs(particle->velocity.x)*particle->mass*0.5;
        if constexpr ( type == SimulationType::BROWNIAN_MOTION ) particle->velocity.x = (-1.0)*particle->velocity.x;
        else particle->velocity.x = (-1.0)*temperature;
    }
    if( newPosition.y - particle->radius <= 0 )
    {
        if constexpr ( type == SimulationType::SANDBOX ) temperature = analyzer->physicsInfo.temperature[PlanePart::UP];
        particle->position.y = particle->radius;
        kineticEnergy = abs(particle->velocity.y)*particle->mass*0.5;
        if constexpr ( type == SimulationType::BROWNIAN_MOTION ) particle->velocity.y = (-1.0)*particle->velocity.y;
        else particle->velocity.y = temperature;
    }
    if( newPosition.y + particle->radius >= planeArea->getHeight() )
    {
        if constexpr ( type == SimulationType::SANDBOX ) temperature = analyzer->physicsInfo.temperature[PlanePart::DOWN];
        particle->position.y = static_cast<double>(planeArea->getHeight()) - particle->radius;
        kineticEnergy = abs(particle->velocity.y)*particle->mass*0.5;
        if constexpr ( type == SimulationType::BROWNIAN_MOTION ) particle->velocity.y = (-1.0)*particle->velocity.y;
        else particle->velocity.y = (-1.0)*temperature;
    }

    return kineticEnergy;
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


