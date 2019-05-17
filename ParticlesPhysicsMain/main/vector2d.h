#pragma once

#include <iostream>

template<typename T>
class vector2D
{

public:

    vector2D( T _x = T() , T _y = T() ) : x {_x}, y {_y} {}

    inline T operator()() const { return sqrt(x*x+y*y); }
    inline T operator~() const { return (x*x+y*y); }
    bool operator==( const vector2D<T>& v ) const { return ((x==v.x)&&(y==v.y)); }
    vector2D<T> operator!() const { return vector2D<T>(y,-x); }    
    vector2D<T> operator*( const T& alpha ) const { return vector2D<T>(alpha*x,alpha*y); }
    void operator*=( const vector2D<T>& v ) { *this = (*this)*v; }
    void operator+=( const vector2D<T>& v ) { *this = (*this)+v; }
    void operator-=( const vector2D<T>& v ) { *this = (*this)-v; }
    void operator*=( const T& alpha ) { *this = (*this)*alpha; }
    inline T operator*( const vector2D<T>& v ) const { return x*v.x+y*v.y; }
    vector2D<T> operator+( const vector2D<T>& v ) const { return vector2D<T>(x+v.x,y+v.y); }
    vector2D<T> operator-( const vector2D<T>& v ) const { return vector2D<T>(x-v.x,y-v.y); }
    bool operator<( const vector2D<T>& v ) const { return this->getHashNumber()<v.getHashNumber(); }

    int getHashNumber() const { return x*10000+y; }    
    vector2D<T> getVectorOfLength( T length ) const { vector2D<T> newVector(*this); newVector.setLength(length); return newVector; }
    inline bool isZero() const noexcept { return (x==T{} && y==T{}); }
    void set( T newX , T newY ) noexcept { x=newX; y=newY; }
    vector2D<T> setLength( T newLength ) { if( x!=T{} || y!=T{} ){ *this *= (newLength/(*this)()); } return *this; }

    T x {0};
    T y {0};

};

template<typename T>
inline std::ostream& operator<<( std::ostream& os , const vector2D<T>& vector )
{
    os << "v.x = " << vector.x << " , v.y = " << vector.y ;
    return os;
}
