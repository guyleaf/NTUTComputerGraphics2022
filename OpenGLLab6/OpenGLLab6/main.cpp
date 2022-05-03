#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <string>
#include <GL/freeglut.h>

#include "algorithms.h"

#define GET_SIGN(NUM) std::signbit(NUM) ? -1 : 1

constexpr char ALGORITHM_MENU_NAME[] = "Algorithm";
constexpr char GRID_SIZE_MENU_NAME[] = "Grid Size";

constexpr float GRID_LINE_WIDTH = 1.5f;
constexpr float LINE_WIDTH = 1.8f;

constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 600;

constexpr double CELL_WIDTH = 1.0;
constexpr double CELL_HALF_WIDTH = CELL_WIDTH / 2;

// precompile
void changeSize(int, int);
void renderScene();

void handleMouseOnLeftClickUp();
void handleMouseOnLeftClickDown(int, int);
void handleMouseOnClick(int, int, int, int);
void handleMouseOnMove(int, int);
void handleAlgorithmMenuOnSelect(int);
void handleGridSizeMenuOnSelect(int);

void setUpRC();
void buildPopupMenu();
void fillCell(double, double);
void drawLines();
void rasterizingLines();

double getGridBoundary();
void clearState();
std::pair<double, double> convertWindowCoordinateToWorldCoordinate(int, int);
void printMouseMessage(double, double);

// Shared variables
Algorithms::Algorithm *selectedAlgorithm;
int gridSize;
std::vector<std::pair<double, double>> selectedPoints;

bool isDragging;
double mouseX;
double mouseY;
std::pair<double, double> startPoint;
std::pair<double, double> endPoint;

// Algorithm menu options
std::vector<std::unique_ptr<Algorithms::Algorithm>> algorithms;
// Grid size menu options
const std::array<int, 5> GRID_SIZES = {10, 15, 20, 25, 30};

// Light values and coordinates
const std::array<GLfloat, 4> ENV_AMBIENT_COLOR = {0.45f, 0.45f, 0.45f, 1.0f};
const std::array<GLfloat, 4> SOURCE_COLOR = {0.25f, 0.25f, 0.25f, 1.0f};
const std::array<GLfloat, 4> LIGHT_POSITION = {0.f, 25.0f, 20.0f, 0.0f};

void initializeAlgorithms()
{
    auto midpoint = std::make_unique<Algorithms::MidPointAlgorithm>();
    algorithms.push_back(std::move(midpoint));
    auto antiAliasing = std::make_unique<Algorithms::AntiAliasingAlgorithm>();
    algorithms.push_back(std::move(antiAliasing));
}

int main(int argc, char **argv)
{
    initializeAlgorithms();
    isDragging = false;
    selectedAlgorithm = algorithms.front().get();
    gridSize = GRID_SIZES.front();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("Rasterizing Lines");

    buildPopupMenu();
    setUpRC();

    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutMouseFunc(handleMouseOnClick);
    glutPassiveMotionFunc(handleMouseOnMove);

    glutMainLoop(); // http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2288.html
    return 0;
}

void handleMouseOnMove(int x, int y)
{
    if (isDragging)
    {
        const auto point = convertWindowCoordinateToWorldCoordinate(x, y);
        mouseX = point.first;
        mouseY = point.second;
    }

    glutPostRedisplay();
}

void handleMouseOnClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        switch (state)
        {
        case GLUT_UP:
            handleMouseOnLeftClickUp();
            break;
        case GLUT_DOWN:
            handleMouseOnLeftClickDown(x, y);
            break;
        default:
            break;
        }
    }

    glutPostRedisplay();
}

void handleAlgorithmMenuOnSelect(int index)
{
    clearState();
    selectedAlgorithm = algorithms[index].get();
    std::cout << "Change to use " << selectedAlgorithm->getName() << " algorithm" << std::endl;
    glutPostRedisplay();
}

void handleGridSizeMenuOnSelect(int size)
{
    clearState();
    gridSize = size;
    std::cout << "Change grid size to " << size << std::endl;
    glutPostRedisplay();
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

void rasterizingLines()
{
    for (auto iter = selectedPoints.begin(); iter != selectedPoints.end(); iter += 2)
    {
        const double &&centerX = std::round(iter->first);
        const double &&centerY = std::round(iter->second);
        fillCell(centerX, centerY);
    }
}

void drawLines()
{
    glColor3d(0.0, 0.0, 1.0);
    glLineWidth(LINE_WIDTH);
    glBegin(GL_LINES);
    for (auto iter = selectedPoints.begin(); iter != selectedPoints.end(); iter += 2)
    {
        glVertex2d(iter->first, iter->second);
        glVertex2d((iter + 1)->first, (iter + 1)->second);
    }
    glEnd();

    if (isDragging)
    {
        glColor3d(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex2d(startPoint.first, startPoint.second);
        glVertex2d(mouseX, mouseY);
        glEnd();
    }
}

void drawGrid()
{
    glColor3d(0.0, 0.0, 0.0);
    glLineWidth(GRID_LINE_WIDTH);
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

    rasterizingLines();
    drawGrid();
    drawLines();

    glutSwapBuffers();
}

// Components
void handleMouseOnLeftClickUp()
{
    if (isDragging)
    {
        selectedPoints.push_back(startPoint);
        selectedPoints.push_back(endPoint);
        printMouseMessage(endPoint.first, endPoint.second);
        isDragging = false;
    }
    else
    {
        mouseX = startPoint.first;
        mouseY = startPoint.second;
        printMouseMessage(startPoint.first, startPoint.second);
        isDragging = true;
    }
}

void handleMouseOnLeftClickDown(int x, int y)
{
    if (isDragging)
    {
        endPoint = convertWindowCoordinateToWorldCoordinate(x, y);
    }
    else
    {
        startPoint = convertWindowCoordinateToWorldCoordinate(x, y);
    }
}

void buildPopupMenu()
{
    const int algorithmMenu = glutCreateMenu(handleAlgorithmMenuOnSelect);
    int counter = 0;
    for (size_t i = 0; i < algorithms.size(); i++, counter++)
    {
        glutAddMenuEntry(algorithms[i]->getName().c_str(), counter);
    }

    const int gridSizeMenu = glutCreateMenu(handleGridSizeMenuOnSelect);
    for (const int &size : GRID_SIZES)
    {
        glutAddMenuEntry(std::to_string(size).c_str(), size);
    }

    glutCreateMenu(nullptr);
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

// Helpers
double getGridBoundary()
{
    return static_cast<double>(gridSize) + CELL_HALF_WIDTH;
}

void clearState()
{
    isDragging = false;
    selectedPoints.clear();
}

std::pair<double, double> convertWindowCoordinateToWorldCoordinate(int x, int y)
{
    const double size = getGridBoundary();
    const double worldX = (2.0 * static_cast<double>(x) / WINDOW_WIDTH - 1.0) * size;
    const double worldY = (1.0 - 2.0 * static_cast<double>(y) / WINDOW_HEIGHT) * size;
    return std::pair<double, double>(worldX, worldY);
}

void printMouseMessage(double x, double y)
{
    std::cout << "Mouse click: (" << x << "," << y << ")" << std::endl;
}