#pragma once
#include <string>
#include <utility>
#include <functional>

#include "../LineAlgorithm.h"
#include "../../Vertex.h"

namespace Algorithms
{
    class AntiAliasingAlgorithm final : public LineAlgorithm
    {
    public:
        explicit AntiAliasingAlgorithm(const std::function<void(const Vertex::Vertex&)>& setPixel);
        std::string getName() const override;

        /// <summary>
        /// �ϥΦ��t��k
        /// </summary>
        void apply(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex) const override;
    private:
        // �B�z�ײv�������u�q
        void rasterizeLineInPositiveSlope(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, const double& slope) const;
        // �B�z�ײv���t���u�q
        void rasterizeLineInNegativeSlope(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, const double& slope) const;

        // �e��l
        const std::function<void(const Vertex::Vertex&)> _setPixel;
        // �t��k�W��
        const std::string _name = "anti-aliasing";
    };
}