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

#include <IZG_Scale1DDragger.h>

#include <osgManipulator/Command>

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/Material>

#include <Student.h>

using namespace osgManipulator;

namespace
{

    double computeScale(const osg::Vec3d& startProjectedPoint,
        const osg::Vec3d& projectedPoint, double scaleCenter)
    {
        double denom = startProjectedPoint[0] - scaleCenter;
        double scale = denom ? (projectedPoint[0] - scaleCenter) / denom : 1.0;
        return scale;
    }

}

IZG_Scale1DDragger::IZG_Scale1DDragger(ScaleMode scaleMode) :
    Dragger(),
    _scaleCenter(0.0),
    _minScale(0.001),
    _scaleMode(scaleMode)
{
    _projector = new LineProjector(osg::Vec3d(-1.2, 0.0, 0.0), osg::Vec3d(1.2, 0.0, 0.0));
    setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    setPickColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
}

IZG_Scale1DDragger::~IZG_Scale1DDragger()
{
}

bool IZG_Scale1DDragger::handle(const PointerInfo & pointer, const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa)
{
    // Check if the dragger node is in the nodepath.
    if (!pointer.contains(this)) return false;

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
            _scaleCenter = 0.0;
            if (_scaleMode == SCALE_WITH_OPPOSITE_HANDLE_AS_PIVOT)
            {
                if (pointer.contains(_leftHandleNode.get()))
                    _scaleCenter = _projector->getLineEnd()[0];
                else if (pointer.contains(_rightHandleNode.get()))
                    _scaleCenter = _projector->getLineStart()[0];
            }

            // Generate the motion command.
            osg::ref_ptr<Scale1DCommand> cmd = new Scale1DCommand();
            cmd->setStage(MotionCommand::START);
            cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(), _projector->getWorldToLocal());

            // Set color to pick color.
            setMaterialColor(_pickColor, *this);

            // Dispatch command.
            dispatch(*cmd);


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
            osg::ref_ptr<Scale1DCommand> cmd = new Scale1DCommand();

            // Compute scale.
            double scale = computeScale(_startProjectedPoint, projectedPoint, _scaleCenter);
            if (scale < getMinScale()) scale = getMinScale();

            // Snap the referencePoint to the line start or line end depending on which is closer.
            double referencePoint = _startProjectedPoint[0];
            if (fabs(_projector->getLineStart()[0] - referencePoint) <
                fabs(_projector->getLineEnd()[0] - referencePoint))
                referencePoint = _projector->getLineStart()[0];
            else
                referencePoint = _projector->getLineEnd()[0];

            cmd->setStage(MotionCommand::MOVE);
            cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(), _projector->getWorldToLocal());

            auto scaleMatrix = getScaleMatrix({ scale,scale,scale });

            cmd->setScale(scaleMatrix.getScale()[0]); 
            
            //cmd->setScale(scale);
            cmd->setScaleCenter(_scaleCenter);
            cmd->setReferencePoint(referencePoint);
            cmd->setMinScale(getMinScale());

            // Dispatch command.
            dispatch(*cmd);

            aa.requestRedraw();
        }
        return true;
    }

    // Pick finish.
    case (osgGA::GUIEventAdapter::RELEASE):
    {
        osg::ref_ptr<Scale1DCommand> cmd = new Scale1DCommand();

        cmd->setStage(MotionCommand::FINISH);
        cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(), _projector->getWorldToLocal());

        // Dispatch command.
        dispatch(*cmd);

        // Reset color.
        setMaterialColor(_color, *this);

        aa.requestRedraw();

        return true;
    }
    default:
        return false;
    }
}

void IZG_Scale1DDragger::setupDefaultGeometry()
{
    // Create a left box.
    {
        osg::Geode* geode = new osg::Geode;
        geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(_projector->getLineStart() - osg::Vec3(0.1, 0, 0), 0.05)));
        addChild(geode);
        setLeftHandleNode(*geode);
        // This ensures correct lighting for scaled draggers.
#if !defined(OSG_GLES2_AVAILABLE)
        geode->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
#endif
    }

    // Create a right box.
    {
        osg::Geode* geode = new osg::Geode;
        geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(_projector->getLineEnd() + osg::Vec3(0.1, 0, 0), 0.05)));
        addChild(geode);
        setRightHandleNode(*geode);
        // This ensures correct lighting for scaled draggers.
#if !defined(OSG_GLES2_AVAILABLE)
        geode->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
#endif
    }
}
