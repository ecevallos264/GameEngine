//
// Created by eceva on 2/5/2025.
//

#include "OctTreeNode.h"

#include <windows.h>

OctTreeNode::OctTreeNode(BoundingBox* region, List<Shape *> entities) {
    this->region = region;
    this->objects = entities;
    this->currLife = -1;
}

OctTreeNode::OctTreeNode() {
    this->region = new BoundingBox(glm::vec3(0), glm::vec3(0));
    this->objects = List<Shape *>();
    this->currLife = -1;
}

OctTreeNode::OctTreeNode(BoundingBox *region) {
    this->region = region;
    this->objects = List<Shape *>();
    this->currLife = -1;
}

void OctTreeNode::updateTree() {
    if (!tree_built) {
        while (!pendingInsertion.empty())
            objects.add(pendingInsertion.front());
        this->build();
    } else {
        while (!pendingInsertion.empty())
            this->insert(pendingInsertion.front());
    }
    this->tree_ready = true;
}

bool OctTreeNode::insert(Shape* entity) {
    if(!objects.empty() && objects.size() == 0 && m_activeNodes == 0) {
        objects.add(entity);
        return true;
    }

    glm::vec3 dimensions = region->max - region->min;
    if(dimensions.x <= MIN_SIZE && dimensions.y <= MIN_SIZE && dimensions.z <= MIN_SIZE) {
        objects.add(entity);
        return true;
    }

    if(!region->contains(entity->boundingBox)) {
        if(this->parent != nullptr)
            return this->parent->insert(entity);
        else
            return false;
    }

    glm::vec3 half = dimensions / 2.0f;
    glm::vec3 center = this->region->min + half;

    BoundingBox* octants[8];
    octants[0] = new BoundingBox(region->min, center);
    octants[1] = new BoundingBox(glm::vec3(center.x, region->min.y, region->min.z), glm::vec3(region->max.x, center.y, center.z));
    octants[2] = new BoundingBox(glm::vec3(center.x, region->min.y, center.z), glm::vec3(region->max.x, center.y, region->max.z));
    octants[3] = new BoundingBox(glm::vec3(region->min.x, region->min.y, center.z), glm::vec3(center.x, center.y, region->max.z));
    octants[4] = new BoundingBox(glm::vec3(region->min.x, center.y, region->min.z), glm::vec3(center.x, region->max.y, center.z));
    octants[5] = new BoundingBox(glm::vec3(center.x, center.y, region->min.z), glm::vec3(region->max.x, region->max.y, center.z));
    octants[6] = new BoundingBox(center, region->max);
    octants[7] = new BoundingBox(glm::vec3(region->min.x, center.y, center.z), glm::vec3(center.x, region->max.y, region->max.z));

    if(entity->boundingBox->max != entity->boundingBox->min && region->contains(entity->boundingBox)) {
        bool found = false;
        for (int i = 0; i < 8; i++) {
            if(octants[i]->contains(entity->boundingBox)) {
                if(children[i] != nullptr) {
                    return children[i]->insert(entity);
                } else {
                    children[i] = CreateNode(octants[i], entity);
                    m_activeNodes |= (unsigned char)(1 << i);
                }
                found = true;
            }
        }

        if(!found) {
            objects.add(entity);
            return true;
        }
    }
    return false;
}

void OctTreeNode::build() {
    std::cout << "Building Tree" << std::endl;
    if (this->objects.empty()) {
        std::cout << "No objects in node" << std::endl;
        return;
    }

    glm::vec3 dimensions = this->region->max - this->region->min;

    if (dimensions == glm::vec3(0)) {
        // FindEnclosingCube();
        dimensions = this->region->max - this->region->min;
    }

    if (dimensions.x < MIN_SIZE || dimensions.y < MIN_SIZE || dimensions.z < MIN_SIZE) {
        std::cout << "Node too small Dimensions: " << dimensions.x << ":" << dimensions.y << ";" << dimensions.z << std::endl;
        return;
    }

    glm::vec3 half = dimensions / 2.0f;
    glm::vec3 center = this->region->min + half;

    BoundingBox* octants[8];
    octants[0] = new BoundingBox(shader, region->min, center);
    octants[1] = new BoundingBox(glm::vec3(center.x, region->min.y, region->min.z), glm::vec3(region->max.x, center.y, center.z));
    octants[2] = new BoundingBox(glm::vec3(center.x, region->min.y, center.z), glm::vec3(region->max.x, center.y, region->max.z));
    octants[3] = new BoundingBox(glm::vec3(region->min.x, region->min.y, center.z), glm::vec3(center.x, center.y, region->max.z));
    octants[4] = new BoundingBox(glm::vec3(region->min.x, center.y, region->min.z), glm::vec3(center.x, region->max.y, center.z));
    octants[5] = new BoundingBox(glm::vec3(center.x, center.y, region->min.z), glm::vec3(region->max.x, region->max.y, center.z));
    octants[6] = new BoundingBox(center, region->max);
    octants[7] = new BoundingBox(glm::vec3(region->min.x, center.y, center.z), glm::vec3(center.x, region->max.y, region->max.z));

    List<Shape *> octList[8];
    for (int i = 0; i < 8; i++) {
        octList[i] = List<Shape *>();
    }

    List<Shape*> deList = List<Shape*>();
    for (Shape* entity: this->objects) {
        if (entity->boundingBox->min != entity->boundingBox->max) {
            for (int i = 0; i < 8; i++) {
                if (octants[i]->contains(entity->boundingBox)) {
                    octList[i].add(entity);
                    deList.add(entity);
                    break;
                }
            }
        }
    }

    for (auto entity: deList) {
        objects.remove(entity);
    }

    for (int i = 0; i < 8; i++) {
        if (!octList[i].empty()) {
            this->children[i] = CreateNode(octants[i], octList[i]);
            this->m_activeNodes |= (1 << i);
            this->children[i]->build();
        }
    }

    this->tree_built = true;
    this->tree_ready = true;
}

