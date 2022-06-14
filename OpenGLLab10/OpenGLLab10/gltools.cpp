#include "libs/gltools.h"
#include "libs/math3d.hpp"

#include <iostream>
#include <fstream>
#include <utility>


void gltDrawTorus(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor)
{
    Math3D::M3DVector3f vNormal{};
    GLfloat majorStep = 2.0f * (GLfloat)Math3D::M3D_PI / (GLfloat)numMajor;
    GLfloat minorStep = 2.0f * (GLfloat)Math3D::M3D_PI / (GLfloat)numMinor;

    for (int i = 0; i < numMajor; ++i)
    {
        GLfloat&& a0 = (GLfloat)i * majorStep;
        GLfloat&& a1 = a0 + majorStep;
        GLfloat x0 = std::cosf(a0);
        GLfloat y0 = std::sinf(a0);
        GLfloat x1 = std::cosf(a1);
        GLfloat y1 = std::sinf(a1);

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= numMinor; ++j)
        {
            GLfloat&& b = (GLfloat)j * minorStep;
            GLfloat&& c = std::cosf(b);
            GLfloat&& r = minorRadius * c + majorRadius;
            GLfloat&& z = minorRadius * std::sinf(b);

            // First point
            glTexCoord2f((GLfloat)(i) / (GLfloat)(numMajor), (GLfloat)(j) / (GLfloat)(numMinor));
            vNormal[0] = x0 * c;
            vNormal[1] = y0 * c;
            vNormal[2] = z / minorRadius;
            Math3D::normalizeVector(vNormal);
            glNormal3fv(vNormal.data());
            glVertex3f(x0 * r, y0 * r, z);

            glTexCoord2f((GLfloat)(i + 1) / (GLfloat)(numMajor), (GLfloat)(j) / (GLfloat)(numMinor));
            vNormal[0] = x1 * c;
            vNormal[1] = y1 * c;
            vNormal[2] = z / minorRadius;
            Math3D::normalizeVector(vNormal);
            glNormal3fv(vNormal.data());
            glVertex3f(x1 * r, y1 * r, z);
        }
        glEnd();
    }
}

void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks)
{
    GLfloat drho = (GLfloat)Math3D::M3D_PI / (GLfloat)iStacks;
    GLfloat dtheta = 2.0f * (GLfloat)Math3D::M3D_PI / (GLfloat)iSlices;
    GLfloat ds = 1.0f / (GLfloat)iSlices;
    GLfloat dt = 1.0f / (GLfloat)iStacks;
    GLfloat t = 1.0f;
    GLfloat s = 0.0f;

    for (int i = 0; i < iStacks; i++)
    {
        GLfloat rho = (GLfloat)i * drho;
        GLfloat srho = std::sinf(rho);
        GLfloat crho = std::cosf(rho);
        GLfloat srhodrho = std::sinf(rho + drho);
        GLfloat crhodrho = std::cosf(rho + drho);

        // Many sources of OpenGL sphere drawing code uses a triangle fan
        // for the caps of the sphere. This however introduces texturing 
        // artifacts at the poles on some OpenGL implementations
        glBegin(GL_TRIANGLE_STRIP);
        s = 0.0f;
        for (int j = 0; j <= iSlices; j++)
        {
            GLfloat theta = (j == iSlices) ? 0.0f : (GLfloat)j * dtheta;
            GLfloat stheta = -std::sin(theta);
            GLfloat ctheta = std::cos(theta);

            GLfloat x = stheta * srho;
            GLfloat y = ctheta * srho;
            GLfloat z = crho;

            glTexCoord2f(s, t);
            glNormal3f(x, y, z);
            glVertex3f(x * fRadius, y * fRadius, z * fRadius);

            x = stheta * srhodrho;
            y = ctheta * srhodrho;
            z = crhodrho;
            glTexCoord2f(s, t - dt);
            s += ds;
            glNormal3f(x, y, z);
            glVertex3f(x * fRadius, y * fRadius, z * fRadius);
        }
        glEnd();

        t -= dt;
    }
}

// Define targa header. This is only used locally.
#pragma pack(1)
using TGAHEADER = struct
{
    GLbyte	identsize;              // Size of ID field that follows header (0)
    GLbyte	colorMapType;           // 0 = None, 1 = paletted
    GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short	colorMapStart;          // First colour map entry
    unsigned short	colorMapLength;         // Number of colors
    unsigned char 	colorMapBits;   // bits per palette entry
    unsigned short	xstart;                 // image x origin
    unsigned short	ystart;                 // image y origin
    unsigned short	width;                  // width in pixels
    unsigned short	height;                 // height in pixels
    GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
    GLbyte	descriptor;             // image descriptor
};
#pragma pack(8)

// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
std::unique_ptr<GLbyte[]> gltLoadTGA(const std::string& szFileName, GLint& iWidth, GLint& iHeight, GLint& iComponents, GLenum& eFormat)
{
    std::fstream file;
    TGAHEADER tgaHeader{};		// TGA file header
    unsigned long lImageSize;		// Size in bytes of image
    short sDepth;			// Pixel depth

    // Default/Failed values
    iWidth = 0;
    iHeight = 0;
    eFormat = GL_BGR_EXT;
    iComponents = GL_RGB8;

    // Attempt to open the file
    file.open(szFileName.c_str(), std::ios::in | std::ios::binary);
    if (!file)
    {
        std::cout << "File not found" << std::endl;
        return nullptr;
    }

    // Read in header (binary)
    file.read((char*)&tgaHeader, 18/* sizeof(TGAHEADER)*/);
    if (!file)
    {
        file.close();
        std::cout << "Failed to read TGA header" << std::endl;
        return nullptr;
    }

    // Do byte swap for big vs little endian
#ifdef __APPLE__
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
    LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
    LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
    LITTLE_ENDIAN_WORD(&tgaHeader.width);
    LITTLE_ENDIAN_WORD(&tgaHeader.height);
#endif

    // Get width, height, and depth of texture
    iWidth = tgaHeader.width;
    iHeight = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;

    // Put some validity checks here. Very simply, I only understand
    // or care about 8, 24, or 32 bit targa's.
    if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        return nullptr;

    // Calculate size of image buffer
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

    // Allocate memory and check for success
    auto pBits = std::make_unique<GLbyte[]>(lImageSize * sizeof(GLbyte));

    // Read in the bits
    // Check for read error. This should catch RLE or other 
    // weird formats that I don't want to recognize
    file.read((char*)(pBits.get()), lImageSize);
    if (!file)
    {
        file.close();
        std::cout << "Failed to read texture" << std::endl;
        return nullptr;
    }

    // Set OpenGL format expected
    switch (sDepth)
    {
    case 3:     // Most likely case
        eFormat = GL_BGR_EXT;
        iComponents = GL_RGB8;
        break;
    case 4:
        eFormat = GL_BGRA_EXT;
        iComponents = GL_RGBA8;
        break;
    case 1:
        eFormat = GL_LUMINANCE;
        iComponents = GL_LUMINANCE8;
        break;
    }

    // Done with File
    file.close();

    // Return pointer to image data
    return pBits;
}