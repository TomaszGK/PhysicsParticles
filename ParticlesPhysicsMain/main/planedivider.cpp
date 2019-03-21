#include "planedivider.h"

PlaneDivider::PlaneDivider( int _dividerPosX, int _dividerWidth, int _dividerHeight, int _dividerGap )
: dividerPosX {_dividerPosX}, dividerWidth {_dividerWidth}, dividerHeight {_dividerHeight}, dividerGap {_dividerGap}
{
    upperRectPoints.first.x  = upperRect.first.x  = static_cast<int>(dividerPosX-dividerWidth/2);
    upperRectPoints.first.y  = upperRect.first.y  = 0;
    upperRect.second.x = static_cast<int>(dividerWidth);
    upperRectPoints.second.y = upperRect.second.y = static_cast<int>(dividerHeight/2-((dividerHeight/2)*dividerGap)/100);

    lowerRectPoints.first.x  = lowerRect.first.x  = static_cast<int>(dividerPosX-dividerWidth/2);
    lowerRectPoints.first.y  = lowerRect.first.y  = static_cast<int>(dividerHeight/2+((dividerHeight/2)*dividerGap)/100);
    lowerRect.second.x = static_cast<int>(dividerWidth);
    lowerRect.second.y = static_cast<int>(dividerHeight/2+((dividerHeight/2)*dividerGap)/100);

    upperRectPoints.second.x = static_cast<int>(dividerPosX+dividerWidth/2);
    lowerRectPoints.second.x = static_cast<int>(dividerPosX+dividerWidth/2);
    lowerRectPoints.second.y = static_cast<int>(dividerHeight);
}

int PlaneDivider::minDistance( const double& d1, const double& d2, const double& d3) const
{
    if( d1<=d2 )
    {
        if( d1<=d3 ) return 1;
        else return 3;
    }
    else
    {
        if( d2<=d3 ) return 2;
        else return 3;
    }
}

void PlaneDivider::setDividerGap( int gap )
{
    if( gap<=100 )
    {
        dividerGap = gap;
        upperRectPoints.second.y = upperRect.second.y = static_cast<int>(dividerHeight/2-((dividerHeight/2)*gap)/100);
        lowerRectPoints.first.y  = lowerRect.first.y  = static_cast<int>(dividerHeight/2+((dividerHeight/2)*gap)/100);
        lowerRect.second.y  = static_cast<int>(dividerHeight/2-((dividerHeight/2)*gap)/100);
    }
}

void PlaneDivider::handleParticleCollision( iterParticle& particle )
{
    vect2D newPosition = particle->calculateNextPosition();

    if( newPosition.x+particle->radius>=upperRectPoints.first.x  &&
        newPosition.x-particle->radius<=upperRectPoints.second.x &&
        newPosition.y+particle->radius>=upperRectPoints.first.y  &&
        newPosition.y-particle->radius<=upperRectPoints.second.y )
    {
        switch( minDistance(abs(newPosition.x-upperRectPoints.first.x),abs(newPosition.x-upperRectPoints.second.x),abs(newPosition.y-upperRectPoints.second.y)) )
        {
         case 1:
             particle->position.x = static_cast<double>(upperRectPoints.first.x) - particle->radius;
             particle->velocity.x *= -1.0;
            break;

         case 2:
             particle->position.x = static_cast<double>(upperRectPoints.second.x) + particle->radius;
             particle->velocity.x *= -1.0;
            break;

         case 3:
             particle->position.y = static_cast<double>(upperRectPoints.second.y) + particle->radius;
             particle->velocity.y *= -1.0;
            break;
        }

    }
    else if( newPosition.x+particle->radius>=lowerRectPoints.first.x  &&
             newPosition.x-particle->radius<=lowerRectPoints.second.x &&
             newPosition.y+particle->radius>=lowerRectPoints.first.y  &&
             newPosition.y-particle->radius<=lowerRectPoints.second.y )
    {
        switch( minDistance(abs(newPosition.x-upperRectPoints.first.x),abs(newPosition.x-upperRectPoints.second.x),abs(newPosition.y-upperRectPoints.first.y)) )
        {
         case 1:
             particle->position.x = static_cast<double>(upperRectPoints.first.x) - particle->radius;
             particle->velocity.x *= -1.0;
            break;

         case 2:
             particle->position.x = static_cast<double>(upperRectPoints.second.x) + particle->radius;
             particle->velocity.x *= -1.0;
            break;

         case 3:
             particle->position.y = static_cast<double>(upperRectPoints.first.y) - particle->radius;
             particle->velocity.y *= -1.0;
            break;
        }

    }

}
