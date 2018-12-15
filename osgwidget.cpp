#include "osgwidget.h"

#include <osg/Camera>
#include <osg/DisplaySettings>
#include <osg/StateSet>
#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/BlendFunc>

#include <QKeyEvent>
#include <QPainter>
#include <QWheelEvent>
#include <vector>
#include <string>

#include "sphereupdatecallback.h"
#include "shipupdatecallback.h"

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

    osg::Vec3d positionEye{osg::Vec3d(0.0,22.0,3.0)};
    osg::Vec3d positionCenter{osg::Vec3d(0,0,0)};
    osg::Vec3d upVector{osg::Vec3d(0,0,1)};
    generate_trackball_manipulator(positionEye, positionCenter, upVector);
    setup_threads();

    draw_position_nodes();
    draw_position_grid();
    mSpaceBoard->generate_default_lists();
    build_object_lists();

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

void OSGWidget::add_ship_team_one(int shipClass, int xPosition, int yPosition, int zPosition)
{
    int teamNumber{1};
    mSpaceBoard->add_starship(teamNumber, shipClass, xPosition, yPosition, zPosition);
    Starship* newShip{(*mSpaceBoard->get_team_one_ships())[mSpaceBoard->get_team_one_ships()->size() - 1]};
    build_team_one_ship(newShip);
}

void OSGWidget::add_ship_team_two(int shipClass, int xPosition, int yPosition, int zPosition)
{
    int teamNumber{2};
    mSpaceBoard->add_starship(teamNumber, shipClass, xPosition, yPosition, zPosition);
    Starship* newShip{(*mSpaceBoard->get_team_two_ships())[mSpaceBoard->get_team_two_ships()->size() - 1]};
    build_team_two_ship(newShip);
}

void OSGWidget::clear_team_one()
{
    mSpaceBoard->clear_team_one();
    clear_team_one_drawables();
}

void OSGWidget::clear_team_two()
{
    mSpaceBoard->clear_team_two();
    clear_team_two_drawables();
}

void OSGWidget::add_planet(int size, int xPosition, int yPosition, int zPosition)
{
    mSpaceBoard->add_stellar_body(size, xPosition, yPosition, zPosition);
    StellarBody* newPlanet{(*mSpaceBoard->get_stellar_body_lists())[mSpaceBoard->get_stellar_body_lists()->size() - 1]};
    create_planet_from_list(newPlanet);
}

