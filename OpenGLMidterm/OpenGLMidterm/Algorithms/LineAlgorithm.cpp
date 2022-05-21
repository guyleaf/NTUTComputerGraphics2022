#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

#include "./LineAlgorithm.h"
#include "../Vertex.h"


namespace Algorithms
{
    LineAlgorithm::~LineAlgorithm() = default;

    void LineAlgorithm::sortPoints(const Vertex::Vertex **startVertex, const Vertex::Vertex **endVertex) const
    {
        const double startX = (*startVertex)->getX();
        const double startY = (*startVertex)->getY();
        const double endX = (*endVertex)->getX();
        const double endY = (*endVertex)->getY();

        if (startX > endX || (startX == endX && startY > endY))
        {
            std::swap(*startVertex, *endVertex);
        }
    }
}