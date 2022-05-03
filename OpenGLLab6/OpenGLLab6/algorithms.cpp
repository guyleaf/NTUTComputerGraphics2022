#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "algorithms.h"

namespace Algorithms
{
    Algorithm::Algorithm(std::string name) : _name(name)
    {
    }

    const std::string Algorithm::getName() const
    {
        return this->_name;
    }

    MidPointAlgorithm::MidPointAlgorithm() : Algorithm("midpoint")
    {
    }

    AntiAliasingAlgorithm::AntiAliasingAlgorithm() : Algorithm("anti-aliasing")
    {
    }

    void MidPointAlgorithm::apply()
    {
    }

    void AntiAliasingAlgorithm::apply()
    {
    }
}