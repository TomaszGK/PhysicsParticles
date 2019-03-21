#include "barchart.h"

#include <algorithm>
#include <numeric>

BarChart::BarChart( size_t size , std::string _label )
: label {_label}
{
    bins.resize(size,0);
}

void BarChart::add( double value )
{
   if( max<value ) max = value;
   if( min>value ) min = value;

   bins.push_back(value);

   if( *bins.begin() >= max ) max = *std::max_element(++bins.begin(), bins.end());
   else if( *bins.begin() <= min ) min = *std::min_element(++bins.begin(), bins.end());

   bins.pop_front();
}

double BarChart::getAvg() const
{
    if( bins.size()>0 ) return std::accumulate(bins.begin(), bins.end(), 0.0)/static_cast<double>(bins.size());
    else return 0;
}
