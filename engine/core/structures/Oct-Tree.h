//
// Created by eceva on 2/2/2025.
//

#ifndef GAMEENGINE_OCT_TREE_H
#define GAMEENGINE_OCT_TREE_H

#include "../../rendering/BoundingRegion.h"
#include "List.h"
#include "BoundingBox.h"

struct Node {
    List<BoundingRegion*> objects;
    char active;
};

enum class Octants : unsigned char {
    O1 = 0x01,
    O2 = 0x02,
    O3 = 0x04,
    O4 = 0x08,
    O5 = 0x10,
    O6 = 0x20,
    O7 = 0x40,
    O8 = 0x80,
};



class OctTree {

};

#endif //GAMEENGINE_OCT_TREE_H
