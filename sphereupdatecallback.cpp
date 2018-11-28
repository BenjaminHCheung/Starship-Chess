#include "sphereupdatecallback.h"

SphereUpdateCallback::SphereUpdateCallback(SpaceBoard* spaceBoard)
{
    mSpaceBoard = spaceBoard;
}

void SphereUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nodevisitor)
{
    //double* newColor{mPositionNode->get_color()};
    //osg::Vec4 color{osg::Vec4(newColor[0], newColor[1], newColor[2], 1.0f)};
    traverse(node, nodevisitor);
}
