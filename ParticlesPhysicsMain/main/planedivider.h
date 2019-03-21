#pragma once

#include "particle.h"

class PlaneDivider
{

    int dividerPosX   ; // plane divider position along X axis
    int dividerWidth  ; // width of the plane divider - in pixels
    int dividerHeight ; // height of the plane divider - in pixels
    int dividerGap    ; // size of gap inside the plane divider - in percent

    // when gap is equal zero upperRect and lowerRect rectangels adjoin to each other
    std::pair<coord2D,coord2D> upperRect; // upper plane divider rectangle coordinates where first contains upperleft rectangle point and second contains its width and height
    std::pair<coord2D,coord2D> lowerRect; // lower plane divider rectangle coordinates where first contains upperleft rectangle point and second contains its width and height

    std::pair<coord2D,coord2D> upperRectPoints; // upper plane divider rectangle coordinates where first contains upperleft rectangle point and second contains lowerright point
    std::pair<coord2D,coord2D> lowerRectPoints; // lower plane divider rectangle coordinates where first contains upperleft rectangle point and second contains lowerright point

    inline int minDistance(const double& ,const double&,const double&) const;

public:

    PlaneDivider( int _dividerPosX, int _dividerWidth, int _dividerHeight, int _dividerGap );

    PlaneDivider( const PlaneDivider& ) = default;
    PlaneDivider( PlaneDivider&& ) = default;

    PlaneDivider& operator=( const PlaneDivider& ) = default;
    PlaneDivider& operator=( PlaneDivider&& ) = default;

    const std::pair<coord2D,coord2D>& getUpperRect() const noexcept { return upperRect; }
    const std::pair<coord2D,coord2D>& getLowerRect() const noexcept { return lowerRect; }
    int getDividerPosX() const noexcept { return dividerPosX; }
    int getDividerGap() const noexcept { return dividerGap; }
    bool isDividerInPlane() const noexcept { return dividerGap<100; }

    void setDividerGap( int gap );

    void handleParticleCollision( iterParticle& );

};
