#include "bvh-node.h"
#include <iostream> // For debugging purposes
#include "../../core/shaders/ShaderManager.h"
#include "../Ray.h"

namespace BVH {
    void Node::insert(BoundingVolume* boundingVolume) {
        if (!boundingVolume) {
            std::cerr << "Error: Attempted to insert a nullptr bounding volume." << std::endl;
            return;
        }

        if (isLeaf && boundingRegion == nullptr) {
            boundingRegion = boundingVolume;
            isLeaf = true;
            left = nullptr;
            right = nullptr;
            return;
        }

        if (isLeaf) {
            Node* newNode = new Node();
            newNode->isLeaf = true;
            newNode->boundingRegion = boundingRegion;

            left = newNode;
            right = new Node();
            right->isLeaf = true;
            right->boundingRegion = boundingVolume;

            isLeaf = false;

            boundingRegion = new BoundingVolume(newNode->boundingRegion->getCenter(), right->boundingRegion->getCenter());
            boundingRegion->expandToFit(*newNode->boundingRegion);
            boundingRegion->expandToFit(*right->boundingRegion);
            return;
        }

        float leftArea = left->boundingRegion->surfaceArea();
        float rightArea = right->boundingRegion->surfaceArea();

        BoundingVolume tempLeft = *left->boundingRegion;
        tempLeft.expandToFit(*boundingVolume);
        float leftCost = tempLeft.surfaceArea() - leftArea;

        BoundingVolume tempRight = *right->boundingRegion;
        tempRight.expandToFit(*boundingVolume);
        float rightCost = tempRight.surfaceArea() - rightArea;

        if (leftCost < rightCost) {
            left->insert(boundingVolume);
            boundingRegion->expandToFit(*left->boundingRegion);
        } else {
            right->insert(boundingVolume);
            boundingRegion->expandToFit(*right->boundingRegion);
        }
    }

    void Node::build(std::vector<BoundingVolume*>& volumes) {
        if (volumes.empty()) return;

        // If only one volume, make it a leaf node
        if (volumes.size() == 1) {
            boundingRegion = volumes[0];
            isLeaf = true;
            left = nullptr;
            right = nullptr;
            return;
        }

        // Calculate the bounding box of all volumes
        boundingRegion = new BoundingVolume(volumes[0]->getCenter(), volumes[0]->getCenter());
        for (auto* volume : volumes) {
            boundingRegion->expandToFit(*volume);
        }

        // Find the longest axis to split on (X, Y, or Z)
        glm::vec3 size = boundingRegion->getMax() - boundingRegion->getMin();
        int axis = (size.x > size.y) ? ((size.x > size.z) ? 0 : 2) : ((size.y > size.z) ? 1 : 2);

        // Sort volumes based on the chosen axis
        std::sort(volumes.begin(), volumes.end(), [axis](BoundingVolume* a, BoundingVolume* b) {
            return a->getCenter()[axis] < b->getCenter()[axis];
        });

        // Split volumes at the median
        size_t mid = volumes.size() / 2;
        std::vector<BoundingVolume*> leftVolumes(volumes.begin(), volumes.begin() + mid);
        std::vector<BoundingVolume*> rightVolumes(volumes.begin() + mid, volumes.end());

        // Create child nodes
        left = new Node();
        right = new Node();

        // Recursively build the subtrees
        left->build(leftVolumes);
        right->build(rightVolumes);

        isLeaf = false;
    }


    void Node::update() {
        if (isLeaf) return;

        if (left) left->update();
        if (right) right->update();

        boundingRegion->expandToFit(*left->boundingRegion);
        boundingRegion->expandToFit(*right->boundingRegion);
    }

