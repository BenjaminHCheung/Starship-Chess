#ifndef SPHEREUPDATECALLBACK_H
#define SPHEREUPDATECALLBACK_H

#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/Node>
#include "positionnodes.h"

class SphereUpdateCallback: public osg::NodeCallback
{
public:
    SphereUpdateCallback(PositionNodes* positionNode);
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nodevisitor);
protected:
    PositionNodes* mPositionNode;
};

#endif // SPHEREUPDATECALLBACK_H
