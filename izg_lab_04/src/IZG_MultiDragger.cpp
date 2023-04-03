#include "IZG_MultiDragger.h"

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/CullFace>
#include <osg/Quat>
#include <osg/AutoTransform>

osgManipulator::IZG_MultiDragger::IZG_MultiDragger(bool useAutoTransform)
{
    //Rotation
    m_xRotateDragger = new IZG_RotateCylinderDragger();
    addChild(m_xRotateDragger.get());
    addDragger(m_xRotateDragger.get());

    m_yRotateDragger = new IZG_RotateCylinderDragger();
    addChild(m_yRotateDragger.get());
    addDragger(m_yRotateDragger.get());

    m_zRotateDragger = new IZG_RotateCylinderDragger();
    addChild(m_zRotateDragger.get());
    addDragger(m_zRotateDragger.get());

    //Translation
    m_xTranslateDragger = new IZG_Translate1DDragger(osg::Vec3(0.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0));
    addChild(m_xTranslateDragger.get());
    addDragger(m_xTranslateDragger.get());

    m_yTranslateDragger = new IZG_Translate1DDragger(osg::Vec3(0.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0));
    addChild(m_yTranslateDragger.get());
    addDragger(m_yTranslateDragger.get());

    m_zTranslateDragger = new IZG_Translate1DDragger(osg::Vec3(0.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0));
    addChild(m_zTranslateDragger.get());
    addDragger(m_zTranslateDragger.get());

    //Scale
    m_xScaleDragger = new IZG_Scale1DDragger();
    addChild(m_xScaleDragger.get());
    addDragger(m_xScaleDragger.get());

    m_yScaleDragger = new IZG_Scale1DDragger();
    addChild(m_yScaleDragger.get());
    addDragger(m_yScaleDragger.get());

    m_zScaleDragger = new IZG_Scale1DDragger();
    addChild(m_zScaleDragger.get());
    addDragger(m_zScaleDragger.get());

    setParentDragger(getParentDragger());
}

void osgManipulator::IZG_MultiDragger::setupMatrices(MatrixTransform* scaleTransform, MatrixTransform* rotateTranslateTransform)
{
    //rotate
    m_xRotateDragger->addTransformUpdating(rotateTranslateTransform, osgManipulator::DraggerTransformCallback::HANDLE_TRANSLATE_IN_LINE);
    m_yRotateDragger->addTransformUpdating(rotateTranslateTransform, osgManipulator::DraggerTransformCallback::HANDLE_TRANSLATE_IN_LINE);
    m_zRotateDragger->addTransformUpdating(rotateTranslateTransform, osgManipulator::DraggerTransformCallback::HANDLE_TRANSLATE_IN_LINE);

    //translate
    m_xTranslateDragger->addTransformUpdating(rotateTranslateTransform, osgManipulator::DraggerTransformCallback::HANDLE_ROTATE_3D);
    m_yTranslateDragger->addTransformUpdating(rotateTranslateTransform, osgManipulator::DraggerTransformCallback::HANDLE_ROTATE_3D);
    m_zTranslateDragger->addTransformUpdating(rotateTranslateTransform, osgManipulator::DraggerTransformCallback::HANDLE_ROTATE_3D);

    //scale
    m_xScaleDragger->addTransformUpdating(scaleTransform, osgManipulator::DraggerTransformCallback::HANDLE_SCALED_1D);
    m_yScaleDragger->addTransformUpdating(scaleTransform, osgManipulator::DraggerTransformCallback::HANDLE_SCALED_1D);
    m_zScaleDragger->addTransformUpdating(scaleTransform, osgManipulator::DraggerTransformCallback::HANDLE_SCALED_1D);

    this->addTransformUpdating(scaleTransform, osgManipulator::DraggerTransformCallback::HANDLE_SCALED_1D);
    this->addTransformUpdating(rotateTranslateTransform, osgManipulator::DraggerTransformCallback::HANDLE_TRANSLATE_IN_LINE | osgManipulator::DraggerTransformCallback::HANDLE_ROTATE_3D);
}

void osgManipulator::IZG_MultiDragger::setupRotationDraggersGeometry()
{
    m_xRotateDragger->setupDefaultGeometry();
    m_yRotateDragger->setupDefaultGeometry();
    m_zRotateDragger->setupDefaultGeometry();

    // Rotate X-axis dragger appropriately.
    {
        osg::Quat rotation; rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(1.0f, 0.0f, 0.0f));
        m_xRotateDragger->setMatrix(osg::Matrix(rotation));
    }

    // Rotate Y-axis dragger appropriately.
    {
        osg::Quat rotation; rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(0.0f, 1.0f, 0.0f));
        m_yRotateDragger->setMatrix(osg::Matrix(rotation));
    }

    // Send different colors for each dragger.
    m_xRotateDragger->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    m_yRotateDragger->setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    m_zRotateDragger->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
}

void osgManipulator::IZG_MultiDragger::setupTranslationDraggersGeometry()
{
    m_xTranslateDragger->setupDefaultGeometry();
    m_yTranslateDragger->setupDefaultGeometry();
    m_zTranslateDragger->setupDefaultGeometry();

    // Rotate X-axis dragger appropriately.
    {
        osg::Quat rotation; rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(1.0f, 0.0f, 0.0f));
        m_xTranslateDragger->setMatrix(osg::Matrix(rotation));
    }

    // Rotate Y-axis dragger appropriately.
    {
        osg::Quat rotation; rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(0.0f, 1.0f, 0.0f));
        m_yTranslateDragger->setMatrix(osg::Matrix(rotation));
    }

    // Send different colors for each dragger.
    m_xTranslateDragger->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    m_yTranslateDragger->setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    m_zTranslateDragger->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
}

void osgManipulator::IZG_MultiDragger::setupScaleDraggersGeometry()
{
    m_xScaleDragger->setupDefaultGeometry();
    m_yScaleDragger->setupDefaultGeometry();
    m_zScaleDragger->setupDefaultGeometry();

    // Rotate Z-axis dragger appropriately.
    {
        osg::Quat rotation; rotation.makeRotate(osg::Vec3(1.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 1.0f));
        m_zScaleDragger->setMatrix(osg::Matrix(rotation));
    }

    // Rotate Y-axis dragger appropriately.
    {
        osg::Quat rotation; rotation.makeRotate(osg::Vec3(1.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 1.0f, 0.0f));
        m_yScaleDragger->setMatrix(osg::Matrix(rotation));
    }

    // Send different colors for each dragger.
    m_xScaleDragger->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    m_yScaleDragger->setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    m_zScaleDragger->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
}

void osgManipulator::IZG_MultiDragger::setupDefaultGeometry()
{
    setupRotationDraggersGeometry();
    setupTranslationDraggersGeometry();
    setupScaleDraggersGeometry();
}
