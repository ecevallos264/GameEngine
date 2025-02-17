//
// Created by eceva on 2/5/2025.
//

#ifndef GAMEENGINE_OCTTREENODE_H
#define GAMEENGINE_OCTTREENODE_H

#define MIN_SIZE 1

#include <queue>

#include "BoundingBox.h"
#include "List.h"
#include "../../entitities/Shape.h"

class OctTreeNode {
private:
    BoundingBox* region;
    List<Shape *> objects = List<Shape*>();

    std::queue<Shape *> pendingInsertion;

    OctTreeNode* children[8];

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

    OctTreeNode(BoundingBox* region, List<Shape*> entities);
    OctTreeNode();
    OctTreeNode(BoundingBox* region);

    void build();

    void render(glm::mat4 mat1, glm::mat4 mat2);

public:
    void updateTree();
    void update(double delta);
    bool insert(Shape* entity);

private:
    OctTreeNode* CreateNode(BoundingBox* region, List<Shape*> objList);
    OctTreeNode* CreateNode(BoundingBox* region, Shape* Item);

};


#endif //GAMEENGINE_OCTTREENODE_H
