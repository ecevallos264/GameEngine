//
// Created by eceva on 1/15/2025.
//

#ifndef GAMEENGINE_EDGE_H
#define GAMEENGINE_EDGE_H

struct Edge {
    int startIndex;  // Index of the starting vertex
    int endIndex;    // Index of the ending vertex

    Edge(int start, int end) : startIndex(start), endIndex(end) {}
};

#endif //GAMEENGINE_EDGE_H
