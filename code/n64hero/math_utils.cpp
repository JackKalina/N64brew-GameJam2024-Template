#include "math_utils.h"

#include <libdragon.h>

#include <cmath>

T3DVec3 axisAngleToEuler(T3DVec3 axis, float angle) {
    t3d_vec3_norm(&axis);

    // Compute the rotation matrix using axis-angle formula
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    float oneMinusCosA = 1.0f - cosA;

    T3DMat4 rotMat = {
        {{cosA + axis.v[0] * axis.v[0] * oneMinusCosA, axis.v[0] * axis.v[1] * oneMinusCosA - axis.v[2] * sinA,
          axis.v[0] * axis.v[2] * oneMinusCosA + axis.v[1] * sinA, 0.0f},
         {axis.v[1] * axis.v[0] * oneMinusCosA + axis.v[2] * sinA, cosA + axis.v[1] * axis.v[1] * oneMinusCosA,
          axis.v[1] * axis.v[2] * oneMinusCosA - axis.v[0] * sinA, 0.0f},
         {axis.v[2] * axis.v[0] * oneMinusCosA - axis.v[1] * sinA, axis.v[2] * axis.v[1] * oneMinusCosA + axis.v[0] * sinA,
          cosA + axis.v[2] * axis.v[2] * oneMinusCosA, 0.0f},
         {0.0f, 0.0f, 0.0f, 1.0f}}};

    // Extract Euler angles from the rotation matrix
    T3DVec3 euler;

    if (rotMat.m[2][0] < 1.0f) {
        if (rotMat.m[2][0] > -1.0f) {
            euler = {{std::atan2(-rotMat.m[2][1], rotMat.m[2][2]), std::atan2(-rotMat.m[1][0], rotMat.m[0][0]),
                      std::asin(rotMat.m[2][0])}};
        } else {
            euler = {{0, -T3D_PI / 2, -std::atan2(rotMat.m[0][1], rotMat.m[1][1])}};
        }
    } else {
        euler = {{0, T3D_PI / 2, std::atan2(rotMat.m[0][1], rotMat.m[1][1])}};
    }

    return euler;
}