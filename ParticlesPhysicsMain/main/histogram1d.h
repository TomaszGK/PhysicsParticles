#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include "langmanager.h"

/** @file
 * @brief Class @ref Histogram1D
 */

/**
 * @class Histogram1D
 * @brief Manages values storing in histogram bins.
 *
 * Handles adding new values to histogram bins, calculate maximum and minimum values in bins,
 * removing expired values.
 * @author Tomasz Gburek
 * @date 2019
 */

class Histogram1D
{  

public:

    /**
     * @brief Constructor
     *
     * @param size      number of bins
     * @param begin     begin position of first bin
     * @param end       end position of last bin
     * @param label     name of barchart
     */
    Histogram1D( size_t size, double begin, double end, std::string_view label = "" );

    /**
     * @brief Adds new value to histograms bins.
     *
     * If new value matches the range of bins that it is added to one of them.
     * If number of values in bin crosses the fadeThreshold, then last added value is removed.
     * @param value     new value to add
     */
    bool fill( const double& value );

    /**
     * @brief Gets the maximum value stored in histogram bins.
     * @return maximum value stored in bins
     */
    inline int getMax() const noexcept { return max; }

    /**
     * @brief Gets the minimum value stored in histogram bins.
     * @return minimum value stored in bins
     */
    inline int getMin() const noexcept { return min; }

    /**
     * @brief Gets the begin position of first bin.
     * @return begin position of first bin
     */
    inline double getBegin() const noexcept { return begin; }

    /**
     * @brief Gets the end position of last bin.
     * @return end position of last bin
     */
    inline double getEnd() const noexcept { return end; }

    /**
     * @brief Gets size of bin.
     * @return size of bin
     */
    inline double getBinSize() const noexcept { return binsize; }

    /**
     * @brief Gets histogram bins.
     * @return histogram bins
     */
    const std::vector<int>& getBins() const noexcept { return bins; }

    /**
     * @brief Gets name of histogram.
     * @return name of histogram
     */
    std::string getLabel() const noexcept { return LangManager::translate(label); }

    /**
     * @brief Gets index of marking bin.
     * @return index of marking bin
     */
    int getMarkingBin() const noexcept { return markingBin; }

    /**
     * @brief Gets average bin value.
     * @return average
     */
    double getAverage() const noexcept { return average; }

    /**
     * @brief Gets bins standard deviation value.
     * @return standard deviation
     */
    double getDeviation() const noexcept { return deviation; }

    /**
     * @brief Marks a certain bin.
     * @param value     index of marking bin
     */
    void markBin( double value );

private:

    /**< stores histogram values, having form of equal intervals */
    std::vector<int>   bins;

    /**< contains bin indexes of last added values */
    std::deque<size_t> binsIndex;

    /**< maximum value stored in bins */
    int max {0};

    /**< minimum value stored in bins */
    int min {0};

    /**< threshold describes maximum binsIndex size, crossing it means that oldest values are being removed from bins */
    size_t fadeThreshold {300000};

    /**< begin position of first bin */
    double begin {0.0};

    /**< end position of last bin */
    double end {10.0};

    /**< size of bins (intervals) */
    double binsize {1.0};

    /**< index of marking (selecting) bin */
    int markingBin {-1};

    /**< histogram name */
    std::string label {""};

    /**< bins average value */
    double average {0.0};

    /**< bins standard deviation */
    double deviation {0.0};

};
