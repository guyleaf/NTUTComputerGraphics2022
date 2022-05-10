#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <climits>
#include <string>
#include <functional>
#include <GL/freeglut.h>

#include "../Algorithms.h"

namespace Algorithms
{
    MidPointAlgorithm::MidPointAlgorithm(const Callback& setPixel) : Algorithm("midpoint", setPixel)
    {
    }

    void MidPointAlgorithm::rasterizeLineInPositiveSlope(const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, const int& dx, const int& dy, const bool& isSlopeBiggerThanOne) const
    {
        const double alpha = 1.0;

        int delE;
        const int delNE = 2 * (dy - dx);
        int d;

        if (isSlopeBiggerThanOne)
        {
            delE = 2 * -dx;
            d = dy + delE;
        }
        else
        {
            delE = 2 * dy;
            d = delE - dx;
        }

        int x = startPoint.first;
        int y = startPoint.second;
        this->_setPixel(x, y, alpha);

        if (isSlopeBiggerThanOne)
        {
            while (y < endPoint.second)
            {
                if (d > 0)
                {
                    d += delE;
                }
                else
                {
                    d += delNE;
                    x++;
                }
                y++;
                this->_setPixel(x, y, alpha);
            }
        }
        else
        {
            while (x < endPoint.first)
            {
                if (d <= 0)
                {
                    d += delE;
                }
                else
                {
                    d += delNE;
                    y++;
                }
                x++;
                this->_setPixel(x, y, alpha);
            }
        }
    }

    void MidPointAlgorithm::rasterizeLineInNegativeSlope(const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, const int& dx, const int& dy, const bool& isSlopeBiggerThanOne) const
    {
        const double alpha = 1.0;

        int delE;
        const int delNE = 2 * (dy + dx);
        int d;

        if (isSlopeBiggerThanOne)
        {
            delE = 2 * dx;
            d = dy + delE;
        }
        else
        {
            delE = 2 * dy;
            d = delE + dx;
        }

        int x = startPoint.first;
        int y = startPoint.second;
        this->_setPixel(x, y, alpha);

        if (isSlopeBiggerThanOne)
        {
            while (y > endPoint.second)
            {
                if (d <= 0)
                {
                    d += delE;
                }
                else
                {
                    d += delNE;
                    x++;
                }
                y--;
                this->_setPixel(x, y, alpha);
            }
        }
        else
        {
            while (x < endPoint.first)
            {
                if (d > 0)
                {
                    d += delE;
                }
                else
                {
                    d += delNE;
                    y--;
                }
                x++;
                this->_setPixel(x, y, alpha);
            }
        }
    }

    void MidPointAlgorithm::apply(const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint) const
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