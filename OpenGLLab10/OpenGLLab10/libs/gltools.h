#pragma once

#ifndef __GLTOOLS_H_
#define __GLTOOLS_H_

#include <memory>
#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// Draw a Torus
void gltDrawTorus(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor);

// Just draw a simple sphere with normals and texture coordinates
void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks);

// Load a .TGA file
std::unique_ptr<GLbyte[]> gltLoadTGA(const std::string& szFileName, GLint& iWidth, GLint& iHeight, GLint& iComponents, GLenum& eFormat);

#endif