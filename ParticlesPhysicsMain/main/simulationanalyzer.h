#pragma once

#include "definitions.h"

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
     */
    SimulationAnalyzer() = default;

    void resetPhysicsData();

    /** Contains main simulation parameters */
    SimulationInfo simulationInfo;

    /** Contains global physics quantities related to states of particles */
    PhysicsInfo physicsInfo;

};