void OSGWidget::clear_planets()
{
    mSpaceBoard->clear_planet_list();
    clear_planet_drawables();
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

void OSGWidget::transform_sphere(osg::Geode* geode)
{
    osg::PositionAttitudeTransform *transform = new osg::PositionAttitudeTransform;
    transform->setUpdateCallback(new SphereUpdateCallback(mSpaceBoard));
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
    this->update();
}

void OSGWidget::draw_position_nodes()
{
    double nodeRadius{.05};
    float transparancy{.1};

    osg::Geode* positionGeode = new osg::Geode;

    int length{mSpaceBoard->get_board_length()};
    int width{mSpaceBoard->get_board_width()};
    int height{mSpaceBoard->get_board_height()};

    osg::Vec3 sphereOrigin{ osg::Vec3(0.f, 0.f, 0.f)};
    osg::ShapeDrawable* drawnSphere{generate_new_sphere(sphereOrigin, .8f)};

    osg::Vec4 nodeColorRGBA{ osg::Vec4(1.0f, 1.0f, 0.f, 1.0f)};
    change_object_color(drawnSphere, nodeColorRGBA);
    positionGeode->addDrawable(drawnSphere);

    for(int lengthIterator{0}; lengthIterator < length; lengthIterator++)
    {
        for(int widthIterator{0}; widthIterator < width; widthIterator++)
        {
            for(int heightIterator{0}; heightIterator < height; heightIterator++)
            {
                PositionNodes* currentNode{mSpaceBoard->get_node_pointer(lengthIterator,widthIterator,heightIterator)};
                Vector3d position{currentNode->get_position()};
                osg::Vec3 sphereOrigin( osg::Vec3(position.get_x_value(),
                                                  position.get_y_value(),
                                                  position.get_z_value()));
                osg::ShapeDrawable* drawnSphere{generate_new_sphere(sphereOrigin, nodeRadius)};

                double* color{currentNode->get_color()};
                osg::Vec4 nodeColorRGBA{ osg::Vec4(color[0],
                                       color[1],
                                       color[2],
                                       transparancy)};
                change_object_color(drawnSphere, nodeColorRGBA);
                positionGeode->addDrawable(drawnSphere);
            }
        }
    }
    osg::StateSet* stateSet(create_state_set(positionGeode));
    osg::Material* material{create_material()};
    set_stateSet_mode(stateSet, material);

    transform_sphere(positionGeode);
}

void OSGWidget::draw_position_grid()
{
    int length{mSpaceBoard->get_board_length()};
    int width{mSpaceBoard->get_board_width()};
    int height{mSpaceBoard->get_board_height()};

    osg::Geometry* geom = new osg::Geometry;

    int totalNodes{length + width + height};
    int totalVertecies{totalNodes * 3};
    osg::Vec3Array* vertexArray = new osg::Vec3Array;
    vertexArray->resize(totalVertecies);

    osg::Vec4 color{osg::Vec4(1.f, 1.f, 1.f, 1.f)};
    osg::Vec4Array* cubeColor = new osg::Vec4Array;
    cubeColor->push_back(color);
    geom->setColorArray(cubeColor, osg::Array::BIND_OVERALL);

    for (int xIterator{0}; xIterator < length; xIterator++)
    {
        add_x_line_to_grid(vertexArray, geom, xIterator, width, height);
    }
    for (int yIterator{0}; yIterator < width; yIterator++)
    {
        add_y_line_to_grid(vertexArray, geom, yIterator, length, height);
    }
    for (int zIterator{0}; zIterator < height; zIterator++)
    {
        add_z_line_to_grid(vertexArray, geom, zIterator, length, width);
    }

    geom->setVertexArray(vertexArray);

    for (int iterator{0}; iterator < totalNodes; iterator++)
    {
        int primaryPosition{iterator * 3};
        int secondaryPosition{(iterator * 3) + 1};
        int tertiaryPosition{(iterator * 3) + 2};
        GLushort idxLines[4] = {primaryPosition, secondaryPosition, primaryPosition, tertiaryPosition};
        geom->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::LINES, 4, idxLines));
    }

    osg::Geode* geode = new osg::Geode;

    geode->addDrawable(geom);
    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
    geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    mRoot->addChild(geode);
}

void OSGWidget::add_x_line_to_grid(osg::Vec3Array* vertexArray, osg::Geometry* geom, int iterator, int yMax, int zMax)
{
    int primaryPosition{iterator * 3};
    int secondaryPosition{iterator * 3 + 1};
    int tertiaryPosition{iterator * 3 + 2};

    PositionNodes* primaryNode{mSpaceBoard->get_node_pointer(iterator, 0, 0)};
    (*vertexArray)[primaryPosition].set(primaryNode->get_position().get_x_value(),
                                        primaryNode->get_position().get_y_value(),
                                        primaryNode->get_position().get_z_value());

    PositionNodes* secondaryNode{mSpaceBoard->get_node_pointer(iterator, (yMax-1), 0)};
    (*vertexArray)[secondaryPosition].set(secondaryNode->get_position().get_x_value(),
                                          secondaryNode->get_position().get_y_value(),
                                          secondaryNode->get_position().get_z_value());

    PositionNodes* tertiaryNode{mSpaceBoard->get_node_pointer(iterator, 0, (zMax - 1))};
    (*vertexArray)[tertiaryPosition].set(tertiaryNode->get_position().get_x_value(),
                                         tertiaryNode->get_position().get_y_value(),
                                         tertiaryNode->get_position().get_z_value());
}

void OSGWidget::add_y_line_to_grid(osg::Vec3Array* vertexArray, osg::Geometry* geom, int iterator, int xMax, int zMax)
{
    int primaryPosition{(iterator + xMax) * 3};
    int secondaryPosition{(iterator + xMax) * 3 + 1};
    int tertiaryPosition{(iterator + xMax) * 3 + 2};

    PositionNodes* primaryNode{mSpaceBoard->get_node_pointer(0, iterator, 0)};
    (*vertexArray)[primaryPosition].set(primaryNode->get_position().get_x_value(),
                                        primaryNode->get_position().get_y_value(),
                                        primaryNode->get_position().get_z_value());

    PositionNodes* secondaryNode{mSpaceBoard->get_node_pointer((xMax - 1), iterator, 0)};
    (*vertexArray)[secondaryPosition].set(secondaryNode->get_position().get_x_value(),
                                          secondaryNode->get_position().get_y_value(),
                                          secondaryNode->get_position().get_z_value());

    PositionNodes* tertiaryNode{mSpaceBoard->get_node_pointer(0, iterator, (zMax - 1))};
    (*vertexArray)[tertiaryPosition].set(tertiaryNode->get_position().get_x_value(),
                                         tertiaryNode->get_position().get_y_value(),
                                         tertiaryNode->get_position().get_z_value());
}

