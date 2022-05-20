#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <string>
#include <functional>
#include <utility>
#include <GL/freeglut.h>

#include "Algorithms/LineAlgorithm.h"
#include "Algorithms/Line/AntiAliasingAlgorithm.h"
#include "Algorithms/Line/MidPointAlgorithm.h"

#define GET_SIGN(NUM) std::signbit(NUM) ? -1 : 1

constexpr char ALGORITHM_MENU_NAME[] = "LineAlgorithm";
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
std::pair<double, double> convertWindowCoordinateToWorldCoordinate(const int&, const int&);
void printMouseMessage(const double&, const double&);
int roundToInt(const double& value);

// Shared variables
// 目前選擇的演算法
Algorithms::LineAlgorithm *selectedAlgorithm;
int gridSize;
// 滑鼠點選了那些座標點
std::vector<std::pair<double, double>> selectedPoints;

bool isDragging;
double mouseX;
double mouseY;
// 紀錄滑鼠按下的起始點
std::pair<double, double> startMousePoint;
// 紀錄滑鼠按下的終點
std::pair<double, double> endMousePoint;

// LineAlgorithm menu options
std::vector<std::unique_ptr<Algorithms::LineAlgorithm>> algorithms;
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
    Algorithms::Callback setPixel = [](double centerX, double centerY, double alpha)
    {
        std::cout << "Drawing pixel (" << centerX << "," << centerY << ")" << std::endl;

        glColor4d(0.5, 0.5, 0.5, alpha);
        glBegin(GL_QUADS);
        const double&& topY = centerY + CELL_HALF_WIDTH;
        const double&& bottomY = centerY - CELL_HALF_WIDTH;
        const double&& leftX = centerX - CELL_HALF_WIDTH;
        const double&& rightX = centerX + CELL_HALF_WIDTH;

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
    isDragging = false;
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
/// 處理滑鼠移動事件
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void handleMouseMotionEvent(int x, int y)
{
    if (isDragging)
    {
        const auto point = convertWindowCoordinateToWorldCoordinate(x, y);
        mouseX = point.first;
        mouseY = point.second;

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
/// 處理鍵盤按鍵事件
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
/// 選單 - 處理選擇 LineAlgorithm 時的選擇事件
/// </summary>
/// <param name="index"></param>
void handleAlgorithmMenuOnSelect(int index)
{
    isDragging = false;
    // 利用多型選擇演算法並使用
    selectedAlgorithm = algorithms[index].get();
    std::cout << "Change to use " << selectedAlgorithm->getName() << " algorithm" << std::endl;
    glutPostRedisplay();
}

/// <summary>
/// 選單 - 處理選擇 Grid Size 時的選擇事件
/// </summary>
/// <param name="size"></param>
void handleGridSizeMenuOnSelect(int size)
{
    isDragging = false;
    gridSize = size;
    std::cout << "Change grid size to " << size << std::endl;
    glutPostRedisplay();
}

/// <summary>
/// 依照所選得演算法進行光柵化
/// </summary>
void rasterizingLines()
{
    for (auto firstIter = selectedPoints.begin(); firstIter != selectedPoints.end(); firstIter += 2)
    {
        auto secondIter = firstIter + 1;

        auto startPoint = std::make_pair<int, int>(roundToInt(firstIter->first), roundToInt(firstIter->second));
        auto endPoint = std::make_pair<int, int>(roundToInt(secondIter->first), roundToInt(secondIter->second));

        // 使用此演算法
        selectedAlgorithm->apply(startPoint, endPoint);
    }
}

/// <summary>
/// 畫線
/// </summary>
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
        glVertex2d(startMousePoint.first, startMousePoint.second);
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

    rasterizingLines();
    drawGrid();
    drawLines();

    glutSwapBuffers();
}

// Components
/// <summary>
/// 處理左鍵放開時的事件
/// </summary>
void handleMouseOnLeftClickUp()
{
    if (isDragging)
    {
        selectedPoints.push_back(startMousePoint);
        selectedPoints.push_back(endMousePoint);
        printMouseMessage(endMousePoint.first, endMousePoint.second);
        isDragging = false;
    }
    else
    {
        mouseX = startMousePoint.first;
        mouseY = startMousePoint.second;
        printMouseMessage(mouseX, mouseY);
        isDragging = true;
    }
}

/// <summary>
/// 處理左鍵按下時的事件
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void handleMouseOnLeftClickDown(int x, int y)
{
    if (isDragging)
    {
        endMousePoint = convertWindowCoordinateToWorldCoordinate(x, y);
    }
    else
    {
        startMousePoint = convertWindowCoordinateToWorldCoordinate(x, y);
    }
}

/// <summary>
/// 建構 menu
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
/// 取得單邊的 Grid 大小
/// </summary>
/// <returns>單邊 Grid 大小</returns>
double getGridBoundary()
{
    return static_cast<double>(gridSize) + CELL_HALF_WIDTH;
}

/// <summary>
/// 清除狀態
/// </summary>
void clearState()
{
    isDragging = false;
    selectedPoints.clear();
}

/// <summary>
/// 轉換 Window 座標至 World 座標
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
std::pair<double, double> convertWindowCoordinateToWorldCoordinate(const int& x, const int& y)
{
    const double size = getGridBoundary();
    const double worldX = (2.0 * static_cast<double>(x) / WINDOW_WIDTH - 1.0) * size;
    const double worldY = (1.0 - 2.0 * static_cast<double>(y) / WINDOW_HEIGHT) * size;
    return std::pair<double, double>(worldX, worldY);
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

/// <summary>
/// 四捨五入並轉型成 int
/// </summary>
/// <param name="value"></param>
/// <returns></returns>
int roundToInt(const double& value)
{
    return static_cast<int>(std::round(value));
}