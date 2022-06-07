#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

#include "./LineAlgorithm.h"
#include "../Graph2D/Vertex.h"


namespace RasterizationAlgorithms
{
    LineAlgorithm::~LineAlgorithm() = default;

    void LineAlgorithm::sortPoints(Graph2D::Vertex& startVertex, Graph2D::Vertex& endVertex) const
    {
        const double startX = startVertex.getX();
        const double startY = startVertex.getY();
        const double endX = endVertex.getX();
        const double endY = endVertex.getY();

        if (startX > endX || (startX == endX && startY > endY))
        {
            std::swap(startVertex, endVertex);
        }
    }

    void LineAlgorithm::roundPoints(Graph2D::Vertex& startVertex, Graph2D::Vertex& endVertex) const
    {
        startVertex.setX(std::round(startVertex.getX()));
        startVertex.setY(std::round(startVertex.getY()));
        endVertex.setX(std::round(endVertex.getX()));
        endVertex.setY(std::round(endVertex.getY()));
    }
}