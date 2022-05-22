#define _USE_MATH_DEFINES
#include <cmath>

#include "AntiAliasingAlgorithm.h"

namespace Algorithms
{
    AntiAliasingAlgorithm::AntiAliasingAlgorithm(const std::function<void(const Vertex::Vertex&)>& setPixel) : _setPixel{ setPixel }
    {
    }

    std::string AntiAliasingAlgorithm::getName() const
    {
        return _name;
    }

    void AntiAliasingAlgorithm::rasterizeLineInPositiveSlope(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, const double& slope) const
    {
        double x = startVertex.getX();
        double y = startVertex.getY();
        std::array<double, 4> color{ _color };

        // if the slope is bigger than 1
        if (slope >= 1.0)
        {
            const double endY = endVertex.getY();

            while (y <= endY)
            {
                const double&& xi = std::floor(x);
                const double&& alpha = x - xi;

                color[3] = alpha;
                this->_setPixel(Vertex::Vertex{ xi + 1.0, y, color });
                color[3] = 1.0 - alpha;
                this->_setPixel(Vertex::Vertex{ xi, y, color });

                x += 1.0 / slope;
                y++;
            }
        }
        else
        {
            const double endX = endVertex.getX();

            while (x <= endX)
            {
                const double&& yi = std::floor(y);
                const double&& alpha = y - yi;

                color[3] = alpha;
                this->_setPixel(Vertex::Vertex{ x, yi + 1.0, color });
                color[3] = 1.0 - alpha;
                this->_setPixel(Vertex::Vertex{ x, yi, color });

                y += slope;
                x++;
            }
        }
    }

    void AntiAliasingAlgorithm::rasterizeLineInNegativeSlope(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, const double& slope) const
    {
        double x = startVertex.getX();
        double y = startVertex.getY();
        std::array<double, 4> color{ _color };

        if (slope <= -1.0)
        {
            const double endY = endVertex.getY();

            while (y >= endY)
            {
                const double&& xi = std::floor(x);
                const double&& alpha = x - xi;

                color[3] = alpha;
                this->_setPixel(Vertex::Vertex{ xi + 1.0, y, color });
                color[3] = 1 - alpha;
                this->_setPixel(Vertex::Vertex{ xi, y, color });

                x -= 1.0 / slope;
                y--;
            }
        }
        else
        {
            const double endX = endVertex.getX();

            while (x <= endX)
            {
                const double&& yi = std::floor(y);
                const double&& alpha = y - yi;

                color[3] = alpha;
                this->_setPixel(Vertex::Vertex{ x, yi + 1.0, color });
                color[3] = 1 - alpha;
                this->_setPixel(Vertex::Vertex{ x, yi, color });

                y += slope;
                x++;
            }
        }
    }

    void AntiAliasingAlgorithm::apply(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex) const
    {
        const Vertex::Vertex* _startVertex = &startVertex;
        const Vertex::Vertex* _endVertex = &endVertex;
        this->sortPoints(&_startVertex, &_endVertex);

        const double dx = _endVertex->getX() - _startVertex->getX();
        const double dy = _endVertex->getY() - _startVertex->getY();

        double slope = dy >= 0.0 ? INT_MAX : INT_MIN;
        if (dx != 0)
        {
            slope = dy / dx;
        }

        if (std::signbit(slope))
        {
            this->rasterizeLineInNegativeSlope(*_startVertex, *_endVertex, slope);
        }
        else
        {
            this->rasterizeLineInPositiveSlope(*_startVertex, *_endVertex, slope);
        }
    }
}