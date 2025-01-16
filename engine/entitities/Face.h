//
// Created by eceva on 1/15/2025.
//

#ifndef GAMEENGINE_FACE_H
#define GAMEENGINE_FACE_H

struct Face {
    int indices[3];  // Indices of the three vertices forming the triangle

    Face(int i1, int i2, int i3) {
        indices[0] = i1;
        indices[1] = i2;
        indices[2] = i3;
    }
};

#endif //GAMEENGINE_FACE_H
