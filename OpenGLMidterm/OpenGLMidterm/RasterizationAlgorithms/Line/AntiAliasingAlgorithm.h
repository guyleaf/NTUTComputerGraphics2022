#pragma once
#include <string>
#include <utility>
#include <functional>

#include "../../Graph2D/Edge.h"
#include "../LineAlgorithm.h"

namespace RasterizationAlgorithms
{
    class AntiAliasingAlgorithm final : public LineAlgorithm
    {
    public:
        explicit AntiAliasingAlgorithm(const std::function<void(const Graph2D::Vertex&)>& setPixel);
        std::string getName() const override;

        /// <summary>
        /// �ϥΦ��t��k
        /// </summary>
        void apply(Graph2D::Vertex startVertex, Graph2D::Vertex endVertex) const override;
    private:
        // �B�z�ײv�������u�q
        void rasterizeLineInPositiveSlope(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, const double& slope) const;
        // �B�z�ײv���t���u�q
        void rasterizeLineInNegativeSlope(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, const double& slope) const;

        // calculate interpolated color
        void increment(Graph2D::Edge& edge) const;
        void differenceX(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, Graph2D::Edge& edge) const;
        void differenceY(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, Graph2D::Edge& edge) const;
        void difference(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, Graph2D::Edge& edge, double diff) const;

        // �e��l
        const std::function<void(const Graph2D::Vertex&)> _setPixel;

        // �t��k�W��
        const std::string _name{ "anti-aliasing" };
    };
}