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
 * handles particle collisions and check if particles hit the plane or collide with its divider.
 * @author Tomasz Gburek
 * @date 2019
 */

class ParticlesPhysicsManager
{

public:

    /**
     * @class Locator
     * @brief Simply service locator for data that must be use outside scope of this class.
     *
     * Provide a static global point of access to particles, plane, histograms and bardisplays.
     */
    class Locator
    {

    public:

        /** @brief Default Constructor */
        Locator() = delete;

        /** @brief Copy constructor */
        Locator( const Locator& ) = delete;

        /** @brief Move constructor */
        Locator( Locator&& ) = delete;

        /**
         * @brief Gets constant pointer to the container of particles.
         *
         * @return constant smart pointer to the container of particles
         */
        static cptrParticlesContainer getParticles() { return cparticles; }

        /**
         * @brief Gets constant pointer to the plane area.
         *
         * @return constant smart pointer to the plane area
         */
        static cptrPlaneArea getPlaneArea() { return cplane; }

        /**
         * @brief Gets constant pointer to the container of bar displays.
         *
         * @return constant smart pointer to the container of bar displays
         */
        static cptrBarDisplay getBarDisplay( ActionType type ) { return barDisplayMap==nullptr?nullptr:(*barDisplayMap).at(type); }

        /**
         * @brief Gets pointer to the container of bar charts.
         *
         * @return smart pointer to the container of bar charts
         */
        static ptrBarChart getBarChart( ActionType type ) { return barChartMap==nullptr?nullptr:(*barChartMap).at(type); }

        /**
         * @brief Gets constant pointer to the container of histograms.
         *
         * @return constant smart pointer to the container of histograms
         */
        static cptrHistogram1D getHistogram1D( ActionType type ) { return histogram1DMap==nullptr?nullptr:(*histogram1DMap).at(type); }

        /**
         * @brief Provides and saves data location.
         *
         * @param manager       pointer to @ref ParticlesPhysicsManager
         */
        static void provide( const ParticlesPhysicsManager *manager )
        {
            cparticles = manager->particles;
            cplane = manager->planeArea;
            barDisplayMap = manager->barDisplays;
            barChartMap = manager->barCharts;
            histogram1DMap = manager->histograms1D;
        }

    private:

        /** constant pointer to the container of particles */
        inline static cptrParticlesContainer cparticles {nullptr};

        /** constant pointer to the particle plane */
        inline static cptrPlaneArea cplane {nullptr};

        /** constant pointer to the map of bar displays */
        inline static cptrMapBarDisplay barDisplayMap {nullptr};

        /** constant pointer to the map of bar charts */
        inline static ptrMapBarChart barChartMap {nullptr};

        /** constant pointer to the map of histograms */
        inline static cptrMapHistogram1D histogram1DMap {nullptr};
    };

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
        return ( format == DataFormat::PERCENT ) ? simulationInfo.numberOfParticles.at(type)*100.0/simulationInfo.maxParticles.at(simulationType) : simulationInfo.numberOfParticles.at(type);
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
        return ( format == DataFormat::PERCENT ) ? physicsInfo.temperature.at(part)*100.0/physicsInfo.maxRapidity : physicsInfo.temperature.at(part);
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
        auto force = ( type == Axis::HORIZONTAL ) ? physicsInfo.pushForce.x : physicsInfo.pushForce.y;
        if( format == DataFormat::PERCENT ) force = force*100/physicsInfo.maxSideForce;

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
    void setForce( vect2D force ){ physicsInfo.pushForce = force; }

    /**
     * @brief Sets (calculates) average value of diffiusion temperature.
     *
     * Using in SimulationType::DIFFUSION
     */
    void setAverageDiffusionTemperature()
    {
        physicsInfo.temperature[PlanePart::RIGHTBOX] = physicsInfo.temperature[PlanePart::LEFTBOX] = (physicsInfo.temperature[PlanePart::RIGHTBOX] + physicsInfo.temperature[PlanePart::LEFTBOX])/2.0;
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
        return static_cast<int>(100*barCharts->at(ActionType::M_KINETIC)->getAvgInLast(5));
    }

