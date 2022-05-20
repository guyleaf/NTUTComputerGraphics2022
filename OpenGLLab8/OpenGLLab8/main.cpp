#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <string>
#include <GL/freeglut.h>

#define X .525731112119133606f
#define Z .850650808352039932f


int depth;
double thetaX, thetaY;
int polygonMode;


constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

constexpr double ANGLES_FOR_ROTATION = 1.0;


const GLfloat vdata[12][3] = {
    {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
    {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
    {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

const GLuint tindices[20][3] = {
    {1,4,0}, {4,9,0}, {4,9,5}, {8,5,4}, {1,8,4},
    {1,10,8}, {10,3,8}, {8,3,5}, {3,2,5}, {3,7,2},
    {3,10,7}, {10,6,7}, {6,11,7}, {6,0,11}, {6,1,0},
    {10,1,6}, {11,0,9}, {2,11,9}, {5,2,9}, {11,2,7}
};

const std::array<std::string, 2> POLYGON_MODES = { "Line", "Fill" };

void renderScene();
void handleKeyboardEvent(unsigned char, int, int);
void handleSpecialKeyEvent(int, int, int);
void setUpRC();
void changeSize(int, int);
void buildPopupMenu();
void handleMenuOnSelect(int);

void drawFirstSphere();
void drawSecondSphere();
void drawThirdSphere();
void subdivide(GLfloat[], GLfloat[], GLfloat[], int);
void rotate();

void normalize(float[]);
void icoNormVec(int);


int main(int argc, char** argv)
{
    thetaX = thetaY = 0.0;
    depth = 10;
    polygonMode = 0;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("Subdividing polygon");

    setUpRC();
    buildPopupMenu();

    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(handleKeyboardEvent);
    glutSpecialFunc(handleSpecialKeyEvent);

    glutMainLoop(); // http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2288.html
    return 0;
}

/// <summary>
/// 渲染
/// </summary>
void renderScene()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    glTranslated(-2.0, 0.0, 0.0);
    rotate();
    drawFirstSphere();
    glPopMatrix();

    glPushMatrix();
    rotate();
    drawSecondSphere();
    glPopMatrix();

    glPushMatrix();
    glTranslated(2.0, 0.0, 0.0);
    rotate();
    drawThirdSphere();
    glPopMatrix();

    glutSwapBuffers();
}

/// <summary>
/// 處理鍵盤按鍵事件
/// </summary>
/// <param name="key"></param>
/// <param name=""></param>
/// <param name=""></param>
void handleKeyboardEvent(unsigned char key, int, int)
{
    if (key == '-')
    {
        depth--;
    }
    else if (key == '+')
    {
        depth++;
    }

    glutPostRedisplay();
}

void handleSpecialKeyEvent(int key, int, int)
{
    switch (key)
    {
        // x 負平移
    case GLUT_KEY_LEFT:
        thetaX -= ANGLES_FOR_ROTATION;
        break;
        // x 正平移
    case GLUT_KEY_RIGHT:
        thetaX += ANGLES_FOR_ROTATION;
        break;
        // y 正平移
    case GLUT_KEY_UP:
        thetaY += ANGLES_FOR_ROTATION;
        break;
        // y 負平移
    case GLUT_KEY_DOWN:
        thetaY -= ANGLES_FOR_ROTATION;
        break;
    default:
        break;
    }
    
    glutPostRedisplay();
}

/// <summary>
/// Setup Rendering Contex
/// </summary>
void setUpRC()
{
    // Setup and enable light 0
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_specular);

    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.6, 0.4, 1.0 };
    GLfloat mat_ambient[] = { 0.8, 0.6, 0.4, 1.0 };
    GLfloat mat_shininess = { 20.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    glShadeModel(GL_SMOOTH); /*enable smooth shading */
    glEnable(GL_LIGHTING); /* enable lighting */
    glEnable(GL_LIGHT0); /* enable light 0 */
}

void changeSize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -5.0, 10.0);
}

