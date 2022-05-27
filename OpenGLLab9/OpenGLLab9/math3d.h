// math3d.h
// Math3D Library, referenced to https://github.com/msteinert/gltools/blob/master/include/math3d.h
// Only implement the needs for my lab9 work

#pragma once

#ifndef MATH_3D_H
#define MATH_3D_H
#include <array>
#include <utility>

namespace math3d
{
    template<typename T>
    class Vector2 : public std::array<T, 2>
    {
    public:
        Vector2(T x, T y);
        friend Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs);
        friend Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs);
        friend T operator*(const Vector2<T>& lhs, const Vector2<T>& rhs);
    };

    template<typename T>
    Vector2<T>::Vector2(T x, T y) : std::array<T, 2>{ std::move(x), std::move(y) }
    {
    }

    template<typename T>
    Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs)
    {
        return { lhs[0] + rhs[0], lhs[1] + rhs[1] };
    }

    template<typename T>
    Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs)
    {
        return { lhs[0] - rhs[0], lhs[1] - rhs[1] };
    }

    template<typename T>
    T operator*(const Vector2<T>& lhs, const Vector2<T>& rhs)
    {
        return lhs[0] * rhs[0] + lhs[1] * rhs[1];
    }

    // 2D vector
    using M3DVector2f = std::array<float, 2>;
    using M3DVector2d = std::array<double, 2>;

    // 3D vector
    using M3DVector3f = std::array<float, 3>;
    using M3DVector3d = std::array<double, 3>;

    // for other usages
    using M3DVector4f = std::array<float, 3>;
    using M3DVector4d = std::array<double, 3>;

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
    M3DVector4f getPlaneEquation(const M3DVector3f& point1, const M3DVector3f& point2, const M3DVector3f& point3);
    M3DVector4d getPlaneEquation(const M3DVector3d& point1, const M3DVector3d& point2, const M3DVector3d& point3);

    // Planar shadow Matrix
    M3DMatrix44d makePlanarShadowMatrix(const M3DVector4d& planeEq, const M3DVector3d& vLightPos);
    M3DMatrix44f makePlanarShadowMatrix(const M3DVector4f& planeEq, const M3DVector3f& vLightPos);

    // Calculates the normal of a triangle specified by the three points
    // p1, p2, and p3. Each pointer points to an array of three floats. The
    // triangle is assumed to be wound counter clockwise.
    M3DVector3f findNormal(const M3DVector3f& point1, const M3DVector3f& point2, const M3DVector3f& point3);
    M3DVector3f findNormal(const M3DVector3d& point1, const M3DVector3d& point2, const M3DVector3d& point3);

    // Cross Product
    // u x v = result
    // 3 component vectors only.
    inline M3DVector3f crossProduct(const M3DVector3f& u, const M3DVector3f& v)
    {
        return { u[1] * v[2] - v[1] * u[2], -u[0] * v[2] + v[0] * u[2], u[0] * v[1] - v[0] * u[1] };
    }

    inline M3DVector3d crossProduct(const M3DVector3d& u, const M3DVector3d& v)
    {
        return { u[1] * v[2] - v[1] * u[2], -u[0] * v[2] + v[0] * u[2], u[0] * v[1] - v[0] * u[1] };
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators for data structures
    inline M3DVector2f operator+(const M3DVector2f& lhs, const M3DVector2f& rhs)
    {
        return { lhs[0] + rhs[0], lhs[1] + rhs[1] };
    }

    inline M3DVector2d operator+(const M3DVector2d& lhs, const M3DVector2d& rhs)
    {
        return { lhs[0] + rhs[0], lhs[1] + rhs[1] };
    }

    inline M3DVector3f operator+(const M3DVector3f& lhs, const M3DVector3f& rhs)
    {
        return { lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2] };
    }

    inline M3DVector3d operator+(const M3DVector3d& lhs, const M3DVector3d& rhs)
    {
        return { lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2] };
    }

    // Minus
    inline M3DVector2f operator-(const M3DVector2f& lhs, const M3DVector2f& rhs)
    {
        return { lhs[0] - rhs[0], lhs[1] - rhs[1] };
    }

    inline M3DVector2d operator-(const M3DVector2d& lhs, const M3DVector2d& rhs)
    {
        return { lhs[0] - rhs[0], lhs[1] - rhs[1] };
    }

    inline M3DVector3f operator-(const M3DVector3f& lhs, const M3DVector3f& rhs)
    {
        return { lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2] };
    }

    inline M3DVector3d operator-(const M3DVector3d& lhs, const M3DVector3d& rhs)
    {
        return { lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2] };
    }

    // Inner Product
    inline float operator*(const M3DVector2f& lhs, const M3DVector2f& rhs)
    {
        return lhs[0] * rhs[0] + lhs[1] * rhs[1];
    }

    inline double operator*(const M3DVector2d& lhs, const M3DVector2d& rhs)
    {
        return lhs[0] * rhs[0] + lhs[1] * rhs[1];
    }

    inline float operator*(const M3DVector3f& lhs, const M3DVector3f& rhs)
    {
        return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
    }

    inline double operator*(const M3DVector3d& lhs, const M3DVector3d& rhs)
    {
        return lhs[0] * rhs[0] + lhs[1] * rhs[1];
    }
}

#endif