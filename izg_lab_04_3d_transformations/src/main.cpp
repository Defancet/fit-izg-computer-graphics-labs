
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/CoordinateSystemNode>
#include <osgText/Text>

#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabBoxTrackballDragger>
#include <osgManipulator/TabPlaneDragger>
#include <osgManipulator/TabPlaneTrackballDragger>
#include <osgManipulator/Scale1DDragger>
#include <osgManipulator/Scale2DDragger>
//#include <osgManipulator/TrackballDragger>
#include <osgManipulator/Translate1DDragger>
#include <osgManipulator/Translate2DDragger>
#include <osgManipulator/TranslateAxisDragger>
#include <osgManipulator/TranslatePlaneDragger>
#include <osgManipulator/RotateCylinderDragger>
#include <osgManipulator/AntiSquish>

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/io_utils>

#include <iostream>

#include <IZG_TrackballDragger.h>
#include <IZG_MultiDragger.h>


class PlaneConstraint : public osgManipulator::Constraint
{
public:
    PlaneConstraint() {}

    virtual bool constrain(osgManipulator::TranslateInLineCommand& command) const
    {
        OSG_NOTICE << "PlaneConstraint TranslateInLineCommand " << command.getTranslation() << std::endl;
        return true;
    }
    virtual bool constrain(osgManipulator::TranslateInPlaneCommand& command) const
    {
        //command.setTranslation(osg::Vec3(0.0f,0.0f,0.0f));
        OSG_NOTICE << "PlaneConstraint TranslateInPlaneCommand " << command.getTranslation() << std::endl;
        return true;
    }
    virtual bool constrain(osgManipulator::Scale1DCommand& command) const
    {
        //command.setScale(1.0f);
        OSG_NOTICE << "PlaneConstraint Scale1DCommand" << command.getScale() << std::endl;
        return true;
    }
    virtual bool constrain(osgManipulator::Scale2DCommand& command) const
    {
        //command.setScale(osg::Vec2d(1.0,1.0));
        OSG_NOTICE << "PlaneConstraint Scale2DCommand " << command.getScale() << std::endl;
        return true;
    }
    virtual bool constrain(osgManipulator::ScaleUniformCommand& command) const
    {
        OSG_NOTICE << "PlaneConstraint ScaleUniformCommand" << command.getScale() << std::endl;
        return true;
    }
};


