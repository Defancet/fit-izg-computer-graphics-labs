#pragma once

#include <osg/Matrix>
#include <osg/Vec3>
 
#define USE_STUDENT_SOLUTION    1

/*------------------------------HELPER FUNCTIONS-------------------------*/

//! \brief                  Computes projection of a point on a plane with given normal.
//! \param  vector          Vector to project.
//! \param  planeNormal     Normal of the plane.
osg::Vec3d projectOnPlane(const osg::Vec3d& vector, const osg::Vec3d& planeNormal);

//! \brief                  Computes dot (inner) product.
//! \param  u               First input vector.
//! \param  v               Second input vector.
double dot(const osg::Vec3d& u, const osg::Vec3d& v);

//! \brief                  Computes cross (outer) product.
//! \param  u               First input vector.
//! \param  v               Second input vector.
osg::Vec3d cross(const osg::Vec3d& u, const osg::Vec3d& v);

//! \brief                  Normalizes given vector.
//! \param  v               Input vector.
void normalize(osg::Vec3d& v);

//! \brief                  Returns normalized vector.
//! \param  v               Input vector.
osg::Vec3d normalized(const osg::Vec3d& v);

//! \brief                  Returns inversed matrix.
//! \param  m               Matrix to be inversed.
osg::Matrix inverse(const osg::Matrix& m);

/*------------------------------STUDENT TASKS-------------------------*/

//! \brief                  Task 1: Return scale matrix.
//! \param  scale           Vector of scale in each direcion.
osg::Matrix getScaleMatrix(const osg::Vec3d& scale);

//! \brief                  Task 2: Return translation matrix.
//! \param  translation     Vector of translation.
osg::Matrix getTranslationMatrix(const osg::Vec3d& translation);

//! \brief                  Task 3: Return rotation matrix around X axis.
//! \param  angle           Angle of rotation.
osg::Matrix rotateAroundX(double angle);

//! \brief                  Task 4: Return rotation matrix around Y axis.
//! \param  angle           Angle of rotation.
osg::Matrix rotateAroundY(double angle);

//! \brief                  Task 5: Return rotation matrix around Z axis.
//! \param  angle           Angle of rotation.
osg::Matrix rotateAroundZ(double angle);

//! \brief                  Task 6: Compute the angle (in radians) between two vectors.
//! \param  u               First input vector.
//! \param  v               Second input vector.
double angleBetweenVectors(osg::Vec3d u, osg::Vec3d v);

//! \brief                  Task 7: Return a matrix that represents rotation from one vector to another.
//! \param  fromVector      First input vector.
//! \param  toVector        Second input vector.
osg::Matrix getRotationMatrix(const osg::Vec3d& fromVector, const osg::Vec3d& toVector);
