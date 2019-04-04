#pragma once

#include <list>
#include <iostream>

//!  BarChart class
/*!
   Handle bar chart data.
*/

class BarChart
{

    std::list<double> bins    ; /*!< contains bar values */
    double            max {0} ; /*!< maximum value inside bins */
    double            min {0} ; /*!< minimum value inside bins */
    std::string       label   ; /*!< bar chart name */

public:

    //! Deafault constructor
    /*!
      for use in std::map -> operator []
    */
    BarChart() = default;

    //! BarChart constructor
    /*!
      \param size bins size
      \param label bar chart name
    */
    BarChart( size_t size , std::string label = "" );

    //! Default copy constructor
    BarChart( const BarChart& ) = default;

    //! Default move constructor
    BarChart( BarChart&& ) = default;

    //! Default copy assigment operator
    BarChart& operator=( const BarChart& ) = default;

    //! Default move assigment operator
    BarChart& operator=( BarChart&& ) = default;

    //! Add new value to bar chart bins
    /*!
      The value is add (push back) as the last element of bins list,
      while the first element of bins list is removing (pop front).
      \param value adding new value
    */
    void add( double value );

    //! Get maximum value from all bins
    /*!
      \return maximum value
    */
    inline double getMax() const noexcept { return max; }

    //! Get minimum value from all bins
    /*!
      \return minimum value
    */
    inline double getMin() const noexcept { return min; }

    //! Get average value from all bins
    /*!
      \return average bins value
    */
    double getAvg() const;

    //! Get bar chart bins
    /*!
      \return bar chart bins
    */
    const std::list<double>& getBins() const noexcept { return bins; }   

    //! Get bar chart name
    /*!
      \return bar chart name
    */
    const std::string& getLabel() const noexcept { return label; }

};
