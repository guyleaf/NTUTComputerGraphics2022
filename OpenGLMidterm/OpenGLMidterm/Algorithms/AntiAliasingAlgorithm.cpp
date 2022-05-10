#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <climits>
#include <GL/freeglut.h>

#include "../Algorithms.h"

namespace Algorithms
{
    AntiAliasingAlgorithm::AntiAliasingAlgorithm(const Callback& setPixel) : Algorithm("anti-aliasing", setPixel)
    {
    }

    void AntiAliasingAlgorithm::rasterizeLineInPositiveSlope(const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, const int& dx, const int& dy, const bool& isSlopeBiggerThanOne) const
    {
        const double slope = static_cast<double>(dy) / static_cast<double>(dx);

        double x = startPoint.first;
        double y = startPoint.second;

        if (isSlopeBiggerThanOne)
        {
            while (y <= endPoint.second)
            {
                const double&& xi = static_cast<int>(std::floor(x));
                const double&& alpha = x - xi;

                this->_setPixel(xi, y, 1.0 - alpha);
                this->_setPixel(xi + 1.0, y, alpha);

                x += 1.0 / slope;
                y++;
            }
        }
        else
        {
            while (x <= endPoint.first)
            {
                const double&& yi = static_cast<int>(std::floor(y));
                const double&& alpha = y - yi;

                this->_setPixel(x, yi, 1.0 - alpha);
                this->_setPixel(x, yi + 1.0, alpha);

                y += slope;
                x++;
            }
        }
    }

    void AntiAliasingAlgorithm::rasterizeLineInNegativeSlope(const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, const int& dx, const int& dy, const bool& isSlopeBiggerThanOne) const
    {
        const double slope = static_cast<double>(dy) / static_cast<double>(dx);

        double x = startPoint.first;
        double y = startPoint.second;

        if (isSlopeBiggerThanOne)
        {
            while (y >= endPoint.second)
            {
                const double&& xi = static_cast<int>(std::floor(x));
                const double&& alpha = x - xi;

                this->_setPixel(xi, y, 1.0 - alpha);
                this->_setPixel(xi + 1.0, y, alpha);

                x -= 1.0 / slope;
                y--;
            }
        }
        else
        {
            while (x <= endPoint.first)
            {
                const double&& yi = static_cast<int>(std::floor(y));
                const double&& alpha = y - yi;

                this->_setPixel(x, yi, 1.0 - alpha);
                this->_setPixel(x, yi + 1.0, alpha);

                y += slope;
                x++;
            }
        }
    }

    void AntiAliasingAlgorithm::apply(const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint) const
    {
        std::pair<int, int> _startPoint = startPoint;
        std::pair<int, int> _endPoint = endPoint;
        this->sortPoints(_startPoint, _endPoint);

        const int dx = _endPoint.first - _startPoint.first;
        const int dy = _endPoint.second - _startPoint.second;
        const bool isSlopeNegative = ((dy * dx) >> 31) & 0x1;
        const int slope = dx != 0 ? dy / dx : INT_MAX;

        if (isSlopeNegative)
        {
            this->rasterizeLineInNegativeSlope(_startPoint, _endPoint, dx, dy, slope);
        }
        else
        {
            this->rasterizeLineInPositiveSlope(_startPoint, _endPoint, dx, dy, slope);
        }
    }
}