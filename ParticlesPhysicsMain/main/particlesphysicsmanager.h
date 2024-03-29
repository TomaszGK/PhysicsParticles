#pragma once

#include <thread>
#include "cluster.h"
#include "planearea.h"
#include "simulationanalyzer.h"

/** @file
 * @brief Class @ref ParticlesPhysicsManager
 */

/**
 * @class ParticlesPhysicsManager
 * @brief Manages particles.
 *
 * Creates of clusters and populates them with particles, calculates next particle positions,
 * handles particle collisions and check if particles hit the plane or collide with its divider.
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

    /** @brief Virtual empty deconstructor */
    virtual ~ParticlesPhysicsManager();

    /** @brief Copy assigment operator */
    ParticlesPhysicsManager& operator=( const ParticlesPhysicsManager& ) = delete;

    /** @brief Move assigment operator */
    ParticlesPhysicsManager& operator=( ParticlesPhysicsManager&& ) = delete;    

    /**
     * @brief Gets number of particles (for a given type) in the plane.
     *
     * @param format                data format
     * @param type                  particle type
     * @return number of particles
     */
    double getNumberOfParticlesInPlane( ParticleType type, DataFormat format ) const noexcept
    {
        return ( format == DataFormat::PERCENT ) ? analyzer->simulationInfo.numberOfParticles.at(type)*100.0/analyzer->simulationInfo.maxParticles.at(simulationType) : analyzer->simulationInfo.numberOfParticles.at(type);
    }

    /**
     * @brief Tries to set a new number of particles in the plane.
     *
     * Uses addParticles() or removeParticles() to set correct number of particles in the plane.
     * @param particleType          particle type
     * @param format                data format ( percent or scalar )
     * @param quantity              value consider as number of particles or percent value of particles [0,100]
     * @return true if success otherwise false
     */
    bool setNumberOfParticlesInPlane( ParticleType particleType, DataFormat format, int quantity );

    /** Updates all bar charts and displays, adds new physics values to chart boxes */
    void updateBars();

    /**
     * @brief Gets percent value of gap divider size
     * @return gap divider value in percent [0,100]
     */
    int getDividerGap() const noexcept { return planeArea->getPlainDivider().getDividerGap(); }

    /**
     * @brief Sets size of gap divider in percent.
     *
     * @param dividerGap            new divider value in percent [0,100]
     */
    void setDividerGap( int dividerGap );

    /**
     * @brief Gets value of temperature in the particle plane.
     *
     * @param part                  part type of particle plane
     * @param format                data format ( percent or scalar )
     * @return temperature value
     */
    double getTemperature( PlanePart part, DataFormat format ) const noexcept
    {
        return ( format == DataFormat::PERCENT ) ? analyzer->physicsInfo.temperature.at(part)*100.0/(sqrt(analyzer->physicsInfo.maxRapidity*2.0)/2.0) : analyzer->physicsInfo.temperature.at(part);
    }

    /**
     * @brief Sets percent value of temperature in the particle plane.
     *
     * @param part                  part type of particle plane
     * @param format                data format ( percent or scalar )
     * @param temperature           new temperature
     */
    void setTemperature( PlanePart part, DataFormat format, double temperature );

    /**
     * @brief Gets value of force.
     *
     * Gets value of force along horizontal or vertical axis.
     * @param type                  type of axis
     * @param type                  data format
     * @return force value
     */
    double getForce( Axis type, DataFormat format ) const noexcept
    {
        auto force = ( type == Axis::HORIZONTAL ) ? analyzer->physicsInfo.pushForce.x : analyzer->physicsInfo.pushForce.y;
        if( format == DataFormat::PERCENT ) force = force*100/analyzer->physicsInfo.maxSideForce;

        return force;
    }

    /**
     * @brief Sets percent value of horizontal force.
     *
     * @param type                  type of axis along a given force operate
     * @param type                  data format
     * @param force                 new force
     */
    void setForce( Axis type, DataFormat format, double force );

    /**
     * @brief Sets value of push force.
     *
     * Using in SimulationType::BROWNIAN_MOTION
     * @param force                 new push force
     */
    void setForce( vect2D force ){ analyzer->physicsInfo.pushForce = force; }

    /**
     * @brief Sets (calculates) average value of diffiusion temperature.
     *
     * Using in SimulationType::DIFFUSION
     */
    void setAverageDiffusionTemperature()
    {
        analyzer->physicsInfo.temperature[PlanePart::RIGHTBOX] = analyzer->physicsInfo.temperature[PlanePart::LEFTBOX] = (analyzer->physicsInfo.temperature[PlanePart::RIGHTBOX] + analyzer->physicsInfo.temperature[PlanePart::LEFTBOX])/2.0;
    }

    /**
     * @brief Sets percent value of the molecule mass.
     *
     * Using in SimulationType::BROWNIAN_MOTION
     * @param percent               new molecule mass in percent [0,100]
     */
    void setMassOfMoleculeInPercent( int percent );

    /** Clears trace of the molecule. */
    void clearMoleculeTrace()
    {
        if( simulationType == SimulationType::BROWNIAN_MOTION )
        {
            particles->begin()->particlePositionsTracking.clear();
        }        
    }

    /** Changes molecule velocity to zero. */
    void stopMolecule()
    {
        if( simulationType == SimulationType::BROWNIAN_MOTION )
        {
            particles->begin()->velocity.set(0.0,0.0);
        }
    }

    /**
     * @brief Adds vector to molecule velocity.
     * @param vector                adds to molecule velocity
     */
    void pushMolecule( vect2D vector )
    {
        if( simulationType == SimulationType::BROWNIAN_MOTION )
        {
            particles->begin()->velocity += vector;
        }
    }

    /**
     * @brief Gets molecule velocity.
     * @return molecule velocity
     */
    double getMoleculeVelocity() const
    {
        if( simulationType == SimulationType::BROWNIAN_MOTION )
        {
            return particles->begin()->velocity();
        }
        else return 0.0;
    }

    /**
     * @brief Gets (calculates) value of pressure in the particle plane.
     * @return current pressure value
     */
    int getPressureValue() const
    {
        return static_cast<int>(100*analyzer->barCharts->at(ActionType::M_KINETIC)->getAvgInLast(5));
    }

    /**
     * @brief Gets maximum number of particles in the plane.
     * @return maximum number of particles
     */
    int getMaxNumberOfParticles() const noexcept { return static_cast<int>( analyzer->simulationInfo.maxParticles.at(simulationType)); }

    /**
     * @brief Gets average number of calulations (next particles position) in the period of time.
     * @return average calculation time
     */
    int getAvgCalculationCount() const noexcept { return analyzer->simulationInfo.calculationsPerSecond; }

    /**
     * @brief Gets particle size for a given particle type.
     * @param type                  particle type
     * @param format                data format
     * @return particle size as raw scalar or percent value
     */
    int getParticleSize( ParticleType type, DataFormat format = DataFormat::SCALAR ) const noexcept
    {
        return ( format == DataFormat::PERCENT ) ? static_cast<int>((analyzer->simulationInfo.particleSize.at(type)-analyzer->simulationInfo.minSizeOfParticle)*100/(analyzer->simulationInfo.maxSizeOfParticle.at(simulationType)-analyzer->simulationInfo.minSizeOfParticle)) : analyzer->simulationInfo.particleSize.at(type);
    }    

    /**
     * @brief Sets new particle size for all particles having the same type.
     * @param type                  particle type
     * @param format                data format ( scalar or percent )
     * @param quantity              new raw or percent value of particle size
     */
    void setParticleSize( ParticleType type, DataFormat format, int quantity );

    /**
     * @brief Sets percent value of attraction force.
     * @param quantity              new percent value of attraction force [-100,100]
     */
    void setAttractionForceInPercent( int quantity );

    /**
     * @brief Gets percent value of attraction force.
     * @return percent value of attraction force [-100,100]
     */
    double getAttractionForceInPercent() const noexcept
    {
        return analyzer->physicsInfo.attractionForce*100/analyzer->physicsInfo.maxAttractionForce;
    }

    /**
     * @brief Sets percent value of plane width.
     * @param quantity              new percent value of plane width
     */
    virtual void setPlaneWidthInPercent( int quantity );

    /** Gets simulation information. */
    const SimulationInfo& getSimulationInfo() const noexcept { return analyzer->simulationInfo; }

    /** Gets clusters information. */
    const ClustersInfo& getClustersInfo() const noexcept { return clustersInfo; }

    /** Gets physics information. */
    const PhysicsInfo& getPhysicsInfo() const noexcept { return analyzer->physicsInfo; }

    /** Enables tracking of selected particle. */
    virtual void enableTracking();

    /** Disables tracking of selected particle. */
    virtual void disableTracking();

    /** Resets the simulation to the initial state */
    void reset();

    /**
     * @brief Changes particle visualization type.
     * @param type                  visualization type
     */
    void setVisualizationType( VisualizationType type );

    /**
     * @brief Pauses simulation.
     *
     * Value userCall have to be true if pause is called from a user action.
     * @param userCall              use call flag
     */
    virtual void pause( bool userCall = false )
    {
        if( calculationState.load() == ThreadCalculationState::RUNNING )
        {
            pUpdate = std::promise<bool>();
            fUpdate = pUpdate.get_future();
            calculationState.store(ThreadCalculationState::PAUSE);
            if( userCall ) pauseByUserFlag = true;            
            while( fUpdate.wait_for(std::chrono::milliseconds(1)) != std::future_status::ready );
        }
    }

    /**
     * @brief Runs simulation.
     *
     * Value userCall have to be true if run is called from a user action.
     * @param userCall              use call flag
     */
    virtual void run( bool userCall = false )
    {
        if( calculationState.load() == ThreadCalculationState::PAUSE )
        {            
            if( userCall ) pauseByUserFlag = false;

            if( !pauseByUserFlag )
            {
                calculationStart = HRClock::now();
                calculationState.store( ThreadCalculationState::RUNNING );
            }
        }
    }

    /** Ends simulation by breaking loop in the caluclation thread. */
    void end()
    {
        calculationState.store( ThreadCalculationState::END );
    }    

    /**
     * @brief Gets current calculation state.
     * @return calculation state
     */
    std::string getCalculationState() const noexcept
    {
        if( calculationState.load() == ThreadCalculationState::RUNNING ) return "Running";
        if( calculationState.load() == ThreadCalculationState::PAUSE   ) return "Pause";
        if( calculationState.load() == ThreadCalculationState::END     ) return "End";

        return "undefined";
    }

    /**
     * @brief Gets information if pause was called by user.
     * @return true if pause was called by user otherwise false
     */
    bool isPauseByUser() const noexcept { return pauseByUserFlag; }

    /**
     * @brief Creates thread where new particle position are calculating in loop.
     *
     * Creates thread and calling function calculateNextPositionsLoop.
     * @return created thread
     */    
    std::thread calculateNextPositionsInThread()
    {
        calculationState = ThreadCalculationState::RUNNING;        
        calculationStart = HRClock::now();
        switch( simulationType )
        {
         case SimulationType::BASIC           : return std::thread( &ParticlesPhysicsManager::mainLoop<SimulationType::BASIC> , this );
         case SimulationType::DIFFUSION       : return std::thread( &ParticlesPhysicsManager::mainLoop<SimulationType::DIFFUSION> , this );
         case SimulationType::BROWNIAN_MOTION : return std::thread( &ParticlesPhysicsManager::mainLoop<SimulationType::BROWNIAN_MOTION> , this );
         case SimulationType::SANDBOX         : return std::thread( &ParticlesPhysicsManager::mainLoop<SimulationType::SANDBOX> , this );         
        }

        return std::thread( [](){} );
    }

