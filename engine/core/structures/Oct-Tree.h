// Oct-Tree.h
#ifndef GAMEENGINE_OCT_TREE_H
#define GAMEENGINE_OCT_TREE_H

#include "../../rendering/BoundingRegion.h"
#include "List.h"
#include "BoundingBox.h"
#include "OctTreeNode.h"
#include <memory>

class OctTree : public Singleton<OctTree> {
    OctTreeNode* root;
public:
    OctTree() : Singleton<OctTree>() {}

    OctTreeNode* getRoot() {
        return root;
    }

    void insert(Shape* entity) {
        if (root == nullptr) {
            List<Shape*> list = List<Shape*>();
            list.add(entity);
            root = new OctTreeNode(entity->boundingBox, list);
            root->updateTree();
        } else {
            root->insert(entity);
        }
    }
};

#endif //GAMEENGINE_OCT_TREE_H