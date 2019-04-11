#pragma once

#include <thread>
#include "particle.h"
#include "cluster.h"
#include "planearea.h"
#include "histogram1d.h"
#include "barchart.h"
#include "bardisplay.h"  // no playing bard here

class ParticlesPhysicsManager
{

protected:

    // simulation type
    SimulationType simulationType;

    // particle visualzation type
    VisualizationType visualizationType {VisualizationType::VELOCITY};

    // contains main simulation parameters
    SimulationInfo simulationInfo;

    // contains all important global physics quantities related to particles in the plane
    PhysicsInfo physicsInfo;

    // initial state of physicsInfo quantities - assigned in definition.h
    const PhysicsInfo physicsInfoInitial;

    // plane area - contains plane divider
    std::shared_ptr<PlaneArea> planeArea;

    // selected particle for tracking or used in brownian motion as macroscopic sized
    iterParticle selectedParticle;

    // high resolution time points
    HRClock::time_point time, calculationStart;

    // calculation time for calculateNextPosition
    double calculationPeriod {0.0};

    // time contribution in calculating the next particle position
    double timeContribution {0.0};

    // map contains bar charts
    std::map<std::string,ptrBarChart> barCharts;

    // map contains bar charts
    std::map<std::string,ptrBarDisplay> barDisplays;

    // map contains 1D histograms
    std::map<std::string,ptrHistogram1D> histograms1D;   

    // flag acts out like mutex
    std::atomic<bool> calculateNextPositionFlag {false};

    // indicate if simulation was paused by user
    bool pauseByUserFlag {false};

    // store cluster`s quantitative basic information
    ClustersInfo clustersInfo;

    // store cluster`s id for each point in the plane
    table2D clusterIters;

    // particles
    ptrParticlesContainer particles;

    // clusters
    ptrClustersContainer clusters;

    // coordinates cluster with cluster iterator map
    MapClustersIterCoordinates clusterCoordinatesMap;

    // calculation state
    std::atomic<ThreadCalculationState> calculationState { ThreadCalculationState::END };

    // pointer to calculation function assigned to simulation state
    void (ParticlesPhysicsManager::*prtCalculateNextPositions)() {nullptr};

    // get cluster iter or throw exception
    iterCluster getClusterIter( const size_t& posx , const size_t& posy );

    // caluclate next particles positions
    void calculateNextPositions();

    // run calculateNextPositions in while() loop
    void calculateNextPositionsLoop();

    colorRGB getRandomColor();

    // update number of red and blue particles inside right and left plane
    void updateParticlesLocationInPlane();

    // get random position of particle being disjoint with other particles
    // minx,maxx,miny,miny : define area where position is getting randomly
    // radius : particle radius
    vect2D getDisjointRandomParticlePosition( double minx, double maxx, double miny, double maxy, double radius );

    // handle particle transition between clusters
    void handleParticleClusterTransition(iterParticle&);

    // handle posible collision of a given particle with other particles
    void handleParticleCollisions(iterParticle&);

    // handle posible collision of a given particle with other particles using alternative algorithm
    void handleParticleCollisionsAlternative(iterParticle&);

    // handle possible collision of a given particle with a plane boundry, return particle kinetic energy that hit the plane or zero if doesn`t
    double handleParticleCollisionWithPlaneBoundries(iterParticle&);

    // check if the given particle position whith its radius overlap with others
    bool isParticlesOverlap( const vect2D& , const double& );

    // create initial set of particles - using in constructor and reset()
    void createParticles();

    // tries to add particles into plane, return true if success
    // particleType - type of particle depends on simulation mode ( e.g. all BLUE particles are located in the left part of particle plane )
    // visualizationType - particle type visualization
    // quantity - number of particles to add
    // particleSize - particles size
    bool addParticles( ParticleType particleType, VisualizationType visualizationType, int quantity, int particleSize );

    // tries to remove particles from plane, return true if success
    // particleType - type of particle to remove
    // quantity - number of particles to remove
    bool removeParticles( ParticleType particleType, int quantity );

    // remove all particles
    void removeAllParticles();

    // random shuffle of particle position inside the plane
    void shuffleParticle( iterParticle& );

    // disjoint a given particle (if is too close to other) by removing it and place somewhere else inside the particle plane
    bool disjoint( iterParticle& );

    // correct particles parameters ( positions and velocities ) if they are invalid ( out of range )
    void correctParticlesInvalidParameters();

    // disjoint overlap particles
    // impactFactor - value from 0 to 1 range using in overlap detection, "if( distance < impactFactor*(particle1->radius + particle2->radius) )"
    void disjointPositions( double impactFactor );

