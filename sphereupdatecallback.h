#ifndef SPHEREUPDATECALLBACK_H
#define SPHEREUPDATECALLBACK_H

#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/Node>

#include "spaceboard.h"

class SphereUpdateCallback: public osg::NodeCallback
{
public:
    SphereUpdateCallback(SpaceBoard* spaceBoard);
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nodevisitor);
protected:
    SpaceBoard* mSpaceBoard;
};

#endif // SPHEREUPDATECALLBACK_H
