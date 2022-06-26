#include <iostream>

#include "ObjLoader.h"
#include "graph2d.h"

Graph2D::Polygon polygon;
bool is_obj_loaded = false;


void loadObj(const std::string& fileName)
{
    Graph2D::ObjLoader loader{ fileName };
    polygon = loader.getResult();
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        if (strlen(argv[1]) != 0)
        {
            loadObj(argv[1]);
            is_obj_loaded = true;
        }
        else
        {
            std::cout << "Invalid argument" << std::endl;
            return 1;
        }
    }

    return 0;
}