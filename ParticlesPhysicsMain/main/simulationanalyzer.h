#pragma once

#include "definitions.h"
#include "particle.h"
#include "histogram1d.h"
#include "barchart.h"
#include "bardisplay.h"

/** @file
 * @brief Class @ref SimulationAnalyzer
 */

/**
 * @class SimulationAnalyzer
 * @brief Holding, collecting and analyzing both physics and simulation data.
 *
 * Responsible for managing physics and simulation data. Collect physics data on notify.
 * @author Tomasz Gburek
 * @date 2020
 */

class SimulationAnalyzer
{

public:

    /**
     * @brief Constructor
     *
     * @param _simulationType      simulation type
     */
    SimulationAnalyzer( SimulationType _simulationType ) : simulationType { _simulationType } {}

    void resetPhysicsData();

    void collect( iterParticle particle );

    void collect( double kineticEnergy );

    void update();

    /** Contains main simulation parameters */
    SimulationInfo simulationInfo;

    /** Contains global physics quantities related to states of particles */
    PhysicsInfo physicsInfo;

    /** Contains bar charts */
    std::map<std::string,ptrBarChart> barCharts;

    /** Contains bar displays */
    std::map<std::string,ptrBarDisplay> barDisplays;

    /** Contains histograms 1D */
    std::map<std::string,ptrHistogram1D> histograms1D;

private:

    SimulationType simulationType;

    std::map<ParticleType,double> velocitySum;
    std::map<ParticleType,int> velocityCounter;

};

