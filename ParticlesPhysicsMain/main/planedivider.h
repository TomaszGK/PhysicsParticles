#pragma once

#include "particle.h"

/** @file
 * @brief Class @ref PlaneDivider
 */

/**
 * @class PlaneDivider
 * @brief Manages plane divider and handles particle collison with it.
 *
 * Defines upper and down rectangles that represent divider state.
 * The length of gap between them measures opening of divider.
 * When gap is equal zero upperRect and lowerRect rectangels adjoin to each other.
 * @author Tomasz Gburek
 * @date 2019
 */

class PlaneDivider
{   

public:

    /**
     * @brief Constructor
     *
     * @param dividerPosX           position of divider along X axis
     * @param dividerWidth          divider width in pixels
     * @param dividerHeight         divider height in pixels
     * @param dividerGap            percent value of divider gap
     */
    PlaneDivider( int dividerPosX, int dividerWidth, int dividerHeight, int dividerGap );

    /**
     * @brief Gets upper rectangle that represents upper part of divider.
     * @return upper rectangle
     */
    const std::pair<coord2D,coord2D>& getUpperRect() const noexcept { return upperRect; }

    /**
     * @brief Gets lower rectangle that represents lower part of divider.
     * @return lower rectangle
     */
    const std::pair<coord2D,coord2D>& getLowerRect() const noexcept { return lowerRect; }

    /**
     * @brief Gets divider position along X axis.
     * @return divider position in pixels
     */
    int getDividerPosX() const noexcept { return dividerPosX; }

    /**
     * @brief Gets divider gap size in percent.
     * @return divider gap size in percent [0,100]
     */
    int getDividerGap() const noexcept { return dividerGap; }

    /**
     * @brief Checks if divider exist in the plane, gap is less then 100.
     * @return true if divider exist otherwise false
     */
    bool isDividerInPlane() const noexcept { return dividerGap<100; }

    /**
     * @brief Sets divider gap.
     * @param gap                   new divider gap in percent
     */
    void setDividerGap( int gap );

    /**
     * @brief Handles particle collision whit plane divider.
     *
     * Checks if particle was collided with upper or lower rectangle.
     * Changes particle vector velocity if collision has been detected.
     * @param particle              iterator to particle
     */
    void handleParticleCollision( const iterParticle particle );

private:

    int dividerPosX   {0} ; /**< position of plane divider along X axis */
    int dividerWidth  {0} ; /**< width of plane divider in pixels */
    int dividerHeight {0} ; /**< height of plane divider in pixels */
    int dividerGap    {0} ; /**< size of gap inside the plane divider in percent */

    /**< Upper plane divider rectangle coordinates, first contains upperleft rectangle position and second its width and height */
    std::pair<coord2D,coord2D> upperRect;

    /**< Lower plane divider rectangle coordinates, first contains upperleft rectangle position and second its width and height */
    std::pair<coord2D,coord2D> lowerRect;

    /**< Upper plane divider rectangle coordinates, first contains upperleft rectangle position and second its lowerright position */
    std::pair<coord2D,coord2D> upperRectPoints;

    /**< Lower plane divider rectangle coordinates, first contains upperleft rectangle position and second its lowerright position */
    std::pair<coord2D,coord2D> lowerRectPoints;

    /**
     * @brief Calculates minimum value from three values and return its index.
     * @param d1                    minimum value index 1
     * @param d2                    minimum value index 2
     * @param d2                    minimum value index 3
     * @return index from {1,2,3}
     */
    inline int minDistance( const double& d1, const double& d2, const double& d3 ) const;

};
