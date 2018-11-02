#ifndef MEEN_570_OSGWIDGET
#define MEEN_570_OSGWIDGET

#include <QOpenGLWidget>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>
#include <osgGA/TrackballManipulator>
#include <osgText/Text>

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Material>
#include <string>

#include "physicsspace.h"

class OSGWidget : public QOpenGLWidget
{
  Q_OBJECT

public:
  OSGWidget( QWidget* parent = 0,
             Qt::WindowFlags f = 0 );

  virtual ~OSGWidget();

  void set_pause(bool pause);

public slots:
  void apply_settings(unsigned int numberOfObjects,
                       double xGravity, double yGravity, double zGravity,
                       double fluidDensity, double maxRadius, double minRadius,
                       double massMax, double massMin, double CrMax,
                       double CrMin, double velocityMax, double velocityMin);

protected:
  void timerEvent(QTimerEvent *);
  virtual void paintEvent( QPaintEvent* paintEvent );
  virtual void paintGL();
  virtual void resizeGL( int width, int height );

  virtual void keyPressEvent( QKeyEvent* event );
  virtual void keyReleaseEvent( QKeyEvent* event );

  virtual void mouseMoveEvent( QMouseEvent* event );
  virtual void mousePressEvent( QMouseEvent* event );
  virtual void mouseReleaseEvent( QMouseEvent* event );
  virtual void wheelEvent( QWheelEvent* event );
  virtual bool event( QEvent* event );


private:
  virtual void on_resize( int width, int height );
  osgGA::EventQueue* getEventQueue() const;

  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mGraphicsWindow;
  osg::ref_ptr<osgViewer::CompositeViewer> mViewer;
  osg::ref_ptr<osgViewer::View> mView;
  osg::ref_ptr<osg::Group> mRoot;
  int mTimerId{0};
  PhysicsSpace* mPhysicsSpace;
  bool mPause{true};

  osg::Camera* generate_camera_viewport(int viewPortX, int viewPortY);
	  void change_camera_color_and_transparency(osg::Camera* camera, osg::Vec4 clearColor);
      void change_camera_perspective(osg::Camera* camera, float fieldOfViewInY, float nearZLimit, float farZLimit);
	  void change_graphics_context(osg::Camera* camera);
      void setup_mView_with_camera(osg::Camera* camera);
      void generate_trackball_manipulator(osg::Vec3d positionEye, osg::Vec3d positionCenter, osg::Vec3d upVector);
	  void setup_threads();
      osg::ShapeDrawable* generate_new_sphere(osg::Vec3 sphereOrigin , float sphereRadius);
      void change_object_color(osg::ShapeDrawable* drawnObject, osg::Vec4 shapeColor);
      osg::Geode* create_geometric_node(osg::ShapeDrawable * drawnObject);
	  osg::StateSet* create_state_set(osg::Geode* geode);
	  osg::Material* create_material();
      void set_stateSet_mode(osg::StateSet* stateSet, osg::Material* material);
      void transform_sphere(osg::Geode* geode, SphereObject* physicsObject);
      void add_child_to_root(osg::Geode* geode);
      void set_widget_focus_policy();
      void set_widget_minimum_size(int xMinimumSize, int yMinimumSize);
      void set_mouse_tracking_on();
      void update_widget();
      void generate_cube_wireframe();
      void draw_object_list();
};

#endif
