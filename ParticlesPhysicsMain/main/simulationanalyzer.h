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
     * @brief Default Constructor
     *
     * @param _ptrParticles          shared pointer to constant vector of Particle objects
     */
    SimulationAnalyzer( cptrParticlesContainer _ptrParticles );

    void resetPhysicsData();

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

    /** Holds shared pointer to constant vector of Particle objects. */
    cptrParticlesContainer ptrParticles;

};

