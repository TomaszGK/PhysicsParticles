#pragma once

#include <iostream>
#include <vector>
#include <deque>


class Histogram1D
{

    std::vector<int> bins;
    std::deque<size_t> binsIndex;

    int max              {0};      // maximum value in bins
    int min              {0};      // minimum value in bins
    size_t fadeThreshold {300000};
    double begin         {0.0};    // bin begin position
    double end           {10.0};   // bin end position
    double binsize       {1.0};
    int    markingBin    {-1};

    std::string label;

public:

    Histogram1D() = default; // for use in std::map -> operator []
    Histogram1D( size_t size, double begin, double end, std::string label = "" );

    Histogram1D( const Histogram1D& ) = default;
    Histogram1D( Histogram1D&& ) = default;

    Histogram1D& operator=( const Histogram1D& ) = default;
    Histogram1D& operator=( Histogram1D&& ) = default;

    bool fill( const double& value );

    inline int getMax() const noexcept { return max; }
    inline int getMin() const noexcept { return min; }

    inline double getBegin() const noexcept { return begin; }
    inline double getEnd() const noexcept { return end; }
    inline double getBinSize() const noexcept { return binsize; }

    const std::vector<int>& getBins() const noexcept { return bins; }
    const std::string& getLabel() const noexcept { return label; }

    int getMarkingBin() const noexcept { return markingBin; }
    void markBin( double value );

};
