#include <iostream>
#include <string>
#include <utility>

#include "ObjLoader.h"
#include "graph2d.h"
#include "GL/freeglut.h"

const std::array<std::pair<std::string, std::string>, 4> FILES{
    std::make_pair("gourd", "resources/gourd.obj"),
    std::make_pair("octahedron", "resources/octahedron.obj"),
    std::make_pair("teapot", "resources/teapot.obj"),
    std::make_pair("teddy", "resources/teddy.obj")
};

enum class ColorModes
{
    Single,
    Random
};

const std::array<std::string, 3> RENDER_MODES{ "Point", "Line", "Face" };
const std::array<std::string, 2> COLOR_MODES{ "Single Color", "Random Colors" };

Graph2D::Polygon polygon;
bool isObjLoaded = false;
ColorModes colorMode = ColorModes::Single;
bool showBoundingBox = false;

void renderScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (isObjLoaded)
    {
        for (const auto& face: polygon.faces)
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_POLYGON);
            for (size_t i: face)
            {
                const auto& point = polygon.points[i - 1];
                glVertex3f(point.getX(), point.getY(), point.getZ());
            }
            glEnd();
        }
    }

    glutSwapBuffers();
}

void loadObj(const std::string& fileName)
{
    Graph2D::ObjLoader loader{ fileName };
    polygon = loader.getResult();
}

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    GLfloat fAspect = (GLfloat)w / (GLfloat)h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the clipping volume
    gluPerspective(35.0f, fAspect, 1.0f, 50.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 10.0f, 0, 0, 0, 0, 1, 0);
}

void setupRC()
{
    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

void handleFileMenuOnSelect(int option)
{
    isObjLoaded = true;
    loadObj(FILES[option].second);

    glutPostRedisplay();
}

void handleRenderModeMenuOnSelect(int option)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT + option);
    glutPostRedisplay();
}

void handleColorModeMenuOnSelect(int option)
{
    colorMode = static_cast<ColorModes>(option);
    glutPostRedisplay();
}

void handleBoundingBoxMenuOnSelect(int option)
{
    showBoundingBox = static_cast<bool>(option);
    glutPostRedisplay();
}

void buildPopupMenu()
{
    const int fileMenu = glutCreateMenu(handleFileMenuOnSelect);
    int counter = 0;
    for (size_t i = 0; i < FILES.size(); i++, counter++)
    {
        glutAddMenuEntry(FILES[i].first.c_str(), counter);
    }

    const int renderModeMenu = glutCreateMenu(handleRenderModeMenuOnSelect);
    counter = 0;
    for (size_t i = 0; i < RENDER_MODES.size(); i++, counter++)
    {
        glutAddMenuEntry(RENDER_MODES[i].c_str(), counter);
    }

    const int colorModeMenu = glutCreateMenu(handleColorModeMenuOnSelect);
    counter = 0;
    for (size_t i = 0; i < COLOR_MODES.size(); i++, counter++)
    {
        glutAddMenuEntry(COLOR_MODES[i].c_str(), counter);
    }

    const int boundingBoxMenu = glutCreateMenu(handleBoundingBoxMenuOnSelect);
    glutAddMenuEntry("On", 1);
    glutAddMenuEntry("Off", 0);

    glutCreateMenu(nullptr);
    glutAddSubMenu("File", fileMenu);
    glutAddSubMenu("Render Mode", renderModeMenu);
    glutAddSubMenu("Color Mode", colorModeMenu);
    glutAddSubMenu("Bounding Box", boundingBoxMenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        if (strlen(argv[1]) != 0)
        {
            loadObj(argv[1]);
            isObjLoaded = true;
        }
        else
        {
            std::cout << "Invalid argument" << std::endl;
            return 1;
        }
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Model Viewer");
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);

    setupRC();
    buildPopupMenu();

    glutMainLoop();
    return 0;
}
