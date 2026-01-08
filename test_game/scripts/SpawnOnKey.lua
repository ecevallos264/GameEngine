-- SpawnOnKey.lua
-- Spawns new entities when a key is pressed

local spawnCount = 0
local maxSpawns = 10

function onStart(entityId)
    print("SpawnOnKey ready - press Enter to spawn cubes (max " .. maxSpawns .. ")")
end

function onUpdate(entityId, deltaTime)
    if isKeyJustPressed("Enter") and spawnCount < maxSpawns then
        -- Get our position to spawn relative to it
        local pos = getPosition(entityId)

        -- Create new entity
        local newEntity = createEntity()

        -- Random offset
        local offsetX = randomFloat(-5, 5)
        local offsetZ = randomFloat(-5, 5)

        -- Note: This creates an entity but without components
        -- A full implementation would need addComponent functions

        spawnCount = spawnCount + 1
        print("Created entity #" .. spawnCount .. " (id: " .. newEntity .. ")")
    end
end

function onDestroy(entityId)
    print("SpawnOnKey destroyed, spawned " .. spawnCount .. " entities total")
end
