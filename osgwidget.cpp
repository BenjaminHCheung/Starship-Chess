#include "osgwidget.h"

#include <osg/Camera>
#include <osg/DisplaySettings>
#include <osg/StateSet>
#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#include <osg/PositionAttitudeTransform>

#include <osg/NodeVisitor>

#include <QKeyEvent>
#include <QPainter>
#include <QWheelEvent>
#include <vector>
#include <string>

#include "sphereupdatecallback.h"
#include "vector3d.h"

OSGWidget::OSGWidget( QWidget* parent, Qt::WindowFlags flags ):
    QOpenGLWidget{ parent,flags },
    mGraphicsWindow{ new osgViewer::GraphicsWindowEmbedded{ this->x(),
                                                            this->y(),
                                                            this->width(),
                                                            this->height() } }
    , mViewer{ new osgViewer::CompositeViewer }
    , mView{ new osgViewer::View }
    , mRoot{ new osg::Group }
    , mSpaceBoard{ new SpaceBoard}
{
    int viewPortX{ 0 };
    int viewPortY{ 0 };
    osg::Camera* camera{ generate_camera_viewport(viewPortX, viewPortY) };
    osg::Vec4 cameraClearColorRGBA{ osg::Vec4(0.f, 0.f, .5f, 1.f) };
    change_camera_color_and_transparency(camera, cameraClearColorRGBA);

    float cameraFieldOfViewInY{ 45 };
    float nearZLimit{ 1 };
    float farZLimit{ 1000 };
    change_camera_perspective(camera, cameraFieldOfViewInY, nearZLimit, farZLimit);
    change_graphics_context(camera);
    setup_mView_with_camera(camera);

    osg::Vec3d positionEye{osg::Vec3d(0.0,-20.0,3.0)};
    osg::Vec3d positionCenter{osg::Vec3d(0,0,0)};
    osg::Vec3d upVector{osg::Vec3d(0,0,1)};
    generate_trackball_manipulator(positionEye, positionCenter, upVector);
    setup_threads();

    draw_position_nodes();

    int xMinimumSize{100};
    int yMinimumSize{100};
    set_widget_minimum_size(xMinimumSize, yMinimumSize);

    set_mouse_tracking_on();
    this->update();

    double framesPerSecond{30};
    double timeStep{1.0/framesPerSecond};
    double timerDurationInMilliSeconds{timeStep * 1000};
    mTimerId=startTimer(timerDurationInMilliSeconds);
}

OSGWidget::~OSGWidget()
{
    killTimer(mTimerId);
}

void OSGWidget::set_pause(bool pause)
{
    mPause = pause;
}

void OSGWidget::timerEvent(QTimerEvent *)
{
    update_widget();
}

void OSGWidget::paintEvent( QPaintEvent* )
{
    this->makeCurrent();

    QPainter painter( this );
    painter.setRenderHint( QPainter::Antialiasing );

    this->paintGL();

    painter.end();

    this->doneCurrent();
}

void OSGWidget::paintGL()
{
    mViewer->frame();
}

void OSGWidget::resizeGL( int width, int height )
{
    this->getEventQueue()->windowResize( this->x(), this->y(), width, height );
    mGraphicsWindow->resized( this->x(), this->y(), width, height );

    this->on_resize( width, height );
}

void OSGWidget::keyPressEvent( QKeyEvent* event )
{
    QString keyString   = event->text();
    const char* keyData = keyString.toLocal8Bit().data();

    if( event->key() == Qt::Key_H )
    {
        mView->home();
        return;
    }

    this->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KeySymbol( *keyData ) );
}

void OSGWidget::keyReleaseEvent( QKeyEvent* event )
{
    QString keyString   = event->text();
    const char* keyData = keyString.toLocal8Bit().data();

    this->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KeySymbol( *keyData ) );
}

void OSGWidget::mouseMoveEvent( QMouseEvent* event )
{

    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseMotion( static_cast<float>( event->x() * pixelRatio ),
                                        static_cast<float>( event->y() * pixelRatio ) );
}

void OSGWidget::mousePressEvent( QMouseEvent* event )
{
    unsigned int button = 0;

    switch( event->button() )
    {
    case Qt::LeftButton:
        button = 1;
        break;

    case Qt::MiddleButton:
        button = 2;
        break;

    case Qt::RightButton:
        button = 3;
        break;

    default:
        break;
    }

    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseButtonPress( static_cast<float>( event->x() * pixelRatio ),
                                             static_cast<float>( event->y() * pixelRatio ),
                                             button );

}

void OSGWidget::mouseReleaseEvent(QMouseEvent* event)
{
    unsigned int button = 0;

    switch( event->button() )
    {
    case Qt::LeftButton:
        button = 1;
        break;

    case Qt::MiddleButton:
        button = 2;
        break;

    case Qt::RightButton:
        button = 3;
        break;

    default:
        break;
    }

    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseButtonRelease( static_cast<float>( pixelRatio * event->x() ),
                                               static_cast<float>( pixelRatio * event->y() ),
                                               button );
}

void OSGWidget::wheelEvent( QWheelEvent* event )
{
    event->accept();
    int delta = event->delta();

    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?   osgGA::GUIEventAdapter::SCROLL_UP
                                                                 : osgGA::GUIEventAdapter::SCROLL_DOWN;

    this->getEventQueue()->mouseScroll( motion );
}

void OSGWidget::on_resize( int width, int height )
{
    std::vector<osg::Camera*> cameras;
    mViewer->getCameras( cameras );

    auto pixelRatio = this->devicePixelRatio();

    cameras[0]->setViewport( 0, 0, width * pixelRatio, height * pixelRatio );
}

