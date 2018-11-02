#include "sphereupdatecallback.h"

SphereUpdateCallback::SphereUpdateCallback(PhysicsObject* physicsObject)
{
    mPhysicsObject = physicsObject;
}

void SphereUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nodevisitor)
{
    Vector3d newPosition{mPhysicsObject->get_position()};
    osg::Vec3d position(newPosition.get_x_value(),newPosition.get_y_value(),newPosition.get_z_value());
    osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *>(node);
    pat->setPosition(position);
    traverse(node, nodevisitor);
}
