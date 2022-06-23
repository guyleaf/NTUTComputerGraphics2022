#pragma once
#include <functional>
#include <vector>

#include "../../Graph2D/Edge.h"
#include "../PolygonAlgorithm.h"

namespace RasterizationAlgorithms
{
    class ScanLineAlgorithm final : public PolygonAlgorithm
    {
    public:
        explicit ScanLineAlgorithm(const std::function<void(const Graph2D::Vertex&)>& setPixel);
        std::string getName() const override;

        /// <summary>
        /// �ϥΦ��t��k
        /// </summary>
        void apply(const std::vector<Graph2D::Vertex>& vertices) const override;
    private:
        void scanXY(const std::vector<Graph2D::Vertex>& vList, size_t iMin) const;
        void scanX(const Graph2D::Vertex& lEdgePoint, const Graph2D::Vertex& rEdgePoint, int y) const;

        void findEdge(const std::vector<Graph2D::Vertex>& vList, size_t& rem, size_t& iVertex, Graph2D::Edge& edge, int scanLineY, bool leftOrRight) const;

        void increment(Graph2D::Edge& edge) const;

        void differenceX(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, Graph2D::Edge& edge, int x) const;
        void differenceY(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, Graph2D::Edge& edge, int y) const;
        void difference(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, Graph2D::Edge& edge, double diff, double amountOfStep) const;

        size_t findSmallestYIndex(const std::vector<Graph2D::Vertex>& vertices) const;

        // �ˬd vertices order is countclockwise order
        bool isVerticesStoredInCounterclockwiseOrder(const std::vector<Graph2D::Vertex>& vertices) const;

        // �e��l
        const std::function<void(const Graph2D::Vertex&)> _setPixel;

        // �t��k�W��
        const std::string _name{ "Scan Line" };
    };
}