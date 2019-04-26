#pragma once

#include "definitions.h"

/** @file
 * @brief Class @ref BarDisplay
 */

/**
 * @class BarDisplay
 * @brief Manages upper and lower bardisplay bin data.
 *
 * @author Tomasz Gburek
 * @date 2019
 */

class BarDisplay
{

    std::vector<std::pair<int,int>>  bins; /**< contains upper and lower bar values */
    std::string                     label; /**< bar display name */

public:

    /**
     * @brief Constructor
     *
     * Note that it was defined as deafult, for the possibility of use operator[] inside std::map container.
     */
    BarDisplay() = default;

    /**
     * @brief Constructor
     * @param size      bins size
     * @param label     name of bardisplay
     */
    BarDisplay( size_t size, std::string label = "" );

    /** @brief Copy constructor */
    BarDisplay( const BarDisplay& ) = default;

    /** @brief Move constructor */
    BarDisplay( BarDisplay&& ) = default;

    /** @brief Copy assigment operator */
    BarDisplay& operator=( const BarDisplay& ) = default;

    /** @brief Move assigment operator */
    BarDisplay& operator=( BarDisplay&& ) = default;

    /**
     * @brief Set upper bardisplay box value
     * @param index     upper box index
     * @param value     new value
     */
    void setUpperBox( size_t index , int value );

    /**
     * @brief Set lower bardisplay box value
     * @param index     lower box index
     * @param value     new value
     */
    void setLowerBox( size_t index , int value );

    /**
     * @brief Get upper bardisplay box value
     * @param index     upper box index
     * @return upper box value
     */
    inline int getUpperBox( size_t index ) const noexcept { return index<bins.size()?bins[index].first:0; }

    /**
     * @brief Get lower bardisplay box value
     * @param index     lower box index
     * @return lower box value
     */
    inline int getLowerBox( size_t index ) const noexcept { return index<bins.size()?bins[index].second:0; }

    /**
     * @brief Get number of bardisplay bins
     * @return number of bardisplay bins
     */
    inline size_t getSize() const noexcept { return bins.size(); }

    /**
     * @brief Get name of bardisplay
     * @return name of bardisplay
     */
    const std::string& getLabel() const noexcept { return label; }

};
