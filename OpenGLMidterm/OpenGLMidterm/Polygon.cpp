#include "Polygon.h"

namespace Polygon
{
    Polygon::Polygon(const std::vector<Vertex::Vertex>& vertices) : _vertices{ vertices }
    {
    }

    const std::vector<Vertex::Vertex>& Polygon::getVertices() const
    {
        return _vertices;
    }
}