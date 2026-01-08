-- Rotator.lua
-- Simple script that rotates an object continuously

-- Configurable properties
local rotationSpeed = 90.0  -- degrees per second

function onStart(entityId)
    print("Rotator started on entity: " .. entityId)
end

function onUpdate(entityId, deltaTime)
    -- Rotate around Y axis
    rotate(entityId, 0, rotationSpeed * deltaTime, 0)
end

function onDestroy(entityId)
    -- cleanup if needed
end
