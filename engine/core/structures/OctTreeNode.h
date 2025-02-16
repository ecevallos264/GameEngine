//
// Created by eceva on 2/5/2025.
//

#ifndef GAMEENGINE_OCTTREENODE_H
#define GAMEENGINE_OCTTREENODE_H

#define MIN_SIZE 1


#include <list>
#include <queue>

#include "BoundingBox.h"
#include "../../rendering/RenderableEntity.h"

class OctTreeNode {
private:
    BoundingBox* region;
    std::list<RenderableEntity *> objects;

    std::queue<RenderableEntity *> pendingInsertion;

    OctTreeNode* children = new OctTreeNode[8];

    unsigned char m_activeNodes = 0;

    // const int MIN_SIZE = 1;
    bool has_children() {
        return this->m_activeNodes != 0;
    }

    int lifeSpan = 8;

    int currLife = -1;

    // OctTreeNode parent;

    bool tree_built = false;
    bool tree_ready = false;
    
public:
    OctTreeNode* parent;

    OctTreeNode(BoundingBox* region, std::list<RenderableEntity*> entities);
    OctTreeNode();
    OctTreeNode(BoundingBox* region);

    void build();

private:
    void updateTree();
    void update(double delta);
    void insert(RenderableEntity* entity);
    OctTreeNode* CreateNode(BoundingBox* region, std::list<RenderableEntity*> objList);
    OctTreeNode* CreateNode(BoundingBox* region, RenderableEntity* Item);

};


#endif //GAMEENGINE_OCTTREENODE_H
