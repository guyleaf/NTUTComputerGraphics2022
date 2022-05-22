#pragma once
#include <string>
#include <utility>
#include <functional>

#include "../../Vertex.h"
#include "../LineAlgorithm.h"

namespace Algorithms
{
    class MidPointAlgorithm final : public LineAlgorithm
    {
    public:
        explicit MidPointAlgorithm(const std::function<void(const Vertex::Vertex&)>& setPixel);
        std::string getName() const override;

        /// <summary>
        /// �ϥΦ��t��k
        /// </summary>
        void apply(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex) const override;
    private:
        // �B�z�ײv�������u�q
        void rasterizeLineInPositiveSlope(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, const double& dx, const double& dy) const;
        // �B�z�ײv���t���u�q
        void rasterizeLineInNegativeSlope(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, const double& dx, const double& dy) const;

        // �e��l
        const std::function<void(const Vertex::Vertex&)> _setPixel;

        // �t��k�W��
        const std::string _name{ "midpoint" };

        // default pixel color
        const std::array<double, 4> _color{ 0.5, 0.5, 0.5, 1.0 };
    };
}