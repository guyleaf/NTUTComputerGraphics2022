#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <string>
#include <functional>
#include <utility>
#include <GL/freeglut.h>

#include "Vertex.h"
#include "Polygon.h"
#include "Algorithms/Line/AntiAliasingAlgorithm.h"
#include "Algorithms/Line/MidPointAlgorithm.h"
#include "Algorithms/Polygon/ScanLineAlgorithm.h"

#define GET_SIGN(NUM) std::signbit(NUM) ? -1 : 1

constexpr char ALGORITHM_MENU_NAME[] = "LineAlgorithm";
constexpr char GRID_SIZE_MENU_NAME[] = "Grid Size";

constexpr float GRID_LINE_WIDTH = 1.5f;
constexpr float LINE_WIDTH = 1.8f;

constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 600;

constexpr double CELL_SIZE = 1.0;
constexpr double CELL_HALF_SIZE = CELL_SIZE / 2;

// precompile
void changeSize(int, int);
void renderScene();

void handleMouseOnLeftClickUp();
void handleMouseOnLeftClickDown(int, int);
void handleMouseEvent(int, int, int, int);
void handleMouseMotionEvent(int, int);
void handleKeyboardEvent(unsigned char, int, int);
void handleAlgorithmMenuOnSelect(int);
void handleGridSizeMenuOnSelect(int);

void setUpRC();
void buildPopupMenu();
void drawLines();
void rasterizingLines();

double getGridBoundary();
void clearState();
Vertex::Vertex convertWindowCoordinateToWorldCoordinate(const int&, const int&);
void printMouseMessage(const double&, const double&);

// Shared variables
// �ثe��ܪ��t��k
Algorithms::LineAlgorithm *selectedAlgorithm;
int gridSize;

// �x�s�ϧ�
std::vector<Polygon::Polygon> polygons;
// �ƹ��I��F���Ǯy���I
std::vector<Vertex::Vertex> selectedPoints;

bool isDrawing;
double mouseX;
double mouseY;

// �����ƹ����U����m
Vertex::Vertex mousePoint;

// LineAlgorithm menu options
std::vector<std::unique_ptr<Algorithms::LineAlgorithm>> algorithms;
// Grid size menu options
const std::array<int, 5> GRID_SIZES = {10, 15, 20, 25, 30};

// Light values and coordinates
const std::array<GLfloat, 4> ENV_AMBIENT_COLOR = {0.45f, 0.45f, 0.45f, 1.0f};
const std::array<GLfloat, 4> SOURCE_COLOR = {0.25f, 0.25f, 0.25f, 1.0f};
const std::array<GLfloat, 4> LIGHT_POSITION = {0.f, 25.0f, 20.0f, 0.0f};

/// <summary>
/// ��l�ƺt��k
/// </summary>
void initializeAlgorithms()
{
    // Lambda: �w�q�p��e��l
    auto setPixel = [](const Vertex::Vertex& centerVertex)
    {
        const double centerX = centerVertex.getX();
        const double centerY = centerVertex.getY();

        const double topY = centerY + CELL_HALF_SIZE;
        const double bottomY = centerY - CELL_HALF_SIZE;
        const double leftX = centerX - CELL_HALF_SIZE;
        const double rightX = centerX + CELL_HALF_SIZE;

        // std::cout << "Drawing pixel (" << centerX << "," << centerY << ")" << std::endl;

        glColor4d(centerVertex.getRed(), centerVertex.getGreen(), centerVertex.getBlue(), centerVertex.getAlpha());
        glBegin(GL_QUADS);
        glVertex2d(leftX, topY);
        glVertex2d(rightX, topY);
        glVertex2d(rightX, bottomY);
        glVertex2d(leftX, bottomY);
        glEnd();
    };

    auto midpoint = std::make_unique<Algorithms::MidPointAlgorithm>(setPixel);
    algorithms.push_back(std::move(midpoint));
    auto antiAliasing = std::make_unique<Algorithms::AntiAliasingAlgorithm>(setPixel);
    algorithms.push_back(std::move(antiAliasing));
}

int main(int argc, char **argv)
{
    std::cout << sizeof(std::size_t) << std::endl;

    initializeAlgorithms();
    isDrawing = false;
    selectedAlgorithm = algorithms.front().get();
    gridSize = GRID_SIZES.front();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("Rasterizing Polygons");

    buildPopupMenu();
    setUpRC();

    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutMouseFunc(handleMouseEvent);
    glutPassiveMotionFunc(handleMouseMotionEvent);
    glutKeyboardFunc(handleKeyboardEvent);

    glutMainLoop(); // http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2288.html
    return 0;
}

/// <summary>
/// �B�z�ƹ����ʨƥ�
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void handleMouseMotionEvent(int x, int y)
{
    if (isDrawing)
    {
        const auto point = convertWindowCoordinateToWorldCoordinate(x, y);
        mouseX = point.getX();
        mouseY = point.getY();

        glutPostRedisplay();
    }
}

/// <summary>
/// �B�z�ƹ��ƥ�
/// </summary>
/// <param name="button"></param>
/// <param name="state"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void handleMouseEvent(int button, int state, int x, int y)
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

        glutPostRedisplay();
    }
}

