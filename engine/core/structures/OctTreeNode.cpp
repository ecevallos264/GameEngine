//
// Created by eceva on 2/5/2025.
//

#include "OctTreeNode.h"

#include <windows.h>

OctTreeNode::OctTreeNode(BoundingBox* region, std::list<RenderableEntity *> entities) {
    this->region = region;
    this->objects = entities;
    this->currLife = -1;
}

OctTreeNode::OctTreeNode() {
    this->region = new BoundingBox(glm::vec3(0), glm::vec3(0));
    this->objects = std::list<RenderableEntity *>();
    this->currLife = -1;
}

OctTreeNode::OctTreeNode(BoundingBox *region) {
    this->region = region;
    this->objects = std::list<RenderableEntity *>();
    this->currLife = -1;
}

void OctTreeNode::updateTree() {
    if (!tree_built) {
        while (!pendingInsertion.empty())
            objects.push_back(pendingInsertion.front());
        this->build();
    } else {
        while (!pendingInsertion.empty())
            this->insert(pendingInsertion.front());
    }
    this->tree_ready = true;
}

void OctTreeNode::insert(RenderableEntity* entity) {
}

void OctTreeNode::build() {
    if (this->objects.empty()) {
        return;
    }

    glm::vec3 dimensions = this->region->max - this->region->min;

    if (dimensions == glm::vec3(0)) {
        // FindEnclosingCube();
        dimensions = this->region->max - this->region->min;
    }

    if (dimensions.x < MIN_SIZE || dimensions.y < MIN_SIZE || dimensions.z < MIN_SIZE) {
        return;
    }

    glm::vec3 half = dimensions / 2.0f;
    glm::vec3 center = this->region->min + half;

    /*
    BoundingBox[] octant = new BoundingBox[8];
    octant[0] = new BoundingBox(m_region.Min, center);
    octant[1] = new BoundingBox(new Vector3(center.X, m_region.Min.Y, m_region.Min.Z), new Vector3(m_region.Max.X, center.Y, center.Z));
    octant[2] = new BoundingBox(new Vector3(center.X, m_region.Min.Y, center.Z), new Vector3(m_region.Max.X, center.Y, m_region.Max.Z));
    octant[3] = new BoundingBox(new Vector3(m_region.Min.X, m_region.Min.Y, center.Z), new Vector3(center.X, center.Y, m_region.Max.Z));
    octant[4] = new BoundingBox(new Vector3(m_region.Min.X, center.Y, m_region.Min.Z), new Vector3(center.X, m_region.Max.Y, center.Z));
    octant[5] = new BoundingBox(new Vector3(center.X, center.Y, m_region.Min.Z), new Vector3(m_region.Max.X, m_region.Max.Y, center.Z));
    octant[6] = new BoundingBox(center, m_region.Max);
    octant[7] = new BoundingBox(new Vector3(m_region.Min.X, center.Y, center.Z), new Vector3(center.X, m_region.Max.Y, m_region.Max.Z));*/

    BoundingBox* octants[8];
    octants[0] = new BoundingBox(region->min, center);
    octants[1] = new BoundingBox(glm::vec3(center.x, region->min.y, region->min.z), glm::vec3(region->max.x, center.y, center.z));
    octants[2] = new BoundingBox(glm::vec3(center.x, region->min.y, center.z), glm::vec3(region->max.x, center.y, region->max.z));
    octants[3] = new BoundingBox(glm::vec3(region->min.x, region->min.y, center.z), glm::vec3(center.x, center.y, region->max.z));
    octants[4] = new BoundingBox(glm::vec3(region->min.x, center.y, region->min.z), glm::vec3(center.x, region->max.y, center.z));
    octants[5] = new BoundingBox(glm::vec3(center.x, center.y, region->min.z), glm::vec3(region->max.x, region->max.y, center.z));
    octants[6] = new BoundingBox(center, region->max);
    octants[7] = new BoundingBox(glm::vec3(region->min.x, center.y, center.z), glm::vec3(center.x, region->max.y, region->max.z));

    std::list<RenderableEntity *> octList[8];
    for (int i = 0; i < 8; i++) {
        octList[i] = std::list<RenderableEntity *>();
    }

    std::list<RenderableEntity*> deList = std::list<RenderableEntity*>();
    for (RenderableEntity* entity: this->objects) {
        if (entity->boundingBox.min != entity->boundingBox.max) {
            for (int i = 0; i < 8; i++) {
                if (octants[i]->contains(entity->boundingBox)) {
                    octList[i].push_back(entity);
                    deList.push_back(entity);
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
            this->children[i] = *CreateNode(octants[i], octList[i]);
            this->m_activeNodes |= (1 << i);
            this->children[i].build();
        }
    }

    this->tree_built = true;
    this->tree_ready = true;
}

OctTreeNode* OctTreeNode::CreateNode(BoundingBox* region, std::list<RenderableEntity*> objList) {
    if (objList.size() == 0)
        return nullptr;
    OctTreeNode* ret = new OctTreeNode(region, objList);
    ret->parent = this;
    return ret;
}

OctTreeNode* OctTreeNode::CreateNode(BoundingBox* region, RenderableEntity* Item) {
    std::list<RenderableEntity*> objList = std::list<RenderableEntity*>();
    objList.push_back(Item);
    OctTreeNode* ret = new OctTreeNode(region, objList);
    ret->parent = this;
    return ret;
}

void OctTreeNode::update(double delta) {
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

        std::list<RenderableEntity*> movedEntities = std::list<RenderableEntity*>();
        for (RenderableEntity* entity: this->objects) {
            if (entity->boundingBox.min != entity->boundingBox.max) {
                if (entity->update(delta) == 1) {

                }
            }
        }
    }
}





