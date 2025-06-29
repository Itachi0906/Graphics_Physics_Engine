#pragma once
#ifndef GRICS_PRECISION_H
#define GRICS_PRECISION_H

#include <cmath>
#include <float.h>

namespace Grics {
    typedef float real;
    #define real_sqrt sqrtf
    #define real_pow powf
    #define real_abs fabsf
    #define REAL_MAX FLT_MAX
    #define PI 3.1415926f
    #define real_epsilon DBL_EPSILON
}

#endif

