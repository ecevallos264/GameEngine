#ifndef GAMEENGINE_OCTREE_H
#define GAMEENGINE_OCTREE_H

#include <queue>
#include "../../physics/RigidBody.h"

#define MIN_SIZE 1

enum class Octant : unsigned char {
    O1 = 0x01,	// = 0b00000001
    O2 = 0x02,	// = 0b00000010
    O3 = 0x04,	// = 0b00000100
    O4 = 0x08,	// = 0b00001000
    O5 = 0x10,	// = 0b00010000
    O6 = 0x20,	// = 0b00100000
    O7 = 0x40,	// = 0b01000000
    O8 = 0x80	// = 0b10000000
};

class Octree {
protected:
    List<BoundingBox*> objects;
    std::queue<BoundingBox*> pendingInsertion;
    Octree* children[8] = {nullptr};
    unsigned char m_activeNodes = 0;
    bool tree_built = false;
    bool tree_ready = false;
    int lifeSpan = 8;
    int currLife = -1;

    bool has_children() {
        return this->m_activeNodes != 0;
    }

public:
    Octree* parent = nullptr;
    BoundingBox* region;

    Octree() : region(new BoundingBox(glm::vec3(-0.5), glm::vec3(0.5))) {
        this->currLife = -1;
    }

    Octree(BoundingBox* region) : region(region) {
        // std::cout << "Octree created with region: " << region->min.x << ", " << region->min.y << ", " << region->min.z << std::endl;
        this->currLife = -1;
    }

    Octree(BoundingBox* region, List<BoundingBox*> entities): region(region) {
        this->objects = entities;
        this->currLife = -1;
    }

    void build();
    void render(glm::mat4 mat1, glm::mat4 mat2);
    void updateTree();
    void update(double delta);
    bool insert(BoundingBox* entity);
    void processPending();
    void addToPending(BoundingBox* shape);
    void calculateBounds(BoundingBox& out, Octant octant, BoundingBox* parentRegion);

private:
    Octree* CreateNode(BoundingBox region, List<BoundingBox*> objList);
    Octree* CreateNode(BoundingBox region, BoundingBox* Item);
};

#endif // GAMEENGINE_OCTREE_H
