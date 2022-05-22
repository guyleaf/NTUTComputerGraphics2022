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
        /// 使用此演算法
        /// </summary>
        void apply(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex) const override;
    private:
        // 處理斜率為正的線段
        void rasterizeLineInPositiveSlope(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, const double& dx, const double& dy) const;
        // 處理斜率為負的線段
        void rasterizeLineInNegativeSlope(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex, const double& dx, const double& dy) const;

        // 畫格子
        const std::function<void(const Vertex::Vertex&)> _setPixel;

        // 演算法名稱
        const std::string _name{ "midpoint" };

        // default pixel color
        const std::array<double, 4> _color{ 0.5, 0.5, 0.5, 1.0 };
    };
}