osgGA::EventQueue* OSGWidget::getEventQueue() const
{
    osgGA::EventQueue* eventQueue = mGraphicsWindow->getEventQueue();

    if( eventQueue )
        return eventQueue;
    else
        throw std::runtime_error( "Unable to obtain valid event queue");
}

bool OSGWidget::event( QEvent* event )
{
    bool handled = QOpenGLWidget::event( event );
    return handled;
}

osg::Camera* OSGWidget::generate_camera_viewport(int viewPortX, int viewPortY)
{
	osg::Camera* camera = new osg::Camera;
    auto pixelRatio{ this->devicePixelRatio()};
	camera->setViewport(viewPortX, viewPortY, this->width() * pixelRatio, this->height() * pixelRatio);
	return camera;
}

void OSGWidget::change_camera_color_and_transparency(osg::Camera* camera, osg::Vec4 clearColor)
{
    camera->setClearColor( clearColor );
}

void OSGWidget::change_camera_perspective(osg::Camera* camera, float fieldOfViewInY, float nearZLimit, float farZLimit)
{
    float aspectRatio = static_cast<float>(this->width()) / static_cast<float>(this->height());
    camera->setProjectionMatrixAsPerspective( fieldOfViewInY, aspectRatio, nearZLimit, farZLimit );
}

void OSGWidget::change_graphics_context(osg::Camera* camera)
{
    camera->setGraphicsContext( mGraphicsWindow );
}

void OSGWidget::setup_mView_with_camera(osg::Camera* camera)
{
    mView->setCamera( camera );
    mView->setSceneData( mRoot.get() );
    mView->addEventHandler( new osgViewer::StatsHandler );
}

void OSGWidget::generate_trackball_manipulator(osg::Vec3d positionEye, osg::Vec3d positionCenter, osg::Vec3d upVector)
{
    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;
    manipulator->setAllowThrow( false );
    manipulator->setHomePosition(positionEye, positionCenter, upVector);
    mView->setCameraManipulator( manipulator );
}

void OSGWidget::setup_threads()
{
    mViewer->addView( mView );
    mViewer->setThreadingModel( osgViewer::CompositeViewer::SingleThreaded );
    mViewer->realize();
    mView->home();
}

osg::ShapeDrawable* OSGWidget::generate_new_sphere(osg::Vec3 sphereOrigin , float sphereRadius)
{
    osg::Sphere* sphere    = new osg::Sphere( sphereOrigin , sphereRadius );
    osg::ShapeDrawable* drawnSphere = new osg::ShapeDrawable( sphere );
    drawnSphere->setName( "Sphere" );
    return drawnSphere;
}

void OSGWidget::change_object_color(osg::ShapeDrawable* drawnObject, osg::Vec4 shapeColor)
{
    drawnObject->setColor( shapeColor );
}

osg::Geode* OSGWidget::create_geometric_node(osg::ShapeDrawable * drawnObject)
{
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( drawnObject );
    return geode;
}

osg::StateSet* OSGWidget::create_state_set(osg::Geode* geode)
{
    osg::StateSet* stateSet = geode->getOrCreateStateSet();
    return stateSet;
}

osg::Material* OSGWidget::create_material()
{
    osg::Material* material = new osg::Material;
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    return material;
}

void OSGWidget::set_stateSet_mode(osg::StateSet* stateSet, osg::Material* material)
{
    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
}

void OSGWidget::transform_sphere(osg::Geode* geode, PositionNodes* positionNode)
{
    osg::PositionAttitudeTransform *transform = new osg::PositionAttitudeTransform;
    transform->setUpdateCallback(new SphereUpdateCallback(positionNode));
    transform->addChild(geode);
    mRoot->addChild(transform);
}

void OSGWidget::set_widget_focus_policy()
{
    this->setFocusPolicy( Qt::StrongFocus );
}

void OSGWidget::set_widget_minimum_size(int xMinimumSize, int yMinimumSize)
{
    this->setMinimumSize( xMinimumSize, yMinimumSize );
}

void OSGWidget::set_mouse_tracking_on()
{
    this->setMouseTracking( true );
}

void OSGWidget::update_widget()
{
    if(mPause)
    {
        this->update();
    }
    else
    {
    this->update();
    }
}

void OSGWidget::draw_position_nodes()
{
    double nodeRadius{.15};

    int length{mSpaceBoard->get_board_length()};
    int width{mSpaceBoard->get_board_width()};
    int height{mSpaceBoard->get_board_height()};

    for(int lengthIterator{0}; lengthIterator < length; lengthIterator++)
    {
        for(int widthIterator{0}; widthIterator < width; widthIterator++)
        {
            for(int heightIterator{0}; heightIterator < height; heightIterator++)
            {
                PositionNodes* currentNode{mSpaceBoard->get_node_pointer(lengthIterator,widthIterator,heightIterator)};
                Vector3d position{currentNode->get_position()};
                osg::Vec3 sphereOrigin{ osg::Vec3(position.get_x_value(),
                                                  position.get_y_value(),
                                                  position.get_z_value())};
                osg::ShapeDrawable* drawnSphere{generate_new_sphere(sphereOrigin, nodeRadius)};

                double* color{currentNode->get_color()};
                osg::Vec4 nodeColorRGBA{ osg::Vec4(color[0],
                                       color[1],
                                       color[2],
                                       1.0f)};
                change_object_color(drawnSphere, nodeColorRGBA);

                osg::Geode* geode(create_geometric_node(drawnSphere));
                osg::StateSet* stateSet(create_state_set(geode));
                osg::Material* material{create_material()};
                set_stateSet_mode(stateSet, material);

                transform_sphere(geode, currentNode);
            }
        }
    }
}