    void Node::render(glm::mat4 view, glm::mat4 projection, int level) {
        if (boundingRegion) {
            glm::vec3 center = boundingRegion->getCenter();
            glm::vec3 size = boundingRegion->getMax() - boundingRegion->getMin();

            // Generate color based on level
            float hue = static_cast<float>(level) * 0.1f;
            hue = fmod(hue, 1.0f);
            glm::vec3 color = glm::vec3(
                    0.5f + 0.5f * cos(6.28318f * (hue + 0.0f)), // Red
                    0.5f + 0.5f * cos(6.28318f * (hue + 0.33f)), // Green
                    0.5f + 0.5f * cos(6.28318f * (hue + 0.66f))  // Blue
            );

            drawWireframeBox(center, size, view, projection, color);
        }

        if (left) left->render(view, projection, level + 1);
        if (right) right->render(view, projection, level + 1);
    }

    void Node::drawWireframeBox(const glm::vec3& center, const glm::vec3& size, glm::mat4 view, glm::mat4 projection, glm::vec3 color) {
        setupWireframeBox(color);  // Pass color to the setup function
        glm::mat4 model = glm::translate(glm::mat4(1.0f), center) * glm::scale(glm::mat4(1.0f), size);

        Shader* shader = ShaderManager::getInstance().getShader("shader1");
        shader->use();
        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 24);
        glBindVertexArray(0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    void Node::setupWireframeBox(glm::vec3 color) {
        float vertices[] = {
                // Bottom face (y = -0.5)
                -0.5f, -0.5f, -0.5f,  color.r, color.g, color.b,
                0.5f, -0.5f, -0.5f,  color.r, color.g, color.b,
                0.5f, -0.5f, -0.5f,  color.r, color.g, color.b,
                0.5f, -0.5f,  0.5f,  color.r, color.g, color.b,
                0.5f, -0.5f,  0.5f,  color.r, color.g, color.b,
                -0.5f, -0.5f,  0.5f,  color.r, color.g, color.b,
                -0.5f, -0.5f,  0.5f,  color.r, color.g, color.b,
                -0.5f, -0.5f, -0.5f,  color.r, color.g, color.b,

                // Top face (y = 0.5)
                -0.5f,  0.5f, -0.5f,  color.r, color.g, color.b,
                0.5f,  0.5f, -0.5f,  color.r, color.g, color.b,
                0.5f,  0.5f, -0.5f,  color.r, color.g, color.b,
                0.5f,  0.5f,  0.5f,  color.r, color.g, color.b,
                0.5f,  0.5f,  0.5f,  color.r, color.g, color.b,
                -0.5f,  0.5f,  0.5f,  color.r, color.g, color.b,
                -0.5f,  0.5f,  0.5f,  color.r, color.g, color.b,
                -0.5f,  0.5f, -0.5f,  color.r, color.g, color.b,

                // Vertical edges
                -0.5f, -0.5f, -0.5f,  color.r, color.g, color.b,
                -0.5f,  0.5f, -0.5f,  color.r, color.g, color.b,
                0.5f, -0.5f, -0.5f,  color.r, color.g, color.b,
                0.5f,  0.5f, -0.5f,  color.r, color.g, color.b,
                0.5f, -0.5f,  0.5f,  color.r, color.g, color.b,
                0.5f,  0.5f,  0.5f,  color.r, color.g, color.b,
                -0.5f, -0.5f,  0.5f,  color.r, color.g, color.b,
                -0.5f,  0.5f,  0.5f,  color.r, color.g, color.b
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Vertex position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

        // Vertex color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    bool Node::intersectsWithRay(Ray ray) {
        return boundingRegion->getFront().intersectsWithRay(ray) ||
               boundingRegion->getBack().intersectsWithRay(ray) ||
               boundingRegion->getTop().intersectsWithRay(ray) ||
               boundingRegion->getBottom().intersectsWithRay(ray) ||
               boundingRegion->getLeft().intersectsWithRay(ray) ||
               boundingRegion->getRight().intersectsWithRay(ray);
    }


    void Node::cleanupWireframeBox() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

}
