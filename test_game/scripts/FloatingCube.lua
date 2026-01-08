-- FloatingCube.lua
-- Makes an object bob up and down with a sine wave

local baseY = 0.0
local amplitude = 1.0      -- How far up/down to move
local frequency = 2.0      -- Oscillations per second
local time = 0.0

function onStart(entityId)
    -- Store the starting Y position
    local pos = getPosition(entityId)
    baseY = pos.y
    print("FloatingCube: base Y = " .. baseY)
end

function onUpdate(entityId, deltaTime)
    time = time + deltaTime

    local pos = getPosition(entityId)
    local newY = baseY + math.sin(time * frequency * math.pi * 2) * amplitude

    setPosition(entityId, pos.x, newY, pos.z)
end

function onDestroy(entityId)
    -- nothing to clean up
end
