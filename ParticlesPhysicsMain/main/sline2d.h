#pragma once

#include "vector2d.h"
#include "definitions.h"
#include <memory>

/** @file
 * @brief Template class @ref sline2D
 */

/**
 * @class sline2D
 * @brief Defines two dimensional simply line.
 *
 * Implements basic functions to operate with line using projection and intersections.
 * @author Tomasz Gburek
 * @date 2019
 */

template<typename T>
class sline2D
{

public:

    /**
     * @brief Default constructor
     */
    sline2D() = default;

    /**
     * @brief Constructor
     *
     * Constructs 2D line geting across v1 and v2.
     * @param v1          point v1
     * @param v2          point v2
     */
    sline2D( vector2D<T> v1 , vector2D<T> v2 ) : v1 {v1}, v2 {v2} { if(v1!=v2) calculateCoeff(); }

    /**
     * @brief Checks if a given line intersect with this line.
     * @param sl                    line
     * @return true if intersect otherwise false
     */
    bool hasIntersect( const sline2D<T>& sl )
    {
        return (A*sl.B-sl.A*B) != 0;
    }

    /**
     * @brief Calculates intersection point with a given line.
     *
     * If lines are parallel then returns nullptr pointer.
     * @param sl                    line
     * @return unique pointer to intersection point
     */
    std::unique_ptr<vector2D<T>> intersection( const sline2D<T>& sl )
    {
       T det = A*sl.B-sl.A*B;
       std::unique_ptr<vector2D<T>> ptr {nullptr};

       if( det != 0 ) ptr = make_unique<vector2D<T>>((sl.A*C-A*sl.C)/det , (B*sl.C-sl.B*C)/det);

       return ptr;
    }

    /**
     * @brief Calculates projection point to this line from a given point.
     *
     * If points are equal then return nullptr.
     * @param sl                    point to project
     * @return unique pointer to projection point
     */
    std::unique_ptr<vector2D<T>> projection( const vector2D<T>& point )
    {
        std::unique_ptr<vector2D<T>> ptr {nullptr};

       if( !pointsEquals )
       {
        double factor = (C + point.x*A + point.y*B)/( A*A+B*B );
        ptr = make_unique<vector2D<T>>( point.x - factor*A , point.y - factor*B);
       }

       return ptr;
    }

    vector2D<T> v1; /**< point v1 defining this line */
    vector2D<T> v2; /**< point v2 defining this line */

private:

    T A {0}; /**< Line coefficient A from Ax+By+Cz=0 */
    T B {0}; /**< Line coefficient A from Ax+By+Cz=0 */
    T C {0}; /**< Line coefficient A from Ax+By+Cz=0 */

    /**< Boolean flag indicates if two points defining this line are equal. */
    bool pointsEquals {true};

    /**< Calulates line coeficients from Ax+By+Cz=0. */
    void calculateCoeff()
    {
        pointsEquals = false;
        if( v1.x != v2.x )
        {
            A = (v2.y-v1.y)/(v1.x-v2.x);
            B = 1;
            C = (v1.y*v2.x-v2.y*v1.x)/(v1.x-v2.x);
        }
        else
        {
            A = 1;
            B = 0;
            C = (v2.y*v1.x-v1.y*v2.x)/(v1.y-v2.y);
        }
    }

};
