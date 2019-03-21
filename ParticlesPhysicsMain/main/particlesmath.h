#pragma once

#include "definitions.h"

namespace ParticlesMath
{

    // return point projection onto the straight crossing over a,b
    inline vect2D projectionPoint( const vect2D& a , const vect2D& b , const vect2D& point )
    {        
        double denominator = (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) ;// + std::numeric_limits<double>::epsilon();
        double factor = ((point.x-b.x)*(a.x-b.x) + (point.y-b.y)*(a.y-b.y))/denominator;

        return b + (a-b)*factor;
    }

    // return vector [point-a] projection onto the straight crossing over a,b
    inline vect2D projectionVector( const vect2D& a , const vect2D& b , const vect2D& point )
    {
        double denominator = (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) ;//+ std::numeric_limits<double>::epsilon();        
        double factor = ((point.x-b.x)*(a.x-b.x) + (point.y-b.y)*(a.y-b.y))/denominator;

        return (a-b)*factor;
    }

    // return cosinus of angle between vectors a,b
    inline double cosAngle( const vect2D& a , const vect2D& b )
    {
       double denominator = a()*b() ;// + std::numeric_limits<double>::epsilon();
       if( !(denominator>0) ) return 0.0;
       return ( a.x*b.x + a.y*b.y )/denominator;
    }

    // return dot product of vectors a,b
    inline double product( const vect2D& a , const vect2D& b )
    {
       return (a.x*b.x + a.y*b.y);
    }

}
