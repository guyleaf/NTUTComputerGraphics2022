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

    void AntiAliasingAlgorithm::apply(Vertex::Vertex startVertex, Vertex::Vertex endVertex) const
    {
        this->sortPoints(startVertex, endVertex);
        this->roundPoints(startVertex, endVertex);

        const double dx = endVertex.getX() - startVertex.getX();
        const double dy = endVertex.getY() - startVertex.getY();

        double slope = dy >= 0.0 ? INT_MAX : INT_MIN;
        if (dx != 0)
        {
            slope = dy / dx;
        }

        if (std::signbit(slope))
        {
            this->rasterizeLineInNegativeSlope(startVertex, endVertex, slope);
        }
        else
        {
            this->rasterizeLineInPositiveSlope(startVertex, endVertex, slope);
        }
    }
}