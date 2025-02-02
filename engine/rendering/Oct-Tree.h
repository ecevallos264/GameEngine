//
// Created by eceva on 2/2/2025.
//

#ifndef GAMEENGINE_OCT_TREE_H
#define GAMEENGINE_OCT_TREE_H

#include "BoundingRegion.h"

struct Node {
    BoundingRegion* boundingRegion;
};

class OctTree {
public:
    OctTree();
};

#endif //GAMEENGINE_OCT_TREE_H