osgManipulator::Dragger* createDragger(const std::string& name)
{
    osgManipulator::Dragger* dragger = 0;
    if ("TabPlaneDragger" == name)
    {
        osgManipulator::TabPlaneDragger* d = new osgManipulator::TabPlaneDragger();
        d->setupDefaultGeometry();
        d->addConstraint(new PlaneConstraint());
        dragger = d;
    }
    else if ("TabPlaneTrackballDragger" == name)
    {
        osgManipulator::TabPlaneTrackballDragger* d = new osgManipulator::TabPlaneTrackballDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ("TabBoxTrackballDragger" == name)
    {
        osgManipulator::TabBoxTrackballDragger* d = new osgManipulator::TabBoxTrackballDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ("TrackballDragger" == name)
    {
        osgManipulator::IZG_TrackballDragger* d = new osgManipulator::IZG_TrackballDragger();
        d->setupDefaultGeometry();
        //d->setAxisLineWidth(5.0f);
        //d->setPickCylinderHeight(0.1f);
        dragger = d;
    }
    else if ("Translate1DDragger" == name)
    {
        osgManipulator::Translate1DDragger* d = new osgManipulator::Translate1DDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ("Translate2DDragger" == name)
    {
        osgManipulator::Translate2DDragger* d = new osgManipulator::Translate2DDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ("TranslateAxisDragger" == name)
    {
        osgManipulator::TranslateAxisDragger* d = new osgManipulator::TranslateAxisDragger();
        d->setupDefaultGeometry();
        d->setAxisLineWidth(5.0f);
        d->setPickCylinderRadius(0.05f);
        d->setConeHeight(0.2f);
        dragger = d;
    }
    else if ("TranslatePlaneDragger" == name)
    {
        osgManipulator::TranslatePlaneDragger* d = new osgManipulator::TranslatePlaneDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ("Scale1DDragger" == name)
    {
        osgManipulator::Scale1DDragger* d = new osgManipulator::Scale1DDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ("Scale2DDragger" == name)
    {
        osgManipulator::Scale2DDragger* d = new osgManipulator::Scale2DDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ("RotateCylinderDragger" == name)
    {
        osgManipulator::RotateCylinderDragger* d = new osgManipulator::RotateCylinderDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ("RotateSphereDragger" == name)
    {
        osgManipulator::RotateSphereDragger* d = new osgManipulator::RotateSphereDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else
    {
        osgManipulator::TabBoxDragger* d = new osgManipulator::TabBoxDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }

    return dragger;
}

// The DraggerContainer node is used to fix the dragger's size on the screen
class DraggerContainer : public osg::Group
{
public:
    DraggerContainer() : _draggerSize(240.0f), _active(true) {}

    DraggerContainer(const DraggerContainer& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
        : osg::Group(copy, copyop),
        _dragger(copy._dragger), _draggerSize(copy._draggerSize), _active(copy._active)
    {}

    META_Node(osgManipulator, DraggerContainer);

    void setDragger(osgManipulator::Dragger* dragger)
    {
        _dragger = dragger;
        if (!containsNode(dragger)) addChild(dragger);
    }

    osgManipulator::Dragger* getDragger() { return _dragger.get(); }
    const osgManipulator::Dragger* getDragger() const { return _dragger.get(); }

    void setDraggerSize(float size) { _draggerSize = size; }
    float getDraggerSize() const { return _draggerSize; }

    void setActive(bool b) { _active = b; }
    bool getActive() const { return _active; }

    void traverse(osg::NodeVisitor& nv)
    {
        if (_dragger.valid())
        {
            if (_active && nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR)
            {
                osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>(&nv);

                float pixelSize = cv->pixelSize(_dragger->getBound().center(), 0.48f);
                if (pixelSize != _draggerSize)
                {
                    float pixelScale = pixelSize > 0.0f ? _draggerSize / pixelSize : 1.0f;
                    osg::Vec3d scaleFactor(pixelScale, pixelScale, pixelScale);

                    osg::Vec3 trans = _dragger->getMatrix().getTrans();
                    _dragger->setMatrix(osg::Matrix::scale(scaleFactor) * osg::Matrix::translate(trans));
                }
            }
        }
        osg::Group::traverse(nv);
    }

protected:
    osg::ref_ptr<osgManipulator::Dragger> _dragger;
    float _draggerSize;
    bool _active;
};

osg::Node* addDraggerToScene(osg::Node* scene, const std::string& name, bool fixedSizeInScreen)
{
    scene->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

    osg::MatrixTransform* transform = new osg::MatrixTransform;
    transform->addChild(scene);

    osgManipulator::Dragger* dragger = createDragger(name);

    osg::Group* root = new osg::Group;
    root->addChild(transform);

    if (fixedSizeInScreen)
    {
        DraggerContainer* draggerContainer = new DraggerContainer;
        draggerContainer->setDragger(dragger);
        root->addChild(draggerContainer);
    }
    else
        root->addChild(dragger);

    float scale = scene->getBound().radius() * 1.6;
    dragger->setMatrix(osg::Matrix::scale(scale, scale, scale) *
        osg::Matrix::translate(scene->getBound().center()));

    if (dynamic_cast<osgManipulator::TabPlaneDragger*>(dragger))
    {
        dragger->addTransformUpdating(transform, osgManipulator::DraggerTransformCallback::HANDLE_TRANSLATE_IN_LINE);
    }
    else
    {
        dragger->addTransformUpdating(transform);
    }

    // we want the dragger to handle it's own events automatically
    dragger->setHandleEvents(true);

    // if we don't set an activation key or mod mask then any mouse click on
    // the dragger will activate it, however if do define either of ActivationModKeyMask or
    // and ActivationKeyEvent then you'll have to press either than mod key or the specified key to
    // be able to activate the dragger when you mouse click on it.  Please note the follow allows
    // activation if either the ctrl key or the 'a' key is pressed and held down.
    dragger->setActivationModKeyMask(osgGA::GUIEventAdapter::MODKEY_CTRL);
    dragger->setActivationKeyEvent('a');

    return root;
}

void makeWindowed(osgViewer::Viewer& viewer)
{
#if 1
    osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
    if (!wsi)
    {
        osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
        return;
    }

    unsigned int width, height;
    osg::GraphicsContext::ScreenIdentifier main_screen_id;

    main_screen_id.readDISPLAY();
    main_screen_id.setUndefinedScreenDetailsToDefaultScreen();
    wsi->getScreenResolution(main_screen_id, width, height);

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = 0;
    traits->y = 30;
    traits->width = width;
    traits->height = height - traits->y;
    traits->sharedContext = 0;
    traits->readDISPLAY();
    traits->setUndefinedScreenDetailsToDefaultScreen();
    traits->windowName = "IZG 05";
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->samples = 4;

    osg::DisplaySettings::instance()->setNumMultiSamples(6);

    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    if (gc.valid())
    {
        osg::notify(osg::INFO) << "  GraphicsWindow has been created successfully." << std::endl;

        // need to ensure that the window is cleared make sure that the complete window is set the correct colour
        // rather than just the parts of the window that are under the camera's viewports
        gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
        gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
        osg::notify(osg::NOTICE) << "  GraphicsWindow has not been created successfully." << std::endl;
    }

    viewer.setUpViewInWindow(traits->x, traits->y, traits->width, traits->height);

#else
    osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
    if (!wsi)
    {
        osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
        return;
    }

    unsigned int width, height;
    osg::GraphicsContext::ScreenIdentifier main_screen_id;

    main_screen_id.readDISPLAY();
    main_screen_id.setUndefinedScreenDetailsToDefaultScreen();
    wsi->getScreenResolution(main_screen_id, width, height);

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = 10;
    traits->y = 10;
    traits->width = width - 10;
    traits->height = height - 10;
    traits->windowName = "OSG application";
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->samples = 4;
    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setGraphicsContext(gc);
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    camera->setClearColor(osg::Vec4(0.2f, 0.2f, 0.4f, 1.0f));
    double aspect = static_cast<double>(traits->width) / static_cast<double>(traits->height);
    camera->setProjectionMatrixAsPerspective(30.0, aspect, 1.0, 1000.0);
    camera->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    viewer.setCamera(camera.get());
#endif

}

osg::Group* createDraggers(osg::Node* geomNode)
{
    geomNode->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);


    osgManipulator::IZG_MultiDragger* multiDragger = new osgManipulator::IZG_MultiDragger();

    //setup transforms correctly
    osg::ref_ptr<osg::MatrixTransform> scaleTransform = new osg::MatrixTransform;
    osg::ref_ptr<osg::MatrixTransform> rotateTranslateTransform = new osg::MatrixTransform;

    rotateTranslateTransform->addChild(scaleTransform);
    scaleTransform->addChild(geomNode);

    multiDragger->setupMatrices(scaleTransform, rotateTranslateTransform);

    multiDragger->setupDefaultGeometry();

    float scale = geomNode->getBound().radius() * 1.6;
    auto m = osg::Matrix::scale(scale, scale, scale) * osg::Matrix::translate(geomNode->getBound().center());
    multiDragger->setMatrix(m);

    // we want the dragger to handle it's own events automatically
    multiDragger->setHandleEvents(true);

    // if we don't set an activation key or mod mask then any mouse click on
    // the dragger will activate it, however if do define either of ActivationModKeyMask or
    // and ActivationKeyEvent then you'll have to press either than mod key or the specified key to
    // be able to activate the dragger when you mouse click on it.  Please note the follow allows
    // activation if either the ctrl key or the 'a' key is pressed and held down.
    multiDragger->setActivationModKeyMask(osgGA::GUIEventAdapter::MODKEY_CTRL);

    osg::Group* draggerRoot = new osg::Group;
    draggerRoot->addChild(multiDragger);

    draggerRoot->addChild(rotateTranslateTransform);

    return draggerRoot;
}

int main(int argc, char** argv)
{
    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::Group>root = new osg::Group;
       
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile({"bunny.obj"});

    osg::ref_ptr<osg::Material> matirial = new osg::Material;
    matirial->setColorMode(osg::Material::DIFFUSE);
    matirial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 0, 1));
    matirial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.9, 0.3, 0.4, 1.0));
    matirial->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1, 1, 1));
    matirial->setShininess(osg::Material::FRONT_AND_BACK, 64.0f);
    loadedModel->getOrCreateStateSet()->setAttributeAndModes(matirial.get(), osg::StateAttribute::ON);


    root->addChild(createDraggers(loadedModel));

    viewer.addEventHandler(new osgViewer::StatsHandler);
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);

    makeWindowed(viewer);

    osgUtil::Optimizer optimizer;
    optimizer.optimize(loadedModel);

    viewer.setSceneData(root);
    viewer.realize();

    return viewer.run();

}