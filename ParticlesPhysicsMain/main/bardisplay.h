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
    std::string getLabel() const noexcept { return LangManager::translate(label); }

private:

    std::vector<std::pair<int,int>>  bins      ; /**< contains upper and lower bar values */
    std::string                     label {""} ; /**< bar display name */

};
