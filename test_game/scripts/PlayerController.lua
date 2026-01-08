-- PlayerController.lua
-- Attached to player entity to handle movement

-- Local state for this script instance
local moveSpeed = 5.0
local isGrounded = true

-- Called once when entity is created
function onStart(entityId)
    print("PlayerController started on entity: " .. entityId)

    -- Get initial position
    local pos = getPosition(entityId)
    print("Starting position: " .. pos.x .. ", " .. pos.y .. ", " .. pos.z)
end

-- Called every frame
function onUpdate(entityId, deltaTime)
    -- WASD movement
    if isKeyPressed("W") then
        translate(entityId, 0, 0, -moveSpeed * deltaTime)
    end
    if isKeyPressed("S") then
        translate(entityId, 0, 0, moveSpeed * deltaTime)
    end
    if isKeyPressed("A") then
        translate(entityId, -moveSpeed * deltaTime, 0, 0)
    end
    if isKeyPressed("D") then
        translate(entityId, moveSpeed * deltaTime, 0, 0)
    end

    -- Up/Down movement
    if isKeyPressed("Space") then
        translate(entityId, 0, moveSpeed * deltaTime, 0)
    end
    if isKeyPressed("LeftShift") then
        translate(entityId, 0, -moveSpeed * deltaTime, 0)
    end
end

-- Called when entity is destroyed
function onDestroy(entityId)
    print("PlayerController destroyed on entity: " .. entityId)
end