OctTreeNode* OctTreeNode::CreateNode(BoundingBox* region, List<Shape*> objList) {
    if (objList.size() == 0)
        return nullptr;
    OctTreeNode* ret = new OctTreeNode(region, objList);
    ret->parent = this;
    return ret;
}

OctTreeNode* OctTreeNode::CreateNode(BoundingBox* region, Shape* Item) {
    List<Shape*> objList = List<Shape*>();
    objList.add(Item);
    OctTreeNode* ret = new OctTreeNode(region, objList);
    ret->parent = this;
    return ret;
}

void OctTreeNode::update(double delta) {
//    std::cout << "Updating Tree Built: " << this->tree_built << "; Tree Ready: " << this->tree_ready << std::endl;
    if (this->tree_ready && this->tree_built) {
        if (this->objects.empty()) {
            if (!this->has_children()) {
                if (currLife == -1) {
                    currLife = lifeSpan;
                } else if (currLife > 0) {
                    currLife--;
                }
            }
        } else {
            if (currLife == -1) {
                if (lifeSpan < 64) {
                    lifeSpan *= 2;
                }
                currLife--;
            }
        }

        List<Shape*> movedEntities = List<Shape*>();
        for (Shape* entity: this->objects) {
            if (entity->boundingBox->min != entity->boundingBox->max) {
                if (entity->update(delta) == 1) {
                    movedEntities.add(entity);
                }
            }
        }

        int listSize = objects.size();
        for(int i = 0; i < listSize; i++) {
            if(!objects[i]->isAlive) {
                if(movedEntities.contains(objects[i])) {
                    movedEntities.remove(objects[i]);
                }
                objects.removeAt(i);
                listSize--;
            }
        }

        for(int flags = m_activeNodes, index = 0; flags > 0; flags >>= 1, index++) {
            if((flags & 1) == 1 && children[index]->currLife == 0) {
                if(children[index]->objects.size() > 0) {
                    children[index]->currLife = -1;
                } else {
                    children[index] = nullptr;
                    m_activeNodes ^= (unsigned char) 1 << index;
                }
            }
        }

        for(int flags = m_activeNodes, index = 0; flags > 0; flags >>= 1, index++) {
            if((flags & 1) == 1) {
                if(children != nullptr && (children[index] != nullptr) ) {
                    children[index]->update(delta);
                }
            }
        }

        for(auto movedObject: movedEntities) {
            OctTreeNode* current = this;

            if(movedObject->boundingBox->max != movedObject->boundingBox->min) {
                while(!current->region->contains(movedObject->boundingBox)) {
                    if(current->parent != nullptr)
                        current = current->parent;
                    else
                        break;
                }
            }
            objects.remove(movedObject);
            current->insert(movedObject);
        }

        if(this->parent == nullptr) {
            // Check for collisions here
        }
    } else {
        if(pendingInsertion.size() > 0) {
            // this.processPending(); // TODO Implement
            update(delta);
        }
    }
}

void OctTreeNode::render(glm::mat4 view, glm::mat4 projection) {
    for(auto item: objects) {
        item->render(view, projection);
    }
    for(int flags = m_activeNodes, index = 0; flags > 0; flags >>= 1, index++) {
        if((flags & 1) == 1) {
            if(children != nullptr && (children[index] != nullptr) ) {
                children[index]->render(view, projection);
            }
        }
    }
}





