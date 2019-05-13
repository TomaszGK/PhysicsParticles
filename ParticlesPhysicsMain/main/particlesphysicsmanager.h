#pragma once

#include <thread>
#include "particle.h"
#include "cluster.h"
#include "planearea.h"
#include "histogram1d.h"
#include "barchart.h"
#include "bardisplay.h"  // no playing bard here

/** @file
 * @brief Class @ref ParticlesPhysicsManager
 */

/**
 * @class ParticlesPhysicsManager
 * @brief Manages particles.
 *
 * Creates of clusters and populates them with particles, calculates next particle positions,
 * handles particle collisions and check particle collisions with plane and divider.
 * @author Tomasz Gburek
 * @date 2019
 */

class ParticlesPhysicsManager
{   

public:

    /**
     * @brief Constructor
     *
     * @param type                  simulation type
     * @param planeWidth            plane width
     * @param planeHeight           plane height
     */
    ParticlesPhysicsManager( SimulationType type, int planeWidth, int planeHeight );

    /** @brief Default Constructor */
    ParticlesPhysicsManager() = delete;

    /** @brief Copy constructor */
    ParticlesPhysicsManager( const ParticlesPhysicsManager& ) = delete;

    /** @brief Move constructor */
    ParticlesPhysicsManager( ParticlesPhysicsManager&& ) = delete;    

    /** @brief Copy assigment operator */
    ParticlesPhysicsManager& operator=( const ParticlesPhysicsManager& ) = delete;

    /** @brief Move assigment operator */
    ParticlesPhysicsManager& operator=( ParticlesPhysicsManager&& ) = delete;    

    /**
     * @brief Creates thread where new particle position are calculating in loop.
     *
     * Creates thread with function call calculateNextPositionsLoop.
     * @return created thread
     */
    std::thread calculateNextPositionsInThread()
    {
        calculationState = ThreadCalculationState::RUNNING;
        calculationStart = HRClock::now();
        return std::thread( &ParticlesPhysicsManager::calculateNextPositionsLoop , this );
    }

    /**
     * @brief Tries to set a new number of particles in the plane.
     *
     * Uses addParticles() or removeParticles() to set correct number of particles in the plane.
     * @param particleType          particle type
     * @param quantity              number of particles
     * @return true if success otherwise false
     */
    bool setParticlesInPlane( ParticleType particleType, int quantity );

    /**
     * @brief Tries to set a new percent value of particles in the plane.
     *
     * @param particleType          particle type
     * @param percent               percent value of particles [0,100]
     * @return true if success otherwise false
     */
    bool setParticlesInPlaneInPercent( ParticleType particleType, int percent )
    {
       return setParticlesInPlane( particleType,static_cast<int>(simulationInfo.maxParticles[simulationType]*static_cast<int>(percent)*0.01)  );
    }

    /** Updates all bar charts and displays, adds new physics values to chart boxes */
    void updateBars();

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
            while( calculateNextPositionFlag.load() ){}; // wait for the end of calculation step
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

protected:

    /** Simulation type */
    SimulationType simulationType;

    /** Visualization type */
    VisualizationType visualizationType {VisualizationType::VELOCITY};

    /** Contains main simulation parameters */
    SimulationInfo simulationInfo;

    /** Contains global physics quantities describes states of particles */
    PhysicsInfo physicsInfo;

    /** Initial state of physicsInfo quantities */
    const PhysicsInfo physicsInfoInitial;

    /** Holds PlaneArea object storing basic information about particle plane */
    std::shared_ptr<PlaneArea> planeArea;

    /** @brief Points out to selected particle.
     *
     *  Used in SimulationType::BASIC to select tracking particle
     *  and in SimulationType::BROWNIAN_MOTION to select particle having macroscopic size.
     */
    iterParticle selectedParticle;

    /** High resolution time points */
    HRClock::time_point time, calculationStart;

    /** Calculation time period of calculateNextPosition call. */
    double calculationPeriod {0.0};

    /** @brief Time contribution in calculation of the next particle position
     *
     *  Used as time (t) in equation s_new = s_old + v*t, where
     *  s_new is new position of particle
     *  s_old is old position of particle
     *  v is velocity of particle
     */
    double timeContribution {0.0};

    /** Contains bar charts */
    std::map<std::string,ptrBarChart> barCharts;

    /** Contains bar displays */
    std::map<std::string,ptrBarDisplay> barDisplays;

    /** Contains histograms 1D */
    std::map<std::string,ptrHistogram1D> histograms1D;

    /** Atomic flag acts out like simple mutex to prevents particles modification before the end of calculation */
    std::atomic<bool> calculateNextPositionFlag {false};

    /** Flag indicates if simulation was paused by user */
    bool pauseByUserFlag {false};

    /** Stores cluster`s basic information */
    ClustersInfo clustersInfo;

    /** Stores cluster iterators for each point (represented by pixel) located in the plane */
    table2D clusterIters;

    /** Holds vector of particles */
    ptrParticlesContainer particles;

    /** Holds vector of clusters */
    ptrClustersContainer clusters;

    /** Maps cluster coordinates with its iterator */
    MapClustersIterCoordinates clusterCoordinatesMap;

    /** Calculation state */
    std::atomic<ThreadCalculationState> calculationState { ThreadCalculationState::END };

    /** Pointer to calculation function assigned to a given simulation state */
    void (ParticlesPhysicsManager::*prtCalculateNextPositions)() {nullptr};

