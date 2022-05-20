#pragma once
#include <string>
#include <functional>

namespace Algorithms
{
    using Callback = std::function<void(double, double, double)>;

    class Algorithm
    {
    public:
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
        virtual void apply(const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint) const = 0;
    protected:
        // 排序座標
        void sortPointsByX(std::pair<int, int>& startPoint, std::pair<int, int>& endPoint) const;
    };

    class MidPointAlgorithm final : public Algorithm
    {
    public:
        explicit MidPointAlgorithm(const Callback& setPixel);
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
        const std::string _name = "midpoint";
    };

    class AntiAliasingAlgorithm final : public Algorithm
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