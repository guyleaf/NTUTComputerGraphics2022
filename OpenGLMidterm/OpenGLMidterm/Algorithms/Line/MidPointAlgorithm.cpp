#include <cmath>

#include "MidPointAlgorithm.h"

namespace Algorithms
{
    MidPointAlgorithm::MidPointAlgorithm(const std::function<void(const Vertex::Vertex&)>& setPixel) : _setPixel{ setPixel }
    {
    }

    std::string MidPointAlgorithm::getName() const
    {
        return _name;
    }

    void MidPointAlgorithm::rasterizeLineInPositiveSlope(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, const double& dx, const double& dy) const
    {
        const bool isSlopeBiggerThanOne = std::abs(dy) >= std::abs(dx);
        const double delNE = 2 * (dy - dx);
        double delE;
        double d;

        if (isSlopeBiggerThanOne)
        {
            delE = 2 * -dx;
            d = dy + delE;
        }
        else
        {
            delE = 2 * dy;
            d = delE - dx;
        }

        double x = startVertex.getX();
        double y = startVertex.getY();
        this->_setPixel(Vertex::Vertex{ x, y, _color });

        if (isSlopeBiggerThanOne)
        {
            const double endY = endVertex.getY();

            while (y < endY)
            {
                if (d > 0)
                {
                    d += delE;
                }
                else
                {
                    d += delNE;
                    x++;
                }
                y++;
                this->_setPixel(Vertex::Vertex{ x, y, _color });
            }
        }
        else
        {
            const double endX = endVertex.getX();

            while (x < endX)
            {
                if (d <= 0)
                {
                    d += delE;
                }
                else
                {
                    d += delNE;
                    y++;
                }
                x++;
                this->_setPixel(Vertex::Vertex{ x, y, _color });
            }
        }
    }

    void MidPointAlgorithm::rasterizeLineInNegativeSlope(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, const double& dx, const double& dy) const
    {
        const bool isSlopeBiggerThanOne = std::abs(dy) >= std::abs(dx);
        const double delNE = 2 * (dy + dx);
        double delE;
        double d;

        if (isSlopeBiggerThanOne)
        {
            delE = 2 * dx;
            d = dy + delE;
        }
        else
        {
            delE = 2 * dy;
            d = delE + dx;
        }

        double x = startVertex.getX();
        double y = startVertex.getY();
        this->_setPixel(Vertex::Vertex{ x, y, _color });

        if (isSlopeBiggerThanOne)
        {
            const double endY = endVertex.getY();

            while (y > endY)
            {
                if (d <= 0)
                {
                    d += delE;
                }
                else
                {
                    d += delNE;
                    x++;
                }
                y--;
                this->_setPixel(Vertex::Vertex{ x, y, _color });
            }
        }
        else
        {
            const double endX = endVertex.getX();

            while (x < endX)
            {
                if (d > 0)
                {
                    d += delE;
                }
                else
                {
                    d += delNE;
                    y--;
                }
                x++;
                this->_setPixel(Vertex::Vertex{ x, y, _color });
            }
        }
    }

    void MidPointAlgorithm::apply(Vertex::Vertex startVertex, Vertex::Vertex endVertex) const
    {
        this->sortPoints(startVertex, endVertex);
        this->roundPoints(startVertex, endVertex);

        const double dx = endVertex.getX() - startVertex.getX();
        const double dy = endVertex.getY() - startVertex.getY();

        if (std::signbit(dy * dx))
        {
            this->rasterizeLineInNegativeSlope(startVertex, endVertex, dx, dy);
        }
        else
        {
            this->rasterizeLineInPositiveSlope(startVertex, endVertex, dx, dy);
        }
    }
}