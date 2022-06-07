#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <string>
#include <functional>
#include <utility>
#include <GL/freeglut.h>

#include "graph2d.h"
#include "rasterizationAlgorithms.h"


constexpr char RASTERIZATION_MODE_MENU_NAME[] = "Rasterization Mode";
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

void handleMouseEvent(int, int, int, int);
void handleMouseMotionEvent(int, int);
void handleKeyboardEvent(unsigned char, int, int);
void handleRasterizationModeMenuOnSelect(int);
void handleGridSizeMenuOnSelect(int);

void setUpRC();
void buildPopupMenu();
void drawLines();
void rasterizeLines();
void rasterizePolygons();

bool isDrawing();
bool isInSameGridCell(const Graph2D::Vertex&, const Graph2D::Vertex&);
double getGridBoundary();
Graph2D::Vertex convertWindowCoordinateToWorldCoordinate(const int&, const int&);
void printMouseMessage(const double&, const double&);

enum class RasterizationMode
{
    Line,
    Polygon
};

// Shared variables
std::unique_ptr<RasterizationAlgorithms::LineAlgorithm> selectedLineAlgorithm;
std::unique_ptr<RasterizationAlgorithms::PolygonAlgorithm> selectedPolygonAlgorithm;
RasterizationMode mode;
int gridSize;

// 儲存圖形
std::vector<Graph2D::Polygon> polygons;
// 滑鼠點選了那些座標點
std::vector<Graph2D::Vertex> selectedPoints;

double mouseX;
double mouseY;


const std::array<std::string, 2> RASTERIZATION_MODE_NAMES = { "Line", "Graph2D" };
// Grid size menu options
const std::array<int, 5> GRID_SIZES = {10, 15, 20, 25, 30};

// Light values and coordinates
const std::array<GLfloat, 4> ENV_AMBIENT_COLOR = {0.45f, 0.45f, 0.45f, 1.0f};
const std::array<GLfloat, 4> SOURCE_COLOR = {0.25f, 0.25f, 0.25f, 1.0f};
const std::array<GLfloat, 4> LIGHT_POSITION = {0.f, 25.0f, 20.0f, 0.0f};

/// <summary>
/// 初始化演算法
/// </summary>
void initializeAlgorithms()
{
    // Lambda: 定義如何畫格子
    auto setPixel = [](const Graph2D::Vertex& centerVertex)
    {
        const double centerX = centerVertex.getX();
        const double centerY = centerVertex.getY();

        const double topY = centerY + CELL_HALF_SIZE;
        const double bottomY = centerY - CELL_HALF_SIZE;
        const double leftX = centerX - CELL_HALF_SIZE;
        const double rightX = centerX + CELL_HALF_SIZE;

        glColor4d(centerVertex.getRed(), centerVertex.getGreen(), centerVertex.getBlue(), centerVertex.getAlpha());
        glBegin(GL_QUADS);
        glVertex2d(leftX, topY);
        glVertex2d(rightX, topY);
        glVertex2d(rightX, bottomY);
        glVertex2d(leftX, bottomY);
        glEnd();
    };

    selectedLineAlgorithm = std::make_unique<RasterizationAlgorithms::AntiAliasingAlgorithm>(setPixel);
    selectedPolygonAlgorithm = std::make_unique<RasterizationAlgorithms::ScanLineAlgorithm>(setPixel);
}

int main(int argc, char **argv)
{
    std::cout << sizeof(std::size_t) << std::endl;

    initializeAlgorithms();
    gridSize = GRID_SIZES.front();
    mode = RasterizationMode::Line;

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
/// 處理滑鼠移動事件
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void handleMouseMotionEvent(int x, int y)
{
    const auto point = convertWindowCoordinateToWorldCoordinate(x, y);
    mouseX = point.getX();
    mouseY = point.getY();

    // while drawing polygon
    if (isDrawing())
    {
        glutPostRedisplay();
    }
}

/// <summary>
/// 處理滑鼠事件
/// </summary>
/// <param name="button"></param>
/// <param name="state"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void handleMouseEvent(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        const auto point = convertWindowCoordinateToWorldCoordinate(x, y);

        // Check if the point and the first selected point are in the same grid cell
        if (isDrawing() && isInSameGridCell(point, selectedPoints.front()))
        {
            polygons.emplace_back(selectedPoints);
            selectedPoints.clear();
        }
        else
        {
            selectedPoints.push_back(point);
        }

        glutPostRedisplay();
    }
}

/// <summary>
/// 處理鍵盤按鍵事件
/// </summary>
/// <param name="key"></param>
/// <param name=""></param>
/// <param name=""></param>
void handleKeyboardEvent(unsigned char key, int, int)
{
    if (key == 'r')
    {
        polygons.clear();
        selectedPoints.clear();

        glutPostRedisplay();
    }
}

/// <summary>
/// 選單 - 處理選擇 LineAlgorithm 時的選擇事件
/// </summary>
/// <param name="index"></param>
void handleRasterizationModeMenuOnSelect(int index)
{
    selectedPoints.clear();
    mode = static_cast<RasterizationMode>(index);
    glutPostRedisplay();
}

