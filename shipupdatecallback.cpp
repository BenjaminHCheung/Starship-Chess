#include "shipupdatecallback.h"

ShipUpdateCallback::ShipUpdateCallback(Starship* starship)
{
    mStarship = starship;
}

void ShipUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nodevisitor)
{
    if(mStarship->get_Defense() <= 0)
    {
        //osg::Geode* myGeode{node->asGeode()};
        //osg::ShapeDrawable myDrawable{myGeode->getDrawable()}
    }
    Vector3d newPosition{mStarship->get_position_node()->get_position()};
    osg::Vec3d position(newPosition.get_x_value(),newPosition.get_y_value(),newPosition.get_z_value());
    osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *>(node);
    pat->setPosition(position);
    traverse(node, nodevisitor);
}
