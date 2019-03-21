#include "histogram1d.h"

#include <stdexcept>
#include <algorithm>

Histogram1D::Histogram1D( size_t size, double _begin, double _end, std::string _label )
: begin {_begin}, end {_end}, label {_label}
{
    if( size==0 ) throw std::length_error("Histogram1D::Histogram1D : size==0");
    if( end<=begin ) throw std::length_error("Histogram1D::Histogram1D : begin>=end");

    bins.resize(size,0);    
    binsize = (end-begin)/size;
}

bool Histogram1D::fill( const double& value )
{
    if( value>end || value<begin ) return false;

    size_t index = static_cast<size_t>((value-begin)/binsize);

    binsIndex.push_back(index);

    if( ++bins[index]>max )
    {
        max = bins[index];
    }
    if( bins[index]-1 == min )
    {
        min = *std::min_element(bins.begin(),bins.end());
    }

    if( binsIndex.size() >= fadeThreshold )
    {
        --bins[*binsIndex.begin()];

        if( bins[*binsIndex.begin()]<min )
        {
            min = bins[*binsIndex.begin()];
        }
        if( bins[*binsIndex.begin()]+1 == max )
        {
            max = *std::max_element(bins.begin(),bins.end());
        }

        binsIndex.pop_front();
    }   

    return true;
}

void Histogram1D::markBin( double value )
{
    markingBin = static_cast<int>((value-begin)/binsize);
}
