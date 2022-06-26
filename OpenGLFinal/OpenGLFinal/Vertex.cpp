#include <array>
#include <utility>

#include "Vertex.h"

namespace Graph2D
{
    Vertex::Vertex()
        : _x(0.0f), _y(0.0f), _z(0.0f)
    {
    }

    Vertex::Vertex(float x, float y, float z)
        : _x(x), _y(y), _z(z)
    {
    }

    double Vertex::getX() const
    {
        return _x;
    }

    void Vertex::setX(float value)
    {
        _x = value;
    }

    double Vertex::getY() const
    {
        return _y;
    }

    void Vertex::setY(float value)
    {
        _y = value;
    }

    double Vertex::getZ() const
    {
        return _z;
    }

    void Vertex::setZ(float value)
    {
        _z = value;
    }

    bool operator==(const Vertex& lhs, const Vertex& rhs)
    {
        return lhs._x == rhs._x && lhs._y == rhs._y && lhs._z == rhs._z;
    }
}