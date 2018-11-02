#ifndef SPHEREUPDATECALLBACK_H
#define SPHEREUPDATECALLBACK_H

#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/Node>
#include "physicsobject.h"

class SphereUpdateCallback: public osg::NodeCallback
{
public:
    SphereUpdateCallback(PhysicsObject* physicsObject);
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nodevisitor);
protected:
    PhysicsObject* mPhysicsObject;
};

#endif // SPHEREUPDATECALLBACK_H
