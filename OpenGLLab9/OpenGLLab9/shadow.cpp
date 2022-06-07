// Shadow.cpp
// OpenGL SuperBible
// Demonstrates simple planar shadows
// Program by Richard S. Wright Jr.

#include <string>
#include <array>

#include "GL/freeglut.h"
#include "math3d.h"


// Rotation amounts
GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;

// These values need to be available globally
// Light values and coordinates
const math3d::M3DVector4f ambientLight = {0.3f, 0.3f, 0.3f, 1.0f};
const math3d::M3DVector4f diffuseLight = {0.7f, 0.7f, 0.7f, 1.0f};
const math3d::M3DVector4f specular = {1.0f, 1.0f, 1.0f, 1.0f};

const std::array<math3d::M3DVector4f, 4> lightPositions = { 
    math3d::M3DVector4f{-75.0f, 150.0f, -50.0f, 0.0f},
    math3d::M3DVector4f{-75.0f, 100.0f, -50.0f, 0.0f},
    math3d::M3DVector4f{-75.0f, 150.0f, 50.0f, 0.0f},
    math3d::M3DVector4f{75.0f, 150.0f, -50.0f, 0.0f} };

const math3d::M3DVector4f specref = {1.0f, 1.0f, 1.0f, 1.0f};

size_t currentLightIndex;
// Transformation matrix to project shadow
math3d::M3DMatrix44f shadowMat;
math3d::M3DVector4f vPlaneEquation;

