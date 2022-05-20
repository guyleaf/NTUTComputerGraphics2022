#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

#include "../Algorithms.h"


namespace Algorithms
{
    void Algorithm::sortPointsByX(std::pair<int, int>& startPoint, std::pair<int, int>& endPoint) const
    {
        if (startPoint.first > endPoint.first || (startPoint.first == endPoint.first && startPoint.second > endPoint.second))
        {
            std::swap(startPoint, endPoint);
        }
    }
}