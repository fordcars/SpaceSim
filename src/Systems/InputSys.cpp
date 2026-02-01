#include "InputSys.hpp"

#include <cmath>
#include <glm/gtx/norm.hpp>
#include <memory>

#include "Entities/ShipEntity.hpp"
#include "Entities/LightEntity.hpp"
#include "Systems/PhysicsSys.hpp"
#include "Systems/ResourceSys/ResourceSys.hpp"
#include "Systems/UISys.hpp"

// Static
InputSys& InputSys::get() {
    static std::unique_ptr<InputSys> instance = std::make_unique<InputSys>();
    return *instance;
}

bool InputSys::init() {
    mInputMapping[SDLK_UP] = InputNeed::Up;
    mInputMapping[SDLK_DOWN] = InputNeed::Down;
    mInputMapping[SDLK_LEFT] = InputNeed::Left;
    mInputMapping[SDLK_RIGHT] = InputNeed::Right;

    // Debug stuff
    mInputMapping[SDLK_F1] = InputNeed::ToggleShowFPS;
    mInputMapping[SDLK_F2] = InputNeed::ChangeDebugRenderMode;
    mInputMapping[SDLK_F4] = InputNeed::ToggleShowCollisionShapes;
    return true;
}

void InputSys::update(float deltaTime) {
    for (auto key : mHeldKeys) {
        auto it = mInputMapping.find(key);
        if (it != mInputMapping.end()) {
            handleHoldNeed(it->second);
        }
    }
}

void InputSys::handleEvent(const SDL_Event& event) {
    auto& physicsComp = ShipEntity::instances[0].get<PhysicsComp>();
    physicsComp.acceleration = {};

    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        SDL_Keycode key = event.key.keysym.sym;
        auto it = mInputMapping.find(key);
        if (it != mInputMapping.end()) {
            if (event.type == SDL_KEYDOWN) {
                if (!mHeldKeys.contains(key)) {
                    handleDownNeed(it->second);
                    mHeldKeys.insert(key);
                }
            } else {
                handleUpNeed(it->second);
                mHeldKeys.erase(key);
            }
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION) {
        handleMouseInput(event);
    }
}

// Handle key down event
void InputSys::handleDownNeed(InputNeed need) {
    switch (need) {
    default:
        break;
    }
}

// Handle key up event
void InputSys::handleUpNeed(InputNeed need) {
    switch (need) {
    case InputNeed::ToggleShowFPS:
        UISys::get().toggleFPSOverlay();
        break;
    case InputNeed::ChangeDebugRenderMode:
        cycleDebugRenderMode();
        break;
    case InputNeed::ToggleShowCollisionShapes:
        PhysicsSys::get().toggleCollisionShapes();
        break;
    default:
        break;
    }
}

// Handle key hold event
void InputSys::handleHoldNeed(InputNeed need) {
    const float THRUST = 5;
    auto& physicsComp = ShipEntity::instances[0].get<PhysicsComp>();

    const float speed = 2;
    switch (need) {
    case InputNeed::Up:
        physicsComp.acceleration.y = THRUST;
        break;
    case InputNeed::Down:
        physicsComp.acceleration.y = -THRUST;
        break;
    case InputNeed::Left:
        physicsComp.acceleration.x = THRUST;
        break;
    case InputNeed::Right:
        physicsComp.acceleration.x = -THRUST;
        break;
    default:
        break;
    }
}

void InputSys::handleMouseInput(const SDL_Event& event) {
    // Handle mouse input
}

void InputSys::cycleDebugRenderMode() {
    switch (mDebugRenderMode) {
    case 0:
        ++mDebugRenderMode;
        LightEntity::instances[0].get<LightComp>().shader = ResourceSys::get().getShaderResource("test_lightPosition");
        Log::debug() << "Debug render mode: worldspace position";
        break;
    case 1:
        ++mDebugRenderMode;
        LightEntity::instances[0].get<LightComp>().shader = ResourceSys::get().getShaderResource("test_lightNormal");
        Log::debug() << "Debug render mode: cameraspace normal";
        break;
    case 2:
        ++mDebugRenderMode;
        LightEntity::instances[0].get<LightComp>().shader = ResourceSys::get().getShaderResource("test_lightAlbedo");
        Log::debug() << "Debug render mode: albedo";
        break;
    case 3:
        ++mDebugRenderMode;
        LightEntity::instances[0].get<LightComp>().shader = ResourceSys::get().getShaderResource("test_lightMetallic");
        Log::debug() << "Debug render mode: metallic";
        break;
    case 4:
        ++mDebugRenderMode;
        LightEntity::instances[0].get<LightComp>().shader = ResourceSys::get().getShaderResource("test_lightRoughness");
        Log::debug() << "Debug render mode: roughness";
        break;
    default:
        mDebugRenderMode = 0;
        LightEntity::instances[0].get<LightComp>().shader = ResourceSys::get().getShaderResource("light_pbr");
        Log::debug() << "Debug render mode disabled.";
        break;
    }
}
