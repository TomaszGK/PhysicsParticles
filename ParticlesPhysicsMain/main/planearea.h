#pragma once

#include "definitions.h"
#include "planedivider.h"
#include "cluster.h"

/** @file
 * @brief Class @ref PlaneArea
 */

/**
 * @class PlaneArea
 * @brief Manages particle plane area with its divider.
 *
 * Implements basic particle plane area parameters such its width and height.
 * Holds plane divider uses in diffusion simulation.
 * @author Tomasz Gburek
 * @date 2019
 */

class PlaneArea
{  

public:

    /**
     * @brief Constructor
     *
     * Constructs rectangle plane area.
     * @param width                 plane width in pixels
     * @param heigth                plane height in pixels
     * @param planeBorderWidth      plane border width in pixels
     */
    PlaneArea( int width, int heigth, int planeBorderWidth );

    /**
     * @brief Constructor
     *
     * Constructs circle plane area. @warning Not yet implemented!
     * @param origin                plane orgin position
     * @param radius                plane radius in pixels
     * @param planeBorderWidth      plane border width in pixels
     */
    PlaneArea( vect2D origin, double radius, int planeBorderWidth );

    /**
     * @brief Handles particle collision with plane divider.
     *
     * @param particle               iterator to particle
     */
    void handleParticleCollisionWithPlaneDivider( const iterParticle particle )
    {
        if( planeDivider->isDividerInPlane() && particle->cluster->PLANE_DIVIDER )
        {
            planeDivider->handleParticleCollision(particle);
        }
    }

    /**
     * @brief Gets plane width.
     * @return plane width in pixels
     */
    int getWidth() const noexcept { return width; }

    /**
     * @brief Gets plane height.
     * @return plane height in pixels
     */
    int getHeight() const noexcept { return height; }

    /**
     * @brief Gets plane divider.
     * @return plane divider
     */
    PlaneDivider& getPlainDivider() const { return *planeDivider; }

    /**
     * @brief Gets plane border width.
     * @return plane border width in pixels
     */
    int getPlaneBorderWidth() const noexcept { return planeBorderWidth; }

    /**
     * @brief Gets plane width constraint.
     * @return plane width constraint in pixels
     */
    int getXConstraint() const noexcept { return axisXConstraint; }

    /**
     * @brief Gets plane field.
     * @return plane field
     */
    int getPlaneField() const noexcept { return width*height; }

    /**
     * @brief Gets plane field with constraint.
     * @return plane field with constraint
     */
    int getPlaneFieldConstraint() const noexcept { return (width-2*axisXConstraint)*height; }

    /**
     * @brief Sets plane width constraint.
     * @param constraint            width constraint in pixels
     */
    void setXConstraint( int constraint ) noexcept { axisXConstraint = constraint; }

private:

    int width  {1}; /**< plane width in pixels : rectangle coordinate */
    int height {1}; /**< plane height in pixels : rectangle coordinate */

    vect2D origin    ; /**< plane orgin position : circle coordinate */
    double radius {0}; /**< plane radius in pixels : circle coordinate */

    int planeBorderWidth {1}; /**< plane border with in pixels */
    int axisXConstraint  {0}; /**< plane width constrain */

    /**< Holds plane divider. */
    std::unique_ptr<PlaneDivider> planeDivider;

};
