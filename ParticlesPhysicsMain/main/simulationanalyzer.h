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
     * Creates histograms, bar chart and bar display objects.
     */
    SimulationAnalyzer();

    /**
     * @brief Resets simulationInfo and physicsInfo to its initial values.
     *
     */
    void reset();

    /**
     * @brief Collects information provided by particle
     *
     * Collects particle velocity and momentum.
     * @param particle            iterator to particle
     */
    void collect( const citerParticle particle );

    /**
     * @brief Collects particle kinetic energy
     *
     * @param kineticEnergy       particle kinetic energy
     */
    void collect( double kineticEnergy );

    /**
     * @brief Updates and recalculates physics quantities used in a given simulation type.
     *
     * @param simulationType      simulation type
     */
    void update( SimulationType simulationType );

    /**
     * @brief Adjusts time contribution
     *
     * @param calculationPeriod   calculation period
     */
    double adjustTimeContribution( double calculationPeriod )
    {
        return calculationPeriod < simulationInfo.maxTimeContribution ? calculationPeriod : simulationInfo.maxTimeContribution;
    }

    /**
     * @brief Checks if difference from a given time point to current time point is greater then fix period time.
     *
     * @param time   a given time point
     */
    bool isTimePeriodUp( const HRClock::time_point& time )
    {
       return std::chrono::duration_cast<Milliseconds>(HRClock::now() - time) > physicsInfo.timePeriod;
    }

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

    /** Map of velocity sum */
    std::map<ParticleType,double> velocitySum;

    /** Map of velocity counter */
    std::map<ParticleType,int> velocityCounter;

};

