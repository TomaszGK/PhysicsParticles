#include "bardisplay.h"
#include <gsl_assert>

BarDisplay::BarDisplay( size_t size, std::string label )
: label {std::move(label)}
{
    bins.resize(size,{0,0});
}

void BarDisplay::setUpperBox( size_t index, int value )
{
    Ensures(value>=0);
    if( index<bins.size() )
    {
        bins[index].first = value;
    }
}

void BarDisplay::setLowerBox( size_t index, int value )
{
    Ensures(value>=0);
    if( index<bins.size() )
    {
        bins[index].second = value;
    }
}
