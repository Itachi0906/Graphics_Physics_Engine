#pragma once
#ifndef GRICS_CORE_H
#define GRICS_CORE_H
#include "precision.h"

namespace Grics {
    class Vector3 {
    public:
        /**Holds the value along x-axis*/
        real x;
        /**Holds the value along y-axis*/
        real y;
        /**Holds the value along z-axis*/
        real z;

    private:
        /**Padding to ensure 4-word alignment*/
        real pad;

    public:
        /**the default constructor creates a zero vector*/
        Vector3() : x(0), y(0), z(0) {}
        /**
        * The explicit constructor creates a vector with the given
        * components.
        */
        Vector3(const real x, const real y, const real z) : x(x), y(y), z(z) {}

        /** Multiplies this vector by the given scalar. */
        void operator*=(const real value)
        {
            x *= value;
            y *= value;
            z *= value;
        }

        /**Returns a copy of this vectore scaled to the given value*/
        Vector3 operator*(const real value) const
        {
            return Vector3(x * value, y * value, z * value);
        }

        /**Adds the given vector to this*/
        void operator+=(const Vector3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
        }

        /**Returns a copy of given vector added to this*/
        Vector3 operator+(const Vector3& v)
        {
            return Vector3(x + v.x, y + v.y, z + v.z);
        }

        /**Subtracts the given vector to this*/
        void operator-=(const Vector3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }

        /**Returns a copy of given vector subtracted to this*/
        Vector3 operator-(const Vector3& v)
        {
            return Vector3(x - v.x, y - v.y, z - v.z);
        }


        /**Adds the given vector to this, scaled by the given amount*/
        void addScaledVector(const Vector3& vector, real scale)
        {
            x += vector.x * scale;
            y += vector.y * scale;
            z += vector.z * scale;

        }

        /**
        * Calculates and returns a component-wise product of this
        * vector with the given vector.
        */
        Vector3 componentProduct(const Vector3& vector) const
        {
            return Vector3(x * vector.x, y * vector.y, z * vector.z);
        }

        /** Performs component wise product with the given vector sets this
        * vector to this result*/
        void componentProductUpdate(const Vector3& vector)
        {
            x *= vector.x;
            y *= vector.y;
            z *= vector.z;
        }

        /**Calculates and returns the scalar product of the this vector
         * with given vector
         */
        real scalarProduct(const Vector3& vector) const
        {
            return x * vector.x + y * vector.y + z * vector.z;
        }

        /**Calculates and returns the scalar product of the this vector
         * with given vector
         */
        real operator *(const Vector3& vector) const
        {
            return x * vector.x + y * vector.y + z * vector.z;
        }

        /**Calculates and returns the vector product of this
         * with given vector
        */
        Vector3 vectorProduct(const Vector3& vector) const
        {
            return Vector3(y * vector.z - z * vector.y,
                z * vector.x - x * vector.z,
                x * vector.y - y * vector.x);
        }

        /**
         * Updates this vector to be the vector product of its current
         * value and the given vector
         */
        void operator%=(const Vector3& vector)
        {
            *this = vectorProduct(vector);
        }

        /**Calculates and returns the vector product of this
         * with given vector
         */
        Vector3 operator%(const Vector3& vector)
        {
            return Vector3(y * vector.z - z * vector.y,
                z * vector.x - x * vector.z,
                x * vector.y - y * vector.x);
        }

        /**Flips all the components of this vector*/
        void invert()
        {
            x = -x;
            y = -y;
            z = -z;
        }

        /**Gets the magnitude of this vector*/
        real magnitude() const
        {
            return real_sqrt(x * x + y * y + z * z);
        }

        /**Gets the squared magnitude of this vector */
        real squareMagnitude() const
        {
            return x * x + y * y + z * z;
        }

        /**Turns a non-zero vector to a unit vector(magnitude-1)*/
        void normalize()
        {
            real l = magnitude();
            if (l > 0)
            {
                (*this) *= ((real)1) / l;
            }
        }

        /**
        * Resets the vector
        */
        void clear()
        {
            *this = Vector3();
        }
    };
}

#endif