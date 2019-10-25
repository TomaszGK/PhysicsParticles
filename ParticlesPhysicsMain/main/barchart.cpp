#include "barchart.h"

#include <algorithm>
#include <numeric>

BarChart::BarChart( size_t size , std::string_view label , std::string_view labelX , std::string_view labelY )
: label {label}, labelX {labelX}, labelY {labelY}
{
    bins.resize(size,0);
}

void BarChart::add( double value )
{
    bins.push_back(value);
    bins.pop_front();

    max = *std::max_element(bins.begin(), bins.end());
    min = *std::min_element(bins.begin(), bins.end());
}

double BarChart::getAvg() const
{
    return bins.empty() ? 0.0 : std::accumulate(bins.cbegin(), bins.cend(), 0.0)/static_cast<double>(bins.size());
}

double BarChart::getAvgInLast( int percent ) const
{
    double average {0};

    Ensures( percent>=0 && percent<=100 );
    const int lastbins = (static_cast<int>(bins.size())*percent)/100;
    int counter = lastbins;

    if( lastbins == 0 ) return 0;

    for( auto iter=bins.rbegin() ; iter!=bins.rend() ; ++iter )
    {
        if( counter-- == 0 ) break;
        average += *iter;
    }

    return average/static_cast<double>(lastbins);
}

void BarChart::resetBins()
{
    std::fill( bins.begin() , bins.end() , 0.0 );
}
