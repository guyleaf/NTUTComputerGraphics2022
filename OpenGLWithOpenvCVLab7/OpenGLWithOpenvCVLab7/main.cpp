// Block.cpp
// OpenGL SuperBible, Chapter 1
// Demonstrates an assortment of basic 3D concepts
// Program by Richard S. Wright Jr.

#include <iostream>
#include <vector>

#include <GL/freeglut.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

constexpr float CLIP_SIZE = 100.0f;

// Calculate new clipping volume
GLfloat windowWidth;
GLfloat windowHeight;

// Keep track of effects step
int nStep = 0;

// Lighting data
GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat lightDiffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat lightSpecular[] = {0.9f, 0.9f, 0.9f};
GLfloat materialColor[] = {0.8f, 0.0f, 0.0f};
GLfloat vLightPos[] = {00.0f, 120.0f, 100.0f, 0.0f};
GLfloat ground[3][3] = {{0.0f, -25.0f, 0.0f},
                        {10.0f, -25.0f, 0.0f},
                        {10.0f, -25.0f, -10.0f}};

GLuint textures[4];


const std::string IMAGES_PATH = "resources";
const std::vector<std::string> IMAGES = { "floor.jpg", "Block6.jpg", "Block4.jpg", "Block5.jpg" };


// Called to draw scene
void RenderScene(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0f, 3.0f, 5.0f, 0, 0, 0, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, vLightPos);

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    // Keep shape centered
    glTranslatef((windowWidth - CLIP_SIZE) / 2.0f, 0.0f, 0.0f);

    glPushMatrix();

    // Draw plane that the cube rests on
    glDisable(GL_LIGHTING);

    glColor3ub(255, 255, 255);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-100.0f, -25.3f, -100.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-100.0f, -25.3f, 100.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(100.0f, -25.3f, 100.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(100.0f, -25.3f, -100.0f);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, lightSpecular);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    // Move the cube slightly forward and to the left
    glTranslatef(0.0f, 0.0f, 10.0f);
    glRotatef(-50.0f, 0.0f, 1.0f, 0.0f);

    glColor3ub(255, 255, 255);

    // Front Face (before rotation)
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(25.0f, 25.0f, 25.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(25.0f, -25.0f, 25.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-25.0f, -25.0f, 25.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-25.0f, 25.0f, 25.0f);
    glEnd();

    // Top of cube
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(25.0f, 25.0f, 25.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(25.0f, 25.0f, -25.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-25.0f, 25.0f, -25.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-25.0f, 25.0f, 25.0f);
    glEnd();

    // Last two segments for effect
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(25.0f, 25.0f, -25.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(25.0f, -25.0f, -25.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(25.0f, -25.0f, 25.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(25.0f, 25.0f, 25.0f);
    glEnd();

    glPopMatrix();

    // Flush drawing commands
    glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
    GLint nComponents = GL_RGB8;
    GLenum format = GL_BGR_EXT;

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glGenTextures(static_cast<int>(IMAGES.size()), textures);

    for (size_t i = 0; i < IMAGES.size(); i++)
    {
        // Load the texture objects
        cv::Mat &&image = cv::imread(IMAGES_PATH + "\\" + IMAGES[i], CV_LOAD_IMAGE_COLOR);
        // cv::flip(image, image, 0);
        const GLint &&nWidth = image.size().width;
        const GLint &&nHeight = image.size().height;
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
            format, GL_UNSIGNED_BYTE, image.data);
    }
}

void ChangeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;

    // Keep the square square
    if (w <= h)
    {
        windowHeight = CLIP_SIZE * (GLfloat)h / (GLfloat)w;
        windowWidth = CLIP_SIZE;
    }
    else
    {
        windowWidth = CLIP_SIZE * (GLfloat)w / (GLfloat)h;
        windowHeight = CLIP_SIZE;
    }

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the clipping volume
    glOrtho(-100.0f, windowWidth, -100.0f, windowHeight, -200.0f, 200.0f);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Effects Demo");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);

    SetupRC();

    glutMainLoop();
    glDeleteTextures(4, textures);
    return 0;
}
