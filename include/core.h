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



    class Quaternion
    {
    public:
        union {
            struct {
                /**
                * Holds the real component of the quaternion.
                */
                real r;

                /**
                 * Holds the first complex component of the
                 * quaternion.
                 */
                real i;

                /**
                 * Holds the second complex component of the
                 * quaternion.
                 */
                real j;

                /**
                 * Holds the third complex component of the
                 * quaternion.
                 */
                real k;
            };
            
            /**
            * Holds the Quaternion in array form
            */
            real data[4];
        };

        /**
         * The default constructor creates a quaternion representing
         * a zero rotation.
         */
        Quaternion() : r(1), i(0), j(0), k(0) {}

        /**
        * The explicit constructor creates the Quaternion with given components
        */
        Quaternion(const real r, const real i, const real j, const real k) : r(r), i(i), j(j), k(k) {}
        
        /**
         * Normalises the quaternion to unit length, making it a valid
         * orientation quaternion.
         */
        void normalize()
        {
            real d = r * r + i * i + j * j + k * k;
            if (d < real_epsilon)
            {
                r = 1;
                return;
            }

            d = ((real)1.0)/ real_sqrt(d);
            r *= d;
            i *= d;
            j *= d;
            k *= d;
        }
        
        /**
        * Multiplies the quaternion by the given quaternion.
        */
        void operator *=(const Quaternion& multiplier)
        {
            Quaternion q = *this;
            r = q.r * multiplier.r - q.i * multiplier.i -
                q.j * multiplier.j - q.k * multiplier.k;
            i = q.r * multiplier.i + q.i * multiplier.r +
                q.j * multiplier.k - q.k * multiplier.j;
            j = q.r * multiplier.j + q.j * multiplier.r +
                q.k * multiplier.i - q.i * multiplier.k;
            k = q.r * multiplier.k + q.k * multiplier.r +
                q.i * multiplier.j - q.j * multiplier.i;
        }

        /**
         * Adds the given vector to this, scaled by the given amount.
         * This is used to update the orientation quaternion by a rotation
         * and time.
         */
        void addScaledVector(const Vector3& vector, real scale)
        {
            Quaternion q(0,
                vector.x * scale,
                vector.y * scale,
                vector.z * scale);
            q *= *this;
            r += q.r * ((real)0.5);
            i += q.i * ((real)0.5);
            j += q.j * ((real)0.5);
            k += q.k * ((real)0.5);
        }

        void rotateByVector(const Vector3& vector)
        {
            Quaternion q(0, vector.x, vector.y, vector.z);
            (*this) *= q;
        }
    };


    /** 
    * Holds an inertia tensor, consisting of a 3x3 row-major matrix. 
    * This matrix is not padding to produce an aligned structure, since 
    * it is most commonly used with a mass (single real) and two 
    * damping coefficients to make the 12-element characteristics array 
    * of a Rigid Body.
    */
    class Matrix3 {
        /**
        * Holds the tensor matrix in array form.
        */
    public:
        real data[9];

        Matrix3();

        Matrix3(const real o1, const real o2, const real o3, const real o4, const real o5, const real o6, const real o7, const real o8, const real o9)
        {
            data[0] = o1;
            data[1] = o2;
            data[2] = o3;
            data[3] = o4;
            data[4] = o5;
            data[5] = o6;
            data[6] = o7;
            data[7] = o8;
            data[8] = o9;
        }

        Matrix3(const real ele[9])
        {
            for (int i = 0;i < 9;i++)
            {
                data[i] = ele[i];
            }
        }

        /** 
        * Returns a matrix that is this matrix multiplied by the given 
        * other matrix.
        */
        Matrix3 operator*(const Matrix3& o) const
        {
            return Matrix3({
                data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6],
                data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7],
                data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8],

                data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6],
                data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7],
                data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8],

                data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6],
                data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7],
                data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8]
                });
        }
        /** 
        * Sets the matrix to be the inverse of the given matrix. 
        */
        void setInverse(const Matrix3& m)
        {
            real t4 = m.data[0] * m.data[4] ;
            real t6 = m.data[0] * m.data[5]; 
            real t8 = m.data[1] * m.data[3]; 
            real t10 = m.data[2] * m.data[3]; 
            real t12 = m.data[1] * m.data[6]; 
            real t14 = m.data[2] * m.data[6];

            // Calculate the determinant.
            real t16 = (t4*m.data[8] - t6*m.data[7] - t8*m.data[8] + t10*m.data[7] + t12*m.data[5] - t14*m.data[4]);

            // Make sure the determinant is non-zero. 
            if (t16 == (real)0.0f) return; 
            real t17 = 1/t16;
            data[0] = (m.data[4] * m.data[8] - m.data[5] * m.data[7]) * t17; 
            data[1] = -(m.data[1] * m.data[8] - m.data[2] * m.data[7]) * t17; 
            data[2] = (m.data[1] * m.data[5] - m.data[2] * m.data[4]) * t17;
            data[3] = -(m.data[3] * m.data[8] - m.data[5] * m.data[6]) * t17; 
            data[4] = (m.data[0] * m.data[8] - t14) * t17; 
            data[5] = -(t6 - t10) * t17; 
            data[6] = (m.data[3] * m.data[7] - m.data[4] * m.data[6]) * t17; 
            data[7] = -(m.data[0] * m.data[7] - t12) * t17; 
            data[8] = (t4 - t8) * t17;
        }

        /**
        * Returns a new matrix containing the inverse of this matrix. 
        */
        Matrix3 inverse() const
        {
            Matrix3 result;
            result.setInverse(*this);
            return result;
        }

        /**
        * Inverts the matrix
        */
        void invert()
        {
            setInverse(*this);
        }

        /**
        * Sets the matrix to be the transpose of the given matrix 
        */
        void setTranspose(const Matrix3& m)
        {
            data[0] = m.data[0];
            data[1] = m.data[3];
            data[2] = m.data[6];
            data[3] = m.data[1];
            data[4] = m.data[4];
            data[5] = m.data[7];
            data[6] = m.data[2];
            data[7] = m.data[5];
            data[8] = m.data[8];
        }

        /**
        * Returns a new matrix containing the transpose of this matrix
        */
        Matrix3 transpose() const
        {
            Matrix3 result;
            result.setTranspose(*this);
            return result;
        }

        /** * Sets this matrix to be the rotation matrix corresponding to 
        * the given quaternion.
        */
        void setOrientation(const Quaternion& q) {
            data[0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k); 
            data[1] = 2 * q.i * q.j + 2 * q.k * q.r; 
            data[2] = 2 * q.i * q.k - 2 * q.j * q.r; 
            data[3] = 2 * q.i * q.j - 2 * q.k * q.r; 
            data[4] = 1 - (2 * q.i * q.i + 2 * q.k * q.k); 
            data[5] = 2 * q.j * q.k + 2 * q.i * q.r; 
            data[6] = 2 * q.i * q.k + 2 * q.j * q.r; 
            data[7] = 2 * q.j * q.k - 2 * q.i * q.r; 
            data[8] = 1 - (2 * q.i * q.i + 2 * q.j * q.j);
        }
    };

    /** 
    * Holds a transform matrix, consisting of a rotation matrix and 
    * a position. The matrix has 12 elements; it is assumed that the 
    * remaining four are (0,0,0,1), producing a homogenous matrix. 
    */
    class Matrix4 {
        /**
        * Holds the transform matrix in array form.
        */
    public:
        real data[12];

        Matrix4 operator*(const Matrix4& o) const
        {
            Matrix4 result; 
            result.data[0] = (o.data[0] * data[0]) + (o.data[4] * data[1]) + (o.data[8] * data[2]);
            result.data[4] = (o.data[0] * data[4]) + (o.data[4] * data[5]) + (o.data[8] * data[6]);
            result.data[8] = (o.data[0] * data[8]) + (o.data[4] * data[9]) + (o.data[8] * data[10]);

            result.data[1] = (o.data[1] * data[0]) + (o.data[5] * data[1]) + (o.data[9] * data[2]);
            result.data[5] = (o.data[1] * data[4]) + (o.data[5] * data[5]) + (o.data[9] * data[6]);
            result.data[9] = (o.data[1] * data[8]) + (o.data[5] * data[9]) + (o.data[9] * data[10]);

            result.data[2] = (o.data[2] * data[0]) + (o.data[6] * data[1]) + (o.data[10] * data[2]);
            result.data[6] = (o.data[2] * data[4]) + (o.data[6] * data[5]) + (o.data[10] * data[6]);
            result.data[10] = (o.data[2] * data[8]) + (o.data[6] * data[9]) + (o.data[10] * data[10]);

            result.data[3] = (o.data[3] * data[0]) + (o.data[7] * data[1]) + (o.data[11] * data[2]) + data[3];
            result.data[7] = (o.data[3] * data[4]) + (o.data[7] * data[5]) + (o.data[11] * data[6]) + data[7];
            result.data[11] = (o.data[3] * data[8]) + (o.data[7] * data[9]) + (o.data[11] * data[10]) + data[11];

            return result;
        }

        Vector3 operator*(const Vector3& vector) const 
        {
            return Vector3(
                vector.x * data[0] + vector.y * data[1] + vector.z * data[2] + data[3],
                vector.x * data[4] + vector.y * data[5] + vector.z * data[6] + data[7],
                vector.x * data[8] + vector.y * data[9] + vector.z * data[10] + data[11]
                );
        }

        /**
        * Returns the determinant of this matrix
        */
        real getDeterminant() const;

        /**
        * Sets the matrix to be the inverse of the given matrix
        */
        void setInverse(const Matrix4& m);

        /**
        * Returns a new matrix containing the inverse of this matrix
        */
        Matrix4 inverse() const {
            Matrix4 result;
            result.setInverse(*this); return result;
        }
        /** 
        * Inverts the matrix.
        */
        void invert() {
            setInverse(*this);
        }

        /**
        * Sets this matrix to be the rotation matrix corresponding to the given Quaternion
        */
        void setOrientationAndPos(const Quaternion& q, const Vector3& pos) {
            data[0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k); 
            data[1] = 2 * q.i * q.j + 2 * q.k * q.r; 
            data[2] = 2 * q.i * q.k - 2 * q.j * q.r; 
            data[3] = pos.x;
            data[4] = 2 * q.i * q.j - 2 * q.k * q.r; 
            data[5] = 1 - (2 * q.i * q.i + 2 * q.k * q.k); 
            data[6] = 2 * q.j * q.k + 2 * q.i * q.r; 
            data[7] = pos.y;
            data[8] = 2 * q.i * q.k + 2 * q.j * q.r; 
            data[9] = 2 * q.j * q.k - 2 * q.i * q.r; 
            data[10] = 1 - (2 * q.i * q.i + 2 * q.j * q.j); 
            data[11] = pos.z;
        }

        /** 
        * Transform the given vector by the transformational inverse * of this matrix.
        */
        Vector3 transformInverse(const Vector3& vector) const {
            Vector3 tmp = vector;
            tmp.x -= data[3];
            tmp.y -= data[7];
            tmp.z -= data[11];
            return Vector3(tmp.x * data[0] + tmp.y * data[4] + tmp.z * data[8],
                tmp.x * data[1] +
                tmp.y * data[5] + tmp.z * data[9],
                tmp.x * data[2] + tmp.y * data[6] + tmp.z * data[10]
            );
        }

        /** 
        * Transform the given direction vector by this matrix. 
        */
        Vector3 transformDirection(const Vector3& vector) const {
            return Vector3(vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
                vector.x * data[4] + vector.y * data[5] + vector.z * data[6],
                vector.x * data[8] + vector.y * data[9] + vector.z * data[10]
            );
        }

        /** 
        * Transform the given direction vector by the * transformational inverse of this matrix.
        */
        Vector3 transformInverseDirection(const Vector3& vector) const {
            return Vector3(vector.x * data[0] + vector.y * data[4] + vector.z * data[8],
                vector.x * data[1] + vector.y * data[5] + vector.z * data[9],
                vector.x * data[2] + vector.y * data[6] + vector.z * data[10]
            );
        }
    };
}

#endif