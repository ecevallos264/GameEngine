# Scripting System

## Overview

The scripting system allows gameplay logic to be written in Lua scripts that are attached to entities. Scripts have access to engine APIs for transforms, input, entity management, and more.

## Setup (Required)

### 1. Add Lua and sol2 to CMakeLists.txt

```cmake
# Option A: FetchContent (recommended)
include(FetchContent)

FetchContent_Declare(
    lua
    GIT_REPOSITORY https://github.com/lua/lua.git
    GIT_TAG v5.4.6
)
FetchContent_MakeAvailable(lua)

FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG v3.3.0
)
FetchContent_MakeAvailable(sol2)

target_link_libraries(GameEngine lua sol2)

# Option B: vcpkg
# vcpkg install lua sol2
# find_package(Lua REQUIRED)
# find_package(sol2 CONFIG REQUIRED)
# target_link_libraries(GameEngine ${LUA_LIBRARIES} sol2)
```

### 2. Add ScriptSystem to your application

```cpp
#include "engine/scripting/ScriptSystem.h"

// In your application initialize():
scriptSystem = &systemManager.addSystem<Scripting::ScriptSystem>(registry);

// The system will automatically:
// - Load scripts when entities with ScriptComponent are created
// - Call onStart() once per entity
// - Call onUpdate() every frame
// - Call onDestroy() when entity is destroyed
```

## Script Lifecycle

```lua
-- Called once when the script is first loaded
function onStart(entityId)
    print("Script started!")
end

-- Called every frame
function onUpdate(entityId, deltaTime)
    -- Game logic here
end

-- Called when the entity is destroyed
function onDestroy(entityId)
    print("Cleaning up!")
end
```

## Available API Functions

### Transform
```lua
getPosition(entityId)        -- Returns Vec3
setPosition(entityId, x, y, z)
getRotation(entityId)        -- Returns Vec3 (euler degrees)
setRotation(entityId, x, y, z)
getScale(entityId)           -- Returns Vec3
setScale(entityId, x, y, z)
```

### Input
```lua
isKeyPressed("W")            -- Returns true while key is held
isKeyJustPressed("Space")    -- Returns true only on first frame of press
```

### Entity Management
```lua
createEntity()               -- Returns new entityId
destroyEntity(entityId)
```

### Utility
```lua
print("message")             -- Outputs to console
getDeltaTime()               -- Frame delta time in seconds
```

## Attaching Scripts to Entities

```cpp
// Create entity with mesh, transform, etc.
ECS::Entity player = registry.create();
registry.emplace<ECS::TransformComponent>(player, glm::vec3(0, 0, 0));

// Attach script
auto& script = registry.emplace<ECS::ScriptComponent>(player, "scripts/PlayerController.lua");

// Optionally set properties accessible from Lua
script.set("moveSpeed", 5.0f);
script.set("health", 100);
```

## Hot Reloading

Scripts can be reloaded at runtime without restarting:

```cpp
scriptSystem->reloadScript("scripts/PlayerController.lua");
```

This allows rapid iteration during development.

## Example Scripts

See `test_game/scripts/` for examples:
- `PlayerController.lua` - WASD movement and jumping
- `Rotator.lua` - Continuous rotation
- `FloatingCube.lua` - Sine wave bobbing motion
- `SpawnOnKey.lua` - Dynamic entity spawning

## Best Practices

1. **Keep scripts focused** - One behavior per script
2. **Use properties** - Configure scripts from C++ rather than hardcoding values
3. **Cache entity references** - Store entityId in local variables
4. **Avoid heavy computation** - Lua is slower than C++, do heavy math in C++
5. **Use events for communication** - Don't poll other entities every frame
