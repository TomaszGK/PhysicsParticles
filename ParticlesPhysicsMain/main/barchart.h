#pragma once

#include <list>
#include <iostream>

/** @file
 * @brief Class @ref BarChart
 */

/**
 * @class BarChart
 * @brief Manages barchart data.
 *
 * Handles adding new values to barchart bins, calculate maximum and minimum values in bins.
 * @author Tomasz Gburek
 * @date 2019
 */

class BarChart
{

    std::list<double> bins    ; /**< contains bar values */
    double            max {0} ; /**< maximum value inside bins */
    double            min {0} ; /**< minimum value inside bins */
    std::string       label   ; /**< bar chart name */

public:

    /**
     * @brief Constructor
     *
     * Note that it was defined as deafult, for the possibility of use operator[] inside std::map container.
     */
    BarChart() = default;

    /**
     * @brief Constructor
     * @param size      bins size
     * @param label     name of barchart
     */
    BarChart( size_t size , std::string label = "" );

    /** @brief Copy constructor */
    BarChart( const BarChart& ) = default;

    /** @brief Move constructor */
    BarChart( BarChart&& ) = default;

    /** @brief Copy assigment operator */
    BarChart& operator=( const BarChart& ) = default;

    /** @brief Move assigment operator */
    BarChart& operator=( BarChart&& ) = default;

    /**
     * @brief Add new value to bar chart bins
     * The value is add (push back) in the last element of bins list,
     * while the first element of bins list is removing (pop front).
     * @param value     new value to add
     */
    void add( double value );

    /**
     * @brief Get maximum value from all bins
     * @return maximum value
     */
    inline double getMax() const noexcept { return max; }

    /**
     * @brief Get manimum value from all bins
     * @return manimum value
     */
    inline double getMin() const noexcept { return min; }

    /**
     * @brief Get average value from all bins
     * @return average value
     */
    double getAvg() const;

    /**
     * @brief Get barchart bins
     * @return barchart bins
     */
    const std::list<double>& getBins() const noexcept { return bins; }   

    /**
     * @brief Get barchart name
     * @return barchart name
     */
    const std::string& getLabel() const noexcept { return label; }

};
