#pragma once
#include <functional>

#include "../PolygonAlgorithm.h"

namespace Algorithms
{
    class ScanLineAlgorithm final : public PolygonAlgorithm
    {
    public:
        explicit ScanLineAlgorithm(const std::function<void(const Vertex::Vertex&)>& setPixel);
        std::string getName() const override;

        /// <summary>
        /// �ϥΦ��t��k
        /// </summary>
        void apply(const std::vector<Vertex::Vertex>& vertices) const override;
    private:
        class Edge;

        void scanXY(const std::vector<Vertex::Vertex>& vList, size_t iMin) const;
        void scanX(const std::vector<Vertex::Vertex>& vList, const size_t& li, const size_t& ri, Edge& lEdge, Edge& rEdge, int& scanLineY) const;

        void findEdge(const std::vector<Vertex::Vertex>& vList, size_t& rem, size_t& iVertex, Edge& edge, int scanLineY, bool leftOrRight) const;

        void increment(Edge& edge) const;

        void differenceX(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, Edge& edge, int x) const;
        void differenceY(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, Edge& edge, int y) const;
        void difference(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, Edge& edge, double diff, double amountOfStep) const;

        size_t findIndexWithSmallestY(const std::vector<Vertex::Vertex>& vertices) const;

        // �ˬd vertices order is countclockwise order
        bool isVerticesStoredInCounterclockwiseOrder(const std::vector<Vertex::Vertex>& vertices) const;

        // �e��l
        const std::function<void(const Vertex::Vertex&)> _setPixel;

        // �t��k�W��
        const std::string _name{ "Scan Line" };

        class Edge
        {
        public:
            Vertex::Vertex position;
            Vertex::Vertex difference;
        };
    };
}