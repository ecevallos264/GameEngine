//
// Created by eceva on 1/7/2025.
//

#ifndef GAMEENGINE_ENTITY_H
#define GAMEENGINE_ENTITY_H

class Entity {
public:
    virtual void update(float deltaTime, Shader* shader) = 0;
    virtual void draw() = 0;
};

#endif //GAMEENGINE_ENTITY_H
