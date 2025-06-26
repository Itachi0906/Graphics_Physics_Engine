#include "core.h"

using namespace Grics;

const Vector3 Vector3::GRAVITY = Vector3(0,-9.81,0);

real Matrix4::getDeterminant() const
{
	return data[8] * data[5] * data[2] + data[4] * data[9] * data[2] + data[8] * data[1] * data[6] -
		data[0] * data[9] * data[6] - data[4] * data[1] * data[10] + data[0] * data[5] * data[10];
}

void Matrix4::setInverse(const Matrix4& m)
{
    // Make sure the determinant is non-zero. 
    real det = getDeterminant();
    if (det == 0) return;
    det = ((real)1.0) / det;

    data[0] = (-m.data[9] * m.data[6] + m.data[5] * m.data[10]) * det;
    data[4] = (m.data[8] * m.data[6] - m.data[4] * m.data[10]) * det;
    data[8] = (-m.data[8] * m.data[5] + m.data[4] * m.data[9]) * det;

    data[1] = (m.data[9] * m.data[2] - m.data[1] * m.data[10]) * det;
    data[5] = (-m.data[8] * m.data[2] + m.data[0] * m.data[10]) * det;
    data[9] = (m.data[8] * m.data[1] - m.data[0] * m.data[9]) * det;

    data[2] = (-m.data[5] * m.data[2] + m.data[1] * m.data[6]) * det;
    data[6] = (+m.data[4] * m.data[2] - m.data[0] * m.data[6]) * det;
    data[10] = (-m.data[4] * m.data[1] + m.data[0] * m.data[5]) * det;

    data[3] = (m.data[9] * m.data[6] * m.data[3]
        - m.data[5] * m.data[10] * m.data[3]
        - m.data[9] * m.data[2] * m.data[7]
        + m.data[1] * m.data[10] * m.data[7]
        + m.data[5] * m.data[2] * m.data[11]
        - m.data[1] * m.data[6] * m.data[11]) * det;
    data[7] = (-m.data[8] * m.data[6] * m.data[3]
        + m.data[4] * m.data[10] * m.data[3]
        + m.data[8] * m.data[2] * m.data[7]
        - m.data[0] * m.data[10] * m.data[7]
        - m.data[4] * m.data[2] * m.data[11]
        + m.data[0] * m.data[6] * m.data[11]) * det;
    data[11] = (m.data[8] * m.data[5] * m.data[3]
        - m.data[4] * m.data[9] * m.data[3]
        - m.data[8] * m.data[1] * m.data[7]
        + m.data[0] * m.data[9] * m.data[7]
        + m.data[4] * m.data[1] * m.data[11]
        - m.data[0] * m.data[5] * m.data[11]) * det;
}

/*
 * Definition of the sleep epsilon extern.
 */
real Grics::sleepEpsilon = ((real)0.3);

/*
 * Functions to change sleepEpsilon.
 */
void Grics::setSleepEpsilon(real value)
{
    Grics::sleepEpsilon = value;
}

real Grics::getSleepEpsilon()
{
    return Grics::sleepEpsilon;
}
