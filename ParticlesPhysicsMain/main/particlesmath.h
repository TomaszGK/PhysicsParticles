#pragma once

#include "definitions.h"

/** @file
 * @brief Particles mathematics namespace
 */

namespace ParticlesMath
{

    /**
     * @brief Calculates point projection
     *
     * Calculates point projection onto the straight crossing over a and b.
     * @param point   start projection point
     * @param a       defines a in straight {a,*}
     * @param b       defines b in straight {*,b}
     * @return end projection point
     */
    inline vect2D projectionPoint( const vect2D& a , const vect2D& b , const vect2D& point )
    {        
        double denominator = (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) ;// + std::numeric_limits<double>::epsilon();
        double factor = ((point.x-b.x)*(a.x-b.x) + (point.y-b.y)*(a.y-b.y))/denominator;

        return b + (a-b)*factor;
    }

    /**
     * @brief Calculates point projection as [point-a]
     *
     * Calculates point projection onto the straight crossing over a and b.
     * @param point   start projection point
     * @param a       defines a in straight {a,*}
     * @param b       defines b in straight {*,b}
     * @return end projection as [point-a]
     */
    inline vect2D projectionVector( const vect2D& a , const vect2D& b , const vect2D& point )
    {
        double denominator = (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) ;//+ std::numeric_limits<double>::epsilon();        
        double factor = ((point.x-b.x)*(a.x-b.x) + (point.y-b.y)*(a.y-b.y))/denominator;

        return (a-b)*factor;
    }

    /**
     * @brief Calculates cosinus of angle.
     *
     * Calculates cosinus of angle between vectors a and b
     * @param a       defines vector of a
     * @param b       defines vector of b
     * @return cosinus of angle between vectors a and b
     */
    inline double cosAngle( const vect2D& a , const vect2D& b )
    {
       double denominator = a()*b() ;// + std::numeric_limits<double>::epsilon();
       if( !(denominator>0) ) return 0.0;
       return ( a.x*b.x + a.y*b.y )/denominator;
    }

    /**
     * @brief Calculates dot product.
     *
     * Calculates dot product of vectors a and b.
     * @param a       defines vector of a
     * @param b       defines vector of b
     * @return dot product of vectors a and b
     */
    inline double product( const vect2D& a , const vect2D& b )
    {
       return (a.x*b.x + a.y*b.y);
    }

}
