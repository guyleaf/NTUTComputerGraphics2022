#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#define GET_SIGN(NUM) std::signbit(NUM) ? -1 : 1

#define ALGORITHM_MENU_NAME "Algorithm"
#define GRID_SIZE_MENU_NAME "Grid Size"

constexpr float LINE_WIDTH = 1.5f;

constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 600;

constexpr double CELL_WIDTH = 1.0;
constexpr double CELL_HALF_WIDTH = CELL_WIDTH / 2;

void changeSize(int, int);
void renderScene();
void handleMouseOnClick(int, int, int, int);
void handleAlgorithmMenuOnSelect(int);
void handleGridSizeMenuOnSelect(int);
void setUpRC();
void buildPopupMenu();
void fillCell(double, double);
void fillCells();
void drawLine();
double getGridBoundary();

// Shared variables
int algorithmIndex;
int gridSize;
std::vector<std::pair<double, double>> selectedPoints;

// Algorithm menu options
const std::array<std::string, 2> ALGORITHMS = {"midpoint", "anti-aliasing"};
// Grid size menu options
const std::array<int, 5> GRID_SIZES = {10, 15, 20, 25, 30};

// Light values and coordinates
const std::array<GLfloat, 4> ENV_AMBIENT_COLOR = {0.45f, 0.45f, 0.45f, 1.0f};
const std::array<GLfloat, 4> SOURCE_COLOR = {0.25f, 0.25f, 0.25f, 1.0f};
const std::array<GLfloat, 4> LIGHT_POSITION = {0.f, 25.0f, 20.0f, 0.0f};

int main(int argc, char **argv)
{
    algorithmIndex = 0;
    gridSize = GRID_SIZES.front();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("Rasterizing lines");

    buildPopupMenu();
    setUpRC();

    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutMouseFunc(handleMouseOnClick);

    glutMainLoop(); // http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2288.html
    return 0;
}

void handleMouseOnClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        const double size = getGridBoundary();
        // Convert window coordinate to global coordinate
        const double mouseX = (2.0 * static_cast<double>(x) / WINDOW_WIDTH - 1.0) * size;
        const double mouseY = (1.0 - 2.0 * static_cast<double>(y) / WINDOW_HEIGHT) * size;
        selectedPoints.push_back(std::make_pair(mouseX, mouseY));
        std::cout << "Mouse click: (" << mouseX << "," << mouseY << ")" << std::endl;
    }

    glutPostRedisplay();
}

void handleAlgorithmMenuOnSelect(int index)
{
    selectedPoints.clear();
    algorithmIndex = index;
    std::cout << "Change to use " << ALGORITHMS[index] << " algorithm" << std::endl;
    glutPostRedisplay();
}

void handleGridSizeMenuOnSelect(int size)
{
    selectedPoints.clear();
    gridSize = size;
    std::cout << "Change grid size to " << size << std::endl;
    glutPostRedisplay();
}

void buildPopupMenu()
{
    const int algorithmMenu = glutCreateMenu(handleAlgorithmMenuOnSelect);
    for (size_t i = 0; i < ALGORITHMS.size(); i++)
    {
        glutAddMenuEntry(ALGORITHMS[i].c_str(), i);
    }

    const int gridSizeMenu = glutCreateMenu(handleGridSizeMenuOnSelect);
    for (const int &size : GRID_SIZES)
    {
        glutAddMenuEntry(std::to_string(size).c_str(), size);
    }

    glutCreateMenu(NULL);
    glutAddSubMenu(ALGORITHM_MENU_NAME, algorithmMenu);
    glutAddSubMenu(GRID_SIZE_MENU_NAME, gridSizeMenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/// <summary>
/// Setup Rendering Contex
/// </summary>
void setUpRC()
{
    // Enable lighting
    glEnable(GL_LIGHTING);

    // Setup and enable light 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ENV_AMBIENT_COLOR.data());
    glLightfv(GL_LIGHT0, GL_AMBIENT, SOURCE_COLOR.data());
    glLightfv(GL_LIGHT0, GL_DIFFUSE, SOURCE_COLOR.data());
    glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POSITION.data());
    glEnable(GL_LIGHT0);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
}

void changeSize(int w, int h)
{
    glViewport(0, 0, w, h);
}

void fillCell(double centerX, double centerY)
{
    glColor4d(0.5, 0.5, 0.5, 1.0);
    glBegin(GL_QUADS);
    const double &&topY = centerY + CELL_HALF_WIDTH;
    const double &&bottomY = centerY - CELL_HALF_WIDTH;
    const double &&leftX = centerX - CELL_HALF_WIDTH;
    const double &&rightX = centerX + CELL_HALF_WIDTH;

    glVertex2d(leftX, topY);
    glVertex2d(rightX, topY);
    glVertex2d(rightX, bottomY);
    glVertex2d(leftX, bottomY);
    glEnd();
}

void fillCells()
{
    for (const auto &point : selectedPoints)
    {
        const double &&centerX = std::round(point.first);
        const double &&centerY = std::round(point.second);
        fillCell(centerX, centerY);
    }
}

void drawGrid()
{
    glColor3d(0.0, 0.0, 0.0);
    glLineWidth(LINE_WIDTH);
    glBegin(GL_LINES);

    const double boundary = getGridBoundary();
    for (double i = -boundary; i <= boundary; i++)
    {
        glVertex2d(-boundary, i);
        glVertex2d(boundary, i);
        glVertex2d(i, boundary);
        glVertex2d(i, -boundary);
    }
    glEnd();
}

void drawLine()
{
}

void renderScene()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const double boundary = getGridBoundary();
    glOrtho(-boundary, boundary, -boundary, boundary, -10.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    fillCells();

    // Draw grid
    drawGrid();
    glutSwapBuffers();
}

// Helpers
double getGridBoundary()
{
    return static_cast<double>(gridSize) + CELL_HALF_WIDTH;
}