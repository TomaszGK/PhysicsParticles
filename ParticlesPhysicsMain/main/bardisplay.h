#pragma once

#include "definitions.h"

//!  BarDisplay class
/*!
   Handle upper and lower bar chart data.
*/

class BarDisplay
{

    std::vector<std::pair<int,int>>  bins; /*!< contains upper and lower bar values */
    std::string                     label; /*!< bar display name */

public:

    //! Deafault constructor
    /*!
      for use in std::map -> operator []
    */
    BarDisplay() = default;

    //! BarDisplay constructor
    /*!
      \param size bins size
      \param label bar chart name
    */
    BarDisplay( size_t size, std::string label = "" );

    //! Default copy constructor
    BarDisplay( const BarDisplay& ) = default;

    //! Default move constructor
    BarDisplay( BarDisplay&& ) = default;

    //! Default copy assigment operator
    BarDisplay& operator=( const BarDisplay& ) = default;

    //! Default move assigment operator
    BarDisplay& operator=( BarDisplay&& ) = default;

    //! Set upper box value
    /*!
      \param index upper box position
      \param value new value
    */
    void setUpperBox( size_t index , int value );

    //! Set lower box value
    /*!
      \param index lower box position
      \param value new value
    */
    void setLowerBox( size_t index , int value );

    //! Get upper box value
    /*!
      \param index upper box position
      \return upper box value
    */
    inline int getUpperBox( size_t index ) const noexcept { return index<bins.size()?bins[index].first:0; }

    //! Get lower box value
    /*!
      \param index lower box position
      \return upper box value
    */
    inline int getLowerBox( size_t index ) const noexcept { return index<bins.size()?bins[index].second:0; }

    //! Get bar display size
    /*!
      \return bar display size
    */
    inline size_t getSize() const noexcept { return bins.size(); }

    //! Get bar display name
    /*!
      \return bar display name
    */
    const std::string& getLabel() const noexcept { return label; }

};
