#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

#include "../Algorithms.h"


namespace Algorithms
{
    Algorithm::Algorithm(const std::string &name, const Callback& setPixel) : _setPixel(setPixel), _name(name)
    {
    }

    std::string Algorithm::getName() const
    {
        return this->_name;
    }

    void Algorithm::sortPoints(std::pair<int, int>& startPoint, std::pair<int, int>& endPoint) const
    {
        if (startPoint.first > endPoint.first || (startPoint.first == endPoint.first && startPoint.second > endPoint.second))
        {
            std::swap(startPoint, endPoint);
        }
    }

    Algorithm::~Algorithm() = default;
}