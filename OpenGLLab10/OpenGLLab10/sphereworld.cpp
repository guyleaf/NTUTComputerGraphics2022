// SphereWorld.cpp
// OpenGL SuperBible
// Demonstrates an immersive 3D environment using actors
// and a camera. This version adds lights and material properties
// and shadows.
// Program by Richard S. Wright Jr.

#include <cmath>
#include <random>
#include <memory>
#include <array>
#include <vector>
#include <string>
#include "libs/gltools.h"
#include "libs/math3d.hpp"

Math3D::M3DVector3d cameraPos{ 0.0, 0.0, 0.0 };

// Light and material Data
const Math3D::M3DVector4f fLightPos = {-100.0f, 100.0f, 50.0f, 1.0f}; // Point source
const Math3D::M3DVector4f fNoLight = {0.0f, 0.0f, 0.0f, 0.0f};
const Math3D::M3DVector4f fLowLight = {0.25f, 0.25f, 0.25f, 1.0f};
const Math3D::M3DVector4f fBrightLight = {1.0f, 1.0f, 1.0f, 1.0f};

Math3D::M3DMatrix44f mShadowMatrix;

constexpr size_t GROUND_TEXTURE = 0;
constexpr size_t TORUS_TEXTURE = 1;
constexpr size_t SPHERE_TEXTURE = 2;
constexpr size_t NUM_TEXTURES = 3;
std::array<GLuint, NUM_TEXTURES> textureObjects;

const std::vector<std::string> szTextureFiles = { "grass.tga", "wood.tga", "orb.tga" };

bool isAnimationPaused = false;
constexpr int animationSpeed = 33;

//////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
    const std::array<Math3D::M3DVector3f, 3> vPoints = { 
        Math3D::M3DVector3f{0.0f, -0.4f, 0.0f},
        Math3D::M3DVector3f{10.0f, -0.4f, 0.0f},
        Math3D::M3DVector3f{5.0f, -0.4f, -5.0f}};

    // Grayish background
    glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);

    // Clear stencil buffer with zero, increment by one whenever anybody
    // draws into it. When stencil function is enabled, only write where
    // stencil value is zero. This prevents the transparent shadow from drawing
    // over itself
    glStencilOp(GL_INCR, GL_INCR, GL_INCR);
    glClearStencil(0);
    glStencilFunc(GL_EQUAL, 0x0, 0x01);

    // Cull backs of polygons
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    // Setup light parameters
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight.data());
    glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight.data());
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight.data());
    glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight.data());
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Calculate shadow matrix
    Math3D::M3DVector4f pPlane = Math3D::getPlaneEquation(vPoints[0], vPoints[1], vPoints[2]);
    mShadowMatrix = Math3D::makePlanarShadowMatrix(pPlane, fLightPos);

    // Mostly use material tracking
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight.data());
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    // Set up texture maps
    glEnable(GL_TEXTURE_2D);
    glGenTextures(NUM_TEXTURES, textureObjects.data());
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    for (size_t i = 0; i < NUM_TEXTURES; i++)
    {
        GLint iWidth;
        GLint iHeight;
        GLint iComponents;
        GLenum eFormat;

        glBindTexture(GL_TEXTURE_2D, textureObjects[i]);

        // Load this texture map
        std::unique_ptr<GLbyte[]> pBytes = gltLoadTGA("resources/" + szTextureFiles[i], iWidth, iHeight, iComponents, eFormat);
        gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pBytes.get());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
}

////////////////////////////////////////////////////////////////////////
// Do shutdown for the rendering context
void ShutdownRC(void)
{
    // Delete the textures
    glDeleteTextures(NUM_TEXTURES, textureObjects.data());
}