/// <summary>
/// �B�z��L����ƥ�
/// </summary>
/// <param name="key"></param>
/// <param name=""></param>
/// <param name=""></param>
void handleKeyboardEvent(unsigned char key, int, int)
{
    if (key == 'r')
    {
        clearState();
    }

    glutPostRedisplay();
}

/// <summary>
/// ��� - �B�z��� LineAlgorithm �ɪ���ܨƥ�
/// </summary>
/// <param name="index"></param>
void handleAlgorithmMenuOnSelect(int index)
{
    isDrawing = false;
    // �Q�Φh����ܺt��k�èϥ�
    selectedAlgorithm = algorithms[index].get();
    std::cout << "Change to use " << selectedAlgorithm->getName() << " algorithm" << std::endl;
    glutPostRedisplay();
}

/// <summary>
/// ��� - �B�z��� Grid Size �ɪ���ܨƥ�
/// </summary>
/// <param name="size"></param>
void handleGridSizeMenuOnSelect(int size)
{
    isDrawing = false;
    gridSize = size;
    std::cout << "Change grid size to " << size << std::endl;
    glutPostRedisplay();
}

/// <summary>
/// �̷өҿ�o�t��k�i����]��
/// </summary>
void rasterizingLines()
{
    for (Polygon::Polygon polygon : polygons)
    {
        const std::vector<Vertex::Vertex>& vertices = polygon.getVertices();

        for (auto firstIter = vertices.begin(); firstIter != vertices.end(); firstIter += 2)
        {
            auto secondIter = firstIter + 1;

            auto startVertex = Vertex::Vertex(std::round(firstIter->getX()), std::round(firstIter->getY()));
            auto endVertex = Vertex::Vertex(std::round(secondIter->getX()), std::round(secondIter->getY()));

            // �ϥΦ��t��k
            selectedAlgorithm->apply(startVertex, endVertex);
        }
    }
}

/// <summary>
/// �e�u
/// </summary>
void drawLines()
{
    glColor3d(0.0, 0.0, 1.0);
    glLineWidth(LINE_WIDTH);

    for (Polygon::Polygon polygon : polygons)
    {
        glBegin(GL_LINES);
        const std::vector<Vertex::Vertex>& vertices = polygon.getVertices();

        for (auto iter = vertices.begin(); iter != vertices.end(); iter += 2)
        {
            glVertex2d(iter->getX(), iter->getY());
            glVertex2d((iter + 1)->getX(), (iter + 1)->getY());
        }
        glEnd();
    }

    if (isDrawing)
    {
        glColor3d(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex2d(startMousePoint.getX(), startMousePoint.getY());
        glVertex2d(mouseX, mouseY);
        glEnd();
    }
}

/// <summary>
/// �e��l
/// </summary>
void drawGrid()
{
    glColor3d(0.0, 0.0, 0.0);
    glLineWidth(GRID_LINE_WIDTH);
    glBegin(GL_LINES);
    const double boundary = getGridBoundary();
    double i = -boundary;

    while (i <= boundary)
    {
        glVertex2d(-boundary, i);
        glVertex2d(boundary, i);
        glVertex2d(i, boundary);
        glVertex2d(i, -boundary);
        i++;
    }
    glEnd();
}

/// <summary>
/// ��V
/// </summary>
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
/// <summary>
/// �B�z�����}�ɪ��ƥ�
/// </summary>
void handleMouseOnLeftClickUp()
{
    if (isDrawing)
    {
        selectedPoints.emplace_back(startMousePoint);
        selectedPoints.emplace_back(endMousePoint);
        printMouseMessage(endMousePoint.getX(), endMousePoint.getY());
        isDrawing = false;
    }
    else
    {
        mouseX = startMousePoint.getX();
        mouseY = startMousePoint.getY();
        printMouseMessage(mouseX, mouseY);
        isDrawing = true;
    }
}

/// <summary>
/// �B�z������U�ɪ��ƥ�
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void handleMouseOnLeftClickDown(int x, int y)
{
    if (isDrawing)
    {
        endMousePoint = convertWindowCoordinateToWorldCoordinate(x, y);
    }
    else
    {
        startMousePoint = convertWindowCoordinateToWorldCoordinate(x, y);
    }
}

/// <summary>
/// �غc menu
/// </summary>
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void changeSize(int w, int h)
{
    glViewport(0, 0, w, h);
}

// Helpers

/// <summary>
/// ���o���䪺 Grid �j�p
/// </summary>
/// <returns>���� Grid �j�p</returns>
double getGridBoundary()
{
    return static_cast<double>(gridSize) + CELL_HALF_SIZE;
}

/// <summary>
/// �M�����A
/// </summary>
void clearState()
{
    isDrawing = false;
    selectedPoints.clear();
}

/// <summary>
/// �ഫ Window �y�Ц� World �y��
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
Vertex::Vertex convertWindowCoordinateToWorldCoordinate(const int& x, const int& y)
{
    const double size = getGridBoundary();
    const double worldX = (2.0 * static_cast<double>(x) / WINDOW_WIDTH - 1.0) * size;
    const double worldY = (1.0 - 2.0 * static_cast<double>(y) / WINDOW_HEIGHT) * size;
    return Vertex::Vertex(worldX, worldY);
}

/// <summary>
/// ��ܷƹ��I���m
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void printMouseMessage(const double& x, const double& y)
{
    std::cout << "Mouse click: (" << x << "," << y << ")" << std::endl;
}