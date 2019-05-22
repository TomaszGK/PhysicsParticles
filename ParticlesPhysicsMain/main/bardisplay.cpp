#include "bardisplay.h"

BarDisplay::BarDisplay( size_t size, std::string_view label )
: label {label}
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