/// <summary>
/// 選單 - 處理選擇 Grid Size 時的選擇事件
/// </summary>
/// <param name="size"></param>
void handleGridSizeMenuOnSelect(int size)
{
    selectedPoints.clear();
    gridSize = size;
    std::cout << "Change grid size to " << size << std::endl;
    glutPostRedisplay();
}

/// <summary>
/// 依照所選得演算法進行光柵化
/// </summary>
void rasterizeLines()
{
    for (Graph2D::Polygon polygon : polygons)
    {
        const std::vector<Graph2D::Vertex>& vertices = polygon.getVertices();

        for (auto iter = vertices.begin(); iter != vertices.end() - 1; iter += 1)
        {
            selectedLineAlgorithm->apply(*iter, *(iter + 1));
        }

        selectedLineAlgorithm->apply(vertices.back(), vertices.front());
    }
}

void rasterizePolygons()
{
    for (Graph2D::Polygon polygon : polygons)
    {
        selectedPolygonAlgorithm->apply(polygon.getVertices());
    }
}

/// <summary>
/// 畫線
/// </summary>
void drawLines()
{
    glColor3d(0.0, 0.0, 1.0);
    glLineWidth(LINE_WIDTH);

    for (const Graph2D::Polygon& polygon : polygons)
    {
        const std::vector<Graph2D::Vertex>& vertices = polygon.getVertices();

        glBegin(GL_LINES);
        for (auto iter = vertices.begin(); iter != vertices.end() - 1; iter += 1)
        {
            glVertex2d(iter->getX(), iter->getY());
            glVertex2d((iter + 1)->getX(), (iter + 1)->getY());
        }

        const auto& firstPoint = vertices.front();
        const auto& lastPoint = vertices.back();
        glVertex2d(lastPoint.getX(), lastPoint.getY());
        glVertex2d(firstPoint.getX(), firstPoint.getY());
        glEnd();
    }

    if (isDrawing())
    {
        glColor3d(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        for (auto iter = selectedPoints.begin(); iter != selectedPoints.end() - 1; iter += 1)
        {
            glVertex2d(iter->getX(), iter->getY());
            glVertex2d((iter + 1)->getX(), (iter + 1)->getY());
        }

        const auto& lastSelectedPoint = selectedPoints.back();
        glVertex2d(lastSelectedPoint.getX(), lastSelectedPoint.getY());
        glVertex2d(mouseX, mouseY);
        glEnd();
    }
}

/// <summary>
/// 畫格子
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
/// 渲染
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

    switch (mode)
    {
    case RasterizationMode::Line:
        rasterizeLines();
        break;
    case RasterizationMode::Polygon:
        rasterizePolygons();
        break;
    default:
        std::cout << "Unknown Rasterization mode" << std::endl;
    }

    drawGrid();
    drawLines();

    glutSwapBuffers();
}

// Components
/// <summary>
/// 建構 menu
/// </summary>
void buildPopupMenu()
{
    const int raterizationModeMenu = glutCreateMenu(handleRasterizationModeMenuOnSelect);
    int counter = 0;
    for (std::string name: RASTERIZATION_MODE_NAMES)
    {
        glutAddMenuEntry(name.c_str(), counter);
        counter++;
    }

    const int gridSizeMenu = glutCreateMenu(handleGridSizeMenuOnSelect);
    for (int size : GRID_SIZES)
    {
        glutAddMenuEntry(std::to_string(size).c_str(), size);
    }

    glutCreateMenu(nullptr);
    glutAddSubMenu(RASTERIZATION_MODE_MENU_NAME, raterizationModeMenu);
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
bool isDrawing()
{
    return !selectedPoints.empty();
}

bool isInSameGridCell(const Graph2D::Vertex& lhs, const Graph2D::Vertex& rhs)
{
    return std::round(lhs.getX()) == std::round(rhs.getX()) && std::round(lhs.getY()) == std::round(rhs.getY());
}

/// <summary>
/// 取得單邊的 Grid 大小
/// </summary>
/// <returns>單邊 Grid 大小</returns>
double getGridBoundary()
{
    return static_cast<double>(gridSize) + CELL_HALF_SIZE;
}

/// <summary>
/// 轉換 Window 座標至 World 座標
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
Graph2D::Vertex convertWindowCoordinateToWorldCoordinate(const int& x, const int& y)
{
    const double size = getGridBoundary();
    const double worldX = (2.0 * static_cast<double>(x) / WINDOW_WIDTH - 1.0) * size;
    const double worldY = (1.0 - 2.0 * static_cast<double>(y) / WINDOW_HEIGHT) * size;
    return Graph2D::Vertex(worldX, worldY);
}

/// <summary>
/// 顯示滑鼠點選位置
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void printMouseMessage(const double& x, const double& y)
{
    std::cout << "Mouse click: (" << x << "," << y << ")" << std::endl;
}