    // preserve particle position inside plane
    void preserveParticleInPlane( iterParticle& );

    // remove all particles from clusters
    void removeParticlesFromClusters();

    // recalculate particles in clusters
    void recalculateParticlesInClusters();

    // connect clusters
    void connectClusters();

    // create clusters
    void createClusters();

    // populate given rectangle of clusterIters with a given cluster
    void populateClusterID( int xstart, int ystart, int xlength, int ylength, iterCluster cluster );

public:

    ParticlesPhysicsManager( SimulationType type, int planeWidth, int planeHeight );
    ParticlesPhysicsManager() = delete;
    ParticlesPhysicsManager( const ParticlesPhysicsManager& ) = delete;
    ParticlesPhysicsManager( ParticlesPhysicsManager&& ) = delete;    

    ParticlesPhysicsManager& operator=( const ParticlesPhysicsManager& ) = delete;
    ParticlesPhysicsManager& operator=( ParticlesPhysicsManager&& ) = delete;    

    std::thread calculateNextPositionsInThread()
    {
        calculationState = ThreadCalculationState::RUNNING;
        calculationStart = HRClock::now();
        return std::thread( &ParticlesPhysicsManager::calculateNextPositionsLoop , this );
    }

    // tries to set a new number of particles in plane, return true if success
    // quantity - number of particles to set
    bool setParticlesInPlane( ParticleType particleType, int quantity );

    // tries to set a new number of particles in plane, return true if success
    // percent - percent number of avaiable particles to set
    bool setParticlesInPlaneInPercent( ParticleType particleType, int percent )
    {
       return setParticlesInPlane( particleType,static_cast<int>(simulationInfo.maxParticles[simulationType]*static_cast<int>(percent)*0.01)  );
    }

    // update all bar charts and displays - add new physics data to chart boxes
    void updateBars();

    // following getters supplies cluster unit test - to remove in the release version
    const table2D& getClusterIters() const { return clusterIters; }
    const ptrClustersContainer&  getClusters() const noexcept { return clusters; }
    ptrParticlesContainer getParticles() const noexcept { return particles; }

    int getDividerGap() const noexcept { return planeArea->getPlainDivider().getDividerGap(); }
    void setDividerGap( int dividerGap ) noexcept { planeArea->getPlainDivider().setDividerGap(dividerGap); }

    void setTemperatureInPercent( int temperature ) { physicsInfo.temperature = (sqrt(physicsInfo.maxRapidity*2)/2)*temperature*0.01; }
    inline double getTemperature() const noexcept { return physicsInfo.temperature; }
    inline int getTemperatureInPercent() const noexcept { return static_cast<int>(physicsInfo.temperature*100/physicsInfo.maxRapidity); }

    void setTemperatureLeftInPercent( int temperature ) { physicsInfo.temperatureLeft = (sqrt(physicsInfo.maxRapidity*2)/2)*temperature*0.01; }
    inline double getTemperatureLeft() const noexcept { return physicsInfo.temperatureLeft; }
    inline int getTemperatureLeftInPercent() const noexcept { return static_cast<int>(physicsInfo.temperatureLeft*100/physicsInfo.maxRapidity); }

    void setTemperatureRightInPercent( int temperature ) { physicsInfo.temperatureRight = (sqrt(physicsInfo.maxRapidity*2)/2)*temperature*0.01; }
    inline double getTemperatureRight() const noexcept { return physicsInfo.temperatureRight; }
    inline int getTemperatureRightInPercent() const noexcept { return static_cast<int>(physicsInfo.temperatureRight*100/physicsInfo.maxRapidity); }

    void setSideTemperatureInPercent( PlaneSide side, int temperature ){ physicsInfo.planeSideTemperature[side] = (sqrt(physicsInfo.maxRapidity*2)/2)*temperature*0.01; }
    inline double getSideTemperature( PlaneSide side ) const noexcept { return physicsInfo.planeSideTemperature.at(side); }
    inline int getSideTemperatureInPercent( PlaneSide side ) const noexcept { return static_cast<int>(physicsInfo.planeSideTemperature.at(side)*100/physicsInfo.maxRapidity); }

    void setHorizontalForceInPercent( int force ){ physicsInfo.pushForce.x = 0.01*force*physicsInfo.maxSideForce; }
    inline double getHorizontalForce() const noexcept { return physicsInfo.pushForce.x; }
    inline int getHorizontalForceInPercent() const noexcept { return static_cast<int>(physicsInfo.pushForce.x*100/physicsInfo.maxSideForce); }

