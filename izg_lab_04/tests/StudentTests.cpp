#include <gtest/gtest.h>


#include <Student.h>
//#include <Student.cpp> //Extremely dirty trick, DON'T DO THIS EVER!! I was just lazy to create a library out of Student.cpp/h.

#include <tests/CustomListener.h>

#define _USE_MATH_DEFINES
#include <math.h>
osg::Vec3d toEuler(const osg::Quat& q)
{
    double sqx = q.x() * q.x();
    double sqy = q.y() * q.y();
    double sqz = q.z() * q.z();
    double sqw = q.w() * q.w();

    double unit = sqx + sqy + sqz + sqw;
    double test = q.x() * q.y() + q.z() * q.w();
    double x, y, z;

    if (test > 0.499)
    {
        y = 2 * std::atan2(q.x(), q.w());
        z = M_PI * 0.5;
        x = 0.0;
    }
    else if (test < -0.499)
    {
        y = -2 * std::atan2(q.x(), q.w());
        z = -M_PI * 0.5;
        x = 0.0;
    }
    else
    {
        y = atan2(2 * q.y() * q.w() - 2 * q.x() * q.z(), sqx - sqy - sqz + sqw);
        z = asin(2 * test / unit);
        x = atan2(2 * q.x() * q.w() - 2 * q.y() * q.z(), -sqx + sqy - sqz + sqw);
    }

    return osg::Vec3d(x, y, z);
}


void compareMatrices(const osg::Matrix& m1, const osg::Matrix& m2, const std::string& function)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            EXPECT_NEAR(m1(i, j), m2(i, j), 1e-5) << "#" << "Failure in: " << function << ": at m[" << i << "," << j << "]"
                                                  << ", your value: " << m1(i, j) << " expected: " << m2(i, j);
        }

    }
}

TEST(MatricesTest, getTranslationMatrix)
{
    osg::Vec3d translation(1, 0, 0);

    compareMatrices(getTranslationMatrix(translation), osg::Matrix::translate(translation), "getTranslationMatrix");

    translation = { 0, 1, 0 };
    compareMatrices(getTranslationMatrix(translation), osg::Matrix::translate(translation), "getTranslationMatrix");

    translation = { 0, 0, 1 };
    compareMatrices(getTranslationMatrix(translation), osg::Matrix::translate(translation), "getTranslationMatrix");

    translation = { 0, 10, 1 };
    compareMatrices(getTranslationMatrix(translation), osg::Matrix::translate(translation), "getTranslationMatrix");

    translation = { 15, 0, 1 };
    compareMatrices(getTranslationMatrix(translation), osg::Matrix::translate(translation), "getTranslationMatrix");
}

TEST(MatricesTest, getScaleMatrix)
{
    osg::Vec3d scale(1, 0, 0);

    compareMatrices(getScaleMatrix(scale), osg::Matrix::scale(scale), "getScaleMatrix");

    scale = { 0, 1, 0 };
    compareMatrices(getScaleMatrix(scale), osg::Matrix::scale(scale), "getScaleMatrix");

    scale = { 0, 0, 1 };
    compareMatrices(getScaleMatrix(scale), osg::Matrix::scale(scale), "getScaleMatrix");

    scale = { 0, 2, 1 };
    compareMatrices(getScaleMatrix(scale), osg::Matrix::scale(scale), "getScaleMatrix");

    scale = { 10, 0, 1 };
    compareMatrices(getScaleMatrix(scale), osg::Matrix::scale(scale), "getScaleMatrix");
}

TEST(MatricesTest, RotationMatrixTest)
{
    double angle = osg::DegreesToRadians(45.0);

    const osg::Vec3d xAxis(1, 0, 0);
    const osg::Vec3d yAxis(0, 1, 0);
    const osg::Vec3d zAxis(0, 0, 1);

    auto mine = rotateAroundX(angle);
    auto osg = osg::Matrix::rotate(angle, xAxis);
    
    compareMatrices(rotateAroundX(angle), osg::Matrix::rotate(angle, xAxis), "rotateAroundX");
    compareMatrices(rotateAroundY(angle), osg::Matrix::rotate(angle, yAxis), "rotateAroundY");
    compareMatrices(rotateAroundZ(angle), osg::Matrix::rotate(angle, zAxis), "rotateAroundZ");


    angle = osg::DegreesToRadians(0.0);
    compareMatrices(rotateAroundX(angle), osg::Matrix::rotate(angle, xAxis), "rotateAroundX");
    compareMatrices(rotateAroundY(angle), osg::Matrix::rotate(angle, yAxis), "rotateAroundY");
    compareMatrices(rotateAroundZ(angle), osg::Matrix::rotate(angle, zAxis), "rotateAroundZ");


    angle = osg::DegreesToRadians(-15.0);
    compareMatrices(rotateAroundX(angle), osg::Matrix::rotate(angle, xAxis), "rotateAroundX");
    compareMatrices(rotateAroundY(angle), osg::Matrix::rotate(angle, yAxis), "rotateAroundY");
    compareMatrices(rotateAroundZ(angle), osg::Matrix::rotate(angle, zAxis), "rotateAroundZ");
}

