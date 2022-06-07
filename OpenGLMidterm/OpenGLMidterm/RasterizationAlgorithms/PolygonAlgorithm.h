#pragma once
#include <string>
#include <vector>
#include <functional>

#include "../Graph2D/Vertex.h"

namespace RasterizationAlgorithms
{
    class PolygonAlgorithm
    {
    public:
        virtual ~PolygonAlgorithm();

        /// <summary>
        /// 取得演算法名稱
        /// </summary>
        /// <returns></returns>
        virtual std::string getName() const = 0;

        /// <summary>
        /// 使用此演算法
        /// </summary>
        virtual void apply(const std::vector<Graph2D::Vertex>& vertices) const = 0;
    };
}