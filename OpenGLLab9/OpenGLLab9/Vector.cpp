#ifndef VECTOR_CPP
#define VECTOR_CPP

#include <cmath>

#include "Vector.hpp"


namespace math3d
{
    template<typename T, size_t N>
    Vector<T, N>::Vector() = default;

    template<typename T, size_t N>
    Vector<T, N>::Vector(const std::initializer_list<T>& values)
    {
        //std::copy(values.begin(), values.end(), _vector);
    }

    template<typename T, size_t N>
    double Vector<T, N>::lengthd() const
    {
        return std::sqrt((*this) * (*this));
    }

    template<typename T, size_t N>
    float Vector<T, N>::lengthf() const
    {
        return std::sqrtf((*this) * (*this));
    }

    template<typename T, size_t N>
    T& Vector<T, N>::operator[](size_t i)
    {
        return _vector[i];
    }

    template<typename T, size_t N>
    T Vector<T, N>::operator[](size_t i) const
    {
        return _vector[i];
    }

    template<typename T, size_t N>
    T* Vector<T, N>::data()
    {
        return _vector.data();
    }

    template<typename T, size_t N>
    const T* Vector<T, N>::data() const
    {
        return _vector.data();
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator+=(const Vector<T, N>& rhs)
    {
        for (size_t i = 0; i < N; i++)
        {
            _vector[i] += rhs[i];
        }
        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator-=(const Vector<T, N>& rhs)
    {
        for (size_t i = 0; i < N; i++)
        {
            _vector[i] -= rhs[i];
        }
        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N> operator+(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
    {
        return Vector<T, N>(lhs) += rhs;
    }

    template<typename T, size_t N>
    Vector<T, N> operator-(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
    {
        return Vector<T, N>(lhs) -= rhs;
    }

    template<typename T, size_t N>
    T operator*(const Vector<T, N>& lhs, const Vector<T, N>& rhs)
    {
        T result = lhs[0] * rhs[0];
        for (size_t i = 1; i < N; i++)
        {
            result += (lhs[i] * rhs[i]);
        }
        return result;
    }

    template<typename T, size_t N>
    Vector<T, N> operator*(const T scale, const Vector<T, N>& rhs)
    {
        return Vector<T, N>(rhs) *= scale;
    }

    template<typename T, size_t N>
    Vector<T, N> operator*(const Vector<T, N>& lhs, const T scale)
    {
        return Vector<T, N>(lhs) *= scale;
    }

    template<typename T, size_t N>
    Vector<T, N> operator/(const T scale, const Vector<T, N>& rhs)
    {
        return Vector<T, N>(rhs) /= scale;
    }

    template<typename T, size_t N>
    Vector<T, N> operator/(const Vector<T, N>& lhs, const T scale)
    {
        return Vector<T, N>(lhs) /= scale;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator*=(const T scale)
    {
        for (size_t i = 0; i < N; i++)
        {
            _vector[i] *= scale;
        }
        return *this;
    }

    template<typename T, size_t N>
    Vector<T, N>& Vector<T, N>::operator/=(const T scale)
    {
        for (size_t i = 0; i < N; i++)
        {
            _vector[i] /= scale;
        }
        return *this;
    }
}

#endif