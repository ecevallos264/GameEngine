//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_SCENECONTROLLER_H
#define GAMEENGINE_SCENECONTROLLER_H


#include <string>
#include <unordered_map>
#include "Scene.h"
#include "../core/patterns/Singleton.h"

class SceneController : public Singleton<SceneController>{
private:
    std::shared_ptr<Scene> currentScene = nullptr;
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;

    std::shared_ptr<Scene> getScene(std::string scene_id) {
        auto it = scenes.find(scene_id);
        if (it != scenes.end()) {
            return it->second;
        }
    }


public:
    SceneController() : Singleton<SceneController>() {}

    void swapScene(std::string scene_id) {
        currentScene = getScene(scene_id);
    }

    std::shared_ptr<Scene> getCurrentScene() {
        if(currentScene == nullptr) {
            return nullptr; // TODO throw exception and handle
        }
        return currentScene;
    }

    void addScene(std::string scene_id, std::shared_ptr<Scene> scene) {
        scenes[scene_id] = scene;
    }
};


#endif //GAMEENGINE_SCENECONTROLLER_H
