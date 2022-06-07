#pragma once
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <array>
#include <utility>

namespace math3d
{
    template<typename T, size_t N>
    class Vector
    {
    public:
        Vector();
        explicit Vector(const std::initializer_list<T>& values);

        double lengthd() const;
        float lengthf() const;

        friend Vector<T, N> operator+(const Vector<T, N>& lhs, const Vector<T, N>& rhs);
        friend Vector<T, N> operator-(const Vector<T, N>& lhs, const Vector<T, N>& rhs);
        friend T operator*(const Vector<T, N>& lhs, const Vector<T, N>& rhs);

        friend Vector<T, N> operator*(const T scale, const Vector<T, N>& rhs);
        friend Vector<T, N> operator*(const Vector<T, N>& lhs, const T scale);

        friend Vector<T, N> operator/(const T scale, const Vector<T, N>& rhs);
        friend Vector<T, N> operator/(const Vector<T, N>& lhs, const T scale);

        Vector<T, N>& operator+=(const Vector<T, N>& rhs);
        Vector<T, N>& operator-=(const Vector<T, N>& rhs);

        Vector<T, N>& operator*=(const T scale);
        Vector<T, N>& operator/=(const T scale);

        T& operator[](size_t i);
        T operator[](size_t i) const;

        T* data();
        const T* data() const;
    private:
        std::array<T, N> _vector;
    };

    #include "Vector.cpp"
}

#endif