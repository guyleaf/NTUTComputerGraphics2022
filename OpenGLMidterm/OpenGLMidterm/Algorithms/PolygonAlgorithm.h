#pragma once
#include <string>
#include <vector>
#include <functional>

#include "../Vertex.h"

namespace Algorithms
{
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
        virtual void apply(const std::vector<Vertex::Vertex>& vertices) const = 0;
    };
}