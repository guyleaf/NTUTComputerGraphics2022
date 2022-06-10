#include "math3d.hpp"

namespace Math3D
{
    // Get plane equation from three points
    M3DVector4f getPlaneEquation(const M3DVector3f &point1, const M3DVector3f &point2, const M3DVector3f &point3)
    {
        // Get two vectors... do the cross product
        M3DVector3f v1{};
        M3DVector3f v2{};

        // V1 = p3 - p1
        v1[0] = point3[0] - point1[0];
        v1[1] = point3[1] - point1[1];
        v1[2] = point3[2] - point1[2];

        // V2 = P2 - p1
        v2[0] = point2[0] - point1[0];
        v2[1] = point2[1] - point1[1];
        v2[2] = point2[2] - point1[2];

        // Unit normal to plane - Not sure which is the best way here
        auto n = crossProduct(v1, v2);
        normalizeVector(n);

        M3DVector4f planeEq{n[0], n[1], n[2]};
        // Back substitute to get D
        planeEq[3] = -(planeEq[0] * point3[0] + planeEq[1] * point3[1] + planeEq[2] * point3[2]);

        return planeEq;
    }

    M3DVector4d getPlaneEquation(const M3DVector3d &point1, const M3DVector3d &point2, const M3DVector3d &point3)
    {
        // Get two vectors... do the cross product
        M3DVector3d v1{};
        M3DVector3d v2{};

        // V1 = p3 - p1
        v1[0] = point3[0] - point1[0];
        v1[1] = point3[1] - point1[1];
        v1[2] = point3[2] - point1[2];

        // V2 = P2 - p1
        v2[0] = point2[0] - point1[0];
        v2[1] = point2[1] - point1[1];
        v2[2] = point2[2] - point1[2];

        // Unit normal to plane - Not sure which is the best way here
        auto n = crossProduct(v1, v2);
        normalizeVector(n);

        M3DVector4d planeEq{n[0], n[1], n[2]};
        // Back substitute to get D
        planeEq[3] = -(planeEq[0] * point3[0] + planeEq[1] * point3[1] + planeEq[2] * point3[2]);

        return planeEq;
    }

    // Planar shadow Matrix
    M3DMatrix44f makePlanarShadowMatrix(const M3DVector4f &planeEq, const M3DVector4f &vLightPos)
    {
        // These just make the code below easier to read. They will be
        // removed by the optimizer.
        float a = planeEq[0];
        float b = planeEq[1];
        float c = planeEq[2];
        float d = planeEq[3];

        // light vector from air to plane, so it need to be reversed
        float dx = -vLightPos[0];
        float dy = -vLightPos[1];
        float dz = -vLightPos[2];

        M3DMatrix44f proj{};
        // Now build the planar projection matrix
        // dot - light_pos[0] * plane[0] == plane[1] * light_pos[1] + plane[2] * light_pos[2]
        proj[0] = b * dy + c * dz;
        proj[1] = -a * dy;
        proj[2] = -a * dz;
        proj[3] = 0.0;

        proj[4] = -b * dx;
        proj[5] = a * dx + c * dz;
        proj[6] = -b * dz;
        proj[7] = 0.0;

        proj[8] = -c * dx;
        proj[9] = -c * dy;
        proj[10] = a * dx + b * dy;
        proj[11] = 0.0;

        proj[12] = -d * dx;
        proj[13] = -d * dy;
        proj[14] = -d * dz;
        proj[15] = a * dx + b * dy + c * dz;
        // Shadow matrix ready

        return proj;
    }

    M3DMatrix44d makePlanarShadowMatrix(const M3DVector4d &planeEq, const M3DVector4d &vLightPos)
    {
        // These just make the code below easier to read. They will be
        // removed by the optimizer.
        double a = planeEq[0];
        double b = planeEq[1];
        double c = planeEq[2];
        double d = planeEq[3];

        // light vector from air to plane, so it need to be reversed
        double dx = -vLightPos[0];
        double dy = -vLightPos[1];
        double dz = -vLightPos[2];

        M3DMatrix44d proj{};
        // Now build the projection matrix
        proj[0] = b * dy + c * dz;
        proj[1] = -a * dy;
        proj[2] = -a * dz;
        proj[3] = 0.0;

        proj[4] = -b * dx;
        proj[5] = a * dx + c * dz;
        proj[6] = -b * dz;
        proj[7] = 0.0;

        proj[8] = -c * dx;
        proj[9] = -c * dy;
        proj[10] = a * dx + b * dy;
        proj[11] = 0.0;

        proj[12] = -d * dx;
        proj[13] = -d * dy;
        proj[14] = -d * dz;
        proj[15] = a * dx + b * dy + c * dz;
        // Shadow matrix ready

        return proj;
    }

    // Calculates the normal of a triangle specified by the three points
    // p1, p2, and p3. Each pointer points to an array of three floats. The
    // triangle is assumed to be wound counter clockwise.
    M3DVector3f findNormal(const M3DVector3f &point1, const M3DVector3f &point2, const M3DVector3f &point3)
    {
        M3DVector3f v1{};
        M3DVector3f v2{}; // Temporary vectors

        // V1 = p1 - p2
        v1[0] = point1[0] - point2[0];
        v1[1] = point1[1] - point2[1];
        v1[2] = point1[2] - point2[2];

        // V2 = P2 - p3
        v2[0] = point2[0] - point3[0];
        v2[1] = point2[1] - point3[1];
        v2[2] = point2[2] - point3[2];

        // Take the cross product of the two vectors to get
        // the normal vector.
        return crossProduct(v1, v2);
    }

    M3DVector3d findNormal(const M3DVector3d &point1, const M3DVector3d &point2, const M3DVector3d &point3)
    {
        M3DVector3d v1{};
        M3DVector3d v2{}; // Temporary vectors

        // V1 = p1 - p2
        v1[0] = point1[0] - point2[0];
        v1[1] = point1[1] - point2[1];
        v1[2] = point1[2] - point2[2];

        // V2 = P2 - p3
        v2[0] = point2[0] - point3[0];
        v2[1] = point2[1] - point3[1];
        v2[2] = point2[2] - point3[2];

        // Take the cross product of the two vectors to get
        // the normal vector.
        return crossProduct(v1, v2);
    }
}