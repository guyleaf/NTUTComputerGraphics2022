// math3d.h
// Math3D Library, referenced to https://github.com/msteinert/gltools/blob/master/include/math3d.h
// Only implement the needs for my lab9 work

#pragma once
#ifndef MATH3D_H
#define MATH3D_H

#include <cmath>
#include <array>
#include <utility>

namespace Math3D
{
    // 2D vector
    using M3DVector2f = std::array<float, 2>;
    using M3DVector2d = std::array<double, 2>;

    // 3D vector
    using M3DVector3f = std::array<float, 3>;
    using M3DVector3d = std::array<double, 3>;

    // for other usages
    using M3DVector4f = std::array<float, 4>;
    using M3DVector4d = std::array<double, 4>;

    // 3x3 matrix - column major. X vector is 0, 1, 2, etc. (OpenGL style)
    //  0	3	6
    //	1	4	7
    //	2	5	8
    using M3DVector33f = std::array<float, 9>;
    using M3DVector33d = std::array<double, 9>;

    // 4x4 matrix - column major. X vector is 0, 1, 2, etc. (OpenGL style)
    //	0	4	8	12
    //	1	5	9	13
    //	2	6	10	14
    //	3	7	11	15
    using M3DMatrix44f = std::array<float, 16>;
    using M3DMatrix44d = std::array<double, 16>;

    // Get plane equation from three points
    M3DVector4f getPlaneEquation(const M3DVector3f &point1, const M3DVector3f &point2, const M3DVector3f &point3);
    M3DVector4d getPlaneEquation(const M3DVector3d &point1, const M3DVector3d &point2, const M3DVector3d &point3);

    // Planar shadow Matrix
    M3DMatrix44d makePlanarShadowMatrix(const M3DVector4d &planeEq, const M3DVector4d &vLightPos);
    M3DMatrix44f makePlanarShadowMatrix(const M3DVector4f &planeEq, const M3DVector4f &vLightPos);

    // Calculates the normal of a triangle specified by the three points
    // p1, p2, and p3. Each pointer points to an array of three floats. The
    // triangle is assumed to be wound counter clockwise.
    M3DVector3f findNormal(const M3DVector3f &point1, const M3DVector3f &point2, const M3DVector3f &point3);
    M3DVector3d findNormal(const M3DVector3d &point1, const M3DVector3d &point2, const M3DVector3d &point3);

    // Cross Product
    // u x v = result
    // 3 component vectors only.
    inline M3DVector3f crossProduct(const M3DVector3f &u, const M3DVector3f &v)
    {
        return {u[1] * v[2] - v[1] * u[2], -u[0] * v[2] + v[0] * u[2], u[0] * v[1] - v[0] * u[1]};
    }

    inline M3DVector3d crossProduct(const M3DVector3d &u, const M3DVector3d &v)
    {
        return {u[1] * v[2] - v[1] * u[2], -u[0] * v[2] + v[0] * u[2], u[0] * v[1] - v[0] * u[1]};
    }

    // Get the length of vector
    inline float getVectorLength(const M3DVector3f &u)
    {
        return std::sqrtf(u[0] * u[0] + u[1] * u[1] + u[2] * u[2]);
    }

    inline double getVectorLength(const M3DVector3d &u)
    {
        return std::sqrt(u[0] * u[0] + u[1] * u[1] + u[2] * u[2]);
    }

    inline float getVectorLength(const M3DVector4f &u)
    {
        return std::sqrtf(u[0] * u[0] + u[1] * u[1] + u[2] * u[2] + u[3] * u[3]);
    }

    inline double getVectorLength(const M3DVector4d &u)
    {
        return std::sqrt(u[0] * u[0] + u[1] * u[1] + u[2] * u[2] + u[3] * u[3]);
    }

    // Scale a vector
    inline void scaleVector(M3DVector3f &u, float scale)
    {
        u[0] *= scale;
        u[1] *= scale;
        u[2] *= scale;
    }

    inline void scaleVector(M3DVector3d &u, double scale)
    {
        u[0] *= scale;
        u[1] *= scale;
        u[2] *= scale;
    }

    inline void scaleVector(M3DVector4f &u, float scale)
    {
        u[0] *= scale;
        u[1] *= scale;
        u[2] *= scale;
        u[3] *= scale;
    }

    inline void scaleVector(M3DVector4d &u, double scale)
    {
        u[0] *= scale;
        u[1] *= scale;
        u[2] *= scale;
        u[3] *= scale;
    }

    // Normalize a vector
    // Scale a vector to unit length. Easy, just scale the vector by it's length
    inline void normalizeVector(M3DVector3f &u)
    {
        scaleVector(u, 1.0f / getVectorLength(u));
    }

    inline void normalizeVector(M3DVector3d &u)
    {
        scaleVector(u, 1.0 / getVectorLength(u));
    }

    inline void normalizeVector(M3DVector4f &u)
    {
        scaleVector(u, 1.0f / getVectorLength(u));
    }

    inline void normalizeVector(M3DVector4d &u)
    {
        scaleVector(u, 1.0 / getVectorLength(u));
    }
}

#endif