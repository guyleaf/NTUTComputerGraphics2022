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

#define GET_SIGN(NUM) NUM >= 0 ? 1 : -1

constexpr double STEPS_FOR_MOVING = 0.1f;
constexpr double STEPS_FOR_SCALING = 0.1f;

constexpr double ANGLES_FOR_ONE_ROUND = 360.0f;
constexpr double ANGLES_FOR_ROTATION = 3.0f;

constexpr double SIZE_OF_CUBE = 10.0f;
constexpr double COODINATE_AXES = 30.0f;

constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 600;

constexpr int GRID_SIZE = 10;


void changeSize(int, int);
void renderScene(void);
void handleMouseOnClick(int button, int state, int x, int y);
void setUpRC();
void buildPopupMenu();


std::vector<std::pair<double, double>> selectedPoints;

const std::array<std::string, 2> algorithms = { "midpoint algorithm", "anti-aliasing algorithm" };

// Light values and coordinates
const std::array<GLfloat, 4> ENV_AMBIENT_COLOR = { 0.45f, 0.45f, 0.45f, 1.0f };
const std::array<GLfloat, 4> SOURCE_COLOR = { 0.25f, 0.25f, 0.25f, 1.0f };
const std::array<GLfloat, 4> LIGHT_POSITION = { 0.f, 25.0f, 20.0f, 0.0f };


int main(int argc, char** argv)
{
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

	glutMainLoop();	// http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2288.html
	return 0;
}

void handleMouseOnClick(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		// Convert window coordinate to global coordinate
		double&& mouseX = (2 * static_cast<double>(x) / WINDOW_WIDTH - 1) * GRID_SIZE;
		double&& mouseY = (1 - 2 * static_cast<double>(y) / WINDOW_HEIGHT) * GRID_SIZE;
		selectedPoints.push_back(std::make_pair(mouseX, mouseY));
		std::cout << "Mouse click: (" << mouseX << "," << mouseY << ")" << std::endl;
	}

	glutPostRedisplay();
}

void handleMenuOnSelect(int option)
{
	selectedPoints.clear();
	std::cout << "Change to use " << algorithms[option] << std::endl;
	glutPostRedisplay();
}

void buildPopupMenu()
{
	glutCreateMenu(handleMenuOnSelect);
	for (size_t i = 0; i < algorithms.size(); i++)
	{
		glutAddMenuEntry(algorithms[i].c_str(), i);
	}
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

void fillCells()
{
	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	glBegin(GL_QUADS);
	for (const auto& point : selectedPoints)
	{
		const double&& x = trunc(point.first);
		const double&& y = trunc(point.second);
		const int&& xSign = GET_SIGN(point.first);
		const int&& ySign = GET_SIGN(point.second);

		glVertex2d(x, y);
		glVertex2d(x, y + ySign);
		glVertex2d(x + xSign, y + ySign);
		glVertex2d(x + xSign, y);
	}
	glEnd();
}

void drawGrid()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(1.5f);
	glBegin(GL_LINES);
	for (int i = -GRID_SIZE; i <= GRID_SIZE; i++)
	{
		glVertex2d(-GRID_SIZE, i);
		glVertex2d(GRID_SIZE, i);
		glVertex2d(i, GRID_SIZE);
		glVertex2d(i, -GRID_SIZE);
	}
	glEnd();
}

void renderScene(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-GRID_SIZE, GRID_SIZE, -GRID_SIZE, GRID_SIZE, -10, 30);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 5.0f, 0, 0, 0, 0, 1, 0);

	fillCells();

	// Draw grid
	drawGrid();
	glutSwapBuffers();
}