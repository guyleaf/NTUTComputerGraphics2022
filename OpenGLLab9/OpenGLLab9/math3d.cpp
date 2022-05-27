#include "math3d.h"

namespace math3d
{
    // Get plane equation from three points
    M3DVector4f getPlaneEquation(const M3DVector3f& point1, const M3DVector3f& point2, const M3DVector3f& point3)
    {
        // Get two vectors... do the cross product
        M3DVector3f v1 = point3 - point1;
        M3DVector3f v2;

        // V1 = point3 - point1
        v1[0] = point3[0] - point1[0];
        v1[1] = point3[1] - point1[1];
        v1[2] = point3[2] - point1[2];

        // V2 = P2 - p1
        v2[0] = p2[0] - p1[0];
        v2[1] = p2[1] - p1[1];
        v2[2] = p2[2] - p1[2];

        // Unit normal to plane - Not sure which is the best way here
        crossProduct(planeEq, v1, v2);
        m3dNormalizeVector(planeEq);
        // Back substitute to get D
        planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
    }

    M3DVector4d getPlaneEquation(const M3DVector3d& point1, const M3DVector3d& point2, const M3DVector3d& point3)
    {

    }
}