    /**
     * @brief Gets maximum number of particles in the plane.
     * @return maximum number of particles
     */
    int getMaxNumberOfParticles() const noexcept { return static_cast<int>( simulationInfo.maxParticles.at(simulationType)); }

    /**
     * @brief Gets average number of calulations (next particles position) in the period of time.
     * @return average calculation time
     */
    int getAvgCalculationCount() const noexcept { return simulationInfo.avgCalculationCount; }

    /**
     * @brief Gets particle size for a given particle type.
     * @param type                  particle type
     * @param format                data format
     * @return particle size as raw scalar or percent value
     */
    int getParticleSize( ParticleType type, DataFormat format = DataFormat::SCALAR ) const noexcept
    {
        return ( format == DataFormat::PERCENT ) ? static_cast<int>((simulationInfo.particleSize.at(type)-simulationInfo.minSizeOfParticle)*100/(simulationInfo.maxSizeOfParticle.at(simulationType)-simulationInfo.minSizeOfParticle)) : simulationInfo.particleSize.at(type);
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
     * @brief Sets percent value of plane width.
     * @param quantity              new percent value of plane width
     */
    void setPlaneWidthInPercent( int quantity );

    /** Gets simulation information. */
    const SimulationInfo& getSimulationInfo() const noexcept { return simulationInfo; }

    /** Gets clusters information. */
    const ClustersInfo& getClustersInfo() const noexcept { return clustersInfo; }

    /** Gets physics information. */
    const PhysicsInfo& getPhysicsInfo() const noexcept { return physicsInfo; }

    /** Enables tracking of selected particle. */
    void enableTracking();

    /** Disables tracking of selected particle. */
    void disableTracking();

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
    void pause( bool userCall = false )
    {
        if( calculationState.load() == ThreadCalculationState::RUNNING )
        {
            calculationState.store(ThreadCalculationState::PAUSE);
            if( userCall ) pauseByUserFlag = true;
            while( calculateNextPositionFlag.load() ); // wait for the end of calculation step
        }
    }

    /**
     * @brief Runs simulation.
     *
     * Value userCall have to be true if run is called from a user action.
     * @param userCall              use call flag
     */
    void run( bool userCall = false )
    {
        if( calculationState.load() == ThreadCalculationState::PAUSE )
        {            
            if( userCall ) pauseByUserFlag = false;
            calculationStart = HRClock::now();
            calculationState.store( ThreadCalculationState::RUNNING );
        }
    }

    /** Ends simulation by breaking loop in the caluclation thread. */
    void end()
    {
        calculationState = ThreadCalculationState::END;
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
        Locator::provide(this);
        calculationStart = HRClock::now();
        return std::thread( &ParticlesPhysicsManager::mainLoop , this );
    }

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

    /** Contains bar charts */
    ptrMapBarChart barCharts;

    /** Contains bar displays */
    ptrMapBarDisplay barDisplays;

    /** Contains histograms 1D */
    ptrMapHistogram1D histograms1D;

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

    /** @brief Gets cluster iterator
     *
     * Gets cluster iterator from a given position (x,y).
     * Throws exception if position is out of the plane.
     * @param posx        x position of point (x,y)
     * @param posx        y position of point (x,y)
     * @return iterator to the cluster
     */
    iterCluster getClusterIter( const size_t& posx , const size_t& posy );

    /** Updates particles positions */
    void update();

    /** Calls updates in while() loop
     *
     * The following loop should be running in separate thread.
     */
    void mainLoop();

    /** Gets random color
     * @return random RGB color
     */
    colorRGB getRandomColor();

    /** Gets average kinetcic energy of particles in the plane.
     * @return average kinetcic energy
     */
    double getAverageKineticEnergyOfParticles();

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
    void shuffleParticle( const iterParticle& particle );

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
