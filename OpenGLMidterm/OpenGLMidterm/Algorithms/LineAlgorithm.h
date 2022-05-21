#pragma once
#include <string>

#include "../Vertex.h"

namespace Algorithms
{
    class LineAlgorithm
    {
    public:
        virtual ~LineAlgorithm();

        /// <summary>
        /// 取得演算法名稱
        /// </summary>
        /// <returns></returns>
        virtual std::string getName() const = 0;

        /// <summary>
        /// 使用此演算法
        /// </summary>
        /// <param name="startPoint"></param>
        /// <param name="endPoint"></param>
        virtual void apply(const Vertex::Vertex& startVertex, const Vertex::Vertex& endVertex) const = 0;
    protected:
        // 排序座標
        void sortPoints(const Vertex::Vertex** startVertex, const Vertex::Vertex** endVertex) const;
    };
}