    void setVerticalForceInPercent( int force ){ physicsInfo.pushForce.y = 0.01*force*physicsInfo.maxSideForce; }
    inline double getVerticalForce() const noexcept { return physicsInfo.pushForce.y; }
    inline int getVerticalForceInPercent() const noexcept { return static_cast<int>(physicsInfo.pushForce.y*100/physicsInfo.maxSideForce); }

    void setPushForce( vect2D force ){ physicsInfo.pushForce = force; }

    void setAverageDiffisionTemperature()
    {
        physicsInfo.temperatureRight = physicsInfo.temperatureLeft = (physicsInfo.temperatureRight + physicsInfo.temperatureLeft)/2;
    }

    void setMassOfMoleculeInPercent( int percent )
    {
        if( simulationType == SimulationType::BROWNIAN_MOTION )
        {
            particles->begin()->setParticleMassInPercent(percent);
        }
    }

    // clear trace of molecule
    void clearMoleculeTrace()
    {
        if( simulationType == SimulationType::BROWNIAN_MOTION )
        {
            particles->begin()->particlePositionsTracking.clear();
        }
    }

    // set molecule velocity to zero
    void stopMolecule()
    {
        if( simulationType == SimulationType::BROWNIAN_MOTION )
        {
            particles->begin()->velocity.set(0.0,0.0);
        }
    }

    // add vector to molecule velocity
    void pushMolecule( vect2D vector )
    {
        if( simulationType == SimulationType::BROWNIAN_MOTION )
        {
            particles->begin()->velocity += vector;
        }
    }

    double getMoleculeVelocity() const
    {
        if( simulationType == SimulationType::BROWNIAN_MOTION )
        {
            return particles->begin()->velocity();
        }
        else return 0.0;
    }

    int getPressureInPercent() const
    {
        return static_cast<int>((barCharts.at("kinetic")->getAvg()*100)/(sqrt(physicsInfo.maxRapidity*2)/2));
    }

    inline int getMaxNumberOfParticles() const noexcept { return static_cast<int>( simulationInfo.maxParticles.at(simulationType)); }
    inline int getNumberOfParticles( ParticleType type ) const noexcept { return static_cast<int>(simulationInfo.numberOfParticles.at(type)); }
    inline int getNumberOfParticlesInPercent() const noexcept { return static_cast<int>(particles->size())*100/simulationInfo.maxParticles.at(simulationType); }

    int inline getAvgCalculationCount() const noexcept { return simulationInfo.avgCalculationCount; }

    int getSizeOfParticle( ParticleType type=ParticleType::NORMAL ) noexcept
    {
        return simulationInfo.particleSize[type];
    }

    int getSizeOfParticleInPercent( ParticleType type=ParticleType::NORMAL ) noexcept
    {
        return static_cast<int>((simulationInfo.particleSize[type]-simulationInfo.minSizeOfParticle)*100/(simulationInfo.maxSizeOfParticle[simulationType]-simulationInfo.minSizeOfParticle));
    }

    void setAttractionForceInPercent( int quantity );

    void setSizeOfParticlesInPercent( ParticleType type, int quantity );

    void setPlaneWidthInPercent( int quantity );

    const SimulationInfo& getSimulationInfo() const noexcept { return simulationInfo; }

    const ClustersInfo& getClustersInfo() const noexcept { return clustersInfo; }

    const PhysicsInfo& getPhysicsInfo() const noexcept { return physicsInfo; }

    // enable tracking of selected particle
    void enableTracking();

    // disable tracking of selected particle
    void disableTracking();

    // reset the simulation to the initial state
    void reset();

    // chnage particle visualization type
    void setVisualizationType( VisualizationType type );

    void pause( bool userCall = false )
    {
        if( calculationState.load() == ThreadCalculationState::RUNNING )
        {
            calculationState.store(ThreadCalculationState::PAUSE);
            if( userCall ) pauseByUserFlag = true;
            while( calculateNextPositionFlag.load() ){}; // wait for the end of caluclation step
        }
    }

    void run( bool userCall = false )
    {
        if( calculationState.load() == ThreadCalculationState::PAUSE )
        {            
            if( userCall ) pauseByUserFlag = false;
            calculationStart = HRClock::now();
            calculationState.store( ThreadCalculationState::RUNNING );
        }
    }

    void end()
    {
        calculationState = ThreadCalculationState::END;
    }    

    std::string getCalculationState() const noexcept
    {
        if( calculationState.load() == ThreadCalculationState::RUNNING ) return "Running";
        if( calculationState.load() == ThreadCalculationState::PAUSE   ) return "Pause";
        if( calculationState.load() == ThreadCalculationState::END     ) return "End";

        return "undefined";
    }

    bool isPauseByUser() const noexcept { return pauseByUserFlag; }

};
