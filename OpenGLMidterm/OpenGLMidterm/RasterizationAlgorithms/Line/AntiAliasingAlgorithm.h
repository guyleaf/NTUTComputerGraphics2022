#pragma once
#include <string>
#include <utility>
#include <functional>

#include "../../Graph2D/Vertex.h"
#include "../LineAlgorithm.h"

namespace RasterizationAlgorithms
{
    class AntiAliasingAlgorithm final : public LineAlgorithm
    {
    public:
        explicit AntiAliasingAlgorithm(const std::function<void(const Graph2D::Vertex&)>& setPixel);
        std::string getName() const override;

        /// <summary>
        /// 使用此演算法
        /// </summary>
        void apply(Graph2D::Vertex startVertex, Graph2D::Vertex endVertex) const override;
    private:
        // 處理斜率為正的線段
        void rasterizeLineInPositiveSlope(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, const double& slope) const;
        // 處理斜率為負的線段
        void rasterizeLineInNegativeSlope(const Graph2D::Vertex& startVertex, const Graph2D::Vertex& endVertex, const double& slope) const;

        // 畫格子
        const std::function<void(const Graph2D::Vertex&)> _setPixel;

        // 演算法名稱
        const std::string _name{ "anti-aliasing" };

        // default pixel color
        const std::array<double, 4> _color{ 0.5, 0.5, 0.5, 1.0 };
    };
}