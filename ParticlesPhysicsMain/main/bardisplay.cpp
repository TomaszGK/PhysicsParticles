#include "bardisplay.h"

BarDisplay::BarDisplay( size_t size )
{
    upperBox.resize(size,0);
    lowerBox.resize(size,0);
}

void BarDisplay::setUpperBox( size_t index, int value )
{
    if( index<upperBox.size() )
    {
        upperBox[index] = value;
    }
}

void BarDisplay::setLowerBox( size_t index, int value )
{
    if( index<lowerBox.size() )
    {
        lowerBox[index] = value;
    }
}
