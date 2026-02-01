#include "GameplaySys.hpp"

#include "Constants.hpp"
#include "Entities/CameraEntity.hpp"
#include "Entities/LightEntity.hpp"
#include "Entities/ShipEntity.hpp"
#include "Entities/SkyboxEntity.hpp"
#include "RenderingSys.hpp"
#include "ResourceSys/ResourceSys.hpp"

// Static
GameplaySys& GameplaySys::get() {
    static std::unique_ptr<GameplaySys> instance = std::make_unique<GameplaySys>();
    return *instance;
}

void GameplaySys::start() {
    // Init stuff
    if (Constants::ENABLE_FXAA) {
        RenderingSys::get().setPostProcessShader(ResourceSys::get().getShaderResource("fxaa"));
    }

    // Create camera
    CameraEntity camera;
    CameraInfoComp& info = camera.get<CameraInfoComp>();
    PositionComp& cameraPos = camera.get<PositionComp>();
    info.horizFOV = Constants::HORIZ_FOV;
    info.aspectRatio = static_cast<float>(Constants::DEFAULT_WINDOW_SIZE_X) / Constants::DEFAULT_WINDOW_SIZE_Y;
    info.nearClippingPlane = 2.0f;
    info.farClippingPlane = 10000.0f;
    info.upVector = {0.0f, 1.0f, 0.0f};
    info.direction = {10, 0, 10, 1};
    cameraPos.coords = {12, 10, 25};

    CameraEntity::instances.emplace_back(std::move(camera));

    // Create ship
    {
        ShipEntity ship;
        auto [position, renderable, sound, physics] = ship.getComponents();
        position.coords.x = -5;
        position.coords.y = 3;
        position.coords.z = 10;
        physics.positionOffset = {0, 0.7, 0};
        renderable.objectResource = ResourceSys::get().getObjResource("skelly");
        renderable.shader = ResourceSys::get().getShaderResource("deferred_pbr_skinned");

        sound.audioResource = ResourceSys::get().getAudioResource("step");
        sound.audioResource->call<ma_sound_set_looping>(true);

        ShipEntity::instances.emplace_back(std::move(ship));
    }

    // Create light
    // TODO: create directional light
    {
        LightEntity mainLight;
        auto [position, light, physics] = mainLight.getComponents();
        position.coords.x = 100;
        position.coords.y = 100;
        position.coords.z = 300;
        light.shader = ResourceSys::get().getShaderResource("light_pbr");
        light.intensity = 8.0f;
        LightEntity::instances.emplace_back(std::move(mainLight));
    }

    // Create skybox
    {
        SkyboxEntity sky;
        auto [position, renderable] = sky.getComponents();
        position.scale = {1000, 1000, 1000};
        renderable.objectResource = ResourceSys::get().getObjResource("spacebox");
        renderable.shader = ResourceSys::get().getShaderResource("flat");
        renderable.shadingType = RenderableComp::ShadingType::ForwardShaded;
        SkyboxEntity::instances.emplace_back(std::move(sky));
    }
}

void GameplaySys::update(float deltaTime) {
    auto& shipPosition = ShipEntity::instances[0].get<PositionComp>().coords;
    auto& cameraPosition = CameraEntity::instances[0].get<PositionComp>().coords;
    CameraEntity::instances[0].get<CameraInfoComp>().direction = glm::vec4(shipPosition, 1);

    // Update skybox
    SkyboxEntity::instances[0].get<PositionComp>().coords = cameraPosition;
}