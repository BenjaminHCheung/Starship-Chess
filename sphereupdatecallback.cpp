#include "sphereupdatecallback.h"

SphereUpdateCallback::SphereUpdateCallback(PositionNodes* positionNode)
{
    mPositionNode = positionNode;
}

void SphereUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nodevisitor)
{
    //double* newColor{mPositionNode->get_color()};
    //osg::Vec4 color{osg::Vec4(newColor[0], newColor[1], newColor[2], 1.0f)};
    //Need to add color change
    traverse(node, nodevisitor);
}
