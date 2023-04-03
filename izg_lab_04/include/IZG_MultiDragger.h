#pragma once

#include <osgManipulator/Dragger>


#include <IZG_Translate1DDragger.h>
#include <IZG_Scale1DDragger.h>
#include <IZG_RotateCylinderDragger.h>

namespace osgManipulator
{

    class IZG_MultiDragger : public CompositeDragger
    {
    public:

        IZG_MultiDragger(bool useAutoTransform = false);


        META_OSGMANIPULATOR_Object(osgManipulator, IZG_MultiDragger)

        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

        /** Sets the width of the axis lines in pixels. */
        //void setAxisLineWidth(float linePixelWidth);

        /** Retrieves the width of the axis lines in pixels. */
        //float getAxisLineWidth() const { return _axisLineWidth; }

        /** Sets the height of the cylinders representing the axis lines for picking. */
        //void setPickCylinderHeight(float pickCylinderHeight);

        /** Retrieves the height of the cylinders representing the axis lines for picking. */
        //float getPickCylinderHeight() const { return _pickCylinderHeight; }

        void setupMatrices(MatrixTransform* scaleTransform, MatrixTransform* rotateTranslateTransform);

    protected:

        void setupRotationDraggersGeometry();
        void setupTranslationDraggersGeometry();
        void setupScaleDraggersGeometry();

    protected:

        virtual ~IZG_MultiDragger()
        {

        }

        osg::ref_ptr<IZG_RotateCylinderDragger> m_xRotateDragger;
        osg::ref_ptr<IZG_RotateCylinderDragger> m_yRotateDragger;
        osg::ref_ptr<IZG_RotateCylinderDragger> m_zRotateDragger;

        osg::ref_ptr< IZG_Translate1DDragger >  m_xTranslateDragger;
        osg::ref_ptr< IZG_Translate1DDragger >  m_yTranslateDragger;
        osg::ref_ptr< IZG_Translate1DDragger >  m_zTranslateDragger;

        osg::ref_ptr< IZG_Scale1DDragger >  m_xScaleDragger;
        osg::ref_ptr< IZG_Scale1DDragger >  m_yScaleDragger;
        osg::ref_ptr< IZG_Scale1DDragger >  m_zScaleDragger;

        //osg::ref_ptr<osg::Geode> m_geode;

        //float _axisLineWidth;
        //float _pickCylinderHeight;

        //osg::ref_ptr<osg::LineWidth> m_lineWidth;
    };
}