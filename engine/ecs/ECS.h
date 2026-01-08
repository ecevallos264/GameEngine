#ifndef GAMEENGINE_ECS_H
#define GAMEENGINE_ECS_H

// Core ECS
#include "Entity.h"
#include "Component.h"
#include "ComponentPool.h"
#include "Registry.h"

// Built-in Components
#include "components/TransformComponent.h"
#include "components/MeshComponent.h"
#include "components/RenderComponent.h"
#include "components/CameraComponent.h"
#include "components/BoundsComponent.h"

// Built-in Systems
#include "systems/CameraSystem.h"
#include "systems/CameraInputSystem.h"
#include "systems/FrustumCullingSystem.h"
#include "systems/MeshRenderSystem.h"

#endif //GAMEENGINE_ECS_H