/// <summary>
/// 建構 menu
/// </summary>
void buildPopupMenu()
{
    glutCreateMenu(handleMenuOnSelect);
    int counter = 0;
    for (std::string item : POLYGON_MODES)
    {
        glutAddMenuEntry(item.c_str(), counter);
        counter++;
    }
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void handleMenuOnSelect(int option)
{
    polygonMode = option;
    std::cout << "Change polygon mode to " << POLYGON_MODES[option] << std::endl;
    glutPostRedisplay();
}

void drawFirstSphere()
{
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 20; i++)
    {
        icoNormVec(i);
        glVertex3fv(vdata[tindices[i][0]]);
        glVertex3fv(vdata[tindices[i][1]]);
        glVertex3fv(vdata[tindices[i][2]]);
    }
    glEnd();
}

void drawSecondSphere()
{
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 20; i++)
    {
        glNormal3fv(vdata[tindices[i][0]]);
        glVertex3fv(vdata[tindices[i][0]]);
        glNormal3fv(vdata[tindices[i][1]]);
        glVertex3fv(vdata[tindices[i][1]]);
        glNormal3fv(vdata[tindices[i][2]]);
        glVertex3fv(vdata[tindices[i][2]]);
    }
    glEnd();
}

void drawThirdSphere()
{
    GLfloat vertex1[3], vertex2[3], vertex3[3];

    for (int i = 0; i < 20; i++)
    {
        std::copy(std::begin(vdata[tindices[i][0]]), std::end(vdata[tindices[i][0]]), std::begin(vertex1));
        std::copy(std::begin(vdata[tindices[i][1]]), std::end(vdata[tindices[i][1]]), std::begin(vertex2));
        std::copy(std::begin(vdata[tindices[i][2]]), std::end(vdata[tindices[i][2]]), std::begin(vertex3));

        subdivide(vertex1, vertex2, vertex3, depth);
    }
    glFlush();
}

void subdivide(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], int depth)
{
    GLfloat v12[3], v23[3], v31[3];

    if (depth <= 0) {
        glBegin(GL_TRIANGLES); //Draw if no further subdivision requested
        glVertex3fv(v1);
        glVertex3fv(v2);
        glVertex3fv(v3);
        glEnd();
        return;
    }
    for (int i = 0; i < 3; i++) {
        v12[i] = (v1[i] + v2[i]) / 2.0f;
        v23[i] = (v2[i] + v3[i]) / 2.0f;
        v31[i] = (v3[i] + v1[i]) / 2.0f;
    }

    normalize(v12);
    normalize(v23);
    normalize(v31);
    subdivide(v1, v12, v31, depth - 1);
    subdivide(v2, v23, v12, depth - 1);
    subdivide(v3, v31, v23, depth - 1);
    subdivide(v12, v23, v31, depth - 1);
}

void rotate()
{
    glRotated(thetaX, 0.0, 1.0, 0.0);
    glRotated(-thetaX, 0.0, 1.0, 0.0);
    glRotated(thetaY, 1.0, 0.0, 0.0);
    glRotated(thetaX, 0.0, 1.0, 0.0);
}


void normalize(float v[])
{
    GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= d; v[1] /= d; v[2] /= d;
}

void normCrossProd(float u[], float v[], float out[])
{
    out[0] = u[1] * v[2] - u[2] * v[1];
    out[1] = u[2] * v[0] - u[0] * v[2];
    out[2] = u[0] * v[1] - u[1] * v[0];
    normalize(out);
}

void icoNormVec(int i)
{
    GLfloat d1[3], d2[3], n[3];

    for (int k = 0; k < 3; k++) {
        d1[k] = vdata[tindices[i][0]][k] - vdata[tindices[i][1]][k];
        d2[k] = vdata[tindices[i][1]][k] - vdata[tindices[i][2]][k];
    }
    normCrossProd(d1, d2, n);
    glNormal3fv(n);
}
