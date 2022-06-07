#include "Polygon.h"

namespace Graph2D
{
    Polygon::Polygon(const std::vector<Graph2D::Vertex>& vertices) : _vertices{ vertices }
    {
    }

    const std::vector<Graph2D::Vertex>& Polygon::getVertices() const
    {
        return _vertices;
    }
}