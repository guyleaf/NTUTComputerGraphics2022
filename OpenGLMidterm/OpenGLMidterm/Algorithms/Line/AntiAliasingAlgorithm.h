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
        /// 使用此演算法
        /// </summary>
        /// <param name="startPoint"></param>
        /// <param name="endPoint"></param>
        void apply(const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint) const override;
    private:
        // 處理斜率為正的線段
        void rasterizeLineInPositiveSlope(const std::pair<int, int>&, const std::pair<int, int>&, const int&, const int&, const bool&) const;
        // 處理斜率為負的線段
        void rasterizeLineInNegativeSlope(const std::pair<int, int>&, const std::pair<int, int>&, const int&, const int&, const bool&) const;

        // 畫格子
        const Callback _setPixel;
        // 演算法名稱
        const std::string _name = "anti-aliasing";
    };
}