void OSGWidget::add_z_line_to_grid(osg::Vec3Array* vertexArray, osg::Geometry* geom, int iterator, int xMax, int yMax)
{
    int primaryPosition{(iterator + xMax + yMax) * 3};
    int secondaryPosition{(iterator + xMax + yMax) * 3 + 1};
    int tertiaryPosition{(iterator + xMax + yMax) * 3 + 2};

    PositionNodes* primaryNode{mSpaceBoard->get_node_pointer(0, 0, iterator)};
    (*vertexArray)[primaryPosition].set(primaryNode->get_position().get_x_value(),
                                        primaryNode->get_position().get_y_value(),
                                        primaryNode->get_position().get_z_value());

    PositionNodes* secondaryNode{mSpaceBoard->get_node_pointer((xMax - 1), 0, iterator)};
    (*vertexArray)[secondaryPosition].set(secondaryNode->get_position().get_x_value(),
                                          secondaryNode->get_position().get_y_value(),
                                          secondaryNode->get_position().get_z_value());

    PositionNodes* tertiaryNode{mSpaceBoard->get_node_pointer(0, (yMax - 1), iterator)};
    (*vertexArray)[tertiaryPosition].set(tertiaryNode->get_position().get_x_value(),
                                         tertiaryNode->get_position().get_y_value(),
                                         tertiaryNode->get_position().get_z_value());
}

void OSGWidget::create_planet_from_list(StellarBody* myPlanet)
{
    double radius{myPlanet->get_radius()};
    const std::string textureName{myPlanet->get_texture_name()};
    PositionNodes* position{myPlanet->get_position_node()};

    osg::Geode* planetGeode{create_planet(radius, textureName, position)};

    mRoot->addChild(planetGeode);
    mPlanetGeodeList.push_back(planetGeode);
}

osg::Geode* OSGWidget::create_planet(double radius, const std::string textureName, PositionNodes* position)
{
    osg::Geometry *planetSphere = new osg::Geometry;
    osg::Vec3 center{osg::Vec3(position->get_position().get_x_value(),
                               position->get_position().get_y_value(),
                               position->get_position().get_z_value())};

    osg::Vec4Array* colors = new osg::Vec4Array(1);
    planetSphere->setColorArray(colors, osg::Array::BIND_OVERALL);

    unsigned int numX{100};
    unsigned int numY{50};
    unsigned int numVertices{numX*numY};

    osg::Vec3Array* coords = new osg::Vec3Array(numVertices);
    planetSphere->setVertexArray(coords);

    osg::Vec3Array* normals = new osg::Vec3Array(numVertices);
    planetSphere->setNormalArray(normals, osg::Array::BIND_PER_VERTEX);

    osg::Vec2Array* texcoords = new osg::Vec2Array(numVertices);
    planetSphere->setTexCoordArray(0,texcoords);
    planetSphere->setTexCoordArray(1,texcoords);

    double delta_elevation = osg::PI / (double)(numY-1);
    double delta_azim = 2.0*osg::PI / (double)(numX-1);
    float delta_tx = 1.0 / (float)(numX-1);
    float delta_ty = 1.0 / (float)(numY-1);

    double elevation = -osg::PI*0.5;
    float ty = 0.0;
    unsigned int vert = 0;
    unsigned j;
    for(j=0; j<numY; ++j, elevation+=delta_elevation, ty+=delta_ty )
    {
        double azim = 0.0;
        float tx = 0.0;
        for(unsigned int i=0;
            i<numX;
            ++i, ++vert, azim+=delta_azim, tx+=delta_tx)
        {
            osg::Vec3 direction(cos(azim)*cos(elevation), sin(azim)*cos(elevation), sin(elevation));
            (*coords)[vert].set(direction*radius + center);
            (*normals)[vert].set(direction);
            (*texcoords)[vert].set(tx,ty);
        }
    }

    for(j=0; j<numY-1; ++j)
    {
        unsigned int curr_row = j*numX;
        unsigned int next_row = curr_row+numX;
        osg::DrawElementsUShort* elements = new osg::DrawElementsUShort(GL_QUAD_STRIP);
        for(unsigned int i=0;
            i<numX;
            ++i)
        {
            elements->push_back(next_row + i);
            elements->push_back(curr_row + i);
        }
        planetSphere->addPrimitiveSet(elements);
    }


    osg::Geode* geodePlanet = new osg::Geode();

    if( !textureName.empty() )
    {
        osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile( textureName );
        if ( image )
        {
            osg::Texture2D* tex2d = new osg::Texture2D( image );
            tex2d->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
            tex2d->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );
            geodePlanet->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex2d, osg::StateAttribute::ON );
        }
    }

    geodePlanet->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

    geodePlanet->addDrawable( planetSphere );

    return( geodePlanet );
}

