#pragma once

#include "definitions.h"
#include "langmanager.h"

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

public:

    /**
     * @brief Constructor
     * @param size      bins size
     * @param label     name of bardisplay
     */
    explicit BarDisplay( size_t size, std::string_view label = "" );

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
    int getUpperBox( int index ) const noexcept
    {
        Ensures( index<static_cast<int>(bins.size()) && index>=0 );
        return bins[static_cast<size_t>(index)].first;
    }

    /**
     * @brief Get lower bardisplay box value
     * @param index     lower box index
     * @return lower box value
     */
    int getLowerBox( int index ) const noexcept
    {
        Ensures( index<static_cast<int>(bins.size()) && index>=0 );
        return bins[static_cast<size_t>(index)].second;
    }

    /**
     * @brief Get sum of upper bardisplay box values
     * @return sum of upper box values
     */
    int getUpperBoxSum() const noexcept
    {
        int sum {0};
        for( const auto& bin : bins ) sum+= bin.first;
        return sum==0?1:sum;
    }

    /**
     * @brief Get sum of lower bardisplay box values
     * @return sum of lower box values
     */
    int getLowerBoxSum() const noexcept
    {
        int sum {0};
        for( const auto& bin : bins ) sum+= bin.second;
        return sum==0?1:sum;
    }

    /**
     * @brief Get number of bardisplay bins
     * @return number of bardisplay bins
     */
    int getSize() const noexcept { return static_cast<int>(bins.size()); }

    /**
     * @brief Get name of bardisplay
     * @return name of bardisplay
     */
    std::string getLabel() const noexcept { return LangManager::translate(label); }

private:

    /** contains upper and lower bar values */
    std::vector<std::pair<int,int>> bins;

    /** bar display name */
    std::string label;

};
