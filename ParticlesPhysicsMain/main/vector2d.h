#pragma once

#include <iostream>
#include <boost/functional/hash.hpp>

/** @file
 * @brief Template class @ref vector2D
 */

/**
 * @class vector2D
 * @brief Defines two dimensional vector.
 *
 * Implements basic functions to operate with 2D vector.
 * @author Tomasz Gburek
 * @date 2019
 */


template<typename T>
class vector2D
{

    /**
     * @class DistanceProxy
     * @brief Defines proxy for distance calculations between vectors.
     *
     * Provides optimalized distance calculation based on squared comparisons.
     */
    class DistanceProxy
    {

    public:

      /**
       * @brief Constructor
       *
       * Constructs proxy distance between two vectors.
       * @param p1          first vector
       * @param p2          second vector
       */
      DistanceProxy( const vector2D<T>& p1 , const vector2D<T>& p2 ): distance_sqrd_{ (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y) } {}

      /**
       * @brief Compares (equal operator) two proxy distances.
       *
       * @param dp              a given proxy distance
       * @return true if equals otherwise false
       */
      bool operator==( const DistanceProxy& dp ) const
      {
          return distance_sqrd_ == dp.distance_sqrd_;
      }

      /**
       * @brief Compares (less then operator) two proxy distances.
       *
       * @param dp              a given proxy distance
       * @return true if equals otherwise false
       */
      bool operator<( const DistanceProxy& dp ) const
      {
          return distance_sqrd_ < dp.distance_sqrd_;
      }

      /**
       * @brief Compares (less then or equal operator) two proxy distances.
       *
       * @param dp              a given proxy distance
       * @return true if equals otherwise false
       */
      bool operator<=( const DistanceProxy& dp ) const
      {
          return distance_sqrd_ <= dp.distance_sqrd_;
      }

      /**
       * @brief Compares (less then operator) proxy distances with a specific distance.
       *
       * @param distance         a given distance
       * @return true if equals otherwise false
       */
      bool operator<( T distance ) const
      {
          return distance_sqrd_ < distance*distance;
      }

      /**
       * @brief Compares (less then or equal operator) proxy distances with a specific distance.
       *
       * @param distance         a given distance
       * @return true if equals otherwise false
       */
      bool operator<=( T distance ) const
      {
          return distance_sqrd_ <= distance*distance;
      }

      /**< Implicit casts to T that returns true distance */
      operator T() const { return std::sqrt(distance_sqrd_); }

    private:

      /**< squared distance value */
      T distance_sqrd_ {};

    };

public:

    /**
     * @brief Default constructor
     */
    vector2D() = default;

    /**
     * @brief Constructor
     *
     * Constructs 2D vector [x,y].
     * @param x           x value
     * @param y           y value
     */
    vector2D( T x , T y ) : x {x}, y {y} {}

    /**
     * @brief Calculates the length of vector.
     *
     * @return length of vector
     */
    T operator()() const { return sqrt(x*x+y*y); }

    /**
     * @brief Calculates squared length of vector.
     *
     * @return squared length of vector
     */
    T operator~() const { return (x*x+y*y); }

    /**
     * @brief Gets distance from this vector to a given vector.
     *
     * @param vector         a given vector
     * @return proxy distance
     */
    DistanceProxy getDistanceTo( const vector2D<T>& vector ) const
    {
        return {*this,vector};
    }

    /**
     * @brief Compares (equal operator) this vector to a given vector.
     *
     * @param v              a given vector
     * @return true if equals otherwise false
     */
    bool operator==( const vector2D<T>& v ) const { return ((x==v.x)&&(y==v.y)); }

    /**
     * @brief Creates perpendicular vector to this vector.
     *
     * @return new created perpendicular vector
     */
    vector2D<T> operator!() const { return {y,-x}; }

    /**
     * @brief Creates vector as result of multiplication of this vector by a given scalar.
     *
     * @param alpha          a given scalar
     * @return new created multiplied vector
     */
    vector2D<T> operator*( const T& alpha ) const { return {alpha*x,alpha*y}; }

    /**
     * @brief Multiples this vector by a given vector.
     *
     * @param v              a given vector
     */
    void operator*=( const vector2D<T>& v ) { *this = (*this)*v; }

    /**
     * @brief Adds this vector to a given vector.
     *
     * @param v              a given vector
     */
    void operator+=( const vector2D<T>& v ) { *this = (*this)+v; }

    /**
     * @brief Subtracts a given vector from this vector.
     *
     * @param v              a given vector
     */
    void operator-=( const vector2D<T>& v ) { *this = (*this)-v; }

    /**
     * @brief Multiples this vector by a given scalar.
     *
     * @param alpha          a given scalar
     * @return new multiplied vector
     */
    void operator*=( const T& alpha ) { *this = (*this)*alpha; }

    /**
     * @brief Creates vector as result of multiplication of this vector by a given vector.
     *
     * @param v              a given vector
     * @return new multiplied vector
     */
    T operator*( const vector2D<T>& v ) const { return x*v.x+y*v.y; }

    /**
     * @brief Creates vector as result of adding of this vector and a given vector.
     *
     * @param v              a given vector
     * @return new added vector
     */
    vector2D<T> operator+( const vector2D<T>& v ) const { return {x+v.x,y+v.y}; }

    /**
     * @brief Creates vector as result of subtraction of this vector and a given vector.
     *
     * @param v              a given vector
     * @return new subtracted vector
     */
    vector2D<T> operator-( const vector2D<T>& v ) const { return {x-v.x,y-v.y}; }

    /**
     * @brief Compares (less then operator) this vector to a given vector.
     *
     * Using hash values to define `less then` relation.
     * @param v              a given vector
     * @return true if this vector less then a given one otherwise false
     */
    bool operator<( const vector2D<T>& v ) const { return getHashValue()<v.getHashValue(); }

    /**
     * @brief Gets (produces) almost unique hash value.
     *
     * @return hash value
     */
    size_t getHashValue() const
    {
        std::size_t seed {0};
        boost::hash_combine(seed,x);
        boost::hash_combine(seed,y);
        return seed;
    }

    /**
     * @brief Creates new vector with the same direction as this but a given length.
     *
     * @param length         length of vector
     * @return new created vector
     */
    vector2D<T> getVectorOfLength( T length ) const { vector2D<T> newVector(*this); newVector.setLength(length); return newVector; }

    /**
     * @brief Checks if this vector has not zeros coefficients.
     *
     * Use only with integral types. For floating point may be trickery.
     * @return true if vector is equal [0,0] otherwise false
     */
    bool isZero() const noexcept { return (x==T{} && y==T{}); }

    /**
     * @brief Sets vectors values.
     *
     * @param newX            new x value
     * @param newY            new y value
     */
    void set( T newX , T newY ) noexcept { x=newX; y=newY; }

    /**
     * @brief Sets a given length to this vector.
     *
     * @param length         length of vector
     * @return copy/move of this vector
     */
    vector2D<T> setLength( T newLength ) { if( x!=T{} || y!=T{} ){ *this *= (newLength/(*this)()); } return *this; }

    /**< x vector value */
    T x {};

    /**< y vector value */
    T y {};

};

template<typename T>
inline std::ostream& operator<<( std::ostream& os , const vector2D<T>& vector )
{
    os << "v.x = " << vector.x << " , v.y = " << vector.y ;
    return os;
}
