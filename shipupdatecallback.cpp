#include "shipupdatecallback.h"

ShipUpdateCallback::ShipUpdateCallback(Starship* starship, osg::ShapeDrawable* drawable)
{
    mStarship = starship;
    mShape = drawable;
}

void ShipUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nodevisitor)
{
    if(mStarship->get_Defense() <= 0)
    {
        mShape->setColor(osg::Vec4(1.f, 1.f, 1.f, 0.f));
    }
    Vector3d newPosition{mStarship->get_position_node()->get_position()};
    osg::Vec3d position(newPosition.get_x_value(),newPosition.get_y_value(),newPosition.get_z_value());
    osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *>(node);
    pat->setPosition(position);
    traverse(node, nodevisitor);
}
