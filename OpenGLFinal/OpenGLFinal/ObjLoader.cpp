#include <iostream>
#include <fstream>

#include "ObjLoader.h"

namespace Graph2D
{
    ObjLoader::ObjLoader(const std::string& fileName) : _fileName(fileName)
    {
    }

    Graph2D::Polygon ObjLoader::getResult() const
    {
        Graph2D::Polygon polygon;
        std::ifstream ifs;

        ifs.open(_fileName);
        if (ifs.fail())
        {
            std::cout << "Error opening file" << std::endl;
            throw std::ios::failure("Error opening file");
        }

        while (!ifs.eof())
        {
            int&& code = ifs.peek();

            if (code == 'v')
            {
                polygon.points.push_back(parseVertex(ifs));
            }
            else if (code == 'f')
            {
                polygon.faces.push_back(parseFace(ifs));
            }
            else
            {
                ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        ifs.close();
        return polygon;
    }

    Graph2D::Vertex ObjLoader::parseVertex(std::ifstream& ifs) const
    {
        char tmp[256];
        Graph2D::Vertex point;

        // ignore code & space
        ifs.ignore(2);

        ifs.getline(tmp, 256, ' ');
        point.setX(std::stof(tmp));
        ifs.getline(tmp, 256, ' ');
        point.setY(std::stof(tmp));
        ifs.getline(tmp, 256, '\n');
        point.setX(std::stof(tmp));

        return point;
    }

    std::array<size_t, 3> ObjLoader::parseFace(std::ifstream& ifs) const
    {
        char tmp[256];
        size_t face1;
        size_t face2;
        size_t face3;

        // ignore code & space
        ifs.ignore(2);

        ifs.getline(tmp, 256, ' ');
        face1 = std::stoul(tmp);
        ifs.getline(tmp, 256, ' ');
        face2 = std::stoul(tmp);
        ifs.getline(tmp, 256, '\n');
        face3 = std::stoul(tmp);

        return std::array<size_t, 3>{face1, face2, face3};
    }
};