protected:

    /** Simulation type */
    const SimulationType simulationType;

    /** Visualization type */
    VisualizationType visualizationType {VisualizationType::VELOCITY};

    /** Holds @ref SimulationAnalyzer object */
    ptrAnalyzer analyzer;

    /** Holds @ref PlaneArea object storing basic information about particle plane */
    ptrPlaneArea planeArea;

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

    /** Promise value used in synchronization of update process after pause invoke */
    std::promise<bool> pUpdate;

    /** Future value used in synchronization of update process after pause invoke */
    std::future<bool> fUpdate;

    /** @brief Gets cluster iterator
     *
     * Gets cluster iterator from a given position (x,y).
     * Throws exception if position is out of the plane.
     * @param posx        x position of point (x,y)
     * @param posx        y position of point (x,y)
     * @return iterator to the cluster
     */
    iterCluster getClusterIter( const size_t& posx , const size_t& posy )
    {
        // if position is out of range then exception is thrown
        return clusterIters.at(posx).at(posy);
    }

    /** Updates particles positions */
    template< SimulationType type >
    void update();

    /** Calls updates in while() loop
     *
     * The following loop should be running in separate thread.
     */
    template< SimulationType type >
    void mainLoop();

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

    /** @brief Tries to get uncollided random position of particle in a given area.
     *
     * Tries to get random particle position from rectangle (minx,maxx,miny,maxy).
     * A returned position should do not overalap with others. Using to disjoint overlap particles.
     * @param minx        minimum x in (minx,maxx,miny,maxy) area
     * @param maxx        maximum x in (minx,maxx,miny,maxy) area
     * @param miny        minimum y in (minx,maxx,miny,maxy) area
     * @param maxy        maximum y in (minx,maxx,miny,maxy) area
     * @param radius      particle radius     
     * @return random position from (minx,maxx,miny,maxy) area
     */
    vect2D getDisjointRandomParticlePosition( double minx, double maxx, double miny, double maxy, double radius );

    /** @brief Tries to get uncollided random position of particle in a given area.
     *
     * Tries to get random particle position from rectangle (minx,maxx,miny,maxy).
     * @param minx        minimum x in (minx,maxx,miny,maxy) area
     * @param maxx        maximum x in (minx,maxx,miny,maxy) area
     * @param miny        minimum y in (minx,maxx,miny,maxy) area
     * @param maxy        maximum y in (minx,maxx,miny,maxy) area
     * @param radius      particle radius
     * @param tries       number of maximum tries to get not overlap particles
     * @return random position from (minx,maxx,miny,maxy) area
     */
    vect2D getDisjointRandomParticlePositionTries( double minx, double maxx, double miny, double maxy, double radius, int tries );

    /**
     * @brief Tries to set a new particle position.
     *
     * Sets a given particle position only if
     * (1) a new position does not overlap with others particle positions
     * (2) a new position is place inside plane boundries
     * (3) simulation is not running
     * @param particle              iterator to particle
     * @param position              particle new position
     * @return true if success otherwise false
     */
    bool setParticlePosition( const iterParticle particle , const vect2D& position );

    /**
     * @brief Sets a new particle velocity.
     *
     * Sets a given particle velocity only if simulation is not running
     * @param particle              iterator to particle
     * @param velocity              particle new velocity
     * @return true if success otherwise false
     */
    bool setParticleVelocity( const iterParticle particle , const vect2D& velocity );

    /**
     * @brief Checks if a new particle position is valid.
     *
     * Checks if a new particle positis is inside particle plane
     * and if is not overlaped with others particles.
     * @param particle              particle iterator
     * @param position              particle new position
     * @return true if success otherwise false
     */
    bool isValidParticlePosition( citerParticle particle , const vect2D& position ) const;

    /** @brief Handles particle transition between clusters.
     *
     * Checks if particle have been transfered between clusters,
     * and then changes appropriate cluster iterator values.
     * @param particle    iterator to particle
     */
    void handleParticleClusterTransition( const iterParticle particle );

    /** @brief Handles particle collisions.
     *
     * Checks if particle have been collided with other particles in belonging cluster.
     * All collided particles are recalculated their vector velocities to comply rebound behavior.
     * @param particle    iterator to particle
     */
    template< SimulationType type >
    void handleParticleCollisions( const iterParticle particle );

    /** @brief Handles particle collisions using alternative algorithm.
     *
     * Checks if particle have been collided with other particles in belonging cluster.
     * All collided particles are recalculated their vector velocities to comply rebound behavior.
     * @param particle    iterator to particle
     */
    void handleParticleCollisionsAlternative( const iterParticle particle );

    /** @brief Handles particle collisions with a plane boundry.
     *
     * Checks if particle have been collided with plane boundry.
     * All particle collided with plane boundry are recalculated thier vector velocities.
     * @param particle    iterator to particle
     * @return particle kinetic energy if collision occured or zero otherwise
     */
    template< SimulationType type >
    double handleParticleCollisionWithPlaneBoundries( const iterParticle particle );

    /** @brief Checks if particle is overlaped with others.
     *
     * Checks if particle defined by position and radius is overlaped with other particles.
     * @param particlePosition    position of particle
     * @param radius              radius of particle
     * @return true if overlap
     */
    bool isParticlesOverlap( const vect2D& particlePosition, double radius );

    /** @brief Checks if particle is overlaped with others.
     *
     * Checks if particle is overlaped with other particles.
     * @param particle            particle iterator
     * @param newPosition         particle new position
     * @return true if overlap
     */
    bool isParticlesOverlap( const citerParticle particle , const vect2D& newPosition ) const;

    /** @brief Checks if particle type is gas.
     *
     * Checks if particle type is GAS1 or GAS2 or GAS3.
     * @param particleType        particle  type
     * @return true if particle type is gas otherwise is false
     */
    bool isParticleTypeGas( ParticleType particleType )
    {
        return ( particleType == ParticleType::GAS1   ||
                 particleType == ParticleType::GAS2   ||
                 particleType == ParticleType::GAS3      ) ? true:false;
    }

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

    /** Removes all particles from the plane. */
    void removeAllParticles();

    /** @brief Shuffles particle position to the random position.
     *
     * @param particle    iterator to particle
     */
    void shuffleParticle( const iterParticle particle );

    /** @brief Checks if the particle plane is full of particles.
     *
     * @return true if the particle plane is full otherwise false
     */
    bool isParticlePlaneFull();

    /** @brief Checks if the particle plane will be full of particles for a given new size of particle.
     *
     * @param particleType          particle type
     * @param newSize               new size of particles
     * @return true if the particle plane is full otherwise false
     */
    bool isParticlePlaneFull( ParticleType particleType , int newSize );

    /** @brief Tries disjoint particle.
     *
     * Disjoints particle if its position is too close to other particles by shuffles it position.
     * @param particle              iterator to particle
     * @return true if disjont success otherwise false
     */
    bool disjoint( const iterParticle particle );

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
    void preserveParticleInPlane( const iterParticle particle );

    /** @brief Checks if a given particle position is inside plane.
     *
     * @param position              particle position
     * @param radius                particle radius
     * @return true if position is inside plane boundries, otherwise false
     */
    bool isParticleInPlane( const vect2D& position , double radius ) const;

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
