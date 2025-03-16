#include "octree.h"

// Constructor with region and entities


// Default constructor


// Constructor with region only


void Octree::calculateBounds(BoundingBox& out, Octant octant, BoundingBox* parentRegion) {
    // find min and max points of corresponding octant

    glm::vec3 center = region->calculateCenter();
    if (octant == Octant::O1) {
        out = BoundingBox(center, region->max);
    }
    else if (octant == Octant::O2) {
        out = BoundingBox(glm::vec3(parentRegion->min.x, center.y, center.z), glm::vec3(center.x, parentRegion->max.y, parentRegion->max.z));
    }
    else if (octant == Octant::O3) {
        out = BoundingBox(glm::vec3(parentRegion->min.x, parentRegion->min.y, center.z), glm::vec3(center.x, center.y, parentRegion->max.z));
    }
    else if (octant == Octant::O4) {
        out = BoundingBox(glm::vec3(center.x, parentRegion->min.y, center.z), glm::vec3(parentRegion->max.x, center.y, parentRegion->max.z));
    }
    else if (octant == Octant::O5) {
        out = BoundingBox(glm::vec3(center.x, center.y, parentRegion->min.z), glm::vec3(parentRegion->max.x, parentRegion->max.y, center.z));
    }
    else if (octant == Octant::O6) {
        out = BoundingBox(glm::vec3(parentRegion->min.x, center.y, parentRegion->min.z), glm::vec3(center.x, parentRegion->max.y, center.z));
    }
    else if (octant == Octant::O7) {
        out = BoundingBox(parentRegion->min, center);
    }
    else if (octant == Octant::O8) {
        out = BoundingBox(glm::vec3(center.x, parentRegion->min.y, parentRegion->min.z), glm::vec3(parentRegion->max.x, center.y, center.z));
    }
}

// Update tree structure
void Octree::updateTree() {
    if (!tree_built) {
        while (!pendingInsertion.empty()) {
            objects.add(pendingInsertion.front());
            pendingInsertion.pop();
        }
        this->build();
    } else {
        while (!pendingInsertion.empty()) {
            this->insert(pendingInsertion.front());
            pendingInsertion.pop();
        }
    }
    this->tree_ready = true;
}

bool Octree::insert(BoundingBox* obj) {
//    std::cout << "Inserting object of dimensions: " << obj->boundingBox->calculateDimensions().x << ":" << obj->boundingBox->calculateDimensions().y << ":" << obj->boundingBox->calculateDimensions().z << std::endl;
    if (objects.empty() && m_activeNodes == 0) {
        objects.add(obj);
//        obj.cell = this;
        return true;
    }

    glm::vec3 dimensions = region->calculateDimensions();
    if (dimensions.x <= MIN_SIZE && dimensions.y <= MIN_SIZE && dimensions.z <= MIN_SIZE) {
        objects.add(obj);
//        obj.cell = this;
        return true;
    }

    if (!region->contains(obj->boundingBox)) {
        // If this is the root node, enlarge the octree.
        if (parent == nullptr) {
            // enlargeTreeToFit(obj->boundingBox);
            std::cout << "Need to enlarge root bounding box." << std::endl;
            return insert(obj);
        } else {
            return parent->insert(obj);
        }
    }


    BoundingBox octants[8];
    List<RigidBody*> octLists[8];

    for (int i = 0; i < 8; i++) {
        calculateBounds(octants[i], (Octant)(1 << i), region);
    }

    for (RigidBody* currentObj : objects) {
        for (int i = 0; i < 8; i++) {
            if (octants[i].contains(currentObj->boundingBox)) {
                octLists[i].add(currentObj);
                break;
            }
        }
    }

    objects.clear();

    for (int i = 0; i < 8; i++) {
        if (!octLists[i].empty()) {
            if (children[i]) {
                for (RigidBody* br : octLists[i]) {
                    children[i]->insert(br);
                }
            } else {
                children[i] = CreateNode(octants[i], octLists[i]);
                m_activeNodes |= (1 << i);
                children[i]->build();
            }
        }
    }

    return true;
}

