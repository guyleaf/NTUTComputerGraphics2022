#pragma once
#include <string>
#include <utility>
#include <functional>

#include "../../Graph2D/Vertex.h"
#include "../LineAlgorithm.h"

namespace RasterizationAlgorithms
{
    class MidPointAlgorithm final : public LineAlgorithm
    {
    public:
        explicit MidPointAlgorithm(const std::function<void(const Graph2D::Vertex&)>& setPixel);
        std::string getName() const override;

        /// <summary>
        /// �ϥΦ��t��k
        /// </summary>
        void apply(Graph2D::Vertex startVertex, Graph2D::Vertex endVertex) const override;
    private:
        // �B�z�ײv�������u�q
        void rasterizeLineInPositiveSlope(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, const double& dx, const double& dy) const;
        // �B�z�ײv���t���u�q
        void rasterizeLineInNegativeSlope(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, const double& dx, const double& dy) const;

        // �e��l
        const std::function<void(const Graph2D::Vertex&)> _setPixel;

        // �t��k�W��
        const std::string _name{ "midpoint" };

        // default pixel color
        const std::array<double, 4> _color{ 0.5, 0.5, 0.5, 1.0 };
    };
}