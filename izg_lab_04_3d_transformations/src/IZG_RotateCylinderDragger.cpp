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

#include <IZG_RotateCylinderDragger.h>

#include <osgManipulator/Command>

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/PolygonMode>
#include <osg/LineWidth>


#include <Student.h> 

using namespace osgManipulator;

namespace
{

//------------------------------------------------------------------------------
osg::Geometry* createDiskGeometry(float radius, float offset, float z, unsigned int numSegments
   )
{
   const float angleDelta = 2.0f*osg::PI/float(numSegments);
   const unsigned int numPoints = (numSegments+1) * 2;
   float angle = 0.0f;
   osg::Vec3Array* vertexArray = new osg::Vec3Array(numPoints);
   osg::Vec3Array* normalArray = new osg::Vec3Array(numPoints);
   unsigned int p = 0;
   for(unsigned int i = 0; i < numSegments; ++i,angle+=angleDelta)
   {
      float c = cosf(angle);
      float s = sinf(angle);
      // Outer point
      (*vertexArray)[p].set(radius*c, radius*s, z);
      (*normalArray)[p].set(0.0, 0.0, -1.0);
      ++p;
      // Inner point
      (*vertexArray)[p].set((radius-offset)*c, (radius-offset)*s, z);
      (*normalArray)[p].set(0.0, 0.0, -1.0);
      ++p;
   }
   // do last points by hand to ensure no round off errors.
   (*vertexArray)[p] = (*vertexArray)[0];
   (*normalArray)[p] = (*normalArray)[0];
   ++p;
   (*vertexArray)[p] = (*vertexArray)[1];
   (*normalArray)[p] = (*normalArray)[1];

   osg::Geometry* geometry = new osg::Geometry;
   geometry->setVertexArray(vertexArray);
   geometry->setNormalArray(normalArray, osg::Array::BIND_PER_VERTEX);
   geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, vertexArray->size()));
   return geometry;
}

}

IZG_RotateCylinderDragger::IZG_RotateCylinderDragger()
{
    _projector = new osgManipulator::PlaneProjector(osg::Plane(osg::Vec3(0.0, 0.0, 1.0), osg::Vec3(0.0, 0.0, 0.0)));
    setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    setPickColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
}

IZG_RotateCylinderDragger::~IZG_RotateCylinderDragger()
{
}

bool IZG_RotateCylinderDragger::handle(const PointerInfo& pointer, const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    // Check if the dragger node is in the nodepath.
    if (!pointer.contains(this))
    {
        return false;
    }

    switch (ea.getEventType())
    {
    case (osgGA::GUIEventAdapter::PUSH):
    {
        // Get the LocalToWorld matrix for this node and set it for the projector.
        osg::NodePath nodePathToRoot;
        computeNodePathToRoot(*this, nodePathToRoot);

        osg::Matrix localToWorld = osg::computeLocalToWorld(nodePathToRoot);
        _projector->setLocalToWorld(localToWorld);

        if (_projector->project(pointer, _startProjectedPoint))
        {
            osg::ref_ptr<Rotate3DCommand> cmd = new Rotate3DCommand();

            cmd->setStage(osgManipulator::MotionCommand::START);
            cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(), _projector->getWorldToLocal());

            // Set color to pick color.
            setMaterialColor(_pickColor, *this);

            dispatch(*cmd);

            aa.requestRedraw();
        }
        return true;
    }
    case (osgGA::GUIEventAdapter::DRAG):
    {
        osg::Vec3d projectedPoint;

        if (_projector->project(pointer, projectedPoint))
        {
            osg::ref_ptr<Rotate3DCommand> cmd = new Rotate3DCommand();

            cmd->setStage(osgManipulator::MotionCommand::MOVE);
            cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(), _projector->getWorldToLocal());

            //get rid of round-off error
            for (int i = 0; i < 3; ++i)
            {
                _startProjectedPoint[i] = std::abs(_startProjectedPoint[i]) < 1e-1 ? 0 : _startProjectedPoint[i];
                projectedPoint[i] = std::abs(projectedPoint[i]) < 1e-1 ? 0 : projectedPoint[i];
            }

            auto rotateMatrix = getRotationMatrix(_startProjectedPoint, projectedPoint);

            //osg::Quat quat;
            //quat.makeRotate(_startProjectedPoint, projectedPoint);

            osg::Quat quat = rotateMatrix.getRotate();

            double angle; osg::Vec3 axis;
            quat.getRotate(angle, axis);

            if (angle < osg::DegreesToRadians(1.0) || std::isnan(angle))
            {
                return true;
            }

            cmd->setRotation(quat);

            dispatch(*cmd);

            aa.requestRedraw();
        }
        return true;
    }
    case (osgGA::GUIEventAdapter::RELEASE):
    {
        osg::Vec3d projectedPoint;

        if (_projector->project(pointer, projectedPoint))
        {
            osg::ref_ptr<Rotate3DCommand> cmd = new Rotate3DCommand();

            cmd->setStage(osgManipulator::MotionCommand::FINISH);
            cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(), _projector->getWorldToLocal());

            // Reset color.
            setMaterialColor(_color, *this);

            dispatch(*cmd);

            aa.requestRedraw();

            return true;
        }
    }
    default:
        return false;
    }

    return false;
}