TEST(AngleTest, angleBetweenVectors)
{
    double angle = osg::DegreesToRadians(45.0);
    const osg::Vec3d zAxis(0, 0, 1);

    osg::Vec3 in(1, 0, 0);
    osg::Vec3 rotated = in * osg::Matrix::rotate(angle, zAxis);

    double result = angleBetweenVectors(in, rotated);
    EXPECT_NEAR(result, angle, 1e-7) << "#" << "Failure in: " << "angleBetweenVectors" << "your value: " << result << " expected: " << angle;

    in = { 10, 0, 0 };
    rotated = { in * osg::Matrix::rotate(angle, zAxis) };
    result = angleBetweenVectors(in, rotated);
    EXPECT_NEAR(result, angle, 1e-7) << "#" << "Failure in: " << "angleBetweenVectors" << "your value: " << result << " expected: " << angle;

    angle = osg::DegreesToRadians(0.0);
    in = { 1, 0, 0 };
    rotated = { in * osg::Matrix::rotate(angle, zAxis) };
    result = angleBetweenVectors(in, rotated);
    EXPECT_NEAR(result, angle, 1e-7) << "#" << "Failure in: " << "angleBetweenVectors" << "your value: " << result << " expected: " << angle;
}

TEST(RotationBetweenVectors, getRotationMatrix)
{
    osg::Vec3d from(1, 0, 0);
    osg::Vec3d to(0, 1, 0);

    compareMatrices(getRotationMatrix(from, to), osg::Matrix::rotate(from, to), "getRotationMatrix");

    from = { 1,0,0 };
    to = { 1,1,0 };
    compareMatrices(getRotationMatrix(from, to), osg::Matrix::rotate(from, to), "getRotationMatrix");

    from = { 0,1,0 };
    to = { -1,0,0 };
    compareMatrices(getRotationMatrix(from, to), osg::Matrix::rotate(from, to), "getRotationMatrix");

    from = { 0,1,0 };
    to = { 1,0,0 };
    compareMatrices(getRotationMatrix(from, to), osg::Matrix::rotate(from, to), "getRotationMatrix");

    from = { 0.45, 0.45,0 };
    to = { 1,0,0 };
    compareMatrices(getRotationMatrix(from, to), osg::Matrix::rotate(from, to), "getRotationMatrix");

    from = { 0.45, 0.45,0 };
    to = { 0,1,0 };
    compareMatrices(getRotationMatrix(from, to), osg::Matrix::rotate(from, to), "getRotationMatrix");
    
    from = { 0.45, 0.45,0 };
    to = { 1,1,0 };
    compareMatrices(getRotationMatrix(from, to), osg::Matrix::rotate(from, to), "getRotationMatrix");

    from = { 1, 1,0 };
    to = { -1,-99,0 };
    compareMatrices(getRotationMatrix(from, to), osg::Matrix::rotate(from, to), "getRotationMatrix");

    from = { 1, 1,0 };
    to = { -1,1,0 };
    compareMatrices(getRotationMatrix(from, to), osg::Matrix::rotate(from, to), "getRotationMatrix");
}

TEST(RotationBetweenVectors, guessWhatIsWrong)
{
    std::string coloredHeader = "\x1B[93m[   INFO   ]\033[0m ";
    osg::Vec3d to(1, 0, 0);
    osg::Vec3d from(0, 1, 0);

    auto result = getRotationMatrix(from, to);
    auto eulerAngles = toEuler(result.getRotate());

    auto correct = osg::Matrix::rotate(from, to);
    auto eulerAnglesCorrect = toEuler(correct.getRotate());

    if (result.isIdentity())
    {
        std::cout << coloredHeader << "getRotationMatrix: You are returning an identity matrix." << std::endl;;
        return;
    }

    if (eulerAngles[2] == 0 && eulerAngles[0] == 0)
    {
        std::cout << coloredHeader << "getRotationMatrix : Probably missing rotation around X." << std::endl;
    }

    if (eulerAngles[2] > 0)
    {
        std::cout << coloredHeader << "getRotationMatrix : Probably missing the condition (7e)." << std::endl;
    }

    if (eulerAngles[0] != 0)
    {
        std::cout << coloredHeader << "getRotationMatrix : Probably missing rotation around Y." << std::endl;
    }
}