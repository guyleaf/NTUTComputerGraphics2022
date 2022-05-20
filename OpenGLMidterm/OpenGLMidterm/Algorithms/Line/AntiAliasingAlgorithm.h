#pragma once
#include <string>
#include <utility>
#include <functional>

#include "../LineAlgorithm.h"

namespace Algorithms
{
    class AntiAliasingAlgorithm final : public LineAlgorithm
    {
    public:
        explicit AntiAliasingAlgorithm(const Callback& setPixel);
        std::string getName() const override;

        /// <summary>
        /// �ϥΦ��t��k
        /// </summary>
        /// <param name="startPoint"></param>
        /// <param name="endPoint"></param>
        void apply(const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint) const override;
    private:
        // �B�z�ײv�������u�q
        void rasterizeLineInPositiveSlope(const std::pair<int, int>&, const std::pair<int, int>&, const int&, const int&, const bool&) const;
        // �B�z�ײv���t���u�q
        void rasterizeLineInNegativeSlope(const std::pair<int, int>&, const std::pair<int, int>&, const int&, const int&, const bool&) const;

        // �e��l
        const Callback _setPixel;
        // �t��k�W��
        const std::string _name = "anti-aliasing";
    };
}