////////////////////////////////////////////////
// This function just specifically draws the jet
void DrawJet(bool shadow)
{
    math3d::M3DVector3f vNormal; // Storeage for calculated surface normal

    // Nose Cone /////////////////////////////
    // Set material color, note we only have to set to black
    // for the shadow once
    if (shadow)
        glColor3ub(102, 0, 0);
    else
        glColor3ub(255, 0, 0);

    // Nose Cone - Points straight down
    // Set material color
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 60.0f);
    glVertex3f(-15.0f, 0.0f, 30.0f);
    glVertex3f(15.0f, 0.0f, 30.0f);

    // Verticies for this panel
    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{15.0f, 0.0f, 30.0f},
                                  math3d::M3DVector3f{0.0f, 15.0f, 30.0f},
                                  math3d::M3DVector3f{0.0f, 0.0f, 60.0f}};

        // Calculate the normal for the plane
        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{0.0f, 0.0f, 60.0f},
                                  math3d::M3DVector3f{0.0f, 15.0f, 30.0f},
                                  math3d::M3DVector3f{-15.0f, 0.0f, 30.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    // Body of the Plane ////////////////////////
    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{-15.0f, 0.0f, 30.0f},
                                  math3d::M3DVector3f{0.0f, 15.0f, 30.0f},
                                  math3d::M3DVector3f{0.0f, 0.0f, -56.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{0.0f, 0.0f, -56.0f},
                                  math3d::M3DVector3f{0.0f, 15.0f, 30.0f},
                                  math3d::M3DVector3f{15.0f, 0.0f, 30.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(15.0f, 0.0f, 30.0f);
    glVertex3f(-15.0f, 0.0f, 30.0f);
    glVertex3f(0.0f, 0.0f, -56.0f);

    ///////////////////////////////////////////////
    // Left wing
    // Large triangle for bottom of wing
    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{0.0f, 2.0f, 27.0f},
                                  math3d::M3DVector3f{-60.0f, 2.0f, -8.0f},
                                  math3d::M3DVector3f{60.0f, 2.0f, -8.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{60.0f, 2.0f, -8.0f},
                                  math3d::M3DVector3f{0.0f, 7.0f, -8.0f},
                                  math3d::M3DVector3f{0.0f, 2.0f, 27.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{60.0f, 2.0f, -8.0f},
                                  math3d::M3DVector3f{-60.0f, 2.0f, -8.0f},
                                  math3d::M3DVector3f{0.0f, 7.0f, -8.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{0.0f, 2.0f, 27.0f},
                                  math3d::M3DVector3f{0.0f, 7.0f, -8.0f},
                                  math3d::M3DVector3f{-60.0f, 2.0f, -8.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    // Tail section///////////////////////////////
    // Bottom of back fin
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-30.0f, -0.50f, -57.0f);
    glVertex3f(30.0f, -0.50f, -57.0f);
    glVertex3f(0.0f, -0.50f, -40.0f);

    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{0.0f, -0.5f, -40.0f},
                                  math3d::M3DVector3f{30.0f, -0.5f, -57.0f},
                                  math3d::M3DVector3f{0.0f, 4.0f, -57.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{0.0f, 4.0f, -57.0f},
                                  math3d::M3DVector3f{-30.0f, -0.5f, -57.0f},
                                  math3d::M3DVector3f{0.0f, -0.5f, -40.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{30.0f, -0.5f, -57.0f},
                                  math3d::M3DVector3f{-30.0f, -0.5f, -57.0f},
                                  math3d::M3DVector3f{0.0f, 4.0f, -57.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{0.0f, 0.5f, -40.0f},
                                  math3d::M3DVector3f{3.0f, 0.5f, -57.0f},
                                  math3d::M3DVector3f{0.0f, 25.0f, -65.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{0.0f, 25.0f, -65.0f},
                                  math3d::M3DVector3f{-3.0f, 0.5f, -57.0f},
                                  math3d::M3DVector3f{0.0f, 0.5f, -40.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    {
        const std::array<math3d::M3DVector3f, 3> vPoints = { math3d::M3DVector3f{3.0f, 0.5f, -57.0f},
                                  math3d::M3DVector3f{-3.0f, 0.5f, -57.0f},
                                  math3d::M3DVector3f{0.0f, 25.0f, -65.0f}};

        vNormal = math3d::findNormal(vPoints[0], vPoints[1], vPoints[2]);
        glNormal3fv(vNormal.data());
        glVertex3fv(vPoints[0].data());
        glVertex3fv(vPoints[1].data());
        glVertex3fv(vPoints[2].data());
    }

    glEnd();
}

// Called to draw scene
void RenderScene(void)
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the ground, we do manual shading to a darker green
    // in the background to give the illusion of depth
    glBegin(GL_QUADS);
    glColor3ub(0, 32, 0); // light green ground
    glVertex3f(400.0f, -150.0f, -200.0f);
    glVertex3f(-400.0f, -150.0f, -200.0f);
    glColor3ub(0, 255, 0); // make it in green gradient
    glVertex3f(-400.0f, -150.0f, 200.0f);
    glVertex3f(400.0f, -150.0f, 200.0f);
    glEnd();

    // Save the matrix state and do the rotations
    glPushMatrix();

    // Draw jet at new orientation, put light in correct position
    // before rotating the jet
    glEnable(GL_LIGHTING);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    DrawJet(false);

    // Restore original matrix state
    glPopMatrix();

    // Get ready to draw the shadow and the ground
    // First disable lighting and save the projection state
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glPushMatrix();

    // Multiply by shadow projection matrix
    glMultMatrixf(shadowMat.data());

    // Now rotate the jet around in the new flattend space
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    // Pass true to indicate drawing shadow
    DrawJet(true);

    // Restore the projection to normal
    glPopMatrix();

    // Draw the light source
    glPushMatrix();
    glTranslatef(lightPositions[currentLightIndex][0], lightPositions[currentLightIndex][1], lightPositions[currentLightIndex][2]);
    glColor3ub(255, 255, 0);
    glutSolidSphere(5.0f, 10, 10);
    glPopMatrix();

    // Restore lighting state variables
    glEnable(GL_DEPTH_TEST);

    // Display the results
    glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
    // Any three points on the ground (counter clockwise order)
    const std::array<math3d::M3DVector3f, 3> points = { math3d::M3DVector3f{-30.0f, -149.0f, -20.0f},
                             math3d::M3DVector3f{-30.0f, -149.0f, 20.0f},
                             math3d::M3DVector3f{40.0f, -149.0f, 20.0f}};

    glEnable(GL_DEPTH_TEST); // Hidden surface removal
    glFrontFace(GL_CCW);     // Counter clock-wise polygons face out
    glEnable(GL_CULL_FACE);  // Do not calculate inside of jet

    // Setup and enable light 0
    for (size_t i = 0; i < lightPositions.size(); i++)
    {
        glLightfv(GL_LIGHT0 + i, GL_AMBIENT, ambientLight.data());
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diffuseLight.data());
        glLightfv(GL_LIGHT0 + i, GL_SPECULAR, specular.data());
        glLightfv(GL_LIGHT0 + i, GL_POSITION, lightPositions[i].data());
    }

    glEnable(GL_LIGHT0);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // All materials hereafter have full specular reflectivity
    // with a high shine
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref.data());
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    // Light blue background
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    // Get the plane equation from three points on the ground
    vPlaneEquation = math3d::getPlaneEquation(points[0], points[1], points[2]);

    // Calculate projection matrix to draw shadow on the ground
    shadowMat = math3d::makePlanarShadowMatrix(vPlaneEquation, lightPositions[currentLightIndex]);

    glEnable(GL_NORMALIZE);
}

void SpecialKeys(int key, int, int)
{
    if (key == GLUT_KEY_UP)
        xRot -= 5.0f;

    if (key == GLUT_KEY_DOWN)
        xRot += 5.0f;

    if (key == GLUT_KEY_LEFT)
        yRot -= 5.0f;

    if (key == GLUT_KEY_RIGHT)
        yRot += 5.0f;

    if (xRot > 356.0f)
        xRot = 0.0f;

    if (xRot < -1.0f)
        xRot = 355.0f;

    if (yRot > 356.0f)
        yRot = 0.0f;

    if (yRot < -1.0f)
        yRot = 355.0f;

    // Refresh the Window
    glutPostRedisplay();
}

void KeyboardKey(unsigned char key, int, int)
{
    if (key == 'r')
    {
        xRot = 0.0f;
        yRot = 0.0f;
    }

    // Refresh the Window
    glutPostRedisplay();
}

void handleMenuOnSelect(int option)
{
    glDisable(GL_LIGHT0 + currentLightIndex);
    currentLightIndex = option;
    glEnable(GL_LIGHT0 + currentLightIndex);
    // Calculate projection matrix to draw shadow on the ground
    shadowMat = math3d::makePlanarShadowMatrix(vPlaneEquation, lightPositions[currentLightIndex]);

    // Refresh the Window
    glutPostRedisplay();
}

/// <summary>
/// «Øºc menu
/// </summary>
void buildPopupMenu()
{
    glutCreateMenu(handleMenuOnSelect);
    int counter = 0;
    for (size_t i = 0; i < lightPositions.size(); i++, counter++)
    {
        glutAddMenuEntry(("Light " + std::to_string(counter)).c_str(), counter);
    }

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void ChangeSize(int w, int h)
{
    GLfloat fAspect;

    // Prevent a divide by zero
    if (h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    fAspect = (GLfloat)w / (GLfloat)h;
    gluPerspective(60.0f, fAspect, 200.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Move out Z axis so we can see everything
    glTranslatef(0.0f, 0.0f, -400.0f);
}

int main(int argc, char *argv[])
{
    currentLightIndex = 0;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Shadow");
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(KeyboardKey);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(RenderScene);
    buildPopupMenu();
    SetupRC();
    glutMainLoop();

    return 0;
}