///////////////////////////////////////////////////////////
// Draw the ground as a series of triangle strips
void DrawGround(void)
{
    const GLfloat fExtent = 20.0f;
    const GLfloat fStep = 1.0f;
    GLfloat y = -0.4f;
    GLfloat s = 0.0f;
    GLfloat t = 0.0f;
    GLfloat texStep = 1.0f / (fExtent * .075f);

    glBindTexture(GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLfloat iStrip = -fExtent;
    while (iStrip <= fExtent)
    {
        t = 0.0f;
        glBegin(GL_TRIANGLE_STRIP);

        GLfloat iRun = fExtent;
        while (iRun >= -fExtent)
        {
            glTexCoord2f(s, t);
            glNormal3f(0.0f, 1.0f, 0.0f); // All Point up
            glVertex3f(iStrip, y, iRun);

            glTexCoord2f(s + texStep, t);
            glNormal3f(0.0f, 1.0f, 0.0f); // All Point up
            glVertex3f(iStrip + fStep, y, iRun);

            t += texStep;
            iRun -= fStep;
        }

        glEnd();
        s += texStep;
        iStrip += fStep;
    }
}

void drawTorso()
{

}

void drawHead()
{

}

void drawUpperArm()
{

}

void drawLowerArm()
{

}

void drawHand()
{

}

void drawUpperLeg()
{

}

void drawLowerLeg()
{

}

void drawFoot()
{

}

void drawArm()
{
    // r = 0.03, H x W = 0.13 * 0.04
    drawUpperArm();

    glPushMatrix();
    glTranslatef(0.0f, -0.16f, 0.0f);
    // r = 0.03, H x W = 0.13 * 0.04
    drawLowerArm();

    glPushMatrix();
    glTranslatef(0.0f, -0.13f, 0.0f);
    drawHand();
    glPopMatrix();
    glPopMatrix();
}

void drawButtock()
{
    glColor3f(0.5f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glEnd();
}

void drawLeg()
{
    // r = 0.03, H x W = 0.13 * 0.04
    drawUpperLeg();

    glPushMatrix();
    glTranslatef(0.0f, -0.16f, 0.0f);
    // r = 0.03, H x W = 0.13 * 0.04
    drawLowerLeg();

    glPushMatrix();
    glTranslatef(0.0f, -0.13f, 0.0f);
    drawLeg();
    glPopMatrix();
    glPopMatrix();
}

void drawRobot(bool drawShadow)
{
    if (drawShadow)
    {
        glColor4f(0.0f, 0.0f, 0.0f, .6f); // Shadow color
    }
    else
    {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }

    glPushMatrix();

    glTranslatef(0.0f, 0.2f, 0.0f);
    // H x W = 0.2 * 0.1
    drawTorso();

    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.0f);
    // r = 0.05
    drawHead();
    glPopMatrix();

    // right arm
    glPushMatrix();
    glTranslatef(-0.08f, 0.0f, 0.0f);
    drawArm();
    glPopMatrix();

    // left arm
    glPushMatrix();
    glTranslatef(0.08f, 0.0f, 0.0f);
    drawArm();
    glPopMatrix();

    glPopMatrix();

    glPushMatrix();
    drawButtock();

    // right leg
    glPushMatrix();
    glTranslatef(-0.03f, 0.0f, 0.0f);
    drawLeg();
    glPopMatrix();

    // left leg
    glPushMatrix();
    glTranslatef(0.03f, 0.0f, 0.0f);
    drawLeg();
    glPopMatrix();
    glPopMatrix();
}

// Called to draw scene
void RenderScene(void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glPushMatrix();

    gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], cameraPos[0], cameraPos[1], cameraPos[2] - 1.0, 0.0, 1.0, 0.0);

    // Position light before any other transformations
    glLightfv(GL_LIGHT0, GL_POSITION, fLightPos.data());

    // Draw the ground
    glColor3f(1.0f, 1.0f, 1.0f);
    DrawGround();

    // Draw the shadow of robot first
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_STENCIL_TEST);

    glPushMatrix();
    glMultMatrixf(mShadowMatrix.data());
    drawRobot(true);
    glPopMatrix();

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    // Draw the robot
    drawRobot(false);

    glPopMatrix();

    // Do the buffer Swap
    glutSwapBuffers();
}

// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(int key, int x, int y)
{
    //if (key == GLUT_KEY_UP)
    //    frameCamera.MoveForward(0.1f);

    //if (key == GLUT_KEY_DOWN)
    //    frameCamera.MoveForward(-0.1f);

    //if (key == GLUT_KEY_LEFT)
    //    frameCamera.RotateLocalY(0.1f);

    //if (key == GLUT_KEY_RIGHT)
    //    frameCamera.RotateLocalY(-0.1f);

    // Refresh the Window
    //glutPostRedisplay();
}

///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)
void TimerFunction(int)
{
    if (isAnimationPaused)
    {
        return;
    }

    // Redraw the scene with new coordinates
    glutPostRedisplay();
    glutTimerFunc(animationSpeed, TimerFunction, 1);
}

void HandleKeyboardEvent(unsigned char key, int, int)
{
    if (key == 'p' || key == 'P')
    {
        isAnimationPaused = !isAnimationPaused;
        glutTimerFunc(animationSpeed, TimerFunction, 1);
    }
}

void ChangeSize(int w, int h)
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
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL SphereWorld Demo + Texture Maps");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(HandleKeyboardEvent);

    SetupRC();
    glutTimerFunc(animationSpeed, TimerFunction, 1);

    glutMainLoop();

    ShutdownRC();

    return 0;
}
