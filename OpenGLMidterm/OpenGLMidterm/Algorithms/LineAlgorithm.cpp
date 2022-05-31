#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

#include "./LineAlgorithm.h"
#include "../Vertex.h"


namespace Algorithms
{
    LineAlgorithm::~LineAlgorithm() = default;

    void LineAlgorithm::sortPoints(Vertex::Vertex& startVertex, Vertex::Vertex& endVertex) const
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

    void LineAlgorithm::roundPoints(Vertex::Vertex& startVertex, Vertex::Vertex& endVertex) const
    {
        startVertex.setX(std::round(startVertex.getX()));
        startVertex.setY(std::round(startVertex.getY()));
        endVertex.setX(std::round(endVertex.getX()));
        endVertex.setY(std::round(endVertex.getY()));
    }
}