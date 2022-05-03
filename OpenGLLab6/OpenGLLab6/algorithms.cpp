#include <string>
#include <GL/freeglut.h>

#include "algorithms.h"

namespace Algorithms
{
    Algorithm::Algorithm(const std::string &name) : _name(name)
    {
    }

    std::string Algorithm::getName() const
    {
        return this->_name;
    }

    Algorithm::~Algorithm() = default;

    MidPointAlgorithm::MidPointAlgorithm() : Algorithm("midpoint")
    {
    }

    AntiAliasingAlgorithm::AntiAliasingAlgorithm() : Algorithm("anti-aliasing")
    {
    }

    void MidPointAlgorithm::apply()
    {
        //TODO
    }

    void AntiAliasingAlgorithm::apply()
    {
        //TODO
    }
}