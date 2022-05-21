#pragma once
#include <string>
#include <functional>

#include "../Vertex.h"

namespace Algorithms
{
    using Callback = std::function<void(Vertex::Vertex)>;

    class PolygonAlgorithm
    {
    public:
        virtual ~PolygonAlgorithm();

        /// <summary>
        /// ���o�t��k�W��
        /// </summary>
        /// <returns></returns>
        virtual std::string getName() const = 0;

        /// <summary>
        /// �ϥΦ��t��k
        /// </summary>
        /// <param name="startPoint"></param>
        /// <param name="endPoint"></param>
        virtual void apply(const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint) const = 0;
    };
}