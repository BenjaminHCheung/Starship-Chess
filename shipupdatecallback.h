#ifndef SHIPUPDATECALLBACK_H
#define SHIPUPDATECALLBACK_H

#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/Node>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include "starship.h"

class ShipUpdateCallback: public osg::NodeCallback
{
public:
    ShipUpdateCallback(Starship* starship);
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nodevisitor);

protected:
    Starship* mStarship;
};

#endif // SHIPUPDATECALLBACK_H