// Build the octree
void Octree::build() {
    std::cout << "Building octree..." << std::endl;
    if (objects.empty()) return;

    glm::vec3 dimensions = abs(region->max - region->min);
    if (dimensions.x < MIN_SIZE || dimensions.y < MIN_SIZE || dimensions.z < MIN_SIZE) {
        std::cout << "Octree dimensions too small[" << dimensions.x << ":" << dimensions.y << ":" << dimensions.z <<  "], not building children." << std::endl;
        return;
    }

    BoundingBox octants[8];
    for (int i = 0; i < 8; i++) {
        calculateBounds(octants[i], (Octant)(1 << i), region);
    }

    List<RigidBody*> octLists[8];
    List<RigidBody*> deList;

    for (RigidBody* entity : objects) {
        for (int i = 0; i < 8; i++) {
            if (octants[i].contains(entity->boundingBox)) {
                octLists[i].add(entity);
                deList.add(entity);
                break;
            }
        }
    }

    for (RigidBody* entity : deList) {
        objects.remove(entity);
    }

    for (int i = 0; i < 8; i++) {
        if (!octLists[i].empty()) {
            children[i] = new Octree(&octants[i], octLists[i]);
            m_activeNodes |= (1 << i);
            children[i]->parent = this;
            children[i]->build();
        }
    }

    int activeNodes = 0;
    for (int i = 0; i < 8; i++) {
        if (children[i] != nullptr) {
            activeNodes++;
        }
    }
    this->tree_built = true;
    this->tree_ready = true;
}

// Create a new octree node
Octree* Octree::CreateNode(BoundingBox region, List<RigidBody*> objList) {
    if (objList.empty()) return nullptr;
    Octree* ret = new Octree(&region, objList);
    ret->parent = this;
    return ret;
}

Octree* Octree::CreateNode(BoundingBox region, RigidBody* Item) {
    List<RigidBody*> objList;
    objList.add(Item);
    Octree* ret = new Octree(&region, objList);
    ret->parent = this;
    return ret;
}

// Update the octree
void Octree::update(double delta) {
    if (!tree_ready || !tree_built) {
        if (!pendingInsertion.empty()) {
            processPending();
            update(delta);
        }
        return;
    }

    if (objects.empty() && !has_children()) {
        currLife = (currLife == -1) ? lifeSpan : std::max(0, currLife - 1);
    }

//    for (auto it = objects.begin(); it != objects.end();) {
//        if (!(*it)->isAlive) {
//            it = objects.remove(it);
//        } else {
//            ++it;
//        }
//    }

    for (auto obj : objects) {
        obj->update(delta);
    }

    for (int i = 0; i < 8; i++) {
        if ((m_activeNodes & (1 << i)) && children[i] != nullptr) {
            children[i]->update(delta);
        }
    }
}

// Process pending insertions
void Octree::processPending() {
    if (!tree_built) {
        std::cout << "Tried to process pending insertions before octree was built." << std::endl;
        while (!pendingInsertion.empty()) {
            objects.add(pendingInsertion.front());
            pendingInsertion.pop();
        }
        build();
    } else {
        int size = pendingInsertion.size();
        if (size > 0) {
            std::cout << "Processing pending insertions... size: " << pendingInsertion.size() << std::endl;
        }
        for (int i = 0; i < size; i++) {
            RigidBody* front = pendingInsertion.front();
            pendingInsertion.pop();
            if (region->contains(front->boundingBox)) {
                insert(front);
            } else {
                pendingInsertion.push(front);
            }
        }
    }
}

void Octree::addToPending(RigidBody *shape) {
    pendingInsertion.push(shape);
}

void Octree::render(glm::mat4 mat1, glm::mat4 mat2) {
//    if(!tree_ready || !tree_built) {
//        return;
//    }
    region->setColor(glm::vec3(1, 0, 0));
//    region->render(mat1, mat2);
    for(RigidBody* shape: this->objects) {
        shape->boundingBox->render(mat1, mat2);
//        shape->render(mat1, mat2);
    }

    for (int i = 0; i < 8; i++) {
        if ((m_activeNodes & (1 << i)) && children[i] != nullptr) {
            children[i]->render(mat1, mat2);
        }
    }
}
