#pragma once
#include <string>
#include <array>

#include "graph2d.h"

namespace Graph2D
{
    class ObjLoader
    {
    public:
        explicit ObjLoader(const std::string& fileName);
        Graph2D::Polygon getResult() const;
    private:
        Graph2D::Vertex parseVertex(std::ifstream& ifs) const;
        std::array<size_t, 3> parseFace(std::ifstream& ifs) const;

        std::string _fileName;
    };
}