void OSGWidget::build_team_one()
{
    std::vector<Starship*>* teamOne{mSpaceBoard->get_team_one_ships()};
    unsigned long long int teamOneSize{teamOne->size()};
    for(unsigned long long int listedObject{0}; listedObject < teamOneSize; listedObject++)
    {
        Starship* currentShip{ (*teamOne)[listedObject] };
        build_team_one_ship(currentShip);
    }
}

void OSGWidget::build_team_two()
{
    std::vector<Starship*>* teamTwo{mSpaceBoard->get_team_two_ships()};
    unsigned long long int teamTwoSize{teamTwo->size()};
    for(unsigned long long int listedObject{0}; listedObject < teamTwoSize; listedObject++)
    {
        Starship* currentShip{ (*teamTwo)[listedObject] };
        build_team_two_ship(currentShip);
    }
}

void OSGWidget::build_planets()
{

    std::vector<StellarBody*>* planets{mSpaceBoard->get_stellar_body_lists()};
    unsigned long long int planetListSize{planets->size()};
    for(unsigned long long int listedObject{0}; listedObject < planetListSize; listedObject++)
    {
        StellarBody* currentPlanet{ (*planets)[listedObject] };
        create_planet_from_list(currentPlanet);
    }
}

void OSGWidget::clear_team_one_drawables()
{
    for(unsigned long long int iterator{mTeamOneGeodeList.size()}; iterator > 0; iterator--)
    {
        mRoot->removeChild(mTeamOneGeodeList[iterator - 1]);
        mTeamOneGeodeList.pop_back();
    }
}

void OSGWidget::clear_team_two_drawables()
{
    for(unsigned long long int iterator{mTeamTwoGeodeList.size()}; iterator > 0; iterator--)
    {
        mRoot->removeChild(mTeamTwoGeodeList[iterator - 1]);
        mTeamTwoGeodeList.pop_back();
    }
}
void OSGWidget::clear_planet_drawables()
{
    for(unsigned long long int iterator{mPlanetGeodeList.size()}; iterator > 0; iterator--)
    {
        mRoot->removeChild(mPlanetGeodeList[iterator -1]);
        mPlanetGeodeList.pop_back();
    }
}

void OSGWidget::build_object_lists()
{
    build_team_one();
    build_team_two();
    build_planets();
}

void OSGWidget::build_team_one_ship(Starship* myShip)
{
    osg::Vec4 teamOneColor{osg::Vec4(0.6f, 0.1f, 0.1f, 1.f)};
    build_ship(myShip, teamOneColor, &mTeamOneGeodeList);
}

void OSGWidget::build_team_two_ship(Starship* theirShip)
{
    osg::Vec4 teamTwoColor{osg::Vec4(0.1f, 0.1f, 0.6f, 1.f)};
    build_ship(theirShip, teamTwoColor, &mTeamTwoGeodeList);
}

void OSGWidget::build_ship(Starship* myStarship, osg::Vec4 colorRGBA, std::vector<osg::PositionAttitudeTransform*>* teamList)
{
    int size{myStarship->get_size()};
    double radius{size * .15};
    osg::Vec3 position(0,0,0);

    osg::ShapeDrawable* drawnShip{generate_new_sphere(position, radius)};
    change_object_color(drawnShip, colorRGBA);

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( drawnShip );
    osg::Material* material{create_material()};
    osg::StateSet* stateSet{geode->getOrCreateStateSet()};
    stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    stateSet->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);
    set_stateSet_mode(stateSet, material);

    osg::PositionAttitudeTransform *transform = new osg::PositionAttitudeTransform;
    transform->setUpdateCallback(new ShipUpdateCallback(myStarship, drawnShip));
    transform->addChild(geode);
    mRoot->addChild(transform);
    (*teamList).push_back(transform);
}
