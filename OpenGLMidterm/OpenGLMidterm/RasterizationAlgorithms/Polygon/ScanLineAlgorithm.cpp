#include <cmath>

#include "ScanLineAlgorithm.h"


namespace RasterizationAlgorithms
{
    ScanLineAlgorithm::ScanLineAlgorithm(const std::function<void(const Graph2D::Vertex&)>& setPixel) : _setPixel{ setPixel }
    {

    }

    std::string ScanLineAlgorithm::getName() const
    {
        return _name;
    }

    /// <summary>
    /// 使用此演算法
    /// </summary>
    void ScanLineAlgorithm::apply(const std::vector<Graph2D::Vertex>& vertices) const
    {
        // ensure vList is stored in counterclockwise order
        std::vector<Graph2D::Vertex> vList;
        if (isVerticesStoredInCounterclockwiseOrder(vertices))
        {
            vList.assign(vertices.begin(), vertices.end());
        }
        else
        {
            vList.assign(vertices.rbegin(), vertices.rend());
        }

        size_t iMin = findSmallestYIndex(vList);
        scanXY(vList, iMin);
    }

    void ScanLineAlgorithm::scanXY(const std::vector<Graph2D::Vertex>& vList, size_t iMin) const
    {
        Edge lEdge;
        Edge rEdge;

        // left & right upper endpoint indices
        size_t li{ iMin };
        size_t ri{ iMin };

        // number of remaining vertices
        size_t rem{ vList.size() };
        // current scanline
        const int&& smallestY = static_cast<int>(std::ceil(vList[iMin].getY()));
        int scanLineY{ smallestY };

        while (rem > 0)
        {
            // left
            findEdge(vList, rem, li, lEdge, scanLineY, false);
            // right
            findEdge(vList, rem, ri, rEdge, scanLineY, true);

            // rasterization
            int&& ly = static_cast<int>(std::ceil(vList[li].getY()));
            int&& ry = static_cast<int>(std::ceil(vList[ri].getY()));
            while (scanLineY < ly && scanLineY < ry)
            {
                scanX(lEdge, rEdge, scanLineY);
                increment(lEdge);
                increment(rEdge);
                scanLineY++;
            }
        }
    }

    void ScanLineAlgorithm::scanX(const Edge& lEdge, const Edge& rEdge, int y) const
    {
        auto lx = static_cast<int>(std::ceil(lEdge.position.getX()));
        auto rx = static_cast<int>(std::ceil(rEdge.position.getX()));

        Edge hEdge;

        if (lx < rx)
        {
            // get interpolated color
            differenceX(lEdge.position, rEdge.position, hEdge, lx);
            for (int x = lx; x < rx; x++)
            {
                _setPixel(Graph2D::Vertex(x, y, hEdge.position.getRGBA()));
                // update interpolated color
                increment(hEdge);
            }
        }
    }

    void ScanLineAlgorithm::findEdge(const std::vector<Graph2D::Vertex>& vList, size_t& rem, size_t& iVertex, Edge& edge, int scanLineY, bool leftOrRight) const
    {
        size_t n{ vList.size() };

        // traverse vList based on finding the left or right edge
        std::function<size_t(const size_t&)> next;
        if (leftOrRight)
        {
            // counterclockwise
            next = [n](const size_t& i)
            {
                return (i + 1) % n;
            };
        }
        else
        {
            // clockwise
            next = [n](const size_t& i)
            {
                return (i - 1 + n) % n;
            };
        }

        size_t i;
        auto y = static_cast<int>(std::ceil(vList[iVertex].getY()));

        while (y <= scanLineY && rem > 0)
        {
            i = next(i);

            y = static_cast<int>(std::ceil(vList[i].getY()));
            if (y > scanLineY)
            {
                differenceY(vList[iVertex], vList[i], edge, y);
            }
            iVertex = i;
            rem--;
        }
    }

    void ScanLineAlgorithm::increment(Edge& edge) const
    {
        Graph2D::Vertex& point = edge.position;
        const Graph2D::Vertex& dPoint = edge.difference;
        point.setX(point.getX() + dPoint.getX());
        point.setRGBA(std::array<double, 4> {point.getRed() + dPoint.getRed(), point.getGreen() + dPoint.getGreen(), point.getBlue() + dPoint.getBlue(), 1.0});
    }

    void ScanLineAlgorithm::differenceX(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, Edge& edge, int x) const
    {
        const double startX = startVertex.getX();
        difference(startVertex, endVertex, edge, (endVertex.getX() - startX), x - startX);
    }

    void ScanLineAlgorithm::differenceY(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, Edge& edge, int y) const
    {
        const double startY = startVertex.getY();
        difference(startVertex, endVertex, edge, (endVertex.getY() - startY), y - startY);
    }

    void ScanLineAlgorithm::difference(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, Edge& edge, double diff, double amountOfStep) const
    {
        Graph2D::Vertex& edgePoint = edge.position;
        Graph2D::Vertex& dEdgePoint = edge.difference;

        const double startX = startVertex.getX();
        const std::array<double, 4> startColor = startVertex.getRGBA();
        const std::array<double, 4> endColor = startVertex.getRGBA();

        double deX = (endVertex.getX() - startX) / diff;
        double deR = (endColor[0] - startColor[0]) / diff;
        double deG = (endColor[1] - startColor[1]) / diff;
        double deB = (endColor[2] - startColor[2]) / diff;

        dEdgePoint.setX(deX);
        edgePoint.setX(startX + amountOfStep * deX);
        dEdgePoint.setRGBA(std::array<double, 4> {deR, deG, deB, 1.0});
        edgePoint.setRGBA(std::array<double, 4> {startColor[0] + amountOfStep * deR, startColor[1] + amountOfStep * deG, startColor[2] + amountOfStep * deB, 1.0});
    }

    size_t ScanLineAlgorithm::findSmallestYIndex(const std::vector<Graph2D::Vertex>& vertices) const
    {
        size_t iMin = 0;
        for (size_t i = 1; i < vertices.size(); i++)
        {
            if (vertices[i].getY() < vertices[iMin].getY())
            {
                iMin = i;
            }
        }

        return iMin;
    }

    // Refer to: shoelace algorithm https://en.wikipedia.org/wiki/Shoelace_formula#Shoelace_formula
    bool ScanLineAlgorithm::isVerticesStoredInCounterclockwiseOrder(const std::vector<Graph2D::Vertex>& vertices) const
    {
        size_t length = vertices.size();
        double area = 0.0;

        for (size_t i = 0; i < length; i++)
        {
            double&& currX = vertices[i].getX();
            double&& prevY = vertices[(i - 1 + length) % length].getY();
            double&& nextY = vertices[(i + 1) % length].getY();
            area += currX * (nextY - prevY);
        }

        return !std::signbit(area);
    }
}