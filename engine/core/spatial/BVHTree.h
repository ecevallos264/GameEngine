#ifndef GAMEENGINE_BVH_TREE_H
#define GAMEENGINE_BVH_TREE_H

#include "../math/AABB.h"
#include "../math/Frustrum.h"
#include "../../ecs/Entity.h"
#include <vector>
#include <algorithm>
#include <stack>
#include <functional>

namespace Spatial {

// BVH Node structure
struct BVHNode {
    AABB bounds;
    int leftChild = -1;   // Index to left child (-1 if leaf)
    int rightChild = -1;  // Index to right child (-1 if leaf)
    int objectIndex = -1; // Index to object if leaf node (-1 if internal)

    bool isLeaf() const {
        return leftChild == -1 && rightChild == -1;
    }
};

// Object info for building the BVH
struct BVHObjectInfo {
    ECS::Entity entity;
    AABB bounds;
    glm::vec3 centroid;

    BVHObjectInfo() = default;
    BVHObjectInfo(ECS::Entity e, const AABB& b)
        : entity(e), bounds(b), centroid(b.getCenter()) {}
};

// Build method for BVH
enum class BVHBuildMethod {
    MedianSplit,    // Split at median object
    SAH,            // Surface Area Heuristic (better quality, slower build)
    Middle          // Split at spatial middle
};

class BVHTree {
public:
    BVHTree() = default;

    // Build the BVH from a list of objects with bounds
    void build(std::vector<BVHObjectInfo>& objects, BVHBuildMethod method = BVHBuildMethod::SAH) {
        nodes.clear();
        if (objects.empty()) return;

        // Reserve space (worst case: 2n-1 nodes for n objects)
        nodes.reserve(objects.size() * 2);

        // Build recursively
        buildRecursive(objects, 0, static_cast<int>(objects.size()), method);
    }

    // Clear the tree
    void clear() {
        nodes.clear();
    }

    // Check if tree is empty
    bool empty() const {
        return nodes.empty();
    }

    // Get number of nodes
    size_t nodeCount() const {
        return nodes.size();
    }

    // Query all objects that intersect with an AABB
    void queryAABB(const AABB& queryBounds, std::vector<int>& results) const {
        if (nodes.empty()) return;

        std::stack<int> stack;
        stack.push(0);

        while (!stack.empty()) {
            int nodeIdx = stack.top();
            stack.pop();

            const BVHNode& node = nodes[nodeIdx];

            if (!node.bounds.intersects(queryBounds)) {
                continue;
            }

            if (node.isLeaf()) {
                results.push_back(node.objectIndex);
            } else {
                if (node.leftChild != -1) stack.push(node.leftChild);
                if (node.rightChild != -1) stack.push(node.rightChild);
            }
        }
    }

    // Query objects visible in frustum
    void queryFrustum(const Frustum& frustum, std::vector<int>& results) const {
        if (nodes.empty()) return;

        std::stack<int> stack;
        stack.push(0);

        while (!stack.empty()) {
            int nodeIdx = stack.top();
            stack.pop();

            const BVHNode& node = nodes[nodeIdx];

            // Frustum-AABB test
            if (!frustum.isAABBInside(node.bounds.min, node.bounds.max)) {
                continue;
            }

            if (node.isLeaf()) {
                results.push_back(node.objectIndex);
            } else {
                if (node.leftChild != -1) stack.push(node.leftChild);
                if (node.rightChild != -1) stack.push(node.rightChild);
            }
        }
    }

    // Traverse all nodes (for debugging/visualization)
    void traverse(std::function<void(const BVHNode&, int depth)> callback) const {
        if (nodes.empty()) return;
        traverseRecursive(0, 0, callback);
    }

    // Get a node by index
    const BVHNode& getNode(int index) const {
        return nodes[index];
    }

    // Get all nodes (for visualization)
    const std::vector<BVHNode>& getNodes() const {
        return nodes;
    }

    // Get max depth of tree
    int getMaxDepth() const {
        if (nodes.empty()) return 0;
        return getDepthRecursive(0);
    }

private:
    std::vector<BVHNode> nodes;

    int buildRecursive(std::vector<BVHObjectInfo>& objects, int start, int end, BVHBuildMethod method) {
        int nodeIndex = static_cast<int>(nodes.size());
        nodes.emplace_back();

        // Compute bounds for this node
        AABB nodeBounds;
        for (int i = start; i < end; ++i) {
            nodeBounds.expand(objects[i].bounds);
        }
        nodes[nodeIndex].bounds = nodeBounds;

        int objectCount = end - start;

        // Leaf node
        if (objectCount == 1) {
            nodes[nodeIndex].objectIndex = start;
            return nodeIndex;
        }

        // Choose split axis
        AABB centroidBounds;
        for (int i = start; i < end; ++i) {
            centroidBounds.expand(objects[i].centroid);
        }
        int axis = centroidBounds.getLongestAxis();

        // If all centroids are at the same position, make a leaf
        if (centroidBounds.min[axis] == centroidBounds.max[axis]) {
            nodes[nodeIndex].objectIndex = start;
            return nodeIndex;
        }

        // Find split point based on method
        int mid = start + objectCount / 2;

        switch (method) {
            case BVHBuildMethod::MedianSplit:
                // Sort by centroid on chosen axis and split at median
                std::nth_element(
                    objects.begin() + start,
                    objects.begin() + mid,
                    objects.begin() + end,
                    [axis](const BVHObjectInfo& a, const BVHObjectInfo& b) {
                        return a.centroid[axis] < b.centroid[axis];
                    }
                );
                break;

            case BVHBuildMethod::Middle:
                // Split at spatial middle
                {
                    float midPoint = (centroidBounds.min[axis] + centroidBounds.max[axis]) * 0.5f;
                    auto midIter = std::partition(
                        objects.begin() + start,
                        objects.begin() + end,
                        [axis, midPoint](const BVHObjectInfo& obj) {
                            return obj.centroid[axis] < midPoint;
                        }
                    );
                    mid = static_cast<int>(midIter - objects.begin());

                    // If partition failed, fall back to median
                    if (mid == start || mid == end) {
                        mid = start + objectCount / 2;
                        std::nth_element(
                            objects.begin() + start,
                            objects.begin() + mid,
                            objects.begin() + end,
                            [axis](const BVHObjectInfo& a, const BVHObjectInfo& b) {
                                return a.centroid[axis] < b.centroid[axis];
                            }
                        );
                    }
                }
                break;

            case BVHBuildMethod::SAH:
                // Surface Area Heuristic
                mid = findSAHSplit(objects, start, end, axis, nodeBounds);
                break;
        }

        // Ensure we don't have empty partitions
        if (mid == start) mid = start + 1;
        if (mid == end) mid = end - 1;

        // Build children
        nodes[nodeIndex].leftChild = buildRecursive(objects, start, mid, method);
        nodes[nodeIndex].rightChild = buildRecursive(objects, mid, end, method);

        return nodeIndex;
    }

    int findSAHSplit(std::vector<BVHObjectInfo>& objects, int start, int end, int axis, const AABB& nodeBounds) {
        const int objectCount = end - start;
        const float traversalCost = 1.0f;
        const float intersectCost = 1.0f;

        // For small number of objects, just use median
        if (objectCount <= 4) {
            int mid = start + objectCount / 2;
            std::nth_element(
                objects.begin() + start,
                objects.begin() + mid,
                objects.begin() + end,
                [axis](const BVHObjectInfo& a, const BVHObjectInfo& b) {
                    return a.centroid[axis] < b.centroid[axis];
                }
            );
            return mid;
        }

        // Sort objects by centroid on this axis
        std::sort(
            objects.begin() + start,
            objects.begin() + end,
            [axis](const BVHObjectInfo& a, const BVHObjectInfo& b) {
                return a.centroid[axis] < b.centroid[axis];
            }
        );

        // Compute costs for each split position
        float bestCost = std::numeric_limits<float>::max();
        int bestSplit = start + objectCount / 2;

        // Precompute right-side bounds
        std::vector<AABB> rightBounds(objectCount);
        rightBounds[objectCount - 1] = objects[end - 1].bounds;
        for (int i = objectCount - 2; i >= 0; --i) {
            rightBounds[i] = AABB::merge(rightBounds[i + 1], objects[start + i].bounds);
        }

        // Sweep from left to right
        AABB leftBounds;
        for (int i = 0; i < objectCount - 1; ++i) {
            leftBounds.expand(objects[start + i].bounds);

            int leftCount = i + 1;
            int rightCount = objectCount - leftCount;

            float leftArea = leftBounds.getSurfaceArea();
            float rightArea = rightBounds[i + 1].getSurfaceArea();
            float parentArea = nodeBounds.getSurfaceArea();

            float cost = traversalCost +
                         intersectCost * (leftCount * leftArea + rightCount * rightArea) / parentArea;

            if (cost < bestCost) {
                bestCost = cost;
                bestSplit = start + leftCount;
            }
        }

        // Check if leaf would be cheaper
        float leafCost = intersectCost * objectCount;
        if (leafCost < bestCost && objectCount <= 8) {
            return start; // Signal to create leaf
        }

        return bestSplit;
    }

    void traverseRecursive(int nodeIdx, int depth, std::function<void(const BVHNode&, int)>& callback) const {
        if (nodeIdx < 0 || nodeIdx >= static_cast<int>(nodes.size())) return;

        const BVHNode& node = nodes[nodeIdx];
        callback(node, depth);

        if (!node.isLeaf()) {
            if (node.leftChild != -1) traverseRecursive(node.leftChild, depth + 1, callback);
            if (node.rightChild != -1) traverseRecursive(node.rightChild, depth + 1, callback);
        }
    }

    int getDepthRecursive(int nodeIdx) const {
        if (nodeIdx < 0 || nodeIdx >= static_cast<int>(nodes.size())) return 0;

        const BVHNode& node = nodes[nodeIdx];
        if (node.isLeaf()) return 1;

        int leftDepth = node.leftChild != -1 ? getDepthRecursive(node.leftChild) : 0;
        int rightDepth = node.rightChild != -1 ? getDepthRecursive(node.rightChild) : 0;

        return 1 + std::max(leftDepth, rightDepth);
    }
};

} // namespace Spatial

#endif //GAMEENGINE_BVH_TREE_H
