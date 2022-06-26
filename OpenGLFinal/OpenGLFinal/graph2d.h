#pragma once
#include <vector>
#include <array>

#include "Vertex.h"

namespace Graph2D
{
    using Polygon = struct
    {
        std::vector<Graph2D::Vertex> points;
        std::vector<std::array<size_t, 3>> faces;
    };
}