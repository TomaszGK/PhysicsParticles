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
     */
    SimulationAnalyzer();

    void reset();

    void collect( iterParticle particle );

    void collect( double kineticEnergy );

    void update( SimulationType simulationType );

    /** Updates all bar charts and displays, adds new physics values to chart boxes */
    void updateBars( SimulationType simulationType );

    /** Contains main simulation parameters */
    SimulationInfo simulationInfo;

    /** Contains global physics quantities related to states of particles */
    PhysicsInfo physicsInfo;

    /** Contains bar charts */
    ptrMapBarChart barCharts;

    /** Contains bar displays */
    ptrMapBarDisplay barDisplays;

    /** Contains histograms 1D */
    ptrMapHistogram1D histograms1D;

private:    

    std::map<ParticleType,double> velocitySum;

    std::map<ParticleType,int> velocityCounter;

};