    /** @brief Gets cluster iterator
     *
     * Gets cluster iterator from a given position (x,y).
     * Throws exception if position is out of the plane.
     * @param posx        x position of point (x,y)
     * @param posx        y position of point (x,y)
     * @return iterator to the cluster
     */
    iterCluster getClusterIter( const size_t& posx , const size_t& posy );

    /** Caluclates next particles positions */
    void calculateNextPositions();

    /** Calls calculateNextPositions in while() loop
     *
     * The following loop is running in separate thread.
     */
    void calculateNextPositionsLoop();

    /** Gets random color
     * @return random RGB color
     */
    colorRGB getRandomColor();

    /** @brief Updates number of ParticleType::RED and ParticleType::BLUE.
     *
     * Calculates numbers of ParticleType::RED and ParticleType::BLUE particles
     * inside right and left rectangle areas splited by plane divider.
     */
    void updateParticlesLocationInPlane();

    /** @brief Gets uncollided random position of particle in a given area.
     *
     * Tries to get random particle position from rectangle (minx,maxx,miny,maxy).
     * A returned position do not overalap with others. Using to disjoint overlap particles.
     * @param minx        minimum x in (minx,maxx,miny,maxy) area
     * @param maxx        maximum x in (minx,maxx,miny,maxy) area
     * @param miny        minimum y in (minx,maxx,miny,maxy) area
     * @param maxy        maximum y in (minx,maxx,miny,maxy) area
     * @param radius      particle radius
     * @return random position from (minx,maxx,miny,maxy) area
     */
    vect2D getDisjointRandomParticlePosition( double minx, double maxx, double miny, double maxy, double radius );

    /** @brief Handles particle transition between clusters.
     *
     * Checks if particle have been transfered between clusters,
     * and then changes appropriate cluster iterator values.
     * @param particle    iterator to particle
     */
    void handleParticleClusterTransition( const iterParticle& particle );

    /** @brief Handles particle collisions.
     *
     * Checks if particle have been collided with other particles in belonging cluster.
     * All collided particles are recalculated their vector velocities to comply rebound behavior.
     * @param particle    iterator to particle
     */
    void handleParticleCollisions( const iterParticle& particle );

    /** @brief Handles particle collisions using alternative algorithm.
     *
     * Checks if particle have been collided with other particles in belonging cluster.
     * All collided particles are recalculated their vector velocities to comply rebound behavior.
     * @param particle    iterator to particle
     */
    void handleParticleCollisionsAlternative( const iterParticle& particle );

    /** @brief Handles particle collisions with a plane boundry.
     *
     * Checks if particle have been collided with plane boundry.
     * All particle collided with plane boundry are recalculated thier vector velocities.
     * @param particle    iterator to particle
     * @return particle kinetic energy if collision occured or zero otherwise
     */
    double handleParticleCollisionWithPlaneBoundries( const iterParticle& particle );

    /** @brief Checks if particle overlaped with others.
     *
     * Checks if particle defined by position and radius is overlaped with other particles.
     * @param particlePosition    position of particle
     * @param radius              radius of particle
     * @return true if overlap
     */
    bool isParticlesOverlap( const vect2D& particlePosition, double radius );

    /** Creates initial set of particles. */
    void createParticles();

    /** @brief Tries to add particles into plane in random position.
     *
     * Type of particle may imply its futher position restriction,
     * e.g. all ParticleType::BLUE type particles are located in the left part of plane.
     * @param particleType          particle type
     * @param visualizationType     visualization type
     * @param quantity              number of particles to add
     * @param particleSize          particle size
     * @return true if success otherwise false
     */
    bool addParticles( ParticleType particleType, VisualizationType visualizationType, int quantity, int particleSize );

    /** @brief Tries to remove particles of a given type from plane.
     *
     * @param particleType          particle type
     * @param quantity              number of particles to remove
     * @return true if success otherwise false
     */
    bool removeParticles( ParticleType particleType, int quantity );

    /** Remove all particles from plane. */
    void removeAllParticles();

    /** @brief Shuffles particle position to the random position.
     *
     * @param particle    iterator to particle
     */
    void shuffleParticle( const iterParticle& particle );

    /** @brief Tries disjoint particle.
     *
     * Disjoints particle if its position is too close to other particles by shuffles it position.
     * @param particle              iterator to particle
     * @return true if disjont success otherwise false
     */
    bool disjoint( const iterParticle& particle );

    /** @brief Corrects particles invalid parameters.
     *
     * Corrects particle position and velocity invalid parameters (e.g. out of range).
     */
    void correctParticlesInvalidParameters();

    /** @brief Disjoints all overlap particles.
     *
     * Searches for overlap particles and disjoints them by moving away from each other.
     * @param impactFactor          value from [0,1] using in overlap detection precision
     */
    void disjointPositions( double impactFactor );

    /** @brief Preserves particle position inside plane.
     *
     * Checks if particle position is outside the plane
     * and modifies its position to fit to plane borders.
     * @param particle              iterator to particle
     */
    void preserveParticleInPlane( const iterParticle& particle );

    /** Removes all particles from all clusters */
    void removeParticlesFromClusters();

    /** Recalculates particle iterators in all clusters and cluster iterator for each particle. */
    void recalculateParticlesInClusters();

    /** Connects clusters */
    void connectClusters();

    /** Creates clusters */
    void createClusters();

    /** @brief Populates clusterIters in a given rectangle area with cluster iterator.
     *
     * @param xstart                rectangle xstart
     * @param ystart                rectangle ystart
     * @param xlength               rectangle xlength
     * @param ylength               rectangle ylength
     * @param cluster               cluster iterator
     */
    void populateClusterID( int xstart, int ystart, int xlength, int ylength, iterCluster cluster );

};
