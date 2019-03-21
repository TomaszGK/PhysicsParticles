#pragma once

#include "definitions.h"
#include "planedivider.h"

class PlaneArea
{
    // rectangle area coordinates
    int width {0};
    int height {0};

    // circle area coordinates
    vect2D origin;
    double radius {0};    

    // plane border with in pixels
    int planeBorderWidth {1};

    // plane X axis constraint
    int axisXConstraint {0};

    std::unique_ptr<PlaneDivider> planeDivider;

public:

    PlaneArea(int,int,int);       // constructor for rectangle area
    PlaneArea(vect2D,double,int); // constructor for circle area

    PlaneArea( const PlaneArea& ) = default;
    PlaneArea( PlaneArea&& ) = default;

    PlaneArea& operator=( const PlaneArea& ) = default;
    PlaneArea& operator=( PlaneArea&& ) = default;

    int getWidth() const noexcept { return width; }
    int getHeight() const noexcept { return height; }
    PlaneDivider& getPlainDivider() const { return *planeDivider; }
    int getPlaneBorderWidth() const noexcept { return planeBorderWidth; }
    inline int getXConstraint() const noexcept { return axisXConstraint; }
    void setXConstraint( int _axisXConstraint ) noexcept { axisXConstraint = _axisXConstraint; }

};
