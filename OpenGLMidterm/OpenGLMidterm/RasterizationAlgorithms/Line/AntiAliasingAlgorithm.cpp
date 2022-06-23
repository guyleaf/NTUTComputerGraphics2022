#include <cmath>

#include "AntiAliasingAlgorithm.h"

namespace RasterizationAlgorithms
{
    AntiAliasingAlgorithm::AntiAliasingAlgorithm(const std::function<void(const Graph2D::Vertex&)>& setPixel) : _setPixel{ setPixel }
    {
    }

    std::string AntiAliasingAlgorithm::getName() const
    {
        return _name;
    }

    void AntiAliasingAlgorithm::rasterizeLineInPositiveSlope(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, const double& slope) const
    {
        double x = startVertex.getX();
        double y = startVertex.getY();

        Graph2D::Edge edge;

        // if the slope is bigger than 1
        if (slope >= 1.0)
        {
            const double endY = endVertex.getY();

            differenceY(startVertex, endVertex, edge);
            while (y <= endY)
            {
                std::array<double, 4>&& color = edge.point.getRGBA();
                const double&& xi = std::floor(x);
                const double&& alpha = x - xi;

                color[3] = alpha;
                this->_setPixel(Graph2D::Vertex{ xi + 1.0, y, color });
                color[3] = 1.0 - alpha;
                this->_setPixel(Graph2D::Vertex{ xi, y, color });

                x += 1.0 / slope;
                y++;
                increment(edge);
            }
        }
        else
        {
            const double endX = endVertex.getX();

            differenceX(startVertex, endVertex, edge);
            while (x <= endX)
            {
                std::array<double, 4>&& color = edge.point.getRGBA();
                const double&& yi = std::floor(y);
                const double&& alpha = y - yi;

                color[3] = alpha;
                this->_setPixel(Graph2D::Vertex{ x, yi + 1.0, color });
                color[3] = 1.0 - alpha;
                this->_setPixel(Graph2D::Vertex{ x, yi, color });

                y += slope;
                x++;
                increment(edge);
            }
        }
    }

    void AntiAliasingAlgorithm::rasterizeLineInNegativeSlope(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, const double& slope) const
    {
        double x = startVertex.getX();
        double y = startVertex.getY();

        Graph2D::Edge edge;

        if (slope <= -1.0)
        {
            const double endY = endVertex.getY();

            differenceY(startVertex, endVertex, edge);
            while (y >= endY)
            {
                std::array<double, 4>&& color = edge.point.getRGBA();
                const double&& xi = std::floor(x);
                const double&& alpha = x - xi;

                color[3] = alpha;
                this->_setPixel(Graph2D::Vertex{ xi + 1.0, y, color });
                color[3] = 1 - alpha;
                this->_setPixel(Graph2D::Vertex{ xi, y, color });

                x -= 1.0 / slope;
                y--;
                increment(edge);
            }
        }
        else
        {
            const double endX = endVertex.getX();

            differenceX(startVertex, endVertex, edge);
            while (x <= endX)
            {
                std::array<double, 4>&& color = edge.point.getRGBA();
                const double&& yi = std::floor(y);
                const double&& alpha = y - yi;

                color[3] = alpha;
                this->_setPixel(Graph2D::Vertex{ x, yi + 1.0, color });
                color[3] = 1 - alpha;
                this->_setPixel(Graph2D::Vertex{ x, yi, color });

                y += slope;
                x++;
                increment(edge);
            }
        }
    }

    void AntiAliasingAlgorithm::apply(Graph2D::Vertex startVertex, Graph2D::Vertex endVertex) const
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

    void AntiAliasingAlgorithm::increment(Graph2D::Edge& edge) const
    {
        Graph2D::Vertex& point = edge.point;
        const Graph2D::Vertex& dPoint = edge.difference;
        point.setRGBA(std::array<double, 4> {point.getRed() + dPoint.getRed(), point.getGreen() + dPoint.getGreen(), point.getBlue() + dPoint.getBlue(), 1.0});
    }

    void AntiAliasingAlgorithm::differenceX(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, Graph2D::Edge& edge) const
    {
        const double startX = startVertex.getX();
        difference(startVertex, endVertex, edge, endVertex.getX() - startX);
    }

    void AntiAliasingAlgorithm::differenceY(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, Graph2D::Edge& edge) const
    {
        const double startY = startVertex.getY();
        difference(startVertex, endVertex, edge, endVertex.getY() - startY);
    }

    void AntiAliasingAlgorithm::difference(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, Graph2D::Edge& edge, double diff) const
    {
        const std::array<double, 4> startColor = startVertex.getRGBA();
        const std::array<double, 4> endColor = endVertex.getRGBA();

        double deR = (endColor[0] - startColor[0]) / diff;
        double deG = (endColor[1] - startColor[1]) / diff;
        double deB = (endColor[2] - startColor[2]) / diff;

        edge.difference.setRGBA(std::array<double, 4> {deR, deG, deB, 1.0});
        edge.point.setRGBA(startColor);
    }
}