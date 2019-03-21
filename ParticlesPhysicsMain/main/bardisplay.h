#pragma once

#include "definitions.h"

class BarDisplay
{

    std::vector<int> upperBox;
    std::vector<int> lowerBox;

public:

    BarDisplay() = default; // for use in std::map -> operator []
    BarDisplay( size_t size );

    BarDisplay( const BarDisplay& ) = default;
    BarDisplay( BarDisplay&& ) = default;

    BarDisplay& operator=( const BarDisplay& ) = default;
    BarDisplay& operator=( BarDisplay&& ) = default;

    void setUpperBox( size_t index , int value );
    void setLowerBox( size_t index , int value );

    inline int getUpperBox( size_t index ) const noexcept { return index<upperBox.size()?upperBox[index]:0; }
    inline int getLowerBox( size_t index ) const noexcept { return index<lowerBox.size()?lowerBox[index]:0; }

    inline size_t getSize() const noexcept { return upperBox.size(); }
};
