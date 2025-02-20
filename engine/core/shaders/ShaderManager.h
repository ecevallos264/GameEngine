//
// Created by eceva on 2/19/2025.
//

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <map>

#include "shader-compiler.h"
#include "../patterns/Singleton.h"

class ShaderManager : public Singleton<ShaderManager> {
    std::map<std::string, Shader*> shaders;
public:
    ShaderManager() : Singleton<ShaderManager>() {}

    Shader* getShader(std::string name) {
        if (shaders.find(name) != shaders.end()) {
            return shaders[name];
        } else {
            throw std::runtime_error("Shader " + name + " not found");
        }
    }

    void setShader(std::string name, Shader* shader) {
        shaders[name] = shader;
    }
};

#endif //SHADERMANAGER_H
