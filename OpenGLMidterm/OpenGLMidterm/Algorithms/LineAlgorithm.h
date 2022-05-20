#pragma once
#include <string>
#include <functional>

namespace Algorithms
{
    using Callback = std::function<void(double, double, double)>;

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
        virtual void apply(const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint) const = 0;
    protected:
        // 排序座標
        void sortPointsByX(std::pair<int, int>& startPoint, std::pair<int, int>& endPoint) const;
    };
}