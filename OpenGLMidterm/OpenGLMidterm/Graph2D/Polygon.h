#pragma once
#include <vector>

#include "Vertex.h"

namespace Graph2D
{
    class Polygon
    {
    public:
        explicit Polygon(const std::vector<Graph2D::Vertex>& vertices);

        const std::vector<Graph2D::Vertex>& getVertices() const;
    private:
        std::vector<Graph2D::Vertex> _vertices;
    };
}