#include <iostream>
#include <string>
#include <utility>
#include <random>

#include "ObjLoader.h"
#include "graph2d.h"
#include "GL/freeglut.h"

const std::array<std::pair<std::string, std::string>, 4> FILES{
    std::make_pair("gourd", "resources/gourd.obj"),
    std::make_pair("octahedron", "resources/octahedron.obj"),
    std::make_pair("teapot", "resources/teapot.obj"),
    std::make_pair("teddy", "resources/teddy.obj")
};

enum class RenderModes
{
    Point,
    Line,
    Face
};

enum class ColorModes
{
    Single,
    Random
};

const std::array<std::string, 3> RENDER_MODES{ "Point", "Line", "Face" };
const std::array<std::string, 2> COLOR_MODES{ "Single Color", "Random Colors" };

constexpr int ANIMATION_SPEED = 16;
constexpr float OBJECT_VIEW_SIZE = 5.0f;

Graph2D::Polygon polygon;
std::array<float, 3> polygonColor{ 1.0f, 1.0f, 1.0f};
RenderModes renderMode = RenderModes::Line;
bool showBoundingBox = false;
bool pauseAnimation = false;

float fov = 50.0f;
float angle = 0;
std::random_device RANDOM_DEVICE;
std::uniform_real_distribution<float> COLOR_RANGE(0.0f, 1.0f);

void drawObjModel()
{
    const float width = std::abs(polygon.lowerBackRightPoint.getX() - polygon.upperFrontLeftPoint.getX());
    const float height = std::abs(polygon.upperFrontLeftPoint.getY() - polygon.lowerBackRightPoint.getY());
    const float depth = std::abs(polygon.upperFrontLeftPoint.getZ() - polygon.lowerBackRightPoint.getZ());
    const float scaleFactor = OBJECT_VIEW_SIZE / std::max(std::max(width, height), depth);

    glScalef(scaleFactor, scaleFactor, scaleFactor);
    for (const auto& face : polygon.faces)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT + static_cast<int>(renderMode));
        glColor3f(polygonColor[0], polygonColor[1], polygonColor[2]);
        glBegin(GL_TRIANGLES);
        for (size_t i : face)
        {
            const auto& point = polygon.points[i - 1];
            glVertex3f(point.getX(), point.getY(), point.getZ());
        }
        glEnd();
    }
}

void drawBoundingBox()
{
    const auto& upperFrontLeftPoint = polygon.upperFrontLeftPoint;
    const auto& lowerBackRightPoint = polygon.lowerBackRightPoint;

    const float upperFrontLeftX = upperFrontLeftPoint.getX();
    const float upperFrontLeftY = upperFrontLeftPoint.getY();
    const float upperFrontLeftZ = upperFrontLeftPoint.getZ();
    const float lowerBackRightX = lowerBackRightPoint.getX();
    const float lowerBackRightY = lowerBackRightPoint.getY();
    const float lowerBackRightZ = lowerBackRightPoint.getZ();

    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(upperFrontLeftX, upperFrontLeftY, upperFrontLeftZ);
    glVertex3f(upperFrontLeftX, lowerBackRightY, upperFrontLeftZ);
    glVertex3f(lowerBackRightX, upperFrontLeftY, upperFrontLeftZ);
    glVertex3f(lowerBackRightX, lowerBackRightY, upperFrontLeftZ);

    glVertex3f(lowerBackRightX, upperFrontLeftY, lowerBackRightZ);
    glVertex3f(lowerBackRightX, lowerBackRightY, lowerBackRightZ);

    glVertex3f(upperFrontLeftX, upperFrontLeftY, lowerBackRightZ);
    glVertex3f(upperFrontLeftX, lowerBackRightY, lowerBackRightZ);

    glVertex3f(upperFrontLeftX, upperFrontLeftY, upperFrontLeftZ);
    glVertex3f(upperFrontLeftX, lowerBackRightY, upperFrontLeftZ);

    glEnd();
    glPopMatrix();
}

void renderScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, OBJECT_VIEW_SIZE + 3.0f, 0, 0, 0, 0, 1, 0);

    glPushMatrix();
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    drawObjModel();
    if (showBoundingBox)
    {
        drawBoundingBox();
    }
    glPopMatrix();
    glutSwapBuffers();
}

void loadObj(const std::string& fileName)
{
    Graph2D::ObjLoader loader{ fileName };
    polygon = loader.getResult();
}

void updateFOV(float w = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH)), float h = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)))
{
    GLfloat fAspect = w / h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the clipping volume
    gluPerspective(fov, fAspect, 0.1f, 100.0f);
}

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);
    updateFOV(static_cast<float>(w), static_cast<float>(h));
}

void setupRC()
{
    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
}

void handleFileMenuOnSelect(int option)
{
    loadObj(FILES[option].second);
    glutPostRedisplay();
}

void handleRenderModeMenuOnSelect(int option)
{
    renderMode = static_cast<RenderModes>(option);
    glutPostRedisplay();
}

void handleColorModeMenuOnSelect(int option)
{
    auto colorMode = static_cast<ColorModes>(option);
    switch (colorMode)
    {
    case ColorModes::Single:
        polygonColor = std::array<float, 3>{ 1.0f, 1.0f, 1.0f};
        break;
    case ColorModes::Random:
        polygonColor = std::array<float, 3>{ COLOR_RANGE(RANDOM_DEVICE), COLOR_RANGE(RANDOM_DEVICE), COLOR_RANGE(RANDOM_DEVICE)};
        break;
    default:
        break;
    }
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

void timerFunction(int)
{
    if (pauseAnimation)
    {
        return;
    }

    angle++;
    if (angle >= 360)
    {
        angle = 0;
    }

    // Redraw the scene with new coordinates
    glutPostRedisplay();
    glutTimerFunc(ANIMATION_SPEED, timerFunction, 1);
}

void handleKeyboardEvent(unsigned char key, int, int)
{
    if (key == 'p' || key == 'P')
    {
        pauseAnimation = !pauseAnimation;
        glutTimerFunc(ANIMATION_SPEED, timerFunction, 1);
    }
}

void handleMouseWheelEvent(int wheel, int direction, int, int)
{
    if (wheel == 0)
    {
        // scroll down => -1, scroll up => +1
        fov -= static_cast<float>(direction) * 10;

        fov = std::min(std::max(fov, 10.0f), 170.0f);

        updateFOV();
        glutPostRedisplay();
    }
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        if (strlen(argv[1]) != 0)
        {
            loadObj(argv[1]);
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
    glutKeyboardFunc(handleKeyboardEvent);
    glutMouseWheelFunc(handleMouseWheelEvent);

    setupRC();
    buildPopupMenu();
    glutTimerFunc(ANIMATION_SPEED, timerFunction, 1);

    glutMainLoop();
    return 0;
}
