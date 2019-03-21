#pragma once

#include "vector2d.h"
#include "definitions.h"
#include <memory>

template<typename T>
class sline2D
{
    T A {0}, B {0}, C {0};
    bool pointsEquals {true};

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

public:

    sline2D() = default;
    sline2D( vector2D<T> _v1 , vector2D<T> _v2 ) : v1 {_v1}, v2 {_v2} { if(v1!=v2) calculateCoeff(); }
    sline2D( const sline2D& ) = default;
    sline2D( sline2D&& ) = default;

    sline2D& operator=( const sline2D& ) = default;
    sline2D& operator=( sline2D&& ) = default;

    bool hasIntersect( const sline2D<T>& sl )
    {
        return (A*sl.B-sl.A*B) != 0;
    }

    std::unique_ptr<vector2D<T>> intersection( const sline2D<T>& sl )
    {
       T det = A*sl.B-sl.A*B;
       std::unique_ptr<vector2D<T>> ptr;

       if( det != 0 ) ptr = make_unique<vector2D<T>>((sl.A*C-A*sl.C)/det , (B*sl.C-sl.B*C)/det);

       return ptr;
    }

    std::unique_ptr<vector2D<T>> projection( const vector2D<T>& point )
    {
       std::unique_ptr<vector2D<T>> ptr;

       if( !pointsEquals )
       {
        double factor = (C + point.x*A + point.y*B)/( A*A+B*B );
        ptr = make_unique<vector2D<T>>( point.x - factor*A , point.y - factor*B);
       }

       return ptr;
    }

    vector2D<T> v1,v2;

};
