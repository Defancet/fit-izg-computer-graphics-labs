#include "Student.h"

#include <osg/Plane>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

osg::Vec3d projectOnPlane(const osg::Vec3d& vector, const osg::Vec3d& planeNormal)
{
    auto pp = vector - planeNormal * (vector * planeNormal);
    //get rid of round-off error
    pp[0] = std::abs(pp[0]) < 1e-6 ? 0 : pp[0];
    pp[1] = std::abs(pp[1]) < 1e-6 ? 0 : pp[1];
    pp[2] = std::abs(pp[2]) < 1e-6 ? 0 : pp[2];

    return pp;
}

double dot(const osg::Vec3d& u, const osg::Vec3d& v)
{
    auto d = u * v;

    //handle rounding errors
    d = std::clamp(d, -1.0, 1.0);

    return d;
}

osg::Vec3d cross(const osg::Vec3d& u, const osg::Vec3d& v)
{
    return u ^ v;
}

void normalize(osg::Vec3d& v)
{
    v.normalize();
}

osg::Vec3d normalized(const osg::Vec3d& v)
{
    osg::Vec3d u(v);
    u.normalize();

    return u;
}

osg::Matrix inverse(const osg::Matrix& m)
{
    return osg::Matrix::inverse(m);
}

#if USE_STUDENT_SOLUTION
osg::Matrix getScaleMatrix(const osg::Vec3d& scale)
{
    //Task 1 - fill in the scale matrix

    osg::Matrix m ( scale[0], 0, 0, 0,
                    0, scale[1], 0, 0,
                    0, 0, scale[2], 0,
                    0, 0, 0, 1 );

    return m;
}

osg::Matrix getTranslationMatrix(const osg::Vec3d& translation)
{
    //Task 2 - fill in the translation matrix

    osg::Matrix m ( 1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    translation.x(), translation.y(), translation.z(), 1);

    return m;
}

osg::Matrix rotateAroundX(double angle)
{

    double sina = std::sin(angle);
    double cosa = std::cos(angle);

    //Task 3 - fill in the rotation matrix around X axis

    osg::Matrix m ( 1, 0, 0, 0,
        		    0, cosa, sina, 0,
        		    0, -sina, cosa, 0,
        		    0, 0, 0, 1 );

    return m;
}

osg::Matrix rotateAroundY(double angle)
{
    double sina = std::sin(angle);
    double cosa = std::cos(angle);

    //Task 4 - fill in the rotation matrix around Y axis

    osg::Matrix m ( cosa, 0, -sina, 0,
        			0, 1, 0, 0,
        			sina, 0, cosa, 0,
        			0, 0, 0, 1 );

    return m;
}

osg::Matrix rotateAroundZ(double angle)
{
    double sina = std::sin(angle);
    double cosa = std::cos(angle);

    //Task 5 - fill in the rotation matrix around Z axis

    osg::Matrix m ( cosa, sina, 0, 0,
        			-sina, cosa, 0, 0,
        			0, 0, 1, 0,
        			0, 0, 0, 1 );

    return m;
}

double angleBetweenVectors(osg::Vec3d u, osg::Vec3d v)
{
    if (u.length2() == 0 || v.length2() == 0)
    {
        return 0.0;
    }

    //Task 6
    //Normalize both vectors - method normalize()
    //Compute cosine of the angle between the vectors using the dot product - function dot
    //Return the angle using arcus cosine - function std::acos()

    normalize(u);
    normalize(v);

    auto cosAngle = dot(u, v);

    auto angle = std::acos(cosAngle);

    return angle;

}

osg::Matrix getRotationMatrix(const osg::Vec3d& fromVector, const osg::Vec3d& toVector)
{
    const osg::Vec3d xAxis(1, 0, 0);
    const osg::Vec3d yAxis(0, 1, 0);
    const osg::Vec3d zAxis(0, 0, 1);

    //Task 7 - compute the rotation matrix around arbitrary axis

    //Compute the angle between input vectors - function angleBetweenVectors

    auto angleBV = angleBetweenVectors(fromVector, toVector);

    //7a - compute the rotation axis using the cross product - function cross

    auto rotationAxis = cross(fromVector, toVector);

    //7b - project rotation axis into XY plane

    auto rotationAxisXY = projectOnPlane(rotationAxis, zAxis);

    //7c - compute the angle between projected rotation axis and X axis

    auto angleXY = angleBetweenVectors(rotationAxisXY, xAxis);

    //7d - compute rotation around Z axis

    auto rotationZ = rotateAroundZ(angleXY);

    //Compute projection of rotation axis onto XZ plane

    auto axisXZ = projectOnPlane(rotationAxis, yAxis);

    //7e - compute angle between (projection of) rotation axis in XZ and X axis and compute rotation matrix around Y corrently, hint - for Pi use M_PI

    auto angleXZ = angleBetweenVectors(axisXZ, xAxis);

    if (axisXZ[2] < 0)
    {
        angleXZ = 2 * M_PI - angleXZ;
    }
    
    auto rotationY = rotateAroundY(angleXZ);

    //7f - compute the desired rotation around X axis

    auto rotationX = rotateAroundX(angleBV);

    //7g - compute and return the final rotation - for matrix inverse use function inverse
    osg::Matrix result = rotationZ * rotationY * rotationX * inverse(rotationY) * inverse(rotationZ);

    return result;
}
#else

osg::Matrix getScaleMatrix(const osg::Vec3d& scale)
{
    return osg::Matrix::scale(scale);
}

osg::Matrix getTranslationMatrix(const osg::Vec3d& translation)
{
    return osg::Matrix::translate(translation);
}

osg::Matrix getRotationMatrix(const osg::Vec3d& fromVector, const osg::Vec3d& toVector)
{
    osg::Quat q;
    q.makeRotate(fromVector, toVector);

    return osg::Matrix(q);
}

osg::Matrix rotateAroundX(double angle)
{
    return {};
}

osg::Matrix rotateAroundY(double angle)
{
    return {};
}

osg::Matrix rotateAroundZ(double angle)
{
    return {};
}

double angleBetweenVectors(osg::Vec3d u, osg::Vec3d v)
{
    return 0.0;
}
#endif