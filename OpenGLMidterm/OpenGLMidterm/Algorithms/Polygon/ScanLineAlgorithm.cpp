#define _USE_MATH_DEFINES
#include <cmath>
#include <functional>

#include "ScanLineAlgorithm.h"


namespace Algorithms
{
    ScanLineAlgorithm::ScanLineAlgorithm(const std::function<void(const Vertex::Vertex&)>& setPixel) : _setPixel{ setPixel }
    {

    }

    std::string ScanLineAlgorithm::getName() const
    {
        return _name;
    }

    /// <summary>
    /// 使用此演算法
    /// </summary>
    void ScanLineAlgorithm::apply(const std::vector<Vertex::Vertex>& vertices) const
    {
        // ensure vList is stored in counterclockwise order
        std::vector<Vertex::Vertex> vList;
        if (isVerticesStoredInCounterclockwiseOrder(vertices))
        {
            vList.assign(vertices.begin(), vertices.end());
        }
        else
        {
            vList.assign(vertices.rbegin(), vertices.rend());
        }

        size_t&& iMin = findIndexWithSmallestY(vList);
        scanXY(vList, iMin);
    }

    void ScanLineAlgorithm::scanXY(const std::vector<Vertex::Vertex>& vList, size_t iMin) const
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
        }
    }

    void ScanLineAlgorithm::scanX(const std::vector<Vertex::Vertex>& vList, const size_t& li, const size_t& ri, Edge& lEdge, Edge& rEdge, int& scanLineY) const
    {
        int&& ly = static_cast<int>(std::ceil(vList[li].getY()));
        int&& ry = static_cast<int>(std::ceil(vList[ri].getY()));

        while (scanLineY < ly && scanLineY < ry)
        {
            int&& lx = static_cast<int>(std::ceil(vList[li].getX()));
            int&& rx = static_cast<int>(std::ceil(vList[ri].getX()));

            if (lx < rx)
            {
                // diff
                for (int x = lx; x < rx; x++)
                {
                    _setPixel(Vertex::Vertex(x, scanLineY, 1.0, 0.0, 0.0, 1.0));
                    // increment
                }
            }

            increment(lEdge);
            increment(rEdge);
            scanLineY++;
        }
    }

    void ScanLineAlgorithm::findEdge(const std::vector<Vertex::Vertex>& vList, size_t& rem, size_t& iVertex, Edge& edge, int scanLineY, bool leftOrRight) const
    {
        size_t&& n{ vList.size() };

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
        int&& y = static_cast<int>(std::ceil(vList[iVertex].getY()));

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
        edge.position.setX(edge.position.getX() + edge.difference.getX());
    }

    void ScanLineAlgorithm::differenceX(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, Edge& edge, int x) const
    {
        double&& startX = startVertex.getX();
        difference(startVertex, endVertex, edge, (endVertex.getX() - startX), x - startX);
    }

    void ScanLineAlgorithm::differenceY(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, Edge& edge, int y) const
    {
        double&& startY = startVertex.getY();
        difference(startVertex, endVertex, edge, (endVertex.getY() - startY), y - startY);
    }

    void ScanLineAlgorithm::difference(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, Edge& edge, double diff, double amountOfStep) const
    {
        double&& deX = (endVertex.getX() - startVertex.getX()) / diff;
        edge.difference.setX(deX);
        edge.position.setX(startVertex.getX() + amountOfStep * deX);
    }

    size_t ScanLineAlgorithm::findIndexWithSmallestY(const std::vector<Vertex::Vertex>& vertices) const
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
    bool ScanLineAlgorithm::isVerticesStoredInCounterclockwiseOrder(const std::vector<Vertex::Vertex>& vertices) const
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