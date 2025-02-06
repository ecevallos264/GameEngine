//
// Created by eceva on 2/5/2025.
//

#ifndef GAMEENGINE_OCTTREENODE_H
#define GAMEENGINE_OCTTREENODE_H


#include "BoundingBox.h"
#include "../../rendering/RenderableEntity.h"
#include "List.h"

class OctTreeNode {
private:
    BoundingBox region;
    List<RenderableEntity> objects;

    std::queue<RenderableEntity> pendingInsertion;

    OctTreeNode* children = new OctTreeNode[8];

    unsigned char m_activeNodes = 0;

    const int MIN_SIZE = 1;

    int lifeSpan = 8;

    int currLife = -1;

    OctTreeNode parent;
    
public:
    OctTreeNode(BoundingBox region, List<RenderableEntity*> entities) {}
    OctTreeNode() {}
    OctTreeNode(BoundingBox region) {}

    void build() {}
};


#endif //GAMEENGINE_OCTTREENODE_H