osg::Geometry* osgManipulator::IZG_RotateCylinderDragger::createCircleGeometry(float radius, unsigned int numSegments)
{
    const float angleDelta = 2.0f * osg::PI / (float)numSegments;
    const float r = radius;
    float angle = 0.0f;
    osg::Vec3Array* vertexArray = new osg::Vec3Array(numSegments);
    osg::Vec3Array* normalArray = new osg::Vec3Array(numSegments);
    for (unsigned int i = 0; i < numSegments; ++i, angle += angleDelta)
    {
        float c = cosf(angle);
        float s = sinf(angle);
        (*vertexArray)[i].set(c * r, s * r, 0.0f);
        (*normalArray)[i].set(c, s, 0.0f);
    }
    osg::Geometry* geometry = new osg::Geometry();
    geometry->setVertexArray(vertexArray);
    geometry->setNormalArray(normalArray, osg::Array::BIND_PER_VERTEX);
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, vertexArray->size()));
    return geometry;
}

void IZG_RotateCylinderDragger::setupDefaultGeometry()
{
#if 0
    osg::Geode* geode = new osg::Geode;
    {
        osg::TessellationHints* hints = new osg::TessellationHints;
        hints->setCreateTop(false);
        hints->setCreateBottom(false);
        hints->setCreateBackFace(false);

        float radius    = 1.0f;
        float height    = 0.1f;
        float thickness = 0.1f;

        // top
        geode->addDrawable(createDiskGeometry(radius, thickness,  height/2, 100));
        // bottom
        geode->addDrawable(createDiskGeometry(radius, thickness, -height/2, 100));
    }
    addChild(geode);
#endif

    float _axisLineWidth = 5.0f;
    float _pickCylinderHeight = 0.15f;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    {
        osg::TessellationHints* hints = new osg::TessellationHints;
        hints->setCreateTop(false);
        hints->setCreateBottom(false);
        hints->setCreateBackFace(false);

        osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder;
        cylinder->setHeight(_pickCylinderHeight);
        osg::ShapeDrawable* cylinderDrawable = new osg::ShapeDrawable(cylinder.get(), hints);
        geode->addDrawable(cylinderDrawable);
        setDrawableToAlwaysCull(*cylinderDrawable);
        geode->addDrawable(createCircleGeometry(1.0f, 100));
    }

    // Draw in line mode.
    {
        osg::PolygonMode* polymode = new osg::PolygonMode;
        polymode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
        geode->getOrCreateStateSet()->setAttributeAndModes(polymode, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
        osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth(_axisLineWidth);
        geode->getOrCreateStateSet()->setAttributeAndModes(lineWidth.get(), osg::StateAttribute::ON);

#if !defined(OSG_GLES2_AVAILABLE)
        geode->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
#endif

    }

    // Add line to all the individual 1D draggers.
    this->addChild(geode.get());

}
