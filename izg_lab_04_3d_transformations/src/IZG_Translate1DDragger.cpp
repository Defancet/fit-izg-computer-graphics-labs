/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/
//osgManipulator - Copyright (C) 2007 Fugro-Jason B.V.

#include <IZG_Translate1DDragger.h>

#include <osgManipulator/Command>

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/Material>

#include <Student.h>

using namespace osgManipulator;

IZG_Translate1DDragger::IZG_Translate1DDragger() : Dragger(), _checkForNodeInNodePath(true)
{
    _projector = new LineProjector;
    setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    setPickColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
}

IZG_Translate1DDragger::IZG_Translate1DDragger(const osg::Vec3d& s, const osg::Vec3d& e) : Dragger(), _checkForNodeInNodePath(true)
{
    _projector = new LineProjector(s, e);
    setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    setPickColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
}

IZG_Translate1DDragger::~IZG_Translate1DDragger()
{
}

bool IZG_Translate1DDragger::handle(const PointerInfo& pointer, const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    // Check if the dragger node is in the nodepath.
    if (_checkForNodeInNodePath)
    {
        if (!pointer.contains(this)) return false;
    }

    switch (ea.getEventType())
    {
        // Pick start.
    case (osgGA::GUIEventAdapter::PUSH):
    {
        // Get the LocalToWorld matrix for this node and set it for the projector.
        osg::NodePath nodePathToRoot;
        computeNodePathToRoot(*this, nodePathToRoot);
        osg::Matrix localToWorld = osg::computeLocalToWorld(nodePathToRoot);
        _projector->setLocalToWorld(localToWorld);

        if (_projector->project(pointer, _startProjectedPoint))
        {
            // Generate the motion command.
            osg::ref_ptr<TranslateInLineCommand> cmd = new TranslateInLineCommand(_projector->getLineStart(),
                _projector->getLineEnd());
            cmd->setStage(MotionCommand::START);
            cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(), _projector->getWorldToLocal());

            // Dispatch command.
            dispatch(*cmd);

            // Set color to pick color.
            setMaterialColor(_pickColor, *this);

            aa.requestRedraw();
        }
        return true;
    }

    // Pick move.
    case (osgGA::GUIEventAdapter::DRAG):
    {
        osg::Vec3d projectedPoint;
        if (_projector->project(pointer, projectedPoint))
        {
            // Generate the motion command.
            osg::ref_ptr<TranslateInLineCommand> cmd = new TranslateInLineCommand(_projector->getLineStart(),
                _projector->getLineEnd());
            cmd->setStage(MotionCommand::MOVE);
            cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(), _projector->getWorldToLocal());

            auto translationMatrix = getTranslationMatrix(projectedPoint - _startProjectedPoint);


            cmd->setTranslation(translationMatrix.getTrans());
            //cmd->setTranslation(projectedPoint - _startProjectedPoint);

            // Dispatch command.
            dispatch(*cmd);

            aa.requestRedraw();
        }
        return true;
    }

    // Pick finish.
    case (osgGA::GUIEventAdapter::RELEASE):
    {
        osg::Vec3d projectedPoint;
        if (_projector->project(pointer, projectedPoint))
        {
            osg::ref_ptr<TranslateInLineCommand> cmd = new TranslateInLineCommand(_projector->getLineStart(),
                _projector->getLineEnd());

            cmd->setStage(MotionCommand::FINISH);
            cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(), _projector->getWorldToLocal());

            // Dispatch command.
            dispatch(*cmd);

            // Reset color.
            setMaterialColor(_color, *this);

            aa.requestRedraw();
        }

        return true;
    }
    default:
        return false;
    }
}

void IZG_Translate1DDragger::setupDefaultGeometry()
{
    //float axisLineWidth = 5.0f;
    float coneHeight = 0.2f;
    float pickCylinderRadius = 0.05f;

    //// Create a line.
    //osg::ref_ptr<osg::Geode> lineGeode = new osg::Geode;
    //{
    //    osg::Geometry* geometry = new osg::Geometry();

    //    osg::Vec3Array* vertices = new osg::Vec3Array(2);
    //    (*vertices)[0] = osg::Vec3(0.0f, 0.0f, 0.0f);
    //    (*vertices)[1] = osg::Vec3(0.0f, 0.0f, 1.0f);

    //    geometry->setVertexArray(vertices);
    //    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));

    //    lineGeode->addDrawable(geometry);
    //}

    //// Turn of lighting for line and set line width.
    //{
    //    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    //    lineWidth->setWidth(axisLineWidth);
    //    lineGeode->getOrCreateStateSet()->setAttributeAndModes(lineWidth.get(), osg::StateAttribute::ON);
    //    lineGeode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    //}

    // Create an invisible cylinder for picking the line.
    //{
    //    osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(osg::Vec3(0.0f, 0.0f, 0.5f), pickCylinderRadius, 1.0f);
    //    osg::Drawable* geometry = new osg::ShapeDrawable(cylinder.get());
    //    setDrawableToAlwaysCull(*geometry);
    //    geode->addDrawable(geometry);
    //}

    // Add line to all the individual 1D draggers.
    //this->addChild(lineGeode.get());


    // Create left cone.
    {
        osg::ref_ptr<osg::Geode> leftGeode = new osg::Geode;

        osg::ref_ptr<osg::Cone> cone = new osg::Cone(osg::Vec3(0.0f, 0.0f, 1.07f), coneHeight * 0.25f, coneHeight);
        osg::ShapeDrawable* coneDrawable = new osg::ShapeDrawable(cone.get());
        // coneDrawable->setColor(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
        leftGeode->addDrawable(coneDrawable);

        // This ensures correct lighting for scaled draggers.
#if !defined(OSG_GLES2_AVAILABLE)
        leftGeode->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
#endif
        this->addChild(leftGeode);

    }

    // Create right cone.
    {
        osg::ref_ptr<osg::Cone> cone = new osg::Cone(osg::Vec3(0.0f, 0.0f, 1.07f), coneHeight * 0.25f, coneHeight);
        osg::ShapeDrawable* coneDrawable = new osg::ShapeDrawable(cone.get());
        // coneDrawable->setColor(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
        //geode->addChild(coneDrawable);

        osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform;

        osg::Quat quat;
        quat.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(0.0f, 0.0f, -1.0f));

        transform->setMatrix(osg::Matrix(quat));

        osg::ref_ptr<osg::Geode> rightGeode = new osg::Geode;

        // This ensures correct lighting for scaled draggers.
#if !defined(OSG_GLES2_AVAILABLE)
        rightGeode->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
#endif

        rightGeode->addDrawable(coneDrawable);
        transform->addChild(rightGeode);

        this->addChild(transform